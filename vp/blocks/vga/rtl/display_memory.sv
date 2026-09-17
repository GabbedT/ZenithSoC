`timescale 1ns/1ns

module display_memory #(
    parameter logic [31:0] BASE_ADDRESS = 32'h08000000,
    parameter int unsigned WIDTH = 640,
    parameter int unsigned HEIGHT = 480
) (
    input logic clk_i,
    input logic rst_n_i,

    /* Logical display monitor interface */
    input logic        display_write_i,
    input logic [11:0] display_pixel_i,

    /* CPU interface */
    input logic        cpu_write_i,
    input logic [31:0] cpu_write_word_address_i,
    input logic [31:0] cpu_write_data_i,
    input logic [3:0]  cpu_write_strobe_i,
    output logic       cpu_write_done_o,
    output logic       cpu_write_error_o,

    input logic        cpu_read_i,
    input logic [31:0] cpu_read_word_address_i,
    output logic [31:0] cpu_read_data_o,
    output logic        cpu_read_done_o,
    output logic        cpu_read_error_o
);

    localparam int unsigned PIXEL_COUNT = WIDTH * HEIGHT;
    localparam logic [31:0] DEPTH_BYTES = PIXEL_COUNT * 2;
    localparam logic [31:0] END_ADDRESS = BASE_ADDRESS + DEPTH_BYTES;
    localparam int unsigned PIXEL_ADDRESS_WIDTH = (PIXEL_COUNT > 1) ? $clog2(PIXEL_COUNT) : 1;
    localparam logic [PIXEL_ADDRESS_WIDTH - 1:0] LAST_PIXEL_ADDRESS = PIXEL_ADDRESS_WIDTH'(PIXEL_COUNT - 1);

    /* Each display pixel is stored as 0000_RRRR_GGGG_BBBB. */
    logic [15:0] memory [0:PIXEL_COUNT - 1];
    logic [PIXEL_ADDRESS_WIDTH - 1:0] display_write_address;

    logic [31:0] cpu_write_byte_address;
    logic [31:0] cpu_read_byte_address;
    logic cpu_read_in_range;

    assign cpu_write_byte_address = cpu_write_word_address_i << 2;
    assign cpu_read_byte_address = cpu_read_word_address_i << 2;

    assign cpu_read_in_range = (cpu_read_byte_address >= BASE_ADDRESS) &&
                               (cpu_read_byte_address < END_ADDRESS);

    /* The display is read-only from the CPU's point of view. */
    assign cpu_write_done_o = cpu_write_i;
    assign cpu_write_error_o = cpu_write_i;
    assign cpu_read_done_o = cpu_read_i;
    assign cpu_read_error_o = cpu_read_i && !cpu_read_in_range;

    always_ff @(posedge clk_i) begin
        if (!rst_n_i) begin
            display_write_address <= '0;
        end else if (display_write_i) begin
            memory[display_write_address] <= {4'b0, display_pixel_i};

            if (display_write_address == LAST_PIXEL_ADDRESS) begin
                display_write_address <= '0;
            end else begin
                display_write_address <= display_write_address + 1'b1;
            end
        end
    end

    function automatic logic [7:0] read_byte(input logic [31:0] address);
        int unsigned byte_offset;
        begin
            read_byte = '0;

            if ((address >= BASE_ADDRESS) && (address < END_ADDRESS)) begin
                byte_offset = address - BASE_ADDRESS;
                read_byte = memory[byte_offset >> 1][8 * (byte_offset & 1) +: 8];
            end
        end
    endfunction

    always_comb begin
        cpu_read_data_o = '0;

        if (cpu_read_in_range) begin
            for (int lane = 0; lane < 4; ++lane) begin
                cpu_read_data_o[8 * lane +: 8] =
                    read_byte(cpu_read_byte_address + lane);
            end
        end
    end

endmodule
