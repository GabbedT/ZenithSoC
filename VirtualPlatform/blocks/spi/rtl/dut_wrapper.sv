`include "IO/SPI/spi.sv"

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

    // SPI external pins: loopback MOSI -> MISO
    logic spi_sclk, spi_mosi, spi_miso;
    logic [0:0] spi_cs_n;
    assign spi_miso = spi_mosi;

    spi #(
        .RX_BUFFER_SIZE ( 512 ),
        .TX_BUFFER_SIZE ( 512 ),
        .SLAVES         ( 1   )
    ) u_spi (
        .clk_i       ( clk   ),
        .rst_n_i     ( rst_n ),
        .interrupt_o ( interrupt_o ),

        .sclk_o ( spi_sclk ),
        .cs_n_o ( spi_cs_n ),
        .mosi_o ( spi_mosi ),
        .miso_i ( spi_miso ),

        .write_i         ( write_i                ),
        .write_address_i ( write_address_i[2:0]   ),
        .write_data_i    ( write_data_i           ),
        .write_strobe_i  ( write_strobe_i         ),
        .write_error_o   ( write_error_o          ),
        .write_done_o    ( write_done_o           ),

        .read_i         ( read_i                ),
        .read_address_i ( read_address_i[2:0]   ),
        .read_data_o    ( read_data_o           ),
        .read_error_o   ( read_error_o          ),
        .read_done_o    ( read_done_o           )
    );

endmodule
