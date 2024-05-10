`ifndef VGA_SPRITE_SV
    `define VGA_SPRITE_SV

`include "../../Utility/Packages/vga_pkg.sv"

module vga_sprite (
    input logic clk_i,
    input logic rst_n_i,

    /* Controller interface */
    input logic read_i,
    output pixel_t pixel_o,
    output logic transparent_o,

    /* Register interface */
    input logic write_ptable_i,
    input logic write_ctable_i,
    input logic [11:0] data_i,
    input logic [6:0] address_i
);

//====================================================================================
//      COLOR TABLE
//====================================================================================

    /* Save the possible color to use inside the sprite to use less memory */
    logic [11:0] color_table [15:0];

    initial begin
        for (int i = 0; i < 16; ++i) begin
            color_table[i] = i * 256;
        end
    end
    
        always_ff @(posedge clk_i) begin
            if (address_i[6] & write_ctable_i) begin 
                color_table[address_i[3:0]] <= data_i;
            end 
        end 


//====================================================================================
//      PATTERN TABLE
//====================================================================================

    /* Each cell of the pattern table memorize an index for the color table */
    logic [3:0] pattern_table [63:0];

    initial begin
        for (int i = 0; i < 64; ++i) begin
            pattern_table[i] = i % 16;
        end
    end

        always_ff @(posedge clk_i) begin
            if (!address_i[6] & write_ptable_i) begin
                pattern_table[address_i[5:0]] <= data_i[3:0];
            end
        end 


//====================================================================================
//      OUTPUT LOGIC
//====================================================================================

    logic [5:0] read_pointer;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                read_pointer <= '0;
            end else if (read_i) begin 
                read_pointer <= read_pointer + 1'b1;
            end 
        end 

    /* Last color of the color table is the transparent color */
    assign transparent_o = pattern_table[read_pointer] == '1;

    /* Read pointer indexes the pattern table, the cell indexed will contain the address 
     * of the color inside the color table */
    assign pixel_o = color_table[pattern_table[read_pointer]];

endmodule : vga_sprite 

`endif 