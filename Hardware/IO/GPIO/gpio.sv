`ifndef GPIO_SV
    `define GPIO_SV

`include "../../System/synchronizer.sv"

module gpio (
    /* Global signals */
    input logic clk_i,
    input logic rst_n_i,
    input logic debounce_i,

    /* SoC external pin */
    inout wire pin_io,

    /* Write interface */
    input logic write_i,
    input logic [1:0] write_address_i,
    input logic write_data_i,

    /* Read interface */
    input logic [1:0] read_address_i,
    output logic read_data_o,

    output logic interrupt_o
);

//====================================================================================
//      REGISTERS
//====================================================================================

    logic [3:0] enable_write; 

    assign enable_write = (1'b1 << write_address_i) & write_i;


    /* Value of the pin */
    logic pin_value, debounced; logic [1:0] sync_pin;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                pin_value <= 1'b0;
            end else begin
                if (direction) begin 
                    /* In input mode the pin can be set only by external events */
                    if (debounced) begin 
                        pin_value <= sync_pin[1];  
                    end
                end else begin
                    /* In output mode, the pin can be set by the CPU */
                    if (enable_write[0]) begin 
                        pin_value <= write_data_i;
                    end
                end 
            end 
        end


    /* Select the direction of the GPIO pin (input / output) */
    logic direction;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                /* Pin set on output */
                direction <= 1'b0;
            end else if (enable_write[1]) begin 
                direction <= write_data_i;
            end 
        end  


    /* Enable interrupt generation while in input mode */
    logic interrupt_enable;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                interrupt_enable <= 1'b0;
            end else if (enable_write[2]) begin 
                interrupt_enable <= write_data_i;
            end 
        end 


    /* Choose if send an interrupting signal on different transitions */
    logic interrupt_level;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                /* 0 to 1 transition */
                interrupt_level <= 1'b1;
            end else if (enable_write[3]) begin 
                interrupt_level <= write_data_i;
            end 
        end 

    
//====================================================================================
//      PIN / DEBOUNCING LOGIC
//====================================================================================

    assign pin_io = direction ? 1'bZ : pin_value;

    /* Pin input synchronizer */
    synchronizer #(2, '0) pin_sync (
        .clk_i   ( clk_i   ),
        .rst_n_i ( rst_n_i ),

        .signal_i ( pin_io   ),
        .sync_o   ( sync_pin )
    );


    logic [4:0] tick_20ms;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                tick_20ms <= '0;
            end else begin 
                /* If direction is input and the pin value matches the logic value to interrupt */
                if ((interrupt_level == sync_pin) & direction) begin
                    if (debounce_i) begin
                        tick_20ms <= tick_20ms + 1'b1;
                    end
                end else begin
                    tick_20ms <= '0;
                end
            end 
        end 

    assign debounced = tick_20ms == '1;

    assign interrupt_o = debounced & interrupt_enable;


//====================================================================================
//      OUTPUT LOGIC
//====================================================================================

        always_comb begin
            case (read_address_i)
                'd0: read_data_o = pin_value;

                'd1: read_data_o = direction;

                'd2: read_data_o = interrupt_enable;

                'd3: read_data_o = interrupt_level;
            endcase 
        end

endmodule : gpio 

`endif 