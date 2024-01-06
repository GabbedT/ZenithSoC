`ifndef ETHERNET_TEST_SV
    `define ETHERNET_TEST_SV

`include "../../Hardware/Utility/Packages/ethernet_pkg.sv"

module ethernet_test;

    logic clk_i = 0;
    logic rst_n_i = 0;
    logic transmit_i = 0;

    /* Packet info */
    logic [5:0][7:0] dest_address_i = '0;
    logic [15:0] payload_length_i = 1;

    /* Data */
    logic read_data_o;
    logic read_descriptor_o;
    logic data_ready_i = '0;
    logic [7:0] payload_data_i = 8'b10101010;

    /* RMII Interface */
    logic [1:0] rmii_txd_o;
    logic rmii_txen_o;

    /* Status */
    logic tx_idle_o;

    ethernet_tx #('1) dut1 (
        clk_i, rst_n_i, transmit_i, dest_address_i, payload_length_i, 
        read_data_o, read_descriptor_o, data_ready_i, payload_data_i, 
        rmii_txd_o, rmii_txen_o, tx_idle_o
    );


    logic sample_i = 0;

    /* Packet info */
    logic [47:0] source_address_o;
    logic [15:0] lenght_type_o;
    logic [7:0] payload_o;
    logic packet_error_o;

    /* RMII Interface */
    logic [1:0] rmii_rxd_i;
    logic rmii_crsdv_i;
    logic rmii_rxer_i = 0;

    assign rmii_rxd_i = rmii_txd_o;
    assign rmii_crsdv_i = !tx_idle_o;

    /* FSM Status */
    logic rx_idle_o;
    logic packet_valid_o;
    logic payload_valid_o;

    ethernet_rx #(48'd1) dut2 (
        clk_i, rst_n_i, sample_i, source_address_o, lenght_type_o,
        payload_o, packet_error_o, rmii_rxd_i, rmii_crsdv_i, rmii_rxer_i,
        rx_idle_o, packet_valid_o, payload_valid_o
    );


    always #5 clk_i <= !clk_i;

    logic clk_50MHz = 0;

        always_ff @(posedge clk_i) begin
            clk_50MHz <= !clk_50MHz;

            transmit_i <= !transmit_i;
            sample_i <= !sample_i;
        end 


    initial begin
        rst_n_i <= 0;
        @(posedge clk_i);
        rst_n_i <= 1;

        data_ready_i <= 1;
        @(posedge clk_i);

        wait (tx_idle_o);

        $finish;
    end

endmodule : ethernet_test

`endif 