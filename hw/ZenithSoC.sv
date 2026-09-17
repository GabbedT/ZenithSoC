`ifndef ZENITH_SOC_SV
    `define ZENITH_SOC_SV

/* The current top-level configuration enables every integrated peripheral.
 * Keep these feature macros at compilation-unit scope so all tools see the
 * same configuration. */
`define _DEF_UART_
`define _DEF_TIMER_
`define _DEF_GPIO_
`define _DEF_SPI_
`define _DEF_PRNG_
`define _DEF_ETHERNET_
`define _DEF_APU_
`define _DEF_SD_
`define _DEF_VGA_
`define _DEF_DDR_MEMORY_

module ZenithSoC #(
    parameter UART = 1,

    parameter TIMER = 1,

    parameter GPIO = 1,

    parameter SPI = 1,

    parameter ETHERNET = 1,

    parameter PRNG = 1,

    parameter APU = 1,

    parameter SD = 1,

    parameter VGA = 1,

    parameter DDR_MEMORY = 1,

    parameter string BOOT_INIT_FILE = "bootloader.hex"
) (
    input logic clk_i,
    input logic rst_n_i,

    /* GPIO pins */
    inout logic [GPIO_DEVICE_NUMBER - 1:0][7:0] pin_io,

    /* UART pins */
    input logic [UART_DEVICE_NUMBER - 1:0] uart_rx_i,
    input logic [UART_DEVICE_NUMBER - 1:0] uart_cts_i,
    output logic [UART_DEVICE_NUMBER - 1:0] uart_tx_o,
    output logic [UART_DEVICE_NUMBER - 1:0] uart_rts_o,

    /* SPI pins */
    output logic [SPI_DEVICE_NUMBER - 1:0] spi_sclk_o,
    output logic [SPI_DEVICE_NUMBER - 1:0][SPI_SLAVES - 1:0] spi_cs_n_o,
    output logic [SPI_DEVICE_NUMBER - 1:0] spi_mosi_o,
    input logic [SPI_DEVICE_NUMBER - 1:0] spi_miso_i,

    /* RMII Interface */
    inout logic [1:0] rmii_rxd_io,
    inout logic rmii_crsdv_io,
    input logic rmii_rxer_i,
    output logic [1:0] rmii_txd_o,
    output logic rmii_txen_o,
    output logic rmii_refclk_o,
    output logic rmii_rstn_o,

    /* SMI interface */
    output logic smi_mdc_o,
    inout logic smi_mdio_io,

    /* PDM Interface */
    input logic pdm_data_i,
    output logic pdm_clk_o,
    output logic pdm_lrsel_o,

    /* PWM Interface */
    inout logic pwm_o,
    output logic audio_enable_o,

    /* PWM from timer */
    output logic tmr_pwm_o,

    /* SD Interface */
    input logic sd_cd_n_i,
    inout logic sd_cmd_io,
    inout logic [3:0] sd_data_io,
    output logic sd_reset_o,
    output logic sd_clk_o,

    /* VGA Interface */
    output logic vga_hsync_o,
    output logic vga_vsync_o,
    output logic [3:0] vga_red_o,
    output logic [3:0] vga_green_o,
    output logic [3:0] vga_blue_o,

    /* DDR Interface */
    inout logic [15:0] ddr2_dq,
    inout logic [1:0] ddr2_dqs_n,
    inout logic [1:0] ddr2_dqs_p,
    output logic [1:0] ddr2_dm,
    output logic [12:0] ddr2_addr,
    output logic [2:0] ddr2_ba,
    output logic ddr2_ras_n,
    output logic ddr2_cas_n,
    output logic ddr2_we_n,
    output logic ddr2_cke,
    output logic ddr2_ck_p,
    output logic ddr2_ck_n,
    output logic ddr2_cs_n,
    output logic ddr2_odt
);

//====================================================================================
//      CLOCKING AND RESET
//====================================================================================

    logic sys_clk, ddr_clk, locked, ddr_ready;

    `ifdef VERILATOR

        assign sys_clk = clk_i;
        assign ddr_clk = clk_i;

        assign locked = 1'b1;
        assign ddr_ready = 1'b1;

    `elsif _DEF_DDR_MEMORY_

        clock_source clock_generator (
            /* External clock source */
            .ext_clk_i ( clk_i ),

            /* Generated clocks */
            .sys_clk_o ( sys_clk ),
            .mem_clk_o ( ddr_clk ),

            /* PLL is locked */
            .locked_o ( locked )
        );

    `else
        
        assign locked = 1'b1;

        assign sys_clk = clk_i;

        assign ddr_ready = 1'b1;

    `endif



    /* System reset syncronizer */
    logic reset_n, rst_sync;

        always_ff @(posedge sys_clk or negedge rst_n_i) begin 
            if (!rst_n_i) begin
                rst_sync <= 1'b0;
                reset_n <= 1'b0;
            end else if (!locked) begin
                rst_sync <= 1'b0;
                reset_n <= 1'b0;
            end else begin
                rst_sync <= rst_n_i;
                reset_n <= rst_sync;
            end 
        end 



    /* DDR reset syncronizer */
    logic ddr_reset_n, ddr_rst_sync;

    `ifdef _DEF_DDR_MEMORY_

        always_ff @(posedge ddr_clk or negedge rst_n_i) begin 
            if (!rst_n_i) begin
                ddr_rst_sync <= 1'b0;
                ddr_reset_n <= 1'b0;
            end else if (!locked) begin 
                ddr_rst_sync <= 1'b0;
                ddr_reset_n <= 1'b0;
            end else if (locked) begin 
                ddr_rst_sync <= rst_n_i;
                ddr_reset_n <= ddr_rst_sync;
            end 
        end 

    `endif


//====================================================================================
//      CPU COMPLEX
//====================================================================================

    /* Memory channel */
    load_interface ddr_load_channel();
    store_interface ddr_store_channel();
    fetch_interface rom_fetch_channel();

    /* IO Device channel */
    load_interface io_load_channel();
    store_interface io_store_channel();

    /* CPU Trace channel */
    trace_interface trace_channel();

    logic single_store_trx, load_instruction, load_trx_room, store_trx_idle;

    /* Interrupt nets */
    logic interrupt, general_interrupt, nmsk_interrupt, timer_interrupt, interrupt_ackn;
    logic [7:0] int_vector;

    /* CPU Halt */
    logic halt_core;

    cpu_complex #(
        .PREDICTOR_SIZE          ( PREDICTOR_SIZE          ),
        .BTB_SIZE                ( BTB_SIZE                ),
        .STORE_BUFFER_SIZE       ( STORE_BUFFER_SIZE       ),
        .INSTRUCTION_BUFFER_SIZE ( INSTRUCTION_BUFFER_SIZE ),
        .ROB_DEPTH               ( ROB_DEPTH               ),
        .DCACHE_SIZE             ( DCACHE_SIZE             ),
        .DBLOCK_SIZE_BYTE        ( DBLOCK_SIZE_BYTE        ),
        .ICACHE_SIZE             ( ICACHE_SIZE             ),
        .IBLOCK_SIZE_BYTE        ( IBLOCK_SIZE_BYTE        )
    ) ApogeoRV (
        .clk_i   ( sys_clk   ),
        .rst_n_i ( reset_n   ),
        .halt_i  ( halt_core ),

        /* Trace channel */
        .trace_channel ( trace_channel ),
        
        /* ROM channel */
        .rom_fetch_channel ( rom_fetch_channel ),

        /* Cache to DDR channels */
        .ddr_load_channel  ( ddr_load_channel  ), 
        .ddr_store_channel ( ddr_store_channel ),

        /* MMIO channels */
        .io_load_channel  ( io_load_channel  ), 
        .io_store_channel ( io_store_channel ),

        /* Store transaction is not burst */
        .single_strx_o ( single_store_trx ),
        .instr_load_o  ( load_instruction ),

        /* Room for load requests */
        .ldr_ready_i ( load_trx_room ),
        .str_ready_i ( store_trx_idle ),

        /* Interrupts */
        .gen_interrupt_i    ( general_interrupt ),
        .nmsk_interrupt_i   ( nmsk_interrupt    ),
        .timer_interrupt_i  ( timer_interrupt   ),
        .interrupt_vector_i ( int_vector        ),
        .interrupt_ackn_o   ( interrupt_ackn    )
    );


//====================================================================================
//      INTERRUPT CONTROLLER
//====================================================================================

    logic [INTERRUPT_SOURCES - 1:0] interrupt_source;

    interrupt_controller #(INTERRUPT_SOURCES) interrupt_controller (
        .clk_i   ( sys_clk   ),
        .rst_n_i ( reset_n ),

        .interrupt_i ( interrupt_source ),

        .acknowledge_i ( interrupt_ackn                              ),
        .interrupt_o   ( interrupt                                   ),
        .vector_o      ( int_vector[$clog2(INTERRUPT_SOURCES) - 1:0] )
    );

    assign timer_interrupt = (int_vector[$clog2(INTERRUPT_SOURCES) - 1:0] == TIMER_IRQ) & interrupt;

    assign nmsk_interrupt = (int_vector[$clog2(INTERRUPT_SOURCES) - 1:0] == BUS_ERROR_IRQ) & interrupt;

    assign general_interrupt = interrupt & !timer_interrupt & !nmsk_interrupt;

    assign int_vector[7:$clog2(INTERRUPT_SOURCES)] = '0;


//====================================================================================
//      AXI INTERCONNECTION
//====================================================================================

    /* Master nets */
    logic write_bus_error, read_bus_error; logic [3:0] master_write_strobe; logic write_cts, read_cts;

    /* Slave nets */
    logic [NETWORK_DEVICES - 1:0] write_error, write_done, write_busy, write_ready, write_request; 
    logic [NETWORK_DEVICES - 1:0][31:0] write_address, write_data; logic [NETWORK_DEVICES - 1:0][3:0] write_strobe;

    logic [NETWORK_DEVICES - 1:0] read_error, read_done, read_busy, read_ready, read_request; 
    logic [NETWORK_DEVICES - 1:0][31:0] read_address, read_data;

    logic store_pending, load_pending;

    axi_network #(NETWORK_DEVICES, LOW_SLAVE_ADDRESS, HIGH_SLAVE_ADDRESS) interconnection (
        .axi_ACLK    ( sys_clk   ),
        .axi_ARESETN ( reset_n ),

        .axi_write_error_o ( write_bus_error ),
        .axi_read_error_o  ( read_bus_error  ),

        .write_start_i   ( io_store_channel.request | store_pending ),
        .write_address_i ( io_store_channel.address                 ),
        .write_data_i    ( io_store_channel.data                    ),
        .write_strobe_i  ( master_write_strobe                      ),
        .write_done_o    ( io_store_channel.done                    ),
        .write_cts_o     ( write_cts                                ),

        .read_start_i   ( io_load_channel.request | load_pending   ),
        .read_invalid_i ( io_load_channel.invalidate               ),
        .read_address_i ( io_load_channel.address                  ),
        .read_data_o    ( io_load_channel.data                     ),
        .read_done_o    ( io_load_channel.valid                    ),
        .read_cts_o     ( read_cts                                 ),

        .write_error_i   ( write_error   ),
        .write_done_i    ( write_done    ),
        .write_busy_i    ( write_busy    ),
        .write_ready_i   ( write_ready   ),
        .write_address_o ( write_address ),
        .write_data_o    ( write_data    ),
        .write_strobe_o  ( write_strobe  ),
        .write_request_o ( write_request ),

        .read_data_i    ( read_data    ),
        .read_error_i   ( read_error   ),
        .read_done_i    ( read_done    ),
        .read_busy_i    ( read_busy    ),
        .read_ready_i   ( read_ready   ),
        .read_address_o ( read_address ),
        .read_request_o ( read_request )
    );


        always_ff @(posedge sys_clk `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                load_pending <= 1'b0;
                store_pending <= 1'b0;
            end else begin 
                if (!read_cts) begin
                    load_pending <= io_load_channel.request;
                end else begin
                    load_pending <= 1'b0;
                end
                
                if (!write_cts) begin
                    store_pending <= io_store_channel.request;
                end else begin
                    store_pending <= 1'b0;
                end
            end 
        end 

    assign interrupt_source[BUS_ERROR_IRQ] = write_bus_error | read_bus_error;

        always_comb begin
            /* Default values */
            master_write_strobe = '0;

            case (io_store_channel.width)
                WORD: begin 
                    master_write_strobe = '1;
                end

                HALF_WORD: begin 
                    master_write_strobe = 2'b11 << ({io_store_channel.address[1], 1'b0});
                end 

                BYTE: begin
                    master_write_strobe = 1'b1 << {io_store_channel.address[1:0]};
                end 
            endcase 
        end


//====================================================================================
//      UART
//====================================================================================

    localparam _UART_ = 1;

    logic [UART_DEVICE_NUMBER - 1:0] uart_interrupt;
    logic uart_tx_full;

    /* From trace unit */
    logic [7:0] trace_chunk; logic write_chunk;


    genvar i;


    `ifdef _DEF_UART_

    generate

        for (i = 0; i < UART_DEVICE_NUMBER; ++i) begin
            uart #(
                .RX_BUFFER_SIZE ( UART_RX_BUFFER_SIZE ),
                .TX_BUFFER_SIZE ( UART_TX_BUFFER_SIZE )
            ) uart_device (
                .clk_i       ( sys_clk ),
                .rst_n_i     ( reset_n ),

                .interrupt_o ( uart_interrupt[i] ),

                .uart_tx_full_o ( uart_tx_full ),

                .trace_data_i  ( trace_chunk                   ),
                .trace_write_i ( (i == 0) ? write_chunk : 1'b0 ),

                .uart_rx_i  ( uart_rx_i[i]  ),
                .uart_tx_o  ( uart_tx_o[i]  ),
                .uart_rts_o ( uart_rts_o[i] ),
                .uart_cts_i ( uart_cts_i[i] ),

                .write_i         ( write_request[_UART_ + i]      ),
                .write_address_i ( write_address[_UART_ + i] >> 2 ),
                .write_data_i    ( write_data[_UART_ + i]         ),
                .write_strobe_i  ( write_strobe[_UART_ + i]       ),
                .write_error_o   ( write_error[_UART_ + i]        ),
                .write_done_o    ( write_done[_UART_ + i]         ),

                .read_i         ( read_request[_UART_ + i]      ),
                .read_address_i ( read_address[_UART_ + i] >> 2 ),
                .read_data_o    ( read_data[_UART_ + i]         ),
                .read_error_o   ( read_error[_UART_ + i]        ),
                .read_done_o    ( read_done[_UART_ + i]         )
            );

            assign write_busy[_UART_ + i] = 1'b0;
            assign write_ready[_UART_ + i] = 1'b1;

            assign read_busy[_UART_ + i] = 1'b0;
            assign read_ready[_UART_ + i] = 1'b1;
        end 

    endgenerate

        assign interrupt_source[UART_IRQ] = uart_interrupt != '0;

    `else
        
        assign interrupt_source[UART_IRQ] = '0;

        assign write_busy[_UART_] = 1'b0;
        assign write_ready[_UART_] = 1'b1;
        assign read_busy[_UART_] = 1'b0;
        assign read_ready[_UART_] = 1'b1;

        assign read_done[_UART_] = read_request[_UART_];
        assign read_error[_UART_] = 1'b0;
        assign write_done[_UART_] = write_request[_UART_];
        assign write_error[_UART_] = 1'b0;

    `endif


//====================================================================================
//      TIMER
//====================================================================================

    localparam _TIMER_ = _UART_ + UART_DEVICE_NUMBER;

    logic [TIMER_DEVICE_NUMBER - 1:0] timers_interrupt;


    `ifdef _DEF_TIMER_

    generate
 
        for (i = 0; i < TIMER_DEVICE_NUMBER; ++i) begin
            timer timer_device (
                .clk_i   ( sys_clk ),
                .rst_n_i ( reset_n ),

                .pwm_o ( tmr_pwm_o ),

                .write_i         ( write_request[_TIMER_ + i]      ),
                .write_data_i    ( write_data[_TIMER_ + i]         ),
                .write_address_i ( write_address[_TIMER_ + i] >> 2 ),
                .write_error_o   ( write_error[_TIMER_ + i]        ),
                .write_strobe_i  ( write_strobe[_TIMER_ + i]       ),

                .read_i         ( read_request[_TIMER_ + i]      ),
                .read_address_i ( read_address[_TIMER_ + i] >> 2 ),
                .read_data_o    ( read_data[_TIMER_ + i]         ),
                .read_error_o   ( read_error[_TIMER_ + i]        ),

                .interrupt_o ( timers_interrupt[i] )
            );

            assign write_busy[_TIMER_+ i] = 1'b0;
            assign write_ready[_TIMER_+ i] = 1'b1;
            assign write_done[_TIMER_+ i] = write_request[_TIMER_+ i];

            assign read_busy[_TIMER_+ i] = 1'b0;
            assign read_ready[_TIMER_+ i] = 1'b1;
            assign read_done[_TIMER_+ i] = read_request[_TIMER_+ i];
        end 

    endgenerate

    assign interrupt_source[TIMER_IRQ] = timers_interrupt != '0;

    `else 
        
    assign interrupt_source[TIMER_IRQ] = '0;

    assign write_busy[_TIMER_] = 1'b0;
    assign write_ready[_TIMER_] = 1'b1;
    assign read_busy[_TIMER_] = 1'b0;
    assign read_ready[_TIMER_] = 1'b1;

    assign read_done[_TIMER_] = read_request[_TIMER_];
    assign read_error[_TIMER_] = 1'b0;
    assign write_done[_TIMER_] = write_request[_TIMER_];
    assign write_error[_TIMER_] = 1'b0;

    assign tmr_pwm_o = 1'b0;
        
    `endif


//====================================================================================
//      GPIO
//====================================================================================

    localparam _GPIO_ = _TIMER_ + TIMER_DEVICE_NUMBER;

    genvar j; logic [GPIO_DEVICE_NUMBER - 1:0][7:0] gpio_interrupt;


    `ifdef _DEF_GPIO_

    generate

        for (i = 0; i < GPIO_DEVICE_NUMBER; ++i) begin
            for (j = 0; j < 8; ++j) begin : gpio_gen
                gpio gpio_device (
                    .clk_i      ( sys_clk ),
                    .rst_n_i    ( reset_n ),

                    .pin_io ( pin_io[i][j] ),

                    /* Write interface */
                    .write_i         ( write_request[_GPIO_ + i]      ),
                    .write_address_i ( write_address[_GPIO_ + i] >> 2 ),
                    .write_data_i    ( write_data[_GPIO_ + i][j]      ),
                    .write_error_o   (                              ),

                    /* Read interface */
                    .read_i         ( read_request[_GPIO_ + i]      ),
                    .read_address_i ( read_address[_GPIO_ + i] >> 2 ),
                    .read_data_o    ( read_data[_GPIO_ + i][j]      ),
                    .read_error_o   (                              ),

                    .interrupt_o ( gpio_interrupt[i][j] )
                );
            end : gpio_gen

            assign read_data[_GPIO_ + i][31:8] = '0;

            assign write_busy[_GPIO_ + i] = 1'b0;
            assign write_ready[_GPIO_ + i] = 1'b1;
            assign write_error[_GPIO_ + i] = 1'b0;
            assign write_done[_GPIO_ + i] = write_request[_GPIO_ + i];

            assign read_busy[_GPIO_ + i] = 1'b0;
            assign read_ready[_GPIO_ + i] = 1'b1;
            assign read_error[_GPIO_ + i] = 1'b0;
            assign read_done[_GPIO_ + i] = read_request[_GPIO_ + i];
        end 
        
    endgenerate

    assign interrupt_source[GPIO_IRQ] = gpio_interrupt != '0;

    `else
        
    assign interrupt_source[GPIO_IRQ] = 0;

    assign write_busy[_GPIO_] = 1'b0;
    assign write_ready[_GPIO_] = 1'b1;
    assign read_busy[_GPIO_] = 1'b0;
    assign read_ready[_GPIO_] = 1'b1;

    assign read_done[_GPIO_] = read_request[_GPIO_];
    assign read_error[_GPIO_] = 1'b0;
    assign write_done[_GPIO_] = write_request[_GPIO_];
    assign write_error[_GPIO_] = 1'b0;

    `endif
    
    
//====================================================================================
//      SPI
//====================================================================================

    localparam _SPI_ = _GPIO_ + GPIO_DEVICE_NUMBER;

    logic [SPI_DEVICE_NUMBER - 1:0] spi_interrupt;


    `ifdef _DEF_SPI_

    generate
        
        for (i = 0; i < SPI_DEVICE_NUMBER; ++i) begin
            spi #(
                .RX_BUFFER_SIZE ( SPI_RX_BUFFER_SIZE ), 
                .TX_BUFFER_SIZE ( SPI_TX_BUFFER_SIZE ),
                .SLAVES         ( SPI_SLAVES         )
            ) spi_device (
                .clk_i       ( sys_clk ),
                .rst_n_i     ( reset_n ),

                .interrupt_o ( spi_interrupt[i] ),

                .sclk_o ( spi_sclk_o ),
                .cs_n_o ( spi_cs_n_o ),
                .mosi_o ( spi_mosi_o ),
                .miso_i ( spi_miso_i ),

                .write_i         ( write_request[_SPI_ + i]      ),
                .write_address_i ( write_address[_SPI_ + i] >> 2 ),
                .write_data_i    ( write_data[_SPI_ + i]         ),
                .write_strobe_i  ( write_strobe[_SPI_ + i]       ),
                .write_error_o   ( write_error[_SPI_ + i]        ),
                .write_done_o    ( write_done[_SPI_ + i]         ),

                .read_i         ( read_request[_SPI_ + i]      ),
                .read_address_i ( read_address[_SPI_ + i] >> 2 ),
                .read_data_o    ( read_data[_SPI_ + i]         ),
                .read_error_o   ( read_error[_SPI_ + i]        ),
                .read_done_o    ( read_done[_SPI_ + i]         )
            );

            assign write_busy[_SPI_ + i] = 1'b0;
            assign write_ready[_SPI_ + i] = 1'b1;

            assign read_busy[_SPI_ + i] = 1'b0;
            assign read_ready[_SPI_ + i] = 1'b1;
        end 

    assign interrupt_source[SPI_IRQ] = spi_interrupt != '0;

    endgenerate

    `else
    
    assign interrupt_source[SPI_IRQ] = '0;

    assign write_busy[_SPI_] = 1'b0;
    assign write_ready[_SPI_] = 1'b1;
    assign read_busy[_SPI_] = 1'b0;
    assign read_ready[_SPI_] = 1'b1;

    assign read_done[_SPI_] = read_request[_SPI_];
    assign read_error[_SPI_] = 1'b0;
    assign write_done[_SPI_] = write_request[_SPI_];
    assign write_error[_SPI_] = 1'b0;

    `endif
    
    
//====================================================================================
//      ETHERNET
//====================================================================================

    localparam _ETHERNET_ = _SPI_ + SPI_DEVICE_NUMBER;

    logic ethernet_busy;


    `ifdef _DEF_ETHERNET_

        ethernet #(
            .CHIP_PHY_ADDRESS ( ETH_PHY_ADDRESS    ),
            .MAC_ADDRESS      ( ETH_MAC_ADDRESS    ),
            .TX_BUFFER_SIZE   ( ETH_TX_BUFFER_SIZE ),
            .RX_BUFFER_SIZE   ( ETH_RX_BUFFER_SIZE ),
            .TX_PACKETS       ( ETH_TX_PACKETS     ),
            .RX_PACKETS       ( ETH_RX_PACKETS     )
        ) ethernet_mac (
            .clk_i       ( sys_clk ),
            .rst_n_i     ( reset_n ),

            .busy_o ( ethernet_busy ),

            .interrupt_o ( interrupt_source[ETHERNET_IRQ] ),

            .write_i         ( write_request[_ETHERNET_]      ),
            .write_address_i ( write_address[_ETHERNET_] >> 2 ),
            .write_data_i    ( write_data[_ETHERNET_]         ),
            .write_error_o   ( write_error[_ETHERNET_]        ),
            .write_done_o    ( write_done[_ETHERNET_]         ),

            .read_i         ( read_request[_ETHERNET_]      ),
            .read_address_i ( read_address[_ETHERNET_] >> 2 ),
            .read_data_o    ( read_data[_ETHERNET_]         ),
            .read_error_o   ( read_error[_ETHERNET_]        ),
            .read_done_o    ( read_done[_ETHERNET_]         ),

            .phy_interrupt_i ( 1'b0         ),
            .rmii_rxd_io     ( rmii_rxd_io       ),
            .rmii_crsdv_io   ( rmii_crsdv_io     ),
            .rmii_rxer_i     ( rmii_rxer_i       ),
            .rmii_txd_o      ( rmii_txd_o        ),
            .rmii_txen_o     ( rmii_txen_o       ),
            .rmii_refclk_o   ( rmii_refclk_o     ),
            .rmii_rstn_o     ( rmii_rstn_o       ),

            .smii_mdc_o   ( smi_mdc_o   ),
            .smii_mdio_io ( smi_mdio_io )
        );

        assign write_busy[_ETHERNET_] = ethernet_busy;
        assign write_ready[_ETHERNET_] = !ethernet_busy;

        assign read_busy[_ETHERNET_] = ethernet_busy;
        assign read_ready[_ETHERNET_] = !ethernet_busy;

    `else
        
        assign write_busy[_ETHERNET_] = 1'b0;
        assign write_ready[_ETHERNET_] = 1'b1;
        assign read_busy[_ETHERNET_] = 1'b0;
        assign read_ready[_ETHERNET_] = 1'b1;


        assign read_done[_ETHERNET_] = read_request[_ETHERNET_];
        assign read_error[_ETHERNET_] = 1'b0;
        assign write_done[_ETHERNET_] = write_request[_ETHERNET_];
        assign write_error[_ETHERNET_] = 1'b0;

    `endif


//====================================================================================
//      PSEUDO RANDOM NUMBER GENERATOR
//====================================================================================

    localparam _PRNG_ = _ETHERNET_ + PRNG_NUMBER;


    `ifdef _DEF_PRNG_

        prng random_generator (
            .clk_i   ( sys_clk ),
            .rst_n_i ( reset_n ),

            /* Write interface */
            .write_i         ( write_request[_PRNG_] ),
            .write_data_i    ( write_data[_PRNG_]    ),
            .write_address_i ( write_address[_PRNG_] ),
            .write_done_o    ( write_done[_PRNG_]    ),
        
            /* Read interface */
            .read_i         ( read_request[_PRNG_] ),
            .read_address_i ( read_address[_PRNG_] ),
            .read_done_o    ( read_done[_PRNG_]    ),
            .read_data_o    ( read_data[_PRNG_]    )
        );

        assign write_busy[_PRNG_] = 1'b0;
        assign write_ready[_PRNG_] = 1'b1;
        assign write_error[_PRNG_] = 1'b0;

        assign read_busy[_PRNG_] = 1'b0;
        assign read_ready[_PRNG_] = 1'b1;
        assign read_error[_PRNG_] = 1'b0;

    `else 
        
        assign write_busy[_PRNG_] = 1'b0;
        assign write_ready[_PRNG_] = 1'b1;
        assign read_busy[_PRNG_] = 1'b0;
        assign read_ready[_PRNG_] = 1'b1;


        assign read_done[_PRNG_] = read_request[_PRNG_];
        assign read_error[_PRNG_] = 1'b0;
        assign write_done[_PRNG_] = write_request[_PRNG_];
        assign write_error[_PRNG_] = 1'b0;

    `endif
    

//====================================================================================
//      BOOT ROM
//====================================================================================

    localparam _BOOT_ = 0;

    on_chip_memory #(BOOT_SIZE, BOOT_INIT_FILE) boot_memory (
        .clk_i   ( sys_clk ),
        .rst_n_i ( reset_n ),

        .store_i         ( write_request[_BOOT_] ),
        .store_address_i ( write_address[_BOOT_] ),
        .store_data_i    ( write_data[_BOOT_]    ),
        .store_width_i   ( write_strobe[_BOOT_]  ),
        .store_done_o    ( write_done[_BOOT_]    ),

        .load_i         ( read_request[_BOOT_]       ),
        .load_address_i ( read_address[_BOOT_] >> 2  ),
        .load_data_o    ( read_data[_BOOT_]          ),
        .load_done_o    ( read_done[_BOOT_]          ),
        .load_invalid_i ( io_load_channel.invalidate ),

        .fetch_i         ( rom_fetch_channel.fetch        ),
        .invalidate_i    ( rom_fetch_channel.invalidate   ),
        .fetch_address_i ( rom_fetch_channel.address >> 2 ),
        .instruction_o   ( rom_fetch_channel.instruction  ),
        .fetch_done_o    ( rom_fetch_channel.valid        )
    );

    assign rom_fetch_channel.stall = 1'b0;
    
    assign write_busy[_BOOT_] = 1'b0;
    assign write_ready[_BOOT_] = 1'b1;
    assign write_error[_BOOT_] = 1'b0;

    assign read_busy[_BOOT_] = 1'b0;
    assign read_ready[_BOOT_] = 1'b1;
    assign read_error[_BOOT_] = 1'b0;


//====================================================================================
//      AUDIO PROCESSING UNIT
//====================================================================================

    localparam _APU_ = _PRNG_ + 1;


    `ifdef _DEF_APU_

        apu #(APU_SAMPLE_BUFFER_SIZE) audio_processing_unit (
            .clk_i   ( sys_clk ),
            .rst_n_i ( reset_n ),

            .interrupt_o ( interrupt_source[APU_IRQ] ),
            
            .write_i         ( write_request[_APU_]      ),
            .write_address_i ( write_address[_APU_] >> 2 ),
            .write_data_i    ( write_data[_APU_]         ),
            .write_strobe_i  ( write_strobe[_APU_]       ),
            .write_done_o    ( write_done[_APU_]         ),
            .write_error_o   ( write_error[_APU_]        ),
            
            .read_i         ( read_request[_APU_]      ),
            .read_address_i ( read_address[_APU_] >> 2 ),
            .read_data_o    ( read_data[_APU_]         ),
            .read_done_o    ( read_done[_APU_]         ),
            .read_error_o   ( read_error[_APU_]        ),
            
            .pdm_data_i  ( pdm_data_i  ),
            .pdm_clk_o   ( pdm_clk_o   ),
            .pdm_lrsel_o ( pdm_lrsel_o ),
            
            .pwm_o          ( pwm_o          ),
            .audio_enable_o ( audio_enable_o )
        );

        assign write_busy[_APU_] = 1'b0;
        assign write_ready[_APU_] = 1'b1;

        assign read_busy[_APU_] = 1'b0;
        assign read_ready[_APU_] = 1'b1;

    `else

        assign write_busy[_APU_] = 1'b0;
        assign write_ready[_APU_] = 1'b1;
        assign read_busy[_APU_] = 1'b0;
        assign read_ready[_APU_] = 1'b1;


        assign read_done[_APU_] = read_request[_APU_];
        assign read_error[_APU_] = 1'b0;
        assign write_done[_APU_] = write_request[_APU_];
        assign write_error[_APU_] = 1'b0;

    `endif


//====================================================================================
//      NON CACHABLE MEMORY
//====================================================================================

    localparam _NC_MEM_ = _APU_ + 1;

    assign write_busy[_NC_MEM_] = 1'b0;
    assign write_ready[_NC_MEM_] = 1'b1;
    assign write_error[_NC_MEM_] = 1'b0;

    assign read_busy[_NC_MEM_] = 1'b0;
    assign read_ready[_NC_MEM_] = 1'b1;
    assign read_error[_NC_MEM_] = 1'b0;


    logic load; 
    
    assign load = read_request[_NC_MEM_] & !io_load_channel.invalidate;

        always_ff @(posedge sys_clk `ifdef ASYNC or negedge reset_n `endif) begin
            if (!reset_n) begin
                read_done[_NC_MEM_] <= 1'b0;
            end else begin 
                read_done[_NC_MEM_] <= load;
            end 
        end


        always_ff @(posedge sys_clk `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin
                write_done[_NC_MEM_] <= 1'b0;
            end else begin 
                write_done[_NC_MEM_] <= write_request[_NC_MEM_];
            end 
        end


    logic [31:0] on_chip_ram [NC_MEMORY_SIZE / 4];

        always_ff @(posedge sys_clk) begin
            if (write_request[_NC_MEM_] & write_strobe[_NC_MEM_]) begin
                for (int k = 0; k < 4; ++k) begin
                    if (write_strobe[_NC_MEM_][k]) begin
                        on_chip_ram[write_address[_NC_MEM_][$clog2(NC_MEMORY_SIZE) - 1:2]][k*8 +: 8] <= write_data[_NC_MEM_][k*8 +: 8];
                    end
                end
            end

            if (read_request[_NC_MEM_]) begin
                read_data[_NC_MEM_] <= on_chip_ram[read_address[_NC_MEM_][$clog2(NC_MEMORY_SIZE) - 1:2]];
            end
        end


//====================================================================================
//      SD CONTROLLER
//====================================================================================

    localparam _SD_ = _NC_MEM_ + 1;


    `ifdef _DEF_SD_

        sd sd_controller (
            .clk_i   ( sys_clk ),
            .rst_n_i ( reset_n ),

            .interrupt_o ( interrupt_source[SD_IRQ] ),

            .write_i         ( write_request[_SD_]      ),
            .write_address_i ( write_address[_SD_] >> 2 ),
            .write_data_i    ( write_data[_SD_]         ),
            .write_strobe_i  ( write_strobe[_SD_]       ),
            .write_done_o    ( write_done[_SD_]         ),
            .write_error_o   ( write_error[_SD_]        ),
            
            .read_i         ( read_request[_SD_]      ),
            .read_address_i ( read_address[_SD_] >> 2 ),
            .read_data_o    ( read_data[_SD_]         ),
            .read_done_o    ( read_done[_SD_]         ),
            .read_error_o   ( read_error[_SD_]        ),

            .sd_cd_n_i    ( sd_cd_n_i  ),
            .sd_cmd_io    ( sd_cmd_io  ),
            .sd_data_io   ( sd_data_io ),
            .sd_reset_o   ( sd_reset_o ),
            .sd_clk_o     ( sd_clk_o   )
        );

        assign write_busy[_SD_] = 1'b0;
        assign write_ready[_SD_] = 1'b1;

        assign read_busy[_SD_] = 1'b0;
        assign read_ready[_SD_] = 1'b1;

    `else
        
        assign write_busy[_SD_] = 1'b0;
        assign write_ready[_SD_] = 1'b1;
        assign read_busy[_SD_] = 1'b0;
        assign read_ready[_SD_] = 1'b1;


        assign read_done[_SD_] = read_request[_SD_];
        assign read_error[_SD_] = 1'b0;
        assign write_done[_SD_] = write_request[_SD_];
        assign write_error[_SD_] = 1'b0;

    `endif


//====================================================================================
//      TRACE UNIT
//====================================================================================

    localparam _TRACE_UNIT_ = _SD_ + 1;

    trace_unit #( 
        .PACKET_BUFFER_SIZE ( TRACE_UNIT_BUFFER_SIZE )
    ) trace_unit (
        .clk_i   ( sys_clk ),
        .rst_n_i ( reset_n ),

        .interrupt_o ( interrupt_source[TRACE_IRQ] ),
        .halt_core_o ( halt_core                               ),

        .uart_tx_full_i ( uart_tx_full ),

        .write_i         ( write_request[_TRACE_UNIT_]      ),
        .write_address_i ( write_address[_TRACE_UNIT_] >> 2 ),
        .write_data_i    ( write_data[_TRACE_UNIT_]         ),
        .write_strobe_i  ( write_strobe[_TRACE_UNIT_]       ),
        .write_done_o    ( write_done[_TRACE_UNIT_]         ),
        .write_error_o   ( write_error[_TRACE_UNIT_]        ),
        
        .read_i         ( read_request[_TRACE_UNIT_]      ),
        .read_address_i ( read_address[_TRACE_UNIT_] >> 2 ),
        .read_data_o    ( read_data[_TRACE_UNIT_]         ),
        .read_done_o    ( read_done[_TRACE_UNIT_]         ),
        .read_error_o   ( read_error[_TRACE_UNIT_]        ),

        .trace_interface_i ( trace_channel ),

        .trace_chunk_o ( trace_chunk ),
        .write_chunk_o ( write_chunk )
    );

    assign write_busy[_TRACE_UNIT_] = 1'b0;
    assign write_ready[_TRACE_UNIT_] = 1'b1;

    assign read_busy[_TRACE_UNIT_] = 1'b0;
    assign read_ready[_TRACE_UNIT_] = 1'b1;


//====================================================================================
//      VGA CONTROLLER
//====================================================================================

    localparam _VGA_ = _TRACE_UNIT_ + 1;

    dev2ddr_interface vga_ddr_channel();
    logic vga_ddr_urgent;

    `ifdef _DEF_VGA_

        vga #(
            .BUFFER_SIZE ( VGA_LINE_BUFFER_SIZE )
        ) video_controller (
            .clk_i       ( sys_clk ),
            .rst_n_i     ( reset_n ),
            .interrupt_o ( interrupt_source[VGA_IRQ] ),

            .write_i         ( write_request[_VGA_]      ),
            .write_address_i ( write_address[_VGA_] >> 2 ),
            .write_data_i    ( write_data[_VGA_]         ),
            .write_strobe_i  ( write_strobe[_VGA_]       ),
            .write_error_o   ( write_error[_VGA_]        ),
            .write_done_o    ( write_done[_VGA_]         ),

            .read_i         ( read_request[_VGA_]      ),
            .read_address_i ( read_address[_VGA_] >> 2 ),
            .read_data_o    ( read_data[_VGA_]         ),
            .read_error_o   ( read_error[_VGA_]        ),
            .read_done_o    ( read_done[_VGA_]         ),

            .hsync_o ( vga_hsync_o ),
            .vsync_o ( vga_vsync_o ),
            .red_o   ( vga_red_o   ),
            .green_o ( vga_green_o ),
            .blue_o  ( vga_blue_o  ),

            .ddr_urgent_o ( vga_ddr_urgent ),
            .ddr_channel  ( vga_ddr_channel )
        );

        assign write_busy[_VGA_] = 1'b0;
        assign write_ready[_VGA_] = 1'b1;
        assign read_busy[_VGA_] = 1'b0;
        assign read_ready[_VGA_] = 1'b1;

    `else

        assign interrupt_source[VGA_IRQ] = 1'b0;
        assign vga_hsync_o = 1'b1;
        assign vga_vsync_o = 1'b1;
        assign vga_red_o = '0;
        assign vga_green_o = '0;
        assign vga_blue_o = '0;
        assign vga_ddr_urgent = 1'b0;

        assign write_busy[_VGA_] = 1'b0;
        assign write_ready[_VGA_] = 1'b1;
        assign read_busy[_VGA_] = 1'b0;
        assign read_ready[_VGA_] = 1'b1;
        assign read_done[_VGA_] = read_request[_VGA_];
        assign read_error[_VGA_] = 1'b0;
        assign read_data[_VGA_] = '0;
        assign write_done[_VGA_] = write_request[_VGA_];
        assign write_error[_VGA_] = 1'b0;

        assign vga_ddr_channel.trx_req = 1'b0;
        assign vga_ddr_channel.trx_type = 1'b0;
        assign vga_ddr_channel.trx_addr = '0;
        assign vga_ddr_channel.wdata = '0;
        assign vga_ddr_channel.wstrobe = '0;

    `endif


//====================================================================================
//      CACHE TO DDR INTERFACE
//====================================================================================

    dev2ddr_interface cache_ddr_channel();
    dev2ddr_interface memory_ddr_channel();
    logic ddr_hold;

    assign ddr_hold = !cache_ddr_channel.ready;

    cache_ddr_interface #(
        .DATA_MAX_BURST        ( DBLOCK_SIZE_BYTE / 4 ),
        .INSTRUCTION_MAX_BURST ( IBLOCK_SIZE_BYTE / 4 )
    ) ddr_controller_interface (
        .clk_i   ( sys_clk ),
        .rst_n_i ( reset_n ),

        .load_channel  ( ddr_load_channel  ),
        .store_channel ( ddr_store_channel ),

        .single_trx_i ( single_store_trx ),
        .instr_req_i  ( load_instruction ),
        .load_empty_o ( load_trx_room    ),
        .store_idle_o ( store_trx_idle   ),

        .ddr_channel ( cache_ddr_channel )
    );


//====================================================================================
//      DDR CONTROLLER
//====================================================================================

    ddr_arbiter ddr_request_arbiter (
        .clk_i   ( sys_clk ),
        .rst_n_i ( reset_n ),

        .cache_channel  ( cache_ddr_channel  ),
        .vga_channel    ( vga_ddr_channel    ),
        .memory_channel ( memory_ddr_channel ),

        .vga_urgent_i ( vga_ddr_urgent )
    );


    `ifdef VERILATOR
        `define _DDR_BEHAVIOURAL_
    `endif
    `ifndef _DEF_DDR_MEMORY_
        `define _DDR_BEHAVIOURAL_
    `endif


    `ifdef _DEF_DDR_MEMORY_  
    `ifdef _VIVADO_ 

        /** VIVADO ONLY **/
        
        ddr_memory_interface ddr_controller (
            .clk_i       ( sys_clk     ),
            .rst_n_i     ( reset_n     ),
            .mem_clk_i   ( ddr_clk     ),
            .mem_rst_n_i ( ddr_reset_n ),

            /* Data */
            .ddr2_dq_io ( ddr2_dq ),
            .ddr2_dm_o  ( ddr2_dm ),

            /* Data strobe pair */
            .ddr2_dqs_n_io ( ddr2_dqs_n ),
            .ddr2_dqs_p_io ( ddr2_dqs_p ),

            /* Addreses */
            .ddr2_addr_o ( ddr2_addr ),
            .ddr2_ba_o   ( ddr2_ba   ),

            /* Enable signals */
            .ddr2_ras_n_o ( ddr2_ras_n ),
            .ddr2_cas_n_o ( ddr2_cas_n ),
            .ddr2_we_n_o  ( ddr2_we_n  ),
            .ddr2_cke_o   ( ddr2_cke   ),

            /* Clock pair */
            .ddr2_ck_p_o ( ddr2_ck_p ),
            .ddr2_ck_n_o ( ddr2_ck_n ),

            /* Chip select */
            .ddr2_cs_n_o ( ddr2_cs_n ),
            .ddr2_odt_o  ( ddr2_odt  ),

            .ddr_channel ( memory_ddr_channel ),

            .start_o (           )
        );
        
    `endif // _VIVADO_
    `endif // _DEF_DDR_MEMORY_

    `ifdef _DDR_BEHAVIOURAL_

    /** USED FOR SIMULATION **/
    localparam int DDR_SIZE_BYTES  = 128 * 1024 * 1024;
    localparam int DDR_WORDS       = DDR_SIZE_BYTES / 8;
    localparam int MODEL_LATENCY   = 8;

    /* Memory to hold data (64-bit words) */
    logic [63:0] ddr_memory [0:DDR_WORDS-1];

    logic [$clog2(DDR_WORDS) - 1:0] ddr_word_address;
    logic [MODEL_LATENCY - 1:0] model_read_valid;
    logic [MODEL_LATENCY - 1:0] model_read_error;
    logic [MODEL_LATENCY - 1:0][127:0] model_read_data;
    logic model_write_valid, model_write_error;

    assign ddr_word_address = memory_ddr_channel.trx_addr[$clog2(DDR_WORDS)+2:3];
    assign memory_ddr_channel.ready = reset_n;
    assign memory_ddr_channel.read_valid = model_read_valid[MODEL_LATENCY - 1];
    assign memory_ddr_channel.read_error = model_read_error[MODEL_LATENCY - 1];
    assign memory_ddr_channel.rdata = model_read_data[MODEL_LATENCY - 1];
    assign memory_ddr_channel.write_valid = model_write_valid;
    assign memory_ddr_channel.write_error = model_write_error;

    always_ff @(posedge sys_clk or negedge reset_n) begin
        if (!reset_n) begin
            model_read_valid <= '0;
            model_read_error <= '0;
            model_read_data <= '0;
            model_write_valid <= 1'b0;
            model_write_error <= 1'b0;
        end else begin
            model_read_valid <= {model_read_valid[MODEL_LATENCY - 2:0], 1'b0};
            model_read_error <= {model_read_error[MODEL_LATENCY - 2:0], 1'b0};
            model_read_data <= {model_read_data[MODEL_LATENCY - 2:0], 128'b0};
            model_write_valid <= 1'b0;
            model_write_error <= 1'b0;

            if (memory_ddr_channel.trx_req & memory_ddr_channel.ready) begin
                if (!memory_ddr_channel.trx_type) begin
                    model_read_valid[0] <= 1'b1;
                    model_read_error[0] <= memory_ddr_channel.trx_addr[3:0] != 4'b0;
                    model_read_data[0] <= {
                        ddr_memory[ddr_word_address + 1'b1],
                        ddr_memory[ddr_word_address]
                    };
                end else begin
                    model_write_valid <= 1'b1;
                    model_write_error <= memory_ddr_channel.trx_addr[3:0] != 4'b0;

                    for (int i = 0; i < 16; i++) begin
                        if (memory_ddr_channel.wstrobe[i]) begin
                            ddr_memory[ddr_word_address + (i >> 3)]
                                      [8 * (i & 7) +: 8] <=
                                memory_ddr_channel.wdata[8 * i +: 8];
                        end
                    end
                end
            end
        end
    end

    `endif // _DDR_BEHAVIOURAL_

endmodule : ZenithSoC

`endif
