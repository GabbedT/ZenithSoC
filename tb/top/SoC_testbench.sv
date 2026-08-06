`ifndef SOC_TESTBENCH_SV
`define SOC_TESTBENCH_SV

`timescale 1ns/1ps

module soc_testbench;

    import soc_parameters::*;
    import uart_pkg::*;

    localparam time TEST_TIMEOUT = 1s;

    logic clk_i = 1'b0;
    logic rst_n_i = 1'b0;

    always #5ns clk_i = ~clk_i;

    /* GPIO */
    wire [GPIO_DEVICE_NUMBER - 1:0][7:0] pin_io;

    /* UART */
    logic [UART_DEVICE_NUMBER - 1:0] uart_rx_i = '1;
    logic [UART_DEVICE_NUMBER - 1:0] uart_cts_i = '0;
    wire  [UART_DEVICE_NUMBER - 1:0] uart_tx_o;
    wire  [UART_DEVICE_NUMBER - 1:0] uart_rts_o;

    /* SPI */
    wire  [SPI_DEVICE_NUMBER - 1:0]                   spi_sclk_o;
    wire  [SPI_DEVICE_NUMBER - 1:0][SPI_SLAVES - 1:0] spi_cs_n_o;
    wire  [SPI_DEVICE_NUMBER - 1:0]                   spi_mosi_o;
    logic [SPI_DEVICE_NUMBER - 1:0]                   spi_miso_i = '0;

    /* RMII and SMI */
    wire [1:0] rmii_rxd_io;
    wire       rmii_crsdv_io;
    logic      rmii_rxer_i = 1'b0;
    wire [1:0] rmii_txd_o;
    wire       rmii_txen_o;
    wire       rmii_refclk_o;
    wire       rmii_rstn_o;
    wire       smi_mdc_o;
    wire       smi_mdio_io;

    /* Audio and timer */
    logic pdm_data_i = 1'b0;
    wire  pdm_clk_o;
    wire  pdm_lrsel_o;
    wire  pwm_o;
    wire  audio_enable_o;
    wire  tmr_pwm_o;

    /* SD card pins have the pull-ups required by the physical interface. */
    logic      sd_cd_n_i = 1'b0;
    tri1       sd_cmd_io;
    tri1 [3:0] sd_data_io;
    wire       sd_reset_o;
    wire       sd_clk_o;

    /* DDR2 component interface */
    wire [15:0] ddr2_dq;
    wire [1:0]  ddr2_dqs_n;
    wire [1:0]  ddr2_dqs_p;
    wire [1:0]  ddr2_dm;
    wire [12:0] ddr2_addr;
    wire [2:0]  ddr2_ba;
    wire        ddr2_ras_n;
    wire        ddr2_cas_n;
    wire        ddr2_we_n;
    wire        ddr2_cke;
    wire        ddr2_ck_p;
    wire        ddr2_ck_n;
    wire        ddr2_cs_n;
    wire        ddr2_odt;

    ZenithSoC #(
        .DDR_MEMORY ( 1 )
    ) dut (.*);

    /* Micron MT47H64M16 DDR2 model used by the Nexys A7 MIG configuration. */
    ddr2_model ddr2 (
        .ck      ( ddr2_ck_p  ),
        .ck_n    ( ddr2_ck_n  ),
        .cke     ( ddr2_cke   ),
        .cs_n    ( ddr2_cs_n  ),
        .ras_n   ( ddr2_ras_n ),
        .cas_n   ( ddr2_cas_n ),
        .we_n    ( ddr2_we_n  ),
        .dm_rdqs ( ddr2_dm    ),
        .ba      ( ddr2_ba    ),
        .addr    ( ddr2_addr  ),
        .dq      ( ddr2_dq    ),
        .dqs     ( ddr2_dqs_p ),
        .dqs_n   ( ddr2_dqs_n ),
        .rdqs_n  (            ),
        .odt     ( ddr2_odt   )
    );

    /* SD protocol/PHY model maintained by vp/blocks/sd. */
    logic clk_50 = 1'b0;
    logic clk_100 = 1'b0;

    always #10ns clk_50 = ~clk_50;
    always #5ns clk_100 = ~clk_100;

    wire       sd_model_cmd_i;
    wire       sd_model_cmd_o;
    wire       sd_model_cmd_t;
    wire [3:0] sd_model_dat_i;
    wire [3:0] sd_model_dat_o;
    wire [3:0] sd_model_dat_t;

    wire        wbm_clk;
    wire [31:0] wbm_addr;
    wire [31:0] wbm_read_data;
    wire [31:0] wbm_write_data;
    wire [3:0]  wbm_select;
    wire        wbm_cycle;
    wire        wbm_strobe;
    wire        wbm_write;
    wire        wbm_ack;
    wire [2:0]  wbm_cycle_type;
    wire [1:0]  wbm_burst_type;

    sd_top sd_card (
        .clk_50        ( clk_50           ),
        .clk_100       ( clk_100          ),
        .clk_200       ( 1'b0             ),
        .reset_n       ( rst_n_i          ),
        .sd_clk        ( sd_clk_o         ),
        .sd_cmd_i      ( sd_model_cmd_i   ),
        .sd_cmd_o      ( sd_model_cmd_o   ),
        .sd_cmd_t      ( sd_model_cmd_t   ),
        .sd_dat_i      ( sd_model_dat_i   ),
        .sd_dat_o      ( sd_model_dat_o   ),
        .sd_dat_t      ( sd_model_dat_t   ),
        .wbm_clk_o     ( wbm_clk          ),
        .wbm_adr_o     ( wbm_addr         ),
        .wbm_dat_i     ( wbm_read_data    ),
        .wbm_dat_o     ( wbm_write_data   ),
        .wbm_sel_o     ( wbm_select       ),
        .wbm_cyc_o     ( wbm_cycle        ),
        .wbm_stb_o     ( wbm_strobe       ),
        .wbm_we_o      ( wbm_write        ),
        .wbm_ack_i     ( wbm_ack          ),
        .wbm_cti_o     ( wbm_cycle_type   ),
        .wbm_bte_o     ( wbm_burst_type   ),
        .opt_enable_hs ( 1'b0             )
    );


    bram_whishbone #(
        .ADDR_WIDTH ( 16 )
    ) sd_storage (
        .clk       ( wbm_clk        ),
        .rst       ( rst_n_i        ),
        .wbm_adr_i ( wbm_addr[17:2] ),
        .wbm_dat_i ( wbm_write_data ),
        .wbm_dat_o ( wbm_read_data  ),
        .wbm_we_i  ( wbm_write      ),
        .wbm_stb_i ( wbm_strobe     ),
        .wbm_cyc_i ( wbm_cycle      ),
        .wbm_sel_i ( wbm_select     ),
        .wbm_ack_o ( wbm_ack        ),
        .wbm_cti_i ( wbm_cycle_type ),
        .wbm_bte_i ( wbm_burst_type )
    );

    assign sd_cmd_io = sd_model_cmd_t ? 1'bz : sd_model_cmd_o;
    assign sd_model_cmd_i = sd_cmd_io;

    genvar sd_bit;
    generate
        for (sd_bit = 0; sd_bit < 4; sd_bit++) begin : sd_card_data_pin
            assign sd_data_io[sd_bit] =
                sd_model_dat_t[sd_bit] ? 1'bz : sd_model_dat_o[sd_bit];
        end
    endgenerate

    assign sd_model_dat_i = sd_data_io;


    integer output_file;

    initial begin
        #1ps;
        $readmemh("coremark_sd_words.hex", sd_storage.mem, 16'h0000);
    end

    initial begin
        output_file = $fopen("output_trace.txt", "w");

        repeat (40) @(posedge clk_i);
        rst_n_i = 1'b1;
        fork
            begin : simulation_timeout
                #TEST_TIMEOUT;
                $display("ERROR: SoC simulation timed out after %0t", TEST_TIMEOUT);
            end

            begin : cpu_stopped
                wait (dut.ApogeoRV.system_cpu.exception === 1'b1);
                $display("CPU stopped at %0t", $time);
            end
        join_any

        disable fork;
        $fclose(output_file);
        $finish;
    end

    /* Keep the legacy fast UART trace without decoding the serial line. */
    always_ff @(posedge clk_i) begin
        if (rst_n_i && dut.genblk1[0].uart_device.write_i &&
            dut.genblk1[0].uart_device.write_address_i == UART_TX_BUFFER) begin

            $fwrite(output_file, "%c", dut.genblk1[0].uart_device.write_data_i[7:0]);
            $fflush(output_file);
            $write("%c", dut.genblk1[0].uart_device.write_data_i[7:0]);
        end
    end

endmodule : soc_testbench

`endif
