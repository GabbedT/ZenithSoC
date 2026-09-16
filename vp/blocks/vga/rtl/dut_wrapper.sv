`timescale 1ns/1ns

module dut_wrapper (
    input  logic        clk,
    input  logic        rst_n,

    input  logic        write_i,
    input  logic [31:0] write_address_i,
    input  logic [31:0] write_data_i,
    input  logic [3:0]  write_strobe_i,
    output logic        write_done_o,
    output logic        write_error_o,

    input  logic        read_i,
    input  logic [31:0] read_address_i,
    output logic [31:0] read_data_o,
    output logic        read_done_o,
    output logic        read_error_o,

    output logic        interrupt_o
);

    localparam logic [31:0] FRAMEBUFFER_BASE = 32'h01000000;
    localparam logic [31:0] FRAMEBUFFER_END  = FRAMEBUFFER_BASE + 32'h000E1000;
    localparam logic [31:0] DISPLAY_BASE     = 32'h08000000;
    localparam logic [31:0] DISPLAY_END      = DISPLAY_BASE + 32'h00096000;

    logic framebuffer_selected_write;
    logic framebuffer_selected_read;
    logic display_selected_write;
    logic display_selected_read;
    logic [31:0] framebuffer_read_data;
    logic [31:0] display_read_data;
    logic framebuffer_write_error;
    logic framebuffer_read_error;
    logic framebuffer_write_done;
    logic framebuffer_read_done;
    logic display_write_error;
    logic display_read_error;
    logic display_write_done;
    logic display_read_done;

    dev2ddr_interface ddr_channel();

    logic vga_write_error;
    logic vga_read_error;
    logic [31:0] vga_read_data;
    logic hsync;
    logic vsync;
    logic [3:0] red;
    logic [3:0] green;
    logic [3:0] blue;

    /* Feed the final VGA pixel stream into the simulated display memory. */
    wire display_write = u_vga.controller_enable &&
                         u_vga.pixel_pulse &&
                         u_vga.video_on;
    wire [11:0] display_pixel = {
        u_vga.pixel_out.red,
        u_vga.pixel_out.green,
        u_vga.pixel_out.blue
    };

    assign framebuffer_selected_write = write_i &&
                                    ((write_address_i << 2) >= FRAMEBUFFER_BASE) &&
                                    ((write_address_i << 2) < FRAMEBUFFER_END);

    assign framebuffer_selected_read = read_i &&
                                    ((read_address_i << 2) >= FRAMEBUFFER_BASE) &&
                                    ((read_address_i << 2) < FRAMEBUFFER_END);

    assign display_selected_write = write_i &&
                                    ((write_address_i << 2) >= DISPLAY_BASE) &&
                                    ((write_address_i << 2) < DISPLAY_END);

    assign display_selected_read = read_i &&
                                    ((read_address_i << 2) >= DISPLAY_BASE) &&
                                    ((read_address_i << 2) < DISPLAY_END);

    vga u_vga (
        .clk_i       ( clk ),
        .rst_n_i     ( rst_n ),
        .interrupt_o ( interrupt_o ),

        .write_i         ( write_i && !framebuffer_selected_write && !display_selected_write ),
        .write_address_i ( write_address_i[6:0]                                              ),
        .write_data_i    ( write_data_i                                                      ),
        .write_strobe_i  ( write_strobe_i                                                    ),
        .write_error_o   ( vga_write_error                                                   ),
        .write_done_o    (                                                                   ),

        .read_i          ( read_i && !framebuffer_selected_read && !display_selected_read ),
        .read_address_i  ( read_address_i[6:0] ),
        .read_data_o     ( vga_read_data ),
        .read_error_o    ( vga_read_error ),
        .read_done_o     ( ),

        .hsync_o ( hsync ),
        .vsync_o ( vsync ),
        .red_o   ( red ),
        .green_o ( green ),
        .blue_o  ( blue ),

        .ddr_channel ( ddr_channel )
    );

    framebuffer_memory u_framebuffer (
        .clk_i ( clk ),
        .rst_n_i ( rst_n ),

        .cpu_write_i               ( framebuffer_selected_write ),
        .cpu_write_word_address_i  ( write_address_i ),
        .cpu_write_data_i          ( write_data_i ),
        .cpu_write_strobe_i        ( write_strobe_i ),
        .cpu_write_done_o          ( framebuffer_write_done ),
        .cpu_write_error_o         ( framebuffer_write_error ),

        .cpu_read_i                ( framebuffer_selected_read ),
        .cpu_read_word_address_i   ( read_address_i ),
        .cpu_read_data_o           ( framebuffer_read_data ),
        .cpu_read_done_o           ( framebuffer_read_done ),
        .cpu_read_error_o          ( framebuffer_read_error ),

        .ddr_channel ( ddr_channel )
    );

    display_memory u_display (
        .clk_i ( clk ),
        .rst_n_i ( rst_n ),

        .display_write_i ( display_write ),
        .display_pixel_i ( display_pixel ),

        .cpu_write_i              ( display_selected_write ),
        .cpu_write_word_address_i ( write_address_i ),
        .cpu_write_data_i         ( write_data_i ),
        .cpu_write_strobe_i       ( write_strobe_i ),
        .cpu_write_done_o         ( display_write_done ),
        .cpu_write_error_o        ( display_write_error ),

        .cpu_read_i              ( display_selected_read ),
        .cpu_read_word_address_i ( read_address_i ),
        .cpu_read_data_o         ( display_read_data ),
        .cpu_read_done_o         ( display_read_done ),
        .cpu_read_error_o        ( display_read_error )
    );

    always_comb begin
        write_done_o = write_i;
        read_done_o = read_i;
        write_error_o = display_selected_write ? display_write_error :
                        framebuffer_selected_write ? framebuffer_write_error :
                        vga_write_error;
        read_error_o = display_selected_read ? display_read_error :
                       framebuffer_selected_read ? framebuffer_read_error :
                       vga_read_error;
        read_data_o = display_selected_read ? display_read_data :
                      framebuffer_selected_read ? framebuffer_read_data :
                      vga_read_data;
    end

endmodule
