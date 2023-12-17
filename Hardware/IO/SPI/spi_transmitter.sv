`ifndef SPI_TRANSMITTER_SV
    `define SPI_TRANSMITTER_SV

`include "../../Utility/Packages/spi_pkg.sv"

`include "../../System/edge_detector.sv"

module spi_transmitter (
    /* Global Signals */
    input logic clk_i,
    input logic rst_n_i,

    /* External command and data */
    input logic transmit_i,
    input logic shift_i,
    input logic [7:0] data_i,
    
    /* SPI Data out */
    output logic spi_mosi_o,

    /* Configuration */
    input bit_order_t bit_order_i
);

    logic [7:0] data; logic transmit;

        always_ff @(posedge clk_i) begin
            if (transmit) begin
                data <= data_i;
            end else begin
                if (shift_i) begin
                    if (bit_order_i == MSB_FIRST) begin
                        /* MSB first */
                        data <= data << 1;
                    end else begin
                        /* LSB first */
                        data <= data >> 1;
                    end
                end
            end
        end 

        always_comb begin
            if (transmit) begin
                spi_mosi_o = (bit_order_i == MSB_FIRST) ? data_i[7] : data_i[0];
            end else begin
                spi_mosi_o = (bit_order_i == MSB_FIRST) ? data[7] : data[0];
            end
        end


    edge_detector #(1, 0) transmit_edge (
        .clk_i   ( clk_i  ),
        .rst_n_i ( rst_n_i ),

        .signal_i ( transmit_i ),
        .edge_o   ( transmit   )
    );

endmodule : spi_transmitter

`endif 