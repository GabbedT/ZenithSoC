`ifndef SPI_RECEIVER_SV
    `define SPI_RECEIVER_SV

`include "../../Utility/Packages/spi_pkg.sv"

module spi_receiver (
    /* Global Signals */
    input logic clk_i,

    /* External command and data */
    input logic sample_i,
    output logic [7:0] data_o,

    /* SPI Data in */
    input logic spi_miso_i,

    /* Configuration */
    input bit_order_t bit_order_i
);

        always_ff @(posedge clk_i) begin
            if (sample_i) begin
                if (bit_order_i == MSB_FIRST) begin
                    data_o <= {data_o[6:0], spi_miso_i};
                end else begin
                    data_o <= {spi_miso_i, data_o[7:1]};
                end
            end
        end 

endmodule : spi_receiver

`endif 