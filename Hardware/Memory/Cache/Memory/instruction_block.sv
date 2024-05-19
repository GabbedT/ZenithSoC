`ifndef INSTRUCTION_BLOCK_SV
    `define INSTRUCTION_BLOCK_SV

`include "block_bank.sv"

module instruction_block #(
    /* Cache address */
    parameter ADDR_WIDTH = 32,

    /* Bank number */
    parameter BANK_ADDRESS = 4
) (
    input logic clk_i,

    /* Write port */
    input logic [ADDR_WIDTH - 1:0] write_address_i,
    input logic write_i,
    input logic [(2 ** BANK_ADDRESS) - 1:0][31:0] instruction_i,

    /* Read port */
    input logic [ADDR_WIDTH - 1:0] read_address_i,
    input logic read_i,
    output logic [(2 ** BANK_ADDRESS) - 1:0][31:0] instruction_o
);

//====================================================================================
//      MEMORY
//====================================================================================

    genvar i;

    /* Generate N chip of 32 bit wide to match the block width */
    generate
        for (i = 0; i < 2 ** BANK_ADDRESS; ++i) begin
            block_bank #(ADDR_WIDTH, 0) cache_block_bank (
                .clk_i ( clk_i ),

                /* Port 0 (W) interface */
                .byte_write_i    ( '1               ),
                .write_address_i ( write_address_i  ),
                .data_i          ( instruction_i[i] ),
                .write_i         ( write_i          ),

                /* Port 1 (R) interface */
                .read_address_i ( read_address_i   ),
                .read_i         ( read_i           ),
                .data_o         ( instruction_o[i] ) 
            );
        end
    endgenerate

endmodule : instruction_block 

`endif 