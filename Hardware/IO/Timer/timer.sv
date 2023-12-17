`ifndef TIMER_SV
    `define TIMER_SV

`include "../../System/edge_detector.sv"

module timer (
    /* Global signals */
    input logic clk_i,
    input logic rst_n_i,

    /* Write interface */
    input logic write_i,
    input logic [31:0] write_data_i,
    input logic [2:0] write_address_i,
    input logic [3:0] write_strobe_i,
    output logic write_error_o,

    /* Read interface */
    input logic read_i,
    input logic [2:0] read_address_i,
    output logic [31:0] read_data_o,
    output logic read_error_o,

    /* Status */
    output logic interrupt_o
);

//====================================================================================
//      PARAMETERS AND FUNCTION
//====================================================================================

    /* REGISTER ADDRESSES */
    localparam COMPARE_LOW = 0;
    localparam COMPARE_HIGH = 1;

    localparam TIMER_VALUE_LOW = 2;
    localparam TIMER_VALUE_HIGH = 3;

    localparam CONFIGURATION = 4;


    typedef struct packed {
        /* Enable generation of interrupts */
        logic interrupt_enable;

        /* Stop timer once it reaches the threshold */
        logic one_shot;

        /* Enable timer to count */
        logic enable;
    } timer_config_t;


    function bit enable_write(input logic [1:0] address, input logic [1:0] write_address);
        return (address == write_address) & write_i;
    endfunction : enable_write

    assign write_error_o = (write_address_i > 4) & write_i;
    assign read_error_o = (read_address_i > 4) & read_i;


    logic [4:0] enable_register;

    assign enable_register = write_i ? (1 << write_address_i) : '0;


//====================================================================================
//      REGISTERS
//====================================================================================

    logic [3:0][7:0] write_data;

    assign write_data = write_data_i;

    /* Used to set a value when the timer should issue an interrupt */
    logic [1:0][3:0][7:0] timer_compare;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin : timer_compare_register_low
            if (!rst_n_i) begin
                timer_compare[0] <= '1;
            end else if (enable_register[COMPARE_LOW]) begin
                for (int i = 0; i < 4; ++i) begin
                    if (write_strobe_i[i]) begin 
                        timer_compare[0][i] <= write_data[i];
                    end
                end
            end
        end : timer_compare_register_low

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin : timer_compare_register_high
            if (!rst_n_i) begin
                timer_compare[1] <= '1;
            end else if (enable_register[COMPARE_HIGH]) begin
                for (int i = 0; i < 4; ++i) begin
                    if (write_strobe_i[i]) begin 
                        timer_compare[1][i] <= write_data[i];
                    end
                end
            end
        end : timer_compare_register_high

    
    /* Configuration register */
    timer_config_t configuration;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                configuration <= '0;
            end else if (enable_register[CONFIGURATION]) begin 
                configuration <= write_data_i[2:0];
            end 
        end 


    logic stop_timer, interrupt;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                stop_timer <= 1'b1;
            end else begin 
                if (configuration.enable) begin
                    if (interrupt & configuration.one_shot) begin
                        stop_timer <= 1'b1;
                    end else if (enable_register[CONFIGURATION]) begin
                        /* User triggers restart by setting the 3-th bit */
                        stop_timer <= write_data_i[3];
                    end
                end else begin
                    stop_timer <= 1'b1;
                end
            end 
        end 


//====================================================================================
//      TIMER LOGIC
//====================================================================================

    logic [1:0][3:0][7:0] timer;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin : timer_register
            if (!rst_n_i) begin
                timer <= '0;
            end else if (enable_register[TIMER_VALUE_LOW]) begin
                for (int i = 0; i < 4; ++i) begin
                    if (write_strobe_i[i]) begin 
                        timer[0][i] <= write_data[i];
                    end
                end
            end else if (enable_register[TIMER_VALUE_HIGH]) begin
                for (int i = 0; i < 4; ++i) begin
                    if (write_strobe_i[i]) begin 
                        timer[1][i] <= write_data[i];
                    end
                end
            end else if (!stop_timer & !interrupt_o & !(interrupt & configuration.one_shot)) begin
                /* On interrupt reach, stop the timer, to clear
                 * it, load the timer with another value */
                timer <= timer + 1'b1;
            end 
        end : timer_register


//====================================================================================
//      OUTPUT LOGIC
//====================================================================================

    edge_detector #(1, 0) event_detector (
        .clk_i   ( clk_i  ),
        .rst_n_i ( rst_n_i ),

        .signal_i ( timer == timer_compare ),
        .edge_o   ( interrupt              )
    );

    assign interrupt_o = interrupt & configuration.interrupt_enable;


        always_comb begin
            /* Default value */
            read_data_o = '0;
            
            case (read_address_i)
                COMPARE_LOW: read_data_o = timer_compare[0];
                COMPARE_HIGH: read_data_o = timer_compare[1];

                TIMER_VALUE_LOW: read_data_o = timer[0];
                TIMER_VALUE_HIGH: read_data_o = timer[1];

                CONFIGURATION: read_data_o = {stop_timer, configuration};
            endcase 
        end

endmodule : timer

`endif 