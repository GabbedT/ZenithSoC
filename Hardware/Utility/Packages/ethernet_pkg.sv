`ifndef ETHERNET_PKG_SV
    `define ETHERNET_PKG_SV

package ethernet_pkg;

    localparam PREAMBLE_BYTES = 7;
    localparam MAC_ADDR_BYTES = 6;
    localparam ETH_TYPE_BYTES = 2;
    localparam CRC_BYTES = 4;
    localparam IPG_BYTES = 12;

    typedef enum logic { IEEE8023, ETHERNET_II } packet_type_t;


endpackage : ethernet_pkg

import ethernet_pkg::*;

`endif 