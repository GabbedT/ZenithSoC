`ifndef UART_RECEIVER_SV
    `define UART_RECEIVER_SV

`include "../../Utility/Packages/uart_pkg.sv"

module uart_receiver (
    /* Global signals */
    input logic clk_i,
    input logic rst_n_i,
    input logic enable_i,
    input logic sample_i,

    /* UART interface */
    input logic uart_rx_i,

    /* System interface */
    output logic [7:0] rx_data_o,
    output logic rx_done_o,
    output logic rx_idle_o,
    output logic parity_error_o,

    /* Configuration */
    input uart_data_lenght_t data_lenght_i,
    input uart_stop_bits_t stop_bits_i,
    input uart_parity_mode_t parity_mode_i,
    input logic parity_enable_i 
);

//====================================================================================
//      WORKING REGISTERS
//====================================================================================

    /* Count 16 times a bit sample */
    logic [3:0] sampling_counter; logic counter_enable, counter_reset;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                sampling_counter <= '0;
            end else if (counter_reset) begin 
                sampling_counter <= '0;
            end else if (enable_i & sample_i) begin 
                if (counter_enable) begin
                    sampling_counter <= sampling_counter + 1'b1;
                end
            end 
        end 

    /* Save bits received */
    logic [7:0] rx_data; logic shift_data, start_receiving;

        always_ff @(posedge clk_i) begin
            if (start_receiving) begin
                rx_data <= '0;
            end else if (shift_data) begin
                /* Shift data every bit received */
                rx_data <= {uart_rx_i, rx_data[7:1]};
            end
        end 

        always_comb begin
            /* Default value */
            rx_data_o = '0;

            /* LSB comes first */
            case (data_lenght_i)
                DBIT5: rx_data_o = rx_data[7:3];
                DBIT6: rx_data_o = rx_data[7:2];
                DBIT7: rx_data_o = rx_data[7:1];
                DBIT8: rx_data_o = rx_data[7:0];
            endcase 
        end


    /* Update parity bit every time a new bit is transmitted and check parity correctness */
    logic parity_bit, parity_error, update_parity, check_parity;

        always_ff @(posedge clk_i) begin
            if (start_receiving) begin
                /* EVEN parity starts with 0, ODD with 1 */
                parity_bit <= parity_mode_i;
                parity_error <= 1'b0;
            end else begin 
                if (update_parity) begin
                    /* XOR the bits sent together */
                    parity_bit <= parity_bit ^ uart_rx_i;
                end 

                if (check_parity) begin
                    parity_error <= parity_bit != uart_rx_i;
                end
            end
        end

    /* Number of data bits sent */
    logic [2:0] bit_count, stop_bit_CRT, stop_bit_NXT; logic reset_bit_count;

        always_ff @(posedge clk_i) begin
            if (start_receiving) begin
                bit_count <= '0;
                stop_bit_CRT <= '0;
            end else begin 
                if (reset_bit_count) begin
                    bit_count <= '0;
                end else if (shift_data) begin
                    bit_count <= bit_count + 1'b1;
                end 

                stop_bit_CRT <= stop_bit_NXT;
            end
        end 


//====================================================================================
//      FSM LOGIC
//====================================================================================

    typedef enum logic [2:0] { IDLE, START, DATA, PARITY, STOP } uart_fsm_states_t;

    uart_fsm_states_t state_CRT, state_NXT;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                state_CRT <= IDLE;
            end else if (enable_i) begin 
                state_CRT <= state_NXT;
            end 
        end 

    logic bit_sent; assign bit_sent = (sampling_counter == '1);

        always_comb begin
            /* Default values */
            rx_idle_o = 1'b0;
            rx_done_o = 1'b0;
            shift_data = 1'b0;
            check_parity = 1'b0;
            update_parity = 1'b0;
            counter_reset = 1'b0;
            counter_enable = 1'b1;
            parity_error_o = 1'b0;
            start_receiving = 1'b0;
            reset_bit_count = 1'b0;

            state_NXT = state_CRT;
            stop_bit_NXT = stop_bit_CRT;

            case (state_CRT)

                IDLE: begin
                    counter_enable = 1'b0;

                    rx_idle_o = 1'b1;

                    /* Transmitter is initiating a transaction */
                    if (!uart_rx_i) begin
                        state_NXT = START;

                        counter_reset = 1'b1;
                        reset_bit_count = 1'b1;
                    end
                end

                START: begin
                    /* Recover from possible line error */
                    if (uart_rx_i) begin
                        state_NXT = IDLE;
                    end else begin
                        /* Sample at the middle of the bit only for the start bit */
                        if ((sampling_counter == 'd7) & sample_i) begin
                            state_NXT = DATA;

                            counter_reset = 1'b1;
                            start_receiving = 1'b1;
                        end
                    end
                end

                DATA: begin
                    if (bit_sent & sample_i) begin
                        /* Since the start bit was sampled at 7 ticks, if we count
                         * to 15 the FSM will be at the middle of the bit sent by 
                         * the transmitter */
                        shift_data = 1'b1;
                        update_parity = 1'b1;
                        counter_reset = 1'b1;

                        /* Choose when to end the data phase, skip parity bit if not enabled */
                        case (data_lenght_i)    
                            DBIT5: begin
                                if (bit_count == 'd4) begin
                                    state_NXT = parity_enable_i ? PARITY : STOP;
                                end
                            end

                            DBIT6: begin
                                if (bit_count == 'd5) begin
                                    state_NXT = parity_enable_i ? PARITY : STOP;
                                end
                            end

                            DBIT7: begin
                                if (bit_count == 'd6) begin
                                    state_NXT = parity_enable_i ? PARITY : STOP;
                                end
                            end

                            DBIT8: begin
                                if (bit_count == 'd7) begin
                                    state_NXT = parity_enable_i ? PARITY : STOP;
                                end
                            end
                        endcase 
                    end
                end

                PARITY: begin
                    if (bit_sent & sample_i) begin
                        state_NXT = STOP;

                        counter_reset = 1'b1;
                        check_parity = 1'b1;
                    end
                end

                STOP: begin
                    case (stop_bits_i)
                        STOP1: begin 
                            if (bit_sent & sample_i) begin 
                                state_NXT = IDLE;

                                rx_done_o = 1'b1;
                                parity_error_o = parity_error;

                                counter_reset = 1'b1;
                            end 
                        end

                        STOP2: begin 
                            if (bit_sent & sample_i) begin
                                stop_bit_NXT = !stop_bit_CRT;

                                if (stop_bit_CRT) begin
                                    state_NXT = IDLE;

                                    rx_done_o = 1'b1;
                                    parity_error_o = parity_error & parity_enable_i;

                                    counter_reset = 1'b1;
                                end
                            end
                        end
                    endcase 
                end

            endcase 
        end 

endmodule : uart_receiver

`endif 