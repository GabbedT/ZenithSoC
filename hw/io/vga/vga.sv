`ifndef VGA_SV 
    `define VGA_SV

module vga #(
    parameter BUFFER_SIZE = 2**9
) (
    input logic clk_i,
    input logic rst_n_i,
    output logic interrupt_o,

    /* Write interface */
    input logic write_i,
    input logic [6:0] write_address_i,
    input logic [3:0][7:0] write_data_i,
    input logic [3:0] write_strobe_i,
    output logic write_error_o,
    output logic write_done_o,

    /* Read interface */
    input logic read_i,
    input logic [6:0] read_address_i,
    output logic [31:0] read_data_o,
    output logic read_error_o,
    output logic read_done_o,

    /* VGA interface */
    output logic hsync_o,
    output logic vsync_o,
    output logic [3:0] red_o,
    output logic [3:0] green_o,
    output logic [3:0] blue_o,

    /* DDR interface */
    dev2ddr_interface.master ddr_channel
);

    assign write_done_o = write_i;
    assign read_done_o = read_i;


//====================================================================================
//      REGISTERS
//====================================================================================

    logic enable_video, video_on, buffer_empty, frame_done, early_frame_done;
    logic flush_sequencer, flush_line_buffer;

    logic [9:0] sprite_x, sprite_y; logic sprite_enable, write_ptable, write_ctable;
    logic [11:0] sprite_data; logic [6:0] sprite_address; logic [9:0] vsync_counter;
    logic ddr_error;
    logic [3:0] outstanding_count;
    logic [26:0] frame_buffer_base; logic [19:0] frame_buffer_size;

    resolution_t resolution;

    vga_registers registers (
        .clk_i       ( clk_i ),
        .rst_n_i     ( rst_n_i ),
        .interrupt_o ( interrupt_o ),

        .enable_video_o ( enable_video ),
        .resolution_o   ( resolution ),
        .frame_buffer_base_o ( frame_buffer_base ),
        .frame_buffer_size_o ( frame_buffer_size ),

        .write_ctable_o   ( write_ctable   ),
        .write_ptable_o   ( write_ptable   ),
        .sprite_data_o    ( sprite_data    ),
        .sprite_address_o ( sprite_address ),
        .sprite_x_o       ( sprite_x       ),
        .sprite_y_o       ( sprite_y       ),
        .sprite_enable_o  ( sprite_enable  ),

        .vsync_counter_i ( vsync_counter ),
        .video_on_i      ( video_on      ),
        .buffer_empty_i  ( buffer_empty  ),
        .frame_done_i    ( frame_done    ),
        .early_frame_done_i ( early_frame_done ),
        .ddr_req_error_i ( ddr_error     ),

        .write_i         ( write_i         ),
        .write_address_i ( write_address_i ),
        .write_data_i    ( write_data_i    ),
        .write_strobe_i  ( write_strobe_i  ),
        .write_error_o   ( write_error_o   ),

        .read_i         ( read_i         ),
        .read_address_i ( read_address_i ),
        .read_data_o    ( read_data_o    ),
        .read_error_o   ( read_error_o   )
    );


//====================================================================================
//      PIXEL SEQUENCER
//====================================================================================

    logic ddr_request, ddr_ready, ddr_valid;
    logic [26:0] ddr_address;
    logic [127:0] ddr_data;
    logic sequencer_write, controller_enable;
    pixel_t sequencer_pixel;

    vga_pixel_sequencer #(
        .PIXEL_WIDTH       ( 16          ),
        .PIXEL_BUFFER_SIZE ( BUFFER_SIZE )
    ) pixel_sequencer (
        .clk_i       ( clk_i       ),
        .rst_n_i     ( rst_n_i     ),
        .display_i   ( enable_video ),
        .flush_i     ( flush_sequencer ),

        .base_address_i ( frame_buffer_base ),
        .size_i         ( frame_buffer_size ),

        .full_i   ( buffer_full   ),

        .write_o ( sequencer_write ),
        .pixel_o ( sequencer_pixel  ),

        .ddr_ready_i ( ddr_ready ),
        .ddr_valid_i ( ddr_valid ),
        .ddr_data_i  ( ddr_data  ),

        .ddr_address_o ( ddr_address ),
        .ddr_read_o    ( ddr_request ),
        .outstanding_count_o ( outstanding_count )
    );

    ddr_master ddr_master (
        .clk_i   ( clk_i   ),
        .rst_n_i ( rst_n_i ),

        .request_i    ( ddr_request ),
        .operation_i  ( 1'b0        ),
        .address_i    ( ddr_address ),

        .write_strobe_i ( '0  ),
        .write_data_i   ( '0  ),

        .valid_o     ( ddr_valid ),
        .error_o     ( ddr_error ),
        .accepted_o  ( ddr_ready ),
        .read_data_o ( ddr_data  ),

        .dev2ddr_channel ( ddr_channel )
    );


//====================================================================================
//      DISPLAY STARTUP
//====================================================================================

        /* Do not start the controller until the buffer has some pixel */
        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin
                controller_enable <= 1'b0;
            end else if (!enable_video) begin
                controller_enable <= 1'b0;
            end else if (flush_sequencer || flush_line_buffer) begin
                controller_enable <= 1'b0;
            end else if (!controller_enable & !buffer_empty) begin
                controller_enable <= 1'b1;
            end
        end

//====================================================================================
//      CLOCK DIVIDER
//====================================================================================

    logic [1:0] divider; logic pixel_pulse;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                divider <= '0;
            end else if (!controller_enable) begin
                divider <= '0;
            end else begin 
                divider <= (divider == 3) ? '0 : (divider + 1'b1);
            end 
        end 

    /* 640x480 is 25MHz */
    assign pixel_pulse = divider == 3;


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

    logic read_line_buffer, buffer_full;
    pixel_t buffer_pixel;

    vga_line_buffer #(BUFFER_SIZE) line_buffer (
        .clk_i   ( clk_i   ),
        .rst_n_i ( rst_n_i ),

        .enable_video_i ( enable_video ),
        .flush_i       ( flush_line_buffer ),
        .resolution_i   ( resolution   ),

        .write_i   ( sequencer_write ),
        .pixel_i   ( sequencer_pixel  ),

        .read_i  ( read_line_buffer ),
        .pixel_o ( buffer_pixel     ),

        .full_o   ( buffer_full   ),
        .empty_o ( buffer_empty )
    );


//====================================================================================
//      FRAMEBUFFER FLUSH
//====================================================================================

    vga_flush_engine flush_engine (
        .clk_i       ( clk_i       ),
        .rst_n_i     ( rst_n_i     ),
        .enable_video_i ( enable_video ),
        .video_on_i  ( video_on    ),

        .frame_buffer_base_i ( frame_buffer_base ),
        .early_frame_done_i  ( early_frame_done  ),
        .outstanding_count_i ( outstanding_count ),

        .flush_sequencer_o   ( flush_sequencer   ),
        .flush_line_buffer_o ( flush_line_buffer )
    );


//====================================================================================
//      INTERFACE CONTROLLER
//====================================================================================

    pixel_t pixel_out; logic hsync, vsync;

    vga_controller controller (
        .clk_i   ( clk_i   ),
        .rst_n_i ( rst_n_i ),

        .display_i       ( controller_enable ),
        .next_pixel_i    ( pixel_pulse   ),
        .video_on_o      ( video_on      ),
        .frame_done_o    ( frame_done    ),
        .early_frame_done_o ( early_frame_done ),
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


//====================================================================================
//      ASSERTIONS
//====================================================================================

`ifndef SYNTHESIS

    assert property (@(posedge clk_i)
        disable iff (!rst_n_i)
        controller_enable |-> enable_video)
        else $error("VGA controller ran while video was disabled");

    assert property (@(posedge clk_i)
        disable iff (!rst_n_i)
        controller_enable |-> !buffer_empty)
        else $error("VGA controller started or continued with an empty line buffer");

    assert property (@(posedge clk_i)
        disable iff (!rst_n_i)
        (controller_enable && read_line_buffer) |-> !buffer_empty)
        else $error("VGA line buffer read was issued while empty");

`endif

endmodule : vga

`endif
