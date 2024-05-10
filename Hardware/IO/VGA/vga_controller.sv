`ifndef VGA_CONTROLLER_SV
    `define VGA_CONTROLLER_SV

`include "../../Utility/Packages/vga_pkg.sv"

module vga_controller (
    input logic clk_i,
    input logic rst_n_i,

    input resolution_t resolution_i,
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
    localparam H_DISPLAY_SIZE_1 = 640;
    localparam V_DISPLAY_SIZE_1 = 480;

    /* Horizontal Scan */
    localparam H_FRONT_PORCH_1 = 16;
    localparam H_BACK_PORCH_1 = 48;
    localparam H_RETRACE_1 = 96;

    /* Number of total pixel in an horizontal scan */
    localparam H_SCAN_PIXEL_1 = H_DISPLAY_SIZE_1 + H_FRONT_PORCH_1 + H_BACK_PORCH_1 + H_RETRACE_1;

    /* Vertical Scan */
    localparam V_FRONT_PORCH_1 = 10;
    localparam V_BACK_PORCH_1 = 33;
    localparam V_RETRACE_1 = 2;

    /* Number of total pixel in a vertical scan */
    localparam V_SCAN_PIXEL_1 = V_DISPLAY_SIZE_1 + V_FRONT_PORCH_1 + V_BACK_PORCH_1 + V_RETRACE_1;



    /* 800x600 display size */
    localparam H_DISPLAY_SIZE_2 = 800;
    localparam V_DISPLAY_SIZE_2 = 600;

    /* Horizontal Scan */
    localparam H_FRONT_PORCH_2 = 56;
    localparam H_BACK_PORCH_2 = 64;
    localparam H_RETRACE_2 = 120;

    /* Number of total pixel in an horizontal scan */
    localparam H_SCAN_PIXEL_2 = H_DISPLAY_SIZE_2 + H_FRONT_PORCH_2 + H_BACK_PORCH_2 + H_RETRACE_2;

    /* Vertical Scan */
    localparam V_FRONT_PORCH_2 = 37;
    localparam V_BACK_PORCH_2 = 23;
    localparam V_RETRACE_2 = 6;

    /* Number of total pixel in a vertical scan */
    localparam V_SCAN_PIXEL_2 = V_DISPLAY_SIZE_2 + V_FRONT_PORCH_2 + V_BACK_PORCH_2 + V_RETRACE_2;


//====================================================================================
//      COUNTERS
//====================================================================================

    logic [$clog2(H_SCAN_PIXEL_2) - 1:0] hsync;
    logic [$clog2(V_SCAN_PIXEL_2) - 1:0] vsync;

    assign vsync_counter_o = vsync;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                hsync <= H_SCAN_PIXEL_1 - 1;
                vsync <= V_SCAN_PIXEL_1 - 1;
            end else if (!display_i) begin 
                hsync <= H_SCAN_PIXEL_1 - 1;
                vsync <= V_SCAN_PIXEL_1 - 1;
            end else if (next_pixel_i) begin 
                case (resolution_i)
                    _640x480_: begin
                        hsync <= (hsync == H_SCAN_PIXEL_1 - 1) ? '0 : (hsync + 1'b1);

                        if (hsync == H_SCAN_PIXEL_1 - 1) begin
                            /* VSYNC Counter is increased if horizontal counter is max */
                            vsync <= (vsync == V_SCAN_PIXEL_1 - 1) ? '0 : (vsync + 1'b1);
                        end
                    end

                    _800x600_: begin
                        hsync <= (hsync == H_SCAN_PIXEL_2 - 1) ? '0 : (hsync + 1'b1);

                        if (hsync == H_SCAN_PIXEL_2 - 1) begin
                            /* VSYNC Counter is increased if horizontal counter is max */
                            vsync <= (vsync == V_SCAN_PIXEL_2 - 1) ? '0 : (vsync + 1'b1);
                        end
                    end
                endcase 
            end 
        end 


        always_comb begin
            case (resolution_i)
                _640x480_: frame_done_o = (vsync == V_SCAN_PIXEL_1 - 1) & (hsync == H_SCAN_PIXEL_1 - 1);

                _800x600_: frame_done_o = (vsync == V_SCAN_PIXEL_2 - 1) & (hsync == H_SCAN_PIXEL_2 - 1);
            endcase
        end


        always_comb begin
            /* Disable HSYNC during horizontal retrace */
            case (resolution_i)
                _640x480_: hsync_o = !((hsync >= H_DISPLAY_SIZE_1 + H_FRONT_PORCH_1) & (hsync <= (H_DISPLAY_SIZE_1 + H_FRONT_PORCH_1 + H_RETRACE_1 - 1)));

                _800x600_: hsync_o = !((hsync >= H_DISPLAY_SIZE_2 + H_FRONT_PORCH_2) & (hsync <= (H_DISPLAY_SIZE_2 + H_FRONT_PORCH_2 + H_RETRACE_2 - 1)));
            endcase 


            /* Disable VSYNC during vertical retrace */
            case (resolution_i)
                _640x480_: vsync_o = !((vsync >= V_DISPLAY_SIZE_1 + V_FRONT_PORCH_1) & (vsync <= (V_DISPLAY_SIZE_1 + V_FRONT_PORCH_1 + V_RETRACE_1 - 1)));

                _800x600_: vsync_o = !((vsync >= V_DISPLAY_SIZE_2 + V_FRONT_PORCH_2) & (vsync <= (V_DISPLAY_SIZE_2 + V_FRONT_PORCH_2 + V_RETRACE_2 - 1)));
            endcase 
        end


//====================================================================================
//      SPRITE LOGIC
//====================================================================================
    
    logic x_match, y_match, sprite_on;
    pixel_t sprite_pixel, final_pixel;


    /* Sprite is 8x8 so the HSYNC and VSYNC counter must fall between the two boundaries */
    assign x_match = (hsync >= sprite_x_i) & (hsync <= (sprite_x_i + 8));
    assign y_match = (vsync >= sprite_y_i) & (vsync <= (sprite_y_i + 8));

    assign read_sprite_o = x_match & y_match & next_pixel_i;

    assign sprite_on = !transparent_i & x_match & y_match & enabled_i;


    assign sprite_pixel = (sprite_on) ? sprite_pixel_i : '0;


//====================================================================================
//      PIXEL LOGIC
//====================================================================================

        always_comb begin
            case (resolution_i)
                _640x480_: video_on_o = (hsync <= (H_DISPLAY_SIZE_1 - 1)) & (vsync <= (V_DISPLAY_SIZE_1 - 1));

                _800x600_: video_on_o = (hsync <= (H_DISPLAY_SIZE_2 - 1)) & (vsync <= (V_DISPLAY_SIZE_2 - 1));
            endcase 
        end


        always_comb begin
            pixel_o = '0;

            if (video_on_o) begin
                if (sprite_on) begin
                    pixel_o = sprite_pixel;
                end else begin
                    pixel_o = pixel_i;
                end
            end
        end


        always_comb begin
            read_pixel_o = 1'b0;

            case (resolution_i)
                _640x480_: begin
                    if (video_on_o & (hsync != H_DISPLAY_SIZE_1 - 1)) begin
                        read_pixel_o = next_pixel_i;
                    end else begin
                        if (((hsync == (H_SCAN_PIXEL_1 - 1)) & (vsync < (V_DISPLAY_SIZE_1 - 1))) | frame_done_o) begin
                            read_pixel_o = next_pixel_i;
                        end
                    end
                end

                _800x600_: begin
                    if (video_on_o & (hsync != H_DISPLAY_SIZE_2 - 1)) begin
                        read_pixel_o = next_pixel_i;
                    end else begin
                        if (((hsync == (H_SCAN_PIXEL_2 - 1)) & (vsync < (V_DISPLAY_SIZE_2 - 1))) | frame_done_o) begin
                            read_pixel_o = next_pixel_i;
                        end
                    end
                end
            endcase 
        end

endmodule : vga_controller

`endif 