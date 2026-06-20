`ifndef BLOCK_BANK_SV
    `define BLOCK_BANK_SV

module block_bank #(
    /* Cache address */
    parameter ADDR_WIDTH = 32,

    /* Enable byte write */
    parameter BYTE_WRITE = 1
) (
    input logic clk_i,

    /* Write port */
    input logic [3:0] byte_write_i,
    input logic [ADDR_WIDTH - 1:0] write_address_i,
    input logic [31:0] data_i,
    input logic write_i,

    /* Read port */
    input logic [ADDR_WIDTH - 1:0] read_address_i,
    input logic read_i,  
    output logic [31:0] data_o
);

//====================================================================================
//      MEMORY
//====================================================================================

    /* How many cache lines */
    localparam CACHE_DEPTH = 2 ** ADDR_WIDTH;

    logic [31:0] bank_memory [CACHE_DEPTH - 1:0];

    initial begin
        for (int i = 0; i < CACHE_DEPTH; ++i) begin
            bank_memory[i] = '0;
        end
    end

        always_ff @(posedge clk_i) begin : bank_write_port
            if (write_i) begin 
                if (BYTE_WRITE) begin 
                    for (int i = 0; i < 4; ++i) begin : byte_write_logic
                        if (byte_write_i[i]) begin
                            bank_memory[write_address_i][(i * 8)+:8] <= data_i[(i * 8)+:8];
                        end
                    end : byte_write_logic
                end else begin
                    bank_memory[write_address_i] <= data_i;
                end
            end
        end : bank_write_port

        always_ff @(posedge clk_i) begin : bank_read_port
            if (read_i) begin 
                data_o <= bank_memory[read_address_i];
            end 
        end : bank_read_port

endmodule : block_bank

`endif 