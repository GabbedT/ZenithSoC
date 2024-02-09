`ifndef ETHERNET_SV 
    `define ETHERNET_SV 

`include "ethernet_tx.sv"
`include "ethernet_rx.sv"
`include "ethernet_registers.sv"
`include "ethernet_mac2phy.sv"

`include "../../System/edge_detector.sv"
`include "../../System/synchronizer.sv"

`include "../../Utility/Packages/ethernet_pkg.sv"

module ethernet #(
    /* Board PHY chip address */
    parameter logic [4:0] CHIP_PHY_ADDRESS = 5'b00001,

    /* Device MAC address */
    parameter logic [5:0][7:0] MAC_ADDRESS = 48'h00_00_00_00_00_00,

    /* Buffer sizes */
    parameter TX_BUFFER_SIZE = 512,
    parameter RX_BUFFER_SIZE = 512,

    /* Number of packets in a descriptor buffer */
    parameter TX_PACKETS = 32,
    parameter RX_PACKETS = 32
) (
    /* Global signals */
    input logic clk_i,
    input logic phy_clk_i,
    input logic rst_n_i,
    output logic interrupt_o,

    /* Write interface */
    input logic write_i,
    input logic [5:0] write_address_i,
    input logic [3:0][7:0] write_data_i,
    output logic write_error_o,
    output logic write_done_o,

    /* Read interface */
    input logic read_i,
    input logic [5:0] read_address_i,
    output logic [31:0] read_data_o,
    output logic read_error_o,
    output logic read_done_o,

    /* RMII Interface */
    input logic phy_interrupt_i,
    input logic [1:0] rmii_rxd_i,
    input logic rmii_crsdv_i,
    input logic rmii_rxer_i,
    output logic [1:0] rmii_txd_o,
    output logic rmii_txen_o,
    output logic rmii_refclk_o,
    output logic rmii_rstn_o,

    /* SMII interface */
    output logic smii_mdc_o,
    inout logic smii_mdio_io
);

//====================================================================================
//      REGISTERS
//====================================================================================

    logic read_payload, read_descriptor, write_payload, write_descriptor; logic tx_idle, rx_idle, rx_error, data_ready;
    logic [5:0][7:0] tx_dest_address, rx_src_address; logic [1:0][7:0] tx_payload_length, rx_payload_length; logic [7:0] tx_payload, rx_payload;
    eth_speed_t eth_speed;

    logic [4:0] smii_address; logic smii_write, smii_read, smii_done; logic [15:0] smii_data_tx, smii_data_rx;

    ethernet_registers #(TX_BUFFER_SIZE, RX_BUFFER_SIZE, TX_PACKETS, RX_PACKETS) registers (
        .clk_i       ( clk_i       ),
        .rst_n_i     ( rst_n_i     ),
        .interrupt_o ( interrupt_o ),
        .eth_speed_o ( eth_speed   ),

        .write_address_i ( write_address_i ),
        .write_i         ( write_i         ),
        .write_data_i    ( write_data_i    ),
        .write_error_o   ( write_error_o   ),
        .write_done_o    ( write_done_o    ),

        .read_address_i ( read_address_i ),
        .read_i         ( read_i         ),
        .data_o         ( read_data_o    ),
        .read_error_o   ( read_error_o   ),
        .read_done_o    ( read_done_o    ),

        .read_payload_i    ( read_payload      ),
        .read_descriptor_i ( read_descriptor   ),
        .tx_idle_i         ( tx_idle           ),
        .dest_address_o    ( tx_dest_address   ),
        .payload_length_o  ( tx_payload_length ),
        .payload_o         ( tx_payload        ),
        .data_ready_o      ( data_ready        ),

        .write_payload_i    ( write_payload     ),
        .write_descriptor_i ( write_descriptor  ),
        .rx_idle_i          ( rx_idle           ),
        .rx_error_i         ( rx_error          ),
        .source_address_i   ( rx_src_address    ),
        .payload_length_i   ( rx_payload_length ),
        .payload_i          ( rx_payload        ),

        .smii_address_o   ( smii_address    ),
        .smii_write_o     ( smii_write      ),
        .smii_read_o      ( smii_read       ),
        .smii_data_o      ( smii_data_tx    ),
        .smii_interrupt_i ( phy_interrupt_i ),
        .smii_data_i      ( smii_data_rx    ),
        .smii_done_i      ( smii_done       )
    );


//====================================================================================
//      SMII INTERFACE
//====================================================================================

    ethernet_mac2phy #(CHIP_PHY_ADDRESS) mac2phy (
        .clk_i   ( clk_i           ),
        .rst_n_i ( rst_n_i         ),

        .address_i ( smii_address ),
        .write_i   ( smii_write   ),
        .read_i    ( smii_read    ),
        .data_i    ( smii_data_tx ),
        .data_o    ( smii_data_rx ),

        .smii_mdc_o   ( smii_mdc_o   ),
        .smii_mdio_io ( smii_mdio_io ),

        .done_o ( smii_done )
    );


//====================================================================================
//      RECEIVER
//====================================================================================

    logic pulse_in;

    ethernet_rx #(MAC_ADDRESS) receiver (
        .clk_i    ( clk_i    ),
        .rst_n_i  ( rst_n_i  ),
        .sample_i ( pulse_in ),

        .source_address_o ( rx_src_address    ), 
        .lenght_type_o    ( rx_payload_length ),
        .payload_o        ( rx_payload        ),
        .packet_error_o   ( rx_error          ),

        .rmii_rxd_i   ( rmii_rxd_i   ),
        .rmii_crsdv_i ( rmii_crsdv_i ),
        .rmii_rxer_i  ( rmii_rxer_i  ),

        .idle_o          ( rx_idle          ),
        .packet_valid_o  ( write_descriptor ),
        .payload_valid_o ( write_payload    )
    );


//====================================================================================
//      TRANSMITTER
//====================================================================================

    ethernet_tx #(MAC_ADDRESS) transmitter (
        .clk_i      ( clk_i    ),
        .rst_n_i    ( rst_n_i  ),
        .transmit_i ( pulse_in ),

        .dest_address_i   ( tx_dest_address   ),
        .payload_length_i ( tx_payload_length ),

        .read_data_o       ( read_payload    ),
        .read_descriptor_o ( read_descriptor ),
        .data_ready_i      ( data_ready      ),
        .payload_data_i    ( tx_payload      ),

        .rmii_txd_o  ( rmii_txd_o  ),
        .rmii_txen_o ( rmii_txen_o ),

        .idle_o ( tx_idle )
    );


//====================================================================================
//      CLOCK GENERATOR
//====================================================================================

    ethernet_reset generator (
        .clk_i    ( clk_i    ),
        .rst_n_i  ( rst_n_i  ),

        .rst_n_o ( rmii_rstn_o   )
    );


    logic [3:0] clk_counter; logic clk_5MHz, clk_50MHz;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                clk_counter <= '0;
            end else begin 
                clk_counter <= (clk_counter == 'd9) ? '0 : (clk_counter + 1'b1);
            end 
        end 

    assign clk_5MHz = clk_counter == 'd9;
    assign clk_50MHz = clk_counter[0];

    /* Pulse to shift bits inside TX and RX FSMs */
    assign pulse_in = (eth_speed == MBPS100) ? clk_50MHz : clk_5MHz;

    /* Reference clock is always 50MHz */
    assign rmii_refclk_o = clk_50MHz;

endmodule : ethernet 

`endif 