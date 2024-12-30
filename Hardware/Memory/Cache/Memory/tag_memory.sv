`ifndef TAG_MEMORY_SV
    `define TAG_MEMORY_SV

module tag_memory #(
    /* Cache address */
    parameter ADDR_WIDTH = 32, 

    /* Tag size for hit check */
    parameter TAG_SIZE = 32
) (
    input logic clk_i,

    /* Write port */ 
    input logic [ADDR_WIDTH - 1:0] read_write_address_i,
    input logic [TAG_SIZE - 1:0] write_tag_i,
    input logic write_i,

    /* Read ports */
    input logic [1:0] read_i,
    input logic [ADDR_WIDTH - 1:0] read_address_i,
    output logic [1:0][TAG_SIZE - 1:0] read_tag_o
);

//====================================================================================
//      MEMORY
//====================================================================================

    localparam CACHE_DEPTH = 2 ** ADDR_WIDTH;

    logic [TAG_SIZE - 1:0] memory [CACHE_DEPTH - 1:0];

    initial begin
        for (int i = 0; i < CACHE_DEPTH; ++i) begin
            memory[i] = '0;
        end
    end

        always_ff @(posedge clk_i) begin : read_write_port
            if (write_i) begin
                memory[read_write_address_i] <= write_tag_i;
            end else if (read_i[0]) begin
                read_tag_o[0] <= memory[read_write_address_i];
            end
        end : read_write_port

        always_ff @(posedge clk_i) begin : read_port
            if (read_i[1]) begin  
                read_tag_o[1] <= memory[read_address_i];
            end 
        end : read_port

endmodule : tag_memory

`endif 