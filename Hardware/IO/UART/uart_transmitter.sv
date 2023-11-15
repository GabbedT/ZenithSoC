`ifndef UART_TRANSMITTER_SV
    `define UART_TRANSMITTER_SV

`include "../../Utility/Packages/uart_pkg.sv"

module uart_transmitter (
    /* Global signals */
    input logic clk_i,
    input logic rst_n_i,
    input logic enable_i,
    input logic sample_i,

    /* UART interface */
    output logic uart_tx_o,

    /* System interface */
    input logic transmit_i,
    input logic [7:0] tx_data_i,
    output logic tx_done_o,
    output logic tx_idle_o,

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

    /* Save data to transmit */
    logic [7:0] tx_data; logic shift_data, load_data;

        always_ff @(posedge clk_i) begin
            if (load_data) begin
                tx_data <= tx_data_i;
            end else if (shift_data) begin
                /* Shift data every bit sent */
                tx_data <= tx_data >> 1'b1;
            end
        end 

    /* Update parity bit every time a new bit is transmitted */
    logic parity_bit, update_parity;

        always_ff @(posedge clk_i) begin
            if (transmit_i) begin
                /* EVEN parity starts with 0, ODD with 1 */
                parity_bit <= parity_mode_i;
            end else if (update_parity) begin
                /* XOR the bits sent together */
                parity_bit <= parity_bit ^ tx_data[0];
            end
        end 

    /* Number of data bits sent */
    logic [2:0] bit_count, stop_bit_CRT, stop_bit_NXT; logic reset_bit_count;

        always_ff @(posedge clk_i) begin
            if (transmit_i) begin
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
            tx_idle_o = 1'b0;
            uart_tx_o = 1'b1;
            tx_done_o = 1'b0;
            load_data = 1'b0;
            shift_data = 1'b0;
            update_parity = 1'b0;
            counter_reset = 1'b0;
            counter_enable = 1'b1;
            reset_bit_count = 1'b0;

            state_NXT = state_CRT;
            stop_bit_NXT = stop_bit_CRT;

            case (state_CRT)
                IDLE: begin
                    counter_enable = 1'b0;
                    tx_idle_o = 1'b1;

                    if (transmit_i & enable_i) begin
                        state_NXT = START;

                        counter_reset = 1'b1;
                        reset_bit_count = 1'b1;
                        
                        tx_idle_o = 1'b1;
                    end
                end

                START: begin
                    /* Lower TX to start transaction */
                    uart_tx_o = 1'b0;

                    if (bit_sent & sample_i) begin
                        state_NXT = DATA;

                        counter_reset = 1'b1;
                        load_data = 1'b1;
                    end
                end

                DATA: begin
                    uart_tx_o = tx_data[0];

                    if (bit_sent & sample_i) begin
                        /* Update registers once the sampling signal arrives
                         * to avoid error caused the difference of frequency 
                         * between the clock and the baud sampling signal */
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
                    /* Lower TX to start transaction */
                    uart_tx_o = parity_bit;

                    if (bit_sent & sample_i) begin
                        state_NXT = STOP;

                        counter_reset = 1'b1;
                    end
                end

                STOP: begin
                    uart_tx_o = 1'b1;

                    case (stop_bits_i)
                        STOP1: begin 
                            if (bit_sent & sample_i) begin 
                                state_NXT = IDLE;

                                tx_done_o = 1'b1;

                                counter_reset = 1'b1;
                            end 
                        end

                        STOP2: begin 
                            if (bit_sent & sample_i) begin
                                stop_bit_NXT = !stop_bit_CRT;

                                if (stop_bit_CRT) begin
                                    state_NXT = IDLE;

                                    tx_done_o = 1'b1;

                                    counter_reset = 1'b1;
                                end
                            end
                        end
                    endcase 
                end
            endcase 
        end

endmodule : uart_transmitter

`endif 