`ifndef VGA_PKG_SV
    `define VGA_PKG_SV

package vga_pkg;
    
    localparam LINES_IN_BUFFER_1 = 96;
    localparam LINES_IN_BUFFER_2 = 75;

    /* MAX address buffers */
    localparam MAX_ADDR_1 = 640 * LINES_IN_BUFFER_1;
    localparam MAX_ADDR_2 = 800 * LINES_IN_BUFFER_2;


    typedef struct packed {
        logic [3:0] red;
        logic [3:0] green;
        logic [3:0] blue;
    } pixel_t;

    
    typedef enum logic [1:0] { _320x240_, _640x480_ } resolution_t;


    typedef struct packed {
        /* The last visible pixel has entered horizontal blanking */
        logic early_frame_done_interrupt;

        /* The last visible pixel has entered horizontal blanking */
        logic early_frame_done;

        /* Value of VSYNC */
        logic [9:0] vsync_counter;

        /* Interrupt generation enable */
        logic [3:0] enable_interrupt;

        /* Enable VGA output */
        logic enable_video;

        /* Setup VGA resolution */
        resolution_t resolution;

        /* VSYNC and HSYNC are in display area */
        logic video_on;

        /* The whole frame has been displayed */
        logic frame_done;
    } control_status_register_t;


    typedef struct packed {
        /* The last visible pixel has entered horizontal blanking */
        logic early_frame_done;

        /* DDR master error */
        logic ddr_error;

        /* The whole frame has been displayed */
        logic frame_done;

        /* VSYNC and HSYNC are in display area */
        logic video_on;

        /* Line buffer must not be empty during video on */
        logic buffer_empty;
    } event_register_t;


    typedef struct packed {
        /* Sprite top left Y-position */
        logic [9:0] y_position;

        /* Sprite top left X-position */
        logic [9:0] x_position;

        /* Sprite is visible */
        logic enable;
    } sprite_register_t;


    /* Sprite table addresses */
    localparam logic [6:0] VGA_REGISTER_SPACE = 7'd5;
    localparam logic [6:0] VGA_CTABLE_BASE = 7'd5;
    localparam logic [6:0] VGA_CTABLE_SIZE = 7'd16;
    localparam logic [6:0] VGA_PTABLE_BASE = VGA_CTABLE_BASE + VGA_CTABLE_SIZE;
    localparam logic [6:0] VGA_PTABLE_SIZE = 7'd64;
    localparam logic [6:0] VGA_DEVICE_SPACE = VGA_PTABLE_BASE + VGA_PTABLE_SIZE;


    typedef enum logic [2:0] { VGA_CTLR_STATUS, VGA_FRM_BUF_BASE, VGA_FRM_BUF_SIZE, VGA_EVENT, VGA_SPRITE } registers_t;

endpackage : vga_pkg

import vga_pkg::*;

`endif
