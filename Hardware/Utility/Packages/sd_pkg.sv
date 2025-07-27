`ifndef SD_PKG_SV
    `define SD_PKG_SV

package sd_pkg;

    /* Registers found in MAC circuit */
    typedef enum logic [2:0] { 
        SD_CTRL, SD_STATUS, SD_CMD_NUMBER, SD_CMD_ARGUMENT, SD_EVENT,
        SD_CMD_RESPONSE, SD_DATA_TX_BUFFER, SD_DATA_RX_BUFFER 
    } sd_registers_t;

    /* Response Types */
    typedef enum logic { RESP48, RESP136 } sd_response_type_t;

    /* SD Control Register */
    typedef struct packed {
        /* Reset external card */
        logic reset_card;

        /* Enable SD control */
        logic enable;

        logic clock_speed; /* 0 = 400kHz, 1 = 25MHz */
        logic bus_width;   /* 0 = 1 bit, 1 = 4 bit  */

        /* Activate CMD FSM */
        logic send_command; 

        /* Interrupt Enable */
        logic [8:0] interrupt_enable;
    } sd_control_t;

    /* SD Status Register */
    typedef struct packed {
        logic card_detected;

        logic [7:0] data_token;

        logic tx_buffer_empty;
        logic tx_buffer_full;
        logic rx_buffer_empty;
        logic rx_buffer_full;

        logic resp_buffer_empty;
        logic resp_buffer_full;

        logic cmd_idle;
        logic cmd_crc_error;
        logic cmd_timeout;
        logic cmd_response_type;

        logic data_idle;
        logic data_crc_error;
        logic data_timeout;
    } sd_status_t;

    /* SD Interrupt Status Register */
    typedef struct packed {
        logic card_detected;

        logic tx_buffer_empty;

        logic rx_buffer_full;

        logic cmd_done;
        logic cmd_crc_error;
        logic cmd_timeout;

        logic data_done;
        logic data_crc_error;
        logic data_timeout;
    } sd_interrupt_status_t;

endpackage : sd_pkg

import sd_pkg::*;

`endif 