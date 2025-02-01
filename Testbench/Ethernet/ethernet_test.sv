`ifndef ETHERNET_TEST_SV
    `define ETHERNET_TEST_SV

`include "../../Hardware/Utility/Packages/ethernet_pkg.sv"

module ethernet_test;

    /* Global signals */
    logic clk_i = 0;
    logic phy_clk_i = 0;
    logic rst_n_i = 0;
    logic interrupt_o;

    /* Write interface */
    logic write_i = 0;
    logic [5:0] write_address_i = 0;
    logic [3:0][7:0] write_data_i = 0;
    logic write_error_o;
    logic write_done_o;

    /* Read interface */
    logic read_i = 0;
    logic [5:0] read_address_i = 0;
    logic [31:0] read_data_o;
    logic read_error_o;
    logic read_done_o;

    /* RMII Interface */
    logic phy_interrupt_i = 0;
    wire [1:0] rmii_rxd_io;
    wire rmii_crsdv_io;
    logic rmii_rxer_i = 0;
    logic [1:0] rmii_txd_o;
    logic rmii_txen_o;
    logic rmii_refclk_o;
    logic rmii_rstn_o;
    
    logic busy_o;

    /* SMII interface */
    logic smii_mdc_o;
    wire smii_mdio_io;

    always #5 clk_i <= !clk_i;
    always #10 phy_clk_i <= !phy_clk_i;

    assign rmii_rxd_i = rmii_txd_o;


    ethernet dut (.*);


    task write_register(input int addr, input logic [31:0] data);
        write_address_i <= addr;
        write_data_i <= data;

        write_i <= 1'b1;
        @(posedge clk_i);
        write_i <= 1'b0;

        wait(write_done_o);

        @(posedge clk_i);
    endtask : write_register

    task read_register(input int addr);
        read_address_i <= addr;

        read_i <= 1'b1;
        @(posedge clk_i);
        read_i <= 1'b0;

        wait(read_done_o);

        @(posedge clk_i);
    endtask : read_register


    eth_descriptor_t packet_descriptor;

    assign rmii_crsdv_i = rmii_txen_o;

    initial begin
        rst_n_i <= 0;
        repeat(2) @(posedge clk_i);
        rst_n_i <= 1;

        write_register(0, 0);
        write_register(5, 'd10);
        read_register(5);

        write_register(32, '1);

        packet_descriptor.lenght = 'd20;
        packet_descriptor.mac_address = '1;

        for (int j = 0; j < 10; ++j) begin
            for (int i = 0; i < 40; ++i) begin
                write_register(35, i);
            end

            write_register(33, packet_descriptor[31:0]);
            write_register(34, packet_descriptor[63:32]);

            repeat(2) begin 
                wait(interrupt_o);
                write_register(39, '0);
            end
        end

        $finish;
    end

endmodule : ethernet_test

`endif 