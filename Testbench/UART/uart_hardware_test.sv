`ifndef UART_HARDWARE_TEST_SV
    `define UART_HARDWARE_TEST_SV

`include "../../Hardware/IO/UART/uart.sv"
`include "../../Hardware/System/synchronizer.sv"

`include "../../Hardware/Utility/Packages/uart_pkg.sv"

module uart_hardware_test (
    input logic clk_i,
    input logic rst_n_i,

    input logic rx_i,
    input logic cts_i,
    output logic rts_o,
    output logic tx_o
);

    /* Reset syncronizer */
    logic reset_n, rst_sync;

        always_ff @(posedge clk_i or negedge rst_n_i) begin 
            if (!rst_n_i) begin
                rst_sync <= 1'b0;
                reset_n <= 1'b0;
            end else begin 
                rst_sync <= rst_n_i;
                reset_n <= rst_sync;
            end 
        end 


    logic interrupt, uart_write, uart_read, write_error, read_error; logic [31:0] write_data, read_data;
    uart_registers_t write_address, read_address;
    logic write_done, read_done;

    uart #(512, 512) uart_core (
        .clk_i       ( clk_i     ),
        .rst_n_i     ( reset_n   ),
        .interrupt_o ( interrupt ),

        .uart_rx_i  ( rx_i  ),
        .uart_tx_o  ( tx_o  ),
        .uart_rts_o ( rts_o ),
        .uart_cts_i ( cts_i ),

        .write_i         ( uart_write    ),
        .write_address_i ( write_address ),
        .write_data_i    ( write_data    ),
        .write_error_o   ( write_error   ),
        .write_done_o    ( write_done    ),

        .read_i         ( uart_read    ),
        .read_address_i ( read_address ),
        .read_data_o    ( read_data    ),
        .read_error_o   ( read_error   ),
        .read_done_o    ( read_done    )
    );


    typedef enum logic [1:0] { RESET, WAIT, CLEAR, FOWARD } state_t;

    state_t state_CRT, state_NXT;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge reset_n `endif) begin
            if (!reset_n) begin 
                state_CRT <= RESET;
            end else begin 
                state_CRT <= state_NXT;
            end 
        end 


    uart_status_t status_register;

        always_comb begin
            uart_write = '0;
            uart_read = '0;
            write_data = '0;
            write_address = STATUS;
            read_address = RX_BUFFER;
            status_register = '0; 
            state_NXT = state_CRT;

            case (state_CRT)
                RESET: begin
                    uart_write = 1'b1;
                    write_address = STATUS;

                    /* 115200 Bauds */
                    status_register.clock_divider = 'd53;

                    /* Communication setup */
                    status_register.data_bits = DBIT8;
                    status_register.stop_bits = STOP1;
                    status_register.parity_mode = EVEN;
                    status_register.parity_enable = 1'b1;

                    /* Operation disabled on reset */
                    status_register.enable_TX = 1'b1;
                    status_register.enable_RX = 1'b1;

                    /* Enable RX interrupts */
                    status_register.interrupt_enable = 'd1;

                    write_data = status_register;

                    state_NXT = WAIT;
                end

                WAIT: begin
                    if (interrupt) begin
                        state_NXT = CLEAR;
                    end
                end

                CLEAR: begin
                    uart_write = 1'b1;
                    write_address = EVENT;
                    write_data = '0;

                    uart_read = 1'b1;
                    read_address = RX_BUFFER;

                    state_NXT = FOWARD;
                end

                FOWARD: begin
                    uart_write = 1'b1;
                    write_address = TX_BUFFER;
                    write_data = read_data;

                    state_NXT = WAIT;
                end
            endcase 
        end


endmodule : uart_hardware_test

`endif 