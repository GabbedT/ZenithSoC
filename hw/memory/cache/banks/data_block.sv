`ifndef DATA_BLOCK_SV
    `define DATA_BLOCK_SV

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
//      MEMORY
//====================================================================================

    localparam WORD_ADDRESS = ADDR_WIDTH + BANK_ADDRESS;
    localparam CACHE_DEPTH = 2 ** WORD_ADDRESS;

    logic [WORD_ADDRESS - 1:0] write_address, read_address;

    assign write_address = {write_address_i, write_bank_i};
    assign read_address = {read_address_i, read_bank_i};

    /* Full-depth byte lanes avoid a word-bank mux after the read ports. */
    generate
        for (genvar i = 0; i < 4; ++i) begin : byte_lane
            logic [7:0] bank_memory [CACHE_DEPTH - 1:0];

            initial begin
                for (int j = 0; j < CACHE_DEPTH; ++j) begin
                    bank_memory[j] = '0;
                end
            end

                always_ff @(posedge clk_i) begin : bank_write_port
                    if (write_i & byte_write_i[i]) begin
                        bank_memory[write_address] <= data_i[i * 8 +: 8];
                    end
                end : bank_write_port

                always_ff @(posedge clk_i) begin : bank_read_port
                    if (read_i) begin
                        data_o[i * 8 +: 8] <= bank_memory[read_address];
                    end
                end : bank_read_port
        end
    endgenerate

endmodule : data_block

`endif
