`ifndef TIMER_SV
    `define TIMER_SV

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

    /* PWM */
    output logic pwm_o,
    
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

    localparam PWM_TOGGLE_LOW = 4;
    localparam PWM_TOGGLE_HIGH = 5;

    localparam CONFIGURATION = 6;

    localparam INTERRUPT_CONFIG = 7;


    typedef enum logic [1:0] { FREE_RUNNING, ONE_SHOT, WRAP_AROUND } timer_mode_t;

    typedef struct packed {
        /* Timer working mode */
        timer_mode_t timer_mode;

        /* Enable PWM */
        logic pwm_enable;

        /* Enable timer to count */
        logic enable;
    } timer_config_t;

    typedef struct packed {
        /* Write 1 to Clear */
        logic interrupt_pending;

        /* Enable interrupt */
        logic interrupt_enable;
    } timer_irq_config_t;


    function bit enable_write(input logic [1:0] address, input logic [1:0] write_address);
        return (address == write_address) & write_i;
    endfunction : enable_write

    assign write_error_o = 1'b0;
    assign read_error_o = 1'b0;


    logic [7:0] enable_register;

    assign enable_register = write_i ? (1 << write_address_i) : '0;


//====================================================================================
//      COMPARE REGISTER
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

    
//====================================================================================
//      PWM TOGGLE
//====================================================================================

    /* Used to set a value when the timer should issue an interrupt */
    logic [1:0][3:0][7:0] pwm_toggle;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin : pwm_toggle_register_low
            if (!rst_n_i) begin
                pwm_toggle[0] <= '1;
            end else if (enable_register[PWM_TOGGLE_LOW]) begin
                for (int i = 0; i < 4; ++i) begin
                    if (write_strobe_i[i]) begin 
                        pwm_toggle[0][i] <= write_data[i];
                    end
                end
            end
        end : pwm_toggle_register_low

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin : pwm_toggle_register_high
            if (!rst_n_i) begin
                pwm_toggle[1] <= '1;
            end else if (enable_register[PWM_TOGGLE_HIGH]) begin
                for (int i = 0; i < 4; ++i) begin
                    if (write_strobe_i[i]) begin 
                        pwm_toggle[1][i] <= write_data[i];
                    end
                end
            end
        end : pwm_toggle_register_high


//====================================================================================
//      CONFIGURATION REGISTER
//====================================================================================

    /* Configuration register */
    timer_config_t configuration;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                configuration <= '0;
            end else if (enable_register[CONFIGURATION]) begin 
                configuration <= write_data_i[3:0];
            end 
        end 


    logic stop_timer, stopped, timer_match;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                stop_timer <= 1'b1;
            end else begin 
                if (enable_register[CONFIGURATION]) begin
                    stop_timer <= write_data_i[4];
                end else if (!configuration.enable) begin
                    stop_timer <= 1'b1;
                end else if ((configuration.timer_mode == ONE_SHOT) & timer_match) begin
                    stop_timer <= 1'b1;
                end
            end 
        end 


//====================================================================================
//      TIMER LOGIC
//====================================================================================

    logic [1:0][3:0][7:0] timer;

    assign timer_match = timer == timer_compare;


        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin : timer_register
            if (!rst_n_i) begin
                timer <= '0;
                stopped <= 1'b1;
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
            end else if (!stop_timer) begin
                case (configuration.timer_mode)
                    FREE_RUNNING: begin
                        timer <= timer + 1'b1;
                    end

                    WRAP_AROUND: begin
                        if (timer_match) begin
                            timer <= '0;
                        end else begin
                            timer <= timer + 1'b1;
                        end
                    end

                    ONE_SHOT: begin
                        if (!timer_match) begin
                            timer <= timer + 1'b1;
                        end else begin
                            stopped <= 1'b1;
                        end
                    end
                endcase

                stopped <= 1'b0;
            end 
        end : timer_register


//====================================================================================
//      INTERRUPT CONFIG REGISTER
//====================================================================================

    timer_irq_config_t irq_config;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin
                irq_config.interrupt_enable <= '0;
            end else if (enable_register[INTERRUPT_CONFIG]) begin
                if (write_strobe_i[0]) begin 
                    irq_config.interrupt_enable <= write_data[0][0];
                end
            end
        end

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin
                irq_config.interrupt_pending <= 1'b0;
            end else if (enable_register[INTERRUPT_CONFIG]) begin
                if (write_strobe_i[0] & write_data[0][1]) begin 
                    irq_config.interrupt_pending <= 1'b0;
                end
            end else if (timer_match & irq_config.interrupt_enable) begin
                irq_config.interrupt_pending <= 1'b1;
            end
        end


//====================================================================================
//      OUTPUT LOGIC
//====================================================================================

    assign interrupt_o = irq_config.interrupt_pending;


        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                pwm_o <= 1'b0;
            end else if (configuration.pwm_enable) begin 
                if (timer == pwm_toggle) begin
                    /* Toggle PWM pin */
                    pwm_o <= !pwm_o;
                end
            end else begin
                pwm_o <= 1'b0;
            end
        end 


        always_comb begin
            /* Default value */
            read_data_o = '0;
            
            case (read_address_i)
                COMPARE_LOW: read_data_o = timer_compare[0];
                COMPARE_HIGH: read_data_o = timer_compare[1];

                TIMER_VALUE_LOW: read_data_o = timer[0];
                TIMER_VALUE_HIGH: read_data_o = timer[1];

                CONFIGURATION: read_data_o = {stop_timer, configuration};

                PWM_TOGGLE_LOW:    read_data_o = pwm_toggle[0];
                PWM_TOGGLE_HIGH:   read_data_o = pwm_toggle[1];

                INTERRUPT_CONFIG:  read_data_o = irq_config; 
            endcase 
        end

endmodule : timer

`endif 