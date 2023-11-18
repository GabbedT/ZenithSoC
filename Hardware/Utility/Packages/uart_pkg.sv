`ifndef UART_PKG_SV 
    `define UART_PKG_SV 

package uart_pkg;

    localparam RX_BUFFER_SIZE = 512;
    localparam TX_BUFFER_SIZE = 512;

    /* Data size during UART transfer */    
    typedef enum logic [1:0] { DBIT5, DBIT6, DBIT7, DBIT8 } uart_data_lenght_t;

    /* Number of stop bits transferred */
    typedef enum logic { STOP1, STOP2 } uart_stop_bits_t;

    typedef enum logic { EVEN, ODD } uart_parity_mode_t;

    /* Error type during UART operation */
    typedef enum logic [4:0] {
        DATA_RECEIVED    = 5'b00001, 
        DATA_TRANSMITTED = 5'b00010,
        RX_BUFFER_FULL   = 5'b00100,
        TX_BUFFER_EMPTY  = 5'b01000,
        RX_PARITY_ERROR  = 5'b10000
    } uart_event_t;


    typedef struct packed {
        /* Divide the main UART clock to generate
         * other baud rates */
        logic [14:0] clock_divider;

        /* Enable CTS - RTS flow control */
        logic flow_control;

        /* Data size transferred */
        uart_data_lenght_t data_bits;

        /* Number of stop bits during transaction end */
        uart_stop_bits_t stop_bits;

        /* Parity type computed */
        uart_parity_mode_t parity_mode;

        /* Enable sending parity bits */
        logic parity_enable;

        /* Enable transmission / reception */
        logic enable_TX;
        logic enable_RX;

        /* Enable interrupt generation */
        logic [4:0] interrupt_enable;

        /* FIFOs status (non writable) */
        logic TX_full;
        logic TX_empty;
        logic RX_full;
        logic RX_empty;
    } uart_status_t;


    /* Register addresses */
    typedef enum logic [1:0] { STATUS, TX_BUFFER, RX_BUFFER, EVENT } uart_registers_t;

endpackage : uart_pkg

import uart_pkg::*;

`endif 