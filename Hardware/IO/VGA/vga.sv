`ifndef VGA_SV 
    `define VGA_SV

`include "../../Utility/Packages/vga_pkg.sv"

`include "vga_registers.sv"
`include "vga_line_buffer.sv"
`include "vga_controller.sv"

module vga #(
    parameter BUFFER_SIZE = 2**16
) (
    input logic clk_i,
    input logic rst_n_i,
    output logic interrupt_o,

    /* Write interface */
    input logic write_i,
    input logic [$clog2(BUFFER_SIZE):0] write_address_i,
    input logic [3:0][7:0] write_data_i,
    output logic write_error_o,
    output logic write_done_o,

    /* Read interface */
    input logic read_i,
    input logic [$clog2(BUFFER_SIZE):0] read_address_i,
    output logic [31:0] read_data_o,
    output logic read_error_o,
    output logic read_done_o,

    /* VGA interface */
    output logic hsync_o,
    output logic vsync_o,
    output logic [3:0] red_o,
    output logic [3:0] green_o,
    output logic [3:0] blue_o
);

    assign write_done_o = write_i;
    assign read_done_o = read_i;


//====================================================================================
//      REGISTERS
//====================================================================================

    logic enable_video, write_buffer, video_on, buffer_empty, frame_done; 
    logic [$clog2(BUFFER_SIZE) - 1:0] buffer_address, buffer_size;

    logic [9:0] sprite_x, sprite_y; logic sprite_enable, write_ptable, write_ctable;
    logic [11:0] sprite_data; logic [6:0] sprite_address; logic [8:0] vsync_counter;

    resolution_t resolution;
    pixel_t pixel;

    vga_registers #(BUFFER_SIZE) registers (
        .clk_i       ( clk_i ),
        .rst_n_i     ( rst_n_i ),
        .interrupt_o ( interrupt_o ),

        .enable_video_o ( enable_video ),
        .resolution_o   ( resolution ),

        .address_o      ( buffer_address ), 
        .write_buffer_o ( write_buffer   ),
        .pixel_o        ( pixel          ),

        .write_ctable_o   ( write_ctable   ),
        .write_ptable_o   ( write_ptable   ),
        .sprite_data_o    ( sprite_data    ),
        .sprite_address_o ( sprite_address ),
        .sprite_x_o       ( sprite_x       ),
        .sprite_y_o       ( sprite_y       ),
        .sprite_enable_o  ( sprite_enable  ),

        .vsync_counter_i ( vsync_counter ),
        .buffer_size_i   ( buffer_size   ),
        .video_on_i      ( video_on      ),
        .buffer_empty_i  ( buffer_empty  ),
        .frame_done_i    ( frame_done    ),

        .write_i         ( write_i         ),
        .write_address_i ( write_address_i ),
        .write_data_i    ( write_data_i    ),
        .write_error_o   ( write_error_o   ),

        .read_i         ( read_i         ),
        .read_address_i ( read_address_i ),
        .read_data_o    ( read_data_o    ),
        .read_error_o   ( read_error_o   )
    );


//====================================================================================
//      CLOCK DIVIDER
//====================================================================================

    logic [1:0] divider; logic pixel_pulse;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                divider <= '0;
            end else if (!enable_video) begin
                divider <= '0;
            end else begin 
                if (resolution == _640x480_) begin
                    divider <= divider + 1'b1;
                end else begin 
                    divider <= (divider == 1) ? '0 : (divider + 1'b1);
                end 
            end 
        end 

    /* 640x480 is 25MHz in 800x600 is 50MHz */
    assign pixel_pulse = (resolution == _640x480_) ? (divider == 3) : (divider == 1);


//====================================================================================
//      SPRITE MODULE
//====================================================================================

    logic read_sprite, transparent; 
    pixel_t sprite_pixel;

    vga_sprite sprite (
        .clk_i       ( clk_i ),
        .rst_n_i     ( rst_n_i ),

        /* Controller interface */
        .read_i        ( read_sprite  ),
        .pixel_o       ( sprite_pixel ),
        .transparent_o ( transparent  ),

        /* Register interface */
        .write_ptable_i ( write_ptable   ),
        .write_ctable_i ( write_ctable   ),
        .data_i         ( sprite_data    ),
        .address_i      ( sprite_address )
    );


//====================================================================================
//      LINE BUFFER
//====================================================================================

    logic read_line_buffer; 
    pixel_t buffer_pixel;

    vga_line_buffer #(BUFFER_SIZE) line_buffer (
        .clk_i   ( clk_i   ),
        .rst_n_i ( rst_n_i ),

        .enable_video_i ( enable_video ),
        .resolution_i   ( resolution   ),

        .write_i   ( write_buffer   ),
        .address_i ( buffer_address ),
        .pixel_i   ( pixel          ),

        .read_i  ( read_line_buffer ),
        .pixel_o ( buffer_pixel     ),

        .size_o  ( buffer_size  ),
        .empty_o ( buffer_empty )
    );


//====================================================================================
//      INTERFACE CONTROLLER
//====================================================================================

    pixel_t pixel_out; logic hsync, vsync;

    vga_controller controller (
        .clk_i   ( clk_i   ),
        .rst_n_i ( rst_n_i ),

        .resolution_i    ( resolution    ),
        .display_i       ( enable_video  ),
        .next_pixel_i    ( pixel_pulse   ),
        .video_on_o      ( video_on      ),
        .frame_done_o    ( frame_done    ),
        .vsync_counter_o ( vsync_counter ),

        .sprite_x_i     ( sprite_x      ),
        .sprite_y_i     ( sprite_y      ),
        .sprite_pixel_i ( sprite_pixel  ),
        .transparent_i  ( transparent   ),
        .enabled_i      ( sprite_enable ),
        .read_sprite_o  ( read_sprite   ),

        .pixel_i      ( buffer_pixel     ), 
        .read_pixel_o ( read_line_buffer ),

        .hsync_o ( hsync     ),
        .vsync_o ( vsync     ),
        .pixel_o ( pixel_out )
    );


        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            hsync_o <= hsync; 
            vsync_o <= vsync; 
        end 

    assign red_o = pixel_out.red;
    assign green_o = pixel_out.green;
    assign blue_o = pixel_out.blue;

endmodule : vga

`endif 