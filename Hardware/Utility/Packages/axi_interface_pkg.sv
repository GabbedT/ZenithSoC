`ifndef AXI_INTERFACE_PKG_SV
    `define AXI_INTERFACE_PKG_SV

package axi_interface_pkg;
    
    /* Size specified in bytes */
    localparam AXI_DATA_SIZE = 4;

    /* Size specified in bits */
    localparam AXI_ADDR_SIZE = 32;

    /* Burst type */
    typedef enum logic [1:0] { FIXED, INCR, WRAP } axi_burst_t;

    /* Burst size in bytes */
    typedef enum logic [1:0] { BYTE, HALFWORD, WORD } axi_burst_size_t;

    /* Response codes */
    typedef enum logic [1:0] { OKAY, EXOKAY, SLVERR, DECERR } axi_response_t;

endpackage : axi_interface_pkg

import axi_interface_pkg::*;

`endif 