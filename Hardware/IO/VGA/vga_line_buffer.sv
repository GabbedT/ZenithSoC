`ifndef VGA_LINE_BUFFER_SV
    `define VGA_LINE_BUFFER_SV

`include "../../Utility/Packages/vga_pkg.sv"

module vga_line_buffer #(
    parameter SIZE = 2**16
) (
    input logic clk_i,
    input logic rst_n_i,
    input logic enable_video_i,

    /* Write interface */
    input logic write_i,
    input logic [$clog2(SIZE) - 1:0] address_i,
    input pixel_t pixel_i,

    /* Read interface */
    input logic read_i,
    output pixel_t pixel_o,

    /* Status */
    output logic [$clog2(SIZE) - 1:0] size_o,
    output logic empty_o
);

//====================================================================================
//      MEMORY
//====================================================================================

    /* Memory */
    logic [$bits(pixel_t) - 1:0] buffer [SIZE - 1:0];

    /* Addresses handled by internal logic */
    logic [$clog2(SIZE) - 1:0] read_address;

        
        /* Write port */
        always_ff @(posedge clk_i) begin
            if (write_i) begin
                buffer[address_i] <= pixel_i;
            end
        end 

        /* Read port */
        always_ff @(posedge clk_i) begin
            if (read_i & !empty_o) begin
                pixel_o <= buffer[read_address];
            end
        end 


        /* Read address logic */
        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                read_address <= '0;
            end else if (!enable_video_i) begin
                read_address <= '0;
            end else begin 
                if (read_i) begin
                    read_address <= read_address + 1'b1;
                end
            end 
        end 

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                size_o <= '0;
            end else begin 
                case ({write_i, read_i})
                    2'b10: size_o <= size_o + 1'b1;

                    2'b01: size_o <= size_o - 1'b1;
                endcase 
            end 
        end 

    assign empty_o = size_o == '0;

endmodule 

`endif