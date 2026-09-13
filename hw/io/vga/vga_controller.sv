`ifndef VGA_CONTROLLER_SV
    `define VGA_CONTROLLER_SV

module vga_controller (
    input logic clk_i,
    input logic rst_n_i,

    input logic display_i,
    input logic next_pixel_i,
    output logic video_on_o,
    output logic frame_done_o,
    output logic [8:0] vsync_counter_o,

    /* Sprite interface */
    input logic [9:0] sprite_x_i,
    input logic [9:0] sprite_y_i,
    input logic [11:0] sprite_pixel_i,
    input logic transparent_i,
    input logic enabled_i,
    output logic read_sprite_o,

    /* Image pixel interface */ 
    input pixel_t pixel_i, 
    output logic read_pixel_o,

    /* VGA interface */
    output logic hsync_o,
    output logic vsync_o,
    output pixel_t pixel_o
);

//====================================================================================
//      PARAMETERS
//====================================================================================

    /* 640x480 display size */
    localparam H_DISPLAY_SIZE = 640;
    localparam V_DISPLAY_SIZE = 480;

    /* Horizontal Scan */
    localparam H_FRONT_PORCH = 16;
    localparam H_BACK_PORCH = 48;
    localparam H_RETRACE = 96;

    /* Number of total pixel in an horizontal scan */
    localparam H_SCAN_PIXEL = H_DISPLAY_SIZE + H_FRONT_PORCH + H_BACK_PORCH + H_RETRACE;

    /* Vertical Scan */
    localparam V_FRONT_PORCH = 10;
    localparam V_BACK_PORCH = 33;
    localparam V_RETRACE = 2;

    /* Number of total pixel in a vertical scan */
    localparam V_SCAN_PIXEL = V_DISPLAY_SIZE + V_FRONT_PORCH + V_BACK_PORCH + V_RETRACE;


//====================================================================================
//      COUNTERS
//====================================================================================

    logic [$clog2(H_SCAN_PIXEL_2) - 1:0] hsync;
    logic [$clog2(V_SCAN_PIXEL_2) - 1:0] vsync;

    assign vsync_counter_o = vsync;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                hsync <= H_SCAN_PIXEL - 1;
                vsync <= V_SCAN_PIXEL - 1;
            end else if (!display_i) begin 
                hsync <= H_SCAN_PIXEL - 1;
                vsync <= V_SCAN_PIXEL - 1;
            end else if (next_pixel_i) begin 
                hsync <= (hsync == H_SCAN_PIXEL - 1) ? '0 : (hsync + 1'b1);

                if (hsync == H_SCAN_PIXEL - 1) begin
                    /* VSYNC Counter is increased if horizontal counter is max */
                    vsync <= (vsync == V_SCAN_PIXEL - 1) ? '0 : (vsync + 1'b1);
                end
            end 
        end 


        /* Every pixel has beed drawn */
        assign frame_done_o = (vsync == V_SCAN_PIXEL - 1) & (hsync == H_SCAN_PIXEL - 1);

        /* Disable HSYNC during horizontal retrace */
        assign hsync_o = !((hsync >= H_DISPLAY_SIZE + H_FRONT_PORCH) & (hsync <= (H_DISPLAY_SIZE + H_FRONT_PORCH + H_RETRACE - 1)));

        /* Disable VSYNC during vertical retrace */
        assign vsync_o = !((vsync >= V_DISPLAY_SIZE + V_FRONT_PORCH) & (vsync <= (V_DISPLAY_SIZE + V_FRONT_PORCH + V_RETRACE - 1)));


//====================================================================================
//      SPRITE LOGIC
//====================================================================================
    
    logic x_match, y_match, sprite_on;
    pixel_t sprite_pixel, final_pixel;


    /* Sprite is 8x8 so the HSYNC and VSYNC counter must fall between the two boundaries */
    assign x_match = (hsync >= sprite_x_i) & (hsync <= (sprite_x_i + 8));
    assign y_match = (vsync >= sprite_y_i) & (vsync <= (sprite_y_i + 8));

    /* Matched sprite pixel position will require a read to the sprite table to advance pointer */
    assign read_sprite_o = x_match & y_match & next_pixel_i;

    assign sprite_on = !transparent_i & x_match & y_match & enabled_i;


    assign sprite_pixel = (sprite_on) ? sprite_pixel_i : '0;


//====================================================================================
//      PIXEL LOGIC
//====================================================================================

    assign video_on_o = (hsync <= (H_DISPLAY_SIZE - 1)) & (vsync <= (V_DISPLAY_SIZE - 1));

        always_comb begin
            pixel_o = '0;

            if (video_on_o) begin
                /* Multiplex between sprite or pixel buffer */
                if (sprite_on) begin
                    pixel_o = sprite_pixel;
                end else begin
                    pixel_o = pixel_i;
                end
            end
        end


        always_comb begin
            read_pixel_o = 1'b0;

            if (video_on_o & (hsync != H_DISPLAY_SIZE - 1)) begin
                read_pixel_o = next_pixel_i;
            end else begin
                if (((hsync == (H_SCAN_PIXEL - 1)) & (vsync < (V_DISPLAY_SIZE - 1))) | frame_done_o) begin
                    read_pixel_o = next_pixel_i;
                end
            end
        end

endmodule : vga_controller

`endif 