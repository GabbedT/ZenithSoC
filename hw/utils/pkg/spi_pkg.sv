`ifndef SPI_PKG_SV
    `define SPI_PKG_SV

package spi_pkg;
    
    /* Transmission / Reception bit order */
    typedef enum logic { MSB_FIRST, LSB_FIRST } bit_order_t;

    /* Event type during SPI operation */
    typedef enum logic [3:0] {
        SPI_DATA_RECEIVED    = 4'b0001, 
        SPI_DATA_TRANSMITTED = 4'b0010,
        SPI_RX_BUFFER_FULL   = 4'b0100,
        SPI_TX_BUFFER_EMPTY  = 4'b1000
    } spi_event_t;

    typedef struct packed {
        /* Divide the clock */
        logic [15:0] clock_divisor;
        
        /* Send MSB or LSB as first bit */
        logic bit_order;

        /* SPI Modes */
        logic cpol;
        logic cpha;

        /* Enable interrupt generation */
        logic interrupt_enable;

        /* The SPI device is idle and not transmitting */
        logic idle;

        /* FIFOs status (non writable) */
        logic TX_full;
        logic TX_empty;
        logic RX_full;
        logic RX_empty;
    } spi_status_t;

    /* SPI Register offsets */
    typedef enum logic [2:0] { SPI_STATUS, SPI_TX_BUFFER, SPI_RX_BUFFER, SPI_EVENT, SPI_SLAVE_SELECT } spi_registers_t;

endpackage : spi_pkg

import spi_pkg::*;

`endif 