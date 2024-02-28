`ifndef ETHERNET_PKG_SV
    `define ETHERNET_PKG_SV

package ethernet_pkg;

    localparam BROADCAST_ADDRESS = 48'hFF_FF_FF_FF_FF_FF;
    
    localparam PREAMBLE_BYTES = 7;
    localparam MAC_ADDR_BYTES = 6;
    localparam ETH_TYPE_BYTES = 2;
    localparam CRC_BYTES = 4;
    localparam IPG_BYTES = 12;

    /* Registers found in MAC circuit */
    typedef enum logic [2:0] { 
        ETH_MAC_STATUS, ETH_TX_DESC_LOW, ETH_TX_DESC_HIGH, 
        ETH_TX_BUFFER, ETH_RX_DESC_LOW, ETH_RX_DESC_HIGH, 
        ETH_RX_BUFFER, ETH_INTERRUPT 
    } mac_registers_t;

    /* Ethernet speed */
    typedef enum logic { MBPS10, MBPS100 } eth_speed_t;

    /* Select chip */
    localparam PHY = 0;
    localparam MAC = 1;


    typedef struct packed {
        /* Select registers of PHY or MAC */
        logic select;

        /* Upper part of register address (used in PHY) */
        logic [1:0] upper_part;
        
        /* Lower part of register address (used in PHY and MAC) */
        mac_registers_t registers;
    } eth_registers_t;


    typedef struct packed {
        /* Ethernet speed */
        eth_speed_t speed;
        
        /* Enable interrupt generation */
        logic [3:0] interrupt_enable;

        /* FSMs status */
        logic TX_idle;
        logic RX_idle;

        /* Packet FIFOs status (non writable) */
        logic TX_packet_full;
        logic TX_packet_empty;
        logic RX_packet_full;
        logic RX_packet_empty;

        /* Payload FIFOs status (non writable) */
        logic TX_payload_full;
        logic TX_payload_empty;
        logic RX_payload_full;
        logic RX_payload_empty;   
    } eth_status_t;


    typedef struct packed {
        /* 48 bit MAC address */
        logic [5:0][7:0] mac_address;

        /* Payload size / Payload type */
        logic [1:0][7:0] lenght;
    } eth_descriptor_t;


    typedef struct packed {
        /* Error during RX */
        logic rx_error;

        /* Reception done */
        logic rx_done;

        /* Transmission done */
        logic tx_done;

        /* Interrupt from PHY chip */
        logic phy_interrupt; 
    } eth_interrupt_t;

endpackage : ethernet_pkg

import ethernet_pkg::*;

`endif 