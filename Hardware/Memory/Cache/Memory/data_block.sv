`ifndef DATA_BLOCK_SV
    `define DATA_BLOCK_SV

`include "block_bank.sv"

module data_block #(
    /* Cache address */
    parameter ADDR_WIDTH = 32,

    /* Bank address width */
    parameter BANK_ADDRESS = 4
) (
    input logic clk_i,

    /* Write port */
    input logic [BANK_ADDRESS - 1:0] write_bank_i,
    input logic [ADDR_WIDTH - 1:0] write_address_i,
    input logic [3:0] byte_write_i,
    input logic write_i,
    input logic [31:0] data_i,

    /* Read port */
    input logic [BANK_ADDRESS - 1:0] read_bank_i,
    input logic [ADDR_WIDTH - 1:0] read_address_i,
    input logic read_i,
    output logic [31:0] data_o
);

//====================================================================================
//      DECODE LOGIC
//====================================================================================

    localparam BANK_NUMBER = 2 ** BANK_ADDRESS;

    logic [BANK_NUMBER - 1:0] write_enable, read_enable;

    assign write_enable = 1'b1 << write_bank_i;
    assign read_enable = 1'b1 << read_bank_i;


//====================================================================================
//      MEMORY
//====================================================================================

    /* Memory chip output data */
    logic [BANK_NUMBER - 1:0][31:0] read_data;

    genvar i;

    /* Generate N chip of 32 bit wide to match the block width */
    generate
        for (i = 0; i < BANK_NUMBER; ++i) begin
            block_bank #(ADDR_WIDTH, 1) cache_block_bank (
                .clk_i ( clk_i ),

                /* Port 0 (W) interface */
                .byte_write_i    ( byte_write_i              ),
                .write_address_i ( write_address_i           ),
                .data_i          ( data_i                    ),
                .write_i         ( write_i & write_enable[i] ),

                /* Port 1 (R) interface */
                .read_address_i ( read_address_i          ),
                .read_i         ( read_i & read_enable[i] ),
                .data_o         ( read_data[i]            ) 
            );
        end
    endgenerate

    /* Since data arrives after 1 clock cycle, the active chip address needs to be stored */
    logic [$clog2(BANK_NUMBER) - 1:0] data_select;

        always_ff @(posedge clk_i) begin
            data_select <= read_bank_i;
        end

    /* Output assignment */
    assign data_o = read_data[data_select];

endmodule : data_block

`endif 