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

    
    typedef enum logic { _640x480_, _800x600_ } resolution_t;


    typedef struct packed {
        /* Value of VSYNC */
        logic [8:0] vsync_counter;

        /* Interrupt generation enable */
        logic [2:0] enable_interrupt;

        /* Enable VGA output */
        logic enable_video;

        /* Setup VGA resolution */
        resolution_t resolution;

        /* No data in line buffer */
        logic buffer_empty;

        /* VSYNC and HSYNC are in display area */
        logic video_on;

        /* The whole frame has been displayed */
        logic frame_done;   

        /* Enable auto increment while writing
         * to line buffer */
        logic auto_increment;
    } status_register_t;


    typedef struct packed {
        /* No data in line buffer */
        logic buffer_empty;

        /* VSYNC and HSYNC are in display area */
        logic video_on;

        /* The whole frame has been displayed */
        logic frame_done;   
    } event_register_t;


    typedef struct packed {
        /* Sprite top left X-position */
        logic [9:0] x_position;

        /* Sprite top left Y-position */
        logic [9:0] y_position;

        /* Sprite is visible */
        logic enable;
    } sprite_register_t;


    typedef enum logic [2:0] { VGA_STATUS, VGA_INCREMENT, VGA_BUFFER_SIZE, VGA_EVENT, VGA_SPRITE } registers_t;

endpackage : vga_pkg

import vga_pkg::*;

`endif 