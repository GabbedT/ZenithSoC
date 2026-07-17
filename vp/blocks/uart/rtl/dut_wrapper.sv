module dut_wrapper (
    input  logic        clk,
    input  logic        rst_n,

    input  logic        write_i,
    input  logic [31:0] write_address_i,
    input  logic [31:0] write_data_i,
    input  logic [3:0]  write_strobe_i,
    output logic        write_done_o,
    output logic        write_error_o,

    input  logic        read_i,
    input  logic [31:0] read_address_i,
    output logic [31:0] read_data_o,
    output logic        read_done_o,
    output logic        read_error_o,

    output logic        interrupt_o
);

    // UART external pins: loopback for self-testing
    logic uart_tx, uart_rx, uart_rts, uart_cts;
    assign uart_rx = uart_tx;
    assign uart_cts = uart_rts;

    uart #(
        .RX_BUFFER_SIZE ( 512 ),
        .TX_BUFFER_SIZE ( 512 )
    ) u_uart (
        .clk_i       ( clk   ),
        .rst_n_i     ( rst_n ),
        .interrupt_o ( interrupt_o ),

        .uart_rx_i  ( uart_rx  ),
        .uart_tx_o  ( uart_tx  ),
        .uart_rts_o ( uart_rts ),
        .uart_cts_i ( uart_cts ),

        .write_i         ( write_i                   ),
        .write_address_i ( write_address_i[1:0]      ),
        .write_data_i    ( write_data_i              ),
        .write_strobe_i  ( write_strobe_i            ),
        .write_error_o   ( write_error_o             ),
        .write_done_o    ( write_done_o              ),

        .read_i         ( read_i                ),
        .read_address_i ( read_address_i[1:0]   ),
        .read_data_o    ( read_data_o           ),
        .read_error_o   ( read_error_o          ),
        .read_done_o    ( read_done_o           )
    );

endmodule
