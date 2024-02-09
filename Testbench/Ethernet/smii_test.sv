`ifndef SMII_TEST_SV
    `define SMII_TEST_SV

`include "../../Hardware/Utility/Packages/ethernet_pkg.sv"

module smii_test;

    logic clk_i = 0;
    logic rst_n_i = 0;
    logic pulse_i;

    /* Register interface */
    logic [4:0] address_i = '0;
    logic write_i = 1'b0;
    logic read_i = 1'b0;
    logic [15:0] data_i = '0;
    logic [15:0] data_o;

    /* SMII interface */
    logic smii_mdc_o;
    wire smii_mdio_io;

    logic done_o;

    ethernet_mac2phy #('1) dut1 (.*);


    always #5 clk_i <= !clk_i;

    logic clk_50MHz = 0; logic clk_25MHz = 0; logic clk_prev;

        always_ff @(posedge clk_i) begin
            clk_50MHz <= !clk_50MHz;

            if (clk_50MHz) begin
                clk_25MHz <= !clk_25MHz;
            end

            clk_prev <= clk_25MHz;
        end 

    assign pulse_i = !clk_prev & clk_25MHz;


    initial begin
        rst_n_i <= 0;
        @(posedge clk_i);
        rst_n_i <= 1;

        address_i <= 5'b10101;
        write_i <= 1'b1;
        data_i <= '1;
        @(posedge clk_i);
        write_i <= 1'b0;

        wait (done_o);

        address_i <= 5'b10101;
        read_i <= 1'b1;
        @(posedge clk_i);

        wait (done_o);

        $finish;
    end

endmodule : smii_test

`endif 