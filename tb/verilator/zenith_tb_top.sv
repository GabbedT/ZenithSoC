// ============================================================================
// Instantiates ZenithSoC with DDR_MEMORY=0 so that:
//   - no Xilinx PLL (clock_source): sys_clk = clk_i directly,
//   - no MIG (ddr_memory_interface is excluded from the filelist),
//   - the behavioural DDR model inside ZenithSoC.sv is used instead.
// ============================================================================

`ifndef ZENITH_TB_TOP_SV
`define ZENITH_TB_TOP_SV

module zenith_tb_top (
    input logic clk,
    input logic rst_n
);

    `define BE      dut.ApogeoRV.system_cpu.apogeo_backend
    `define LDU     dut.ApogeoRV.cpu_load_channel
    `define STRBUF `BE.execute_stage.LSU.stu.str_buffer

    /* instruction_status_t encodings used for the trace (apogeo_pkg.sv) */
    localparam int unsigned STORE_OP = 18;
    localparam int unsigned LOAD_OP  = 19;

// ============================================================================
//      EXTERNAL PIN NETS (tied off / open)
// ============================================================================

    /* GPIO */
    wire [GPIO_DEVICE_NUMBER - 1:0][7:0] pin_io;

    /* UART */
    wire  [UART_DEVICE_NUMBER - 1:0] uart_rx_i;
    logic [UART_DEVICE_NUMBER - 1:0] uart_cts_i = '0;
    wire  [UART_DEVICE_NUMBER - 1:0] uart_tx_o;
    wire  [UART_DEVICE_NUMBER - 1:0] uart_rts_o;

    /* Local loopback used by the interrupt firmware to exercise RX events */
    assign uart_rx_i = uart_tx_o;

    /* SPI */
    wire  [SPI_DEVICE_NUMBER - 1:0]                   spi_sclk_o;
    wire  [SPI_DEVICE_NUMBER - 1:0][SPI_SLAVES - 1:0] spi_cs_n_o;
    wire  [SPI_DEVICE_NUMBER - 1:0]                   spi_mosi_o;
    wire  [SPI_DEVICE_NUMBER - 1:0]                   spi_miso_i;

    /* Local loopback: a complete transaction does not require a slave model */
    assign spi_miso_i = spi_mosi_o;

    /* GPIO0 drives GPIO1 so software can generate deterministic input edges */
    assign pin_io[0][1] = pin_io[0][0];

    /* RMII Ethernet */
    wire  [1:0] rmii_rxd_io;
    wire        rmii_crsdv_io;
    logic       rmii_rxer_i = '0;
    wire  [1:0] rmii_txd_o;
    wire        rmii_txen_o;
    wire        rmii_refclk_o;
    wire        rmii_rstn_o;

    /* SMI */
    wire smi_mdc_o;
    wire smi_mdio_io;

    /* PDM audio */
    logic pdm_data_i = '0;
    wire  pdm_clk_o;
    wire  pdm_lrsel_o;

    /* PWM audio */
    wire pwm_o;
    wire audio_enable_o;

    /* Timer PWM */
    wire tmr_pwm_o;

    /* SD card */
    logic      sd_cd_n_i = '1; // no card present
    wire       sd_cmd_io;
    wire [3:0] sd_data_io;
    wire       sd_reset_n_o;
    wire       sd_clk_o;

    /* DDR2 pins (unused with DDR_MEMORY=0, the internal model is used) */
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


// ============================================================================
//      DUT
// ============================================================================

    ZenithSoC #(
        .DDR_MEMORY ( 0 )
    ) dut (
        .clk_i   ( clk   ),
        .rst_n_i ( rst_n ),

        .pin_io ( pin_io ),

        .uart_rx_i  ( uart_rx_i  ),
        .uart_cts_i ( uart_cts_i ),
        .uart_tx_o  ( uart_tx_o  ),
        .uart_rts_o ( uart_rts_o ),

        .spi_sclk_o ( spi_sclk_o ),
        .spi_cs_n_o ( spi_cs_n_o ),
        .spi_mosi_o ( spi_mosi_o ),
        .spi_miso_i ( spi_miso_i ),

        .rmii_rxd_io   ( rmii_rxd_io   ),
        .rmii_crsdv_io ( rmii_crsdv_io ),
        .rmii_rxer_i   ( rmii_rxer_i   ),
        .rmii_txd_o    ( rmii_txd_o    ),
        .rmii_txen_o   ( rmii_txen_o   ),
        .rmii_refclk_o ( rmii_refclk_o ),
        .rmii_rstn_o   ( rmii_rstn_o   ),

        .smi_mdc_o   ( smi_mdc_o   ),
        .smi_mdio_io ( smi_mdio_io ),

        .pdm_data_i  ( pdm_data_i  ),
        .pdm_clk_o   ( pdm_clk_o   ),
        .pdm_lrsel_o ( pdm_lrsel_o ),

        .pwm_o          ( pwm_o          ),
        .audio_enable_o ( audio_enable_o ),

        .tmr_pwm_o ( tmr_pwm_o ),

        .sd_cd_n_i    ( sd_cd_n_i    ),
        .sd_cmd_io    ( sd_cmd_io    ),
        .sd_data_io   ( sd_data_io   ),
        .sd_reset_n_o ( sd_reset_n_o ),
        .sd_clk_o     ( sd_clk_o     ),

        .ddr2_dq    ( ddr2_dq    ),
        .ddr2_dqs_n ( ddr2_dqs_n ),
        .ddr2_dqs_p ( ddr2_dqs_p ),
        .ddr2_dm    ( ddr2_dm    ),
        .ddr2_addr  ( ddr2_addr  ),
        .ddr2_ba    ( ddr2_ba    ),
        .ddr2_ras_n ( ddr2_ras_n ),
        .ddr2_cas_n ( ddr2_cas_n ),
        .ddr2_we_n  ( ddr2_we_n  ),
        .ddr2_cke   ( ddr2_cke   ),
        .ddr2_ck_p  ( ddr2_ck_p  ),
        .ddr2_ck_n  ( ddr2_ck_n  ),
        .ddr2_cs_n  ( ddr2_cs_n  ),
        .ddr2_odt   ( ddr2_odt   )
    );


// ============================================================================
//      FIRMWARE PRELOAD (DPI export, called from the harness before reset)
// ============================================================================

    /* Write a 32-bit word into the behavioural DDR model. 'byte_addr' is DDR-relative  
     *
     * The model stores 64-bit words, ddr_memory[byte_addr>>3], selecting the
     * upper/lower 32-bit lane by byte_addr[2]. Mirrors cosim_ddr's loader.
     */
    export "DPI-C" function zenith_ddr_preload_word;

    function void zenith_ddr_preload_word(
        input int unsigned byte_addr,
        input int unsigned data
    );
        automatic int unsigned widx = byte_addr >> 3;

        if (widx < (128 * 1024 * 1024) / 8) begin
            if (byte_addr[2])
                dut.ddr_memory[widx][63:32] = data;
            else
                dut.ddr_memory[widx][31:0] = data;
        end
    endfunction


    /* Read back a 32-bit word from the DDR model (DDR-relative byte addr). */
    export "DPI-C" function zenith_ddr_peek_word;

    function int unsigned zenith_ddr_peek_word(
        input int unsigned byte_addr
    );
        automatic int unsigned widx = byte_addr >> 3;

        if (widx >= (128 * 1024 * 1024) / 8)
            return 32'd0;

        if (byte_addr[2])
            return dut.ddr_memory[widx][63:32];
        else
            return dut.ddr_memory[widx][31:0];
    endfunction;


    /* Write a 32-bit word into the boot ROM (4 byte-banks of on_chip_memory).
     * 'byte_addr' is absolute (boot region starts at 0x0). Each bank holds one
     * byte lane of the 32-bit word (memory_bank.sv / on_chip_memory.sv).
     */
    export "DPI-C" function zenith_rom_preload_word;

    function void zenith_rom_preload_word(
        input int unsigned byte_addr,
        input int unsigned data
    );
        automatic int unsigned widx = byte_addr >> 2;

        if (widx < BOOT_SIZE / 4) begin
            dut.boot_memory.mem_bank[0].bank.memory[widx] = data[7:0];
            dut.boot_memory.mem_bank[1].bank.memory[widx] = data[15:8];
            dut.boot_memory.mem_bank[2].bank.memory[widx] = data[23:16];
            dut.boot_memory.mem_bank[3].bank.memory[widx] = data[31:24];
        end
    endfunction;



    export "DPI-C" function zenith_rom_peek_word;

    function int unsigned zenith_rom_peek_word(input int unsigned byte_addr);
        automatic int unsigned widx = byte_addr >> 2;

        if (widx >= BOOT_SIZE / 4) begin
            return 32'b0; 
        end

        return {dut.boot_memory.mem_bank[0].bank.memory[widx],
                dut.boot_memory.mem_bank[1].bank.memory[widx],
                dut.boot_memory.mem_bank[2].bank.memory[widx],
                dut.boot_memory.mem_bank[3].bank.memory[widx]};
    endfunction;


    import "DPI-C" function void zenith_uart_tx_byte(input int unsigned data);

    localparam UART_IDX = 1;
    localparam UART_TX_BUF_ADDR = 2'b01;

    always_ff @(posedge clk) begin
        if (dut.write_request[UART_IDX] && dut.write_address[UART_IDX][3:2] == UART_TX_BUF_ADDR) begin
            zenith_uart_tx_byte({24'b0, dut.write_data[UART_IDX][7:0]});
        end
    end


    import "DPI-C" function void zenith_trace_commit(
        input int unsigned is_exception,
        input int unsigned pc,
        input int unsigned info,
        input int unsigned rd,
        input int unsigned rd_value,
        input int unsigned is_store,
        input int unsigned is_load,
        input int unsigned mem_addr,
        input int unsigned mem_data,
        input int unsigned mem_width
    );

    logic [31:0] load_buffer [$];

    logic forward_match;
    logic forward_match_prev;

    assign forward_match = `BE.execute_stage.LSU.ldu.forward_match_i;

    always_ff @(posedge clk) begin
        forward_match_prev <= forward_match;
    end


    always_ff @(posedge clk) begin
        if (!rst_n) begin
            load_buffer.delete();
        end else begin

            /* Track load addresses at request time */
            if (`LDU.request) begin
                load_buffer.push_back(`LDU.address);
            end

            if (`LDU.invalidate) begin
                if (!forward_match_prev && (load_buffer.size() != 0)) begin
                    void'(load_buffer.pop_front());
                end
            end

            /* Retired (written-back) instruction */
            if (`BE.writeback_o) begin
                automatic int unsigned ev     = `BE.exception_vector;
                automatic int unsigned pc     = `BE.trap_iaddress;
                automatic int unsigned rd     = `BE.reg_destination_o;
                automatic int unsigned rdv    = `BE.writeback_result_o;
                automatic int unsigned is_s   = (ev == STORE_OP);
                automatic int unsigned is_l   = (ev == LOAD_OP);
                automatic int unsigned maddr  = 0;
                automatic int unsigned mdata  = 0;
                automatic int unsigned mwidth = 2;

                if (is_s) begin
                    maddr  = `STRBUF.metadata_buffer[`STRBUF.valid_ptr].address;
                    mdata  = `STRBUF.data_buffer[0][`STRBUF.valid_ptr];
                    mwidth = `STRBUF.store_width_buffer[`STRBUF.valid_ptr];
                end
                else if (is_l && (load_buffer.size() != 0)) begin
                    maddr = load_buffer.pop_front();
                end

                zenith_trace_commit( 32'd0, pc, ev, rd, rdv, is_s, is_l, maddr, mdata, mwidth );
            end

            /* A committed synchronous trap is still a retired event */
            if (`BE.exception_o) begin
                zenith_trace_commit( 32'd1, `BE.trap_iaddress, `BE.exception_vector, 32'd0, 32'd0, 32'd0, 32'd0, 32'd0, 32'd0, 32'd2 );
            end
        end
    end


// ============================================================================
//      MANUAL-CONTROL SCAFFOLDING (future feature, not implemented yet)
// ============================================================================
//
// The harness owns the clock and can already pause between ticks. To let it
// poke state or inject stimulus while paused, add export "DPI-C" functions
// here that drive `dut` hierarchical signals, e.g.:
//
//     export "DPI-C" function zenith_poke_ddr;    // already covered by preload
//     export "DPI-C" function zenith_force_irq;   // drive an interrupt source
//     export "DPI-C" function zenith_write_mem;   // drive an interrupt source
//     export "DPI-C" function zenith_peek_gpr;    // read register_file iregister
// ============================================================================

endmodule : zenith_tb_top

`endif
