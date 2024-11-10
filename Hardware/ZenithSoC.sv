`ifndef ZENITH_SOC_SV
    `define ZENITH_SOC_SV

`include "Utility/Packages/soc_parameters.sv"

`include "CPU/ApogeoRV/Hardware/Include/Headers/apogeo_memory_map.svh"
`include "CPU/interrupt_controller.sv"
`include "CPU/cpu_complex.sv"

`include "Memory/on_chip_memory.sv"

`include "Bus/axi_network.sv"

`include "IO/GPIO/gpio.sv"
`include "IO/UART/uart.sv"
`include "IO/Timer/timer.sv"
`include "IO/SPI/spi.sv"
`include "IO/Ethernet/ethernet.sv"
`include "IO/PRNG/prng.sv"

`include "Memory/DDR/cache_ddr_interface.sv"
`include "Memory/DDR/ddr_memory_interface.sv"

`include "System/synchronizer.sv"
`include "System/clock_source.sv"

module ZenithSoC (
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

    clock_source clock_generator (
        /* External clock source */
        .ext_clk_i ( clk_i ),

        /* Generated clocks */
        .sys_clk_o ( sys_clk ),
        .mem_clk_o ( ddr_clk ),

        /* PLL is locked */
        .locked_o ( locked )
    );


    /* System reset syncronizer */
    logic reset_n, rst_sync;

        always_ff @(posedge sys_clk or negedge rst_n_i) begin 
            if (!rst_n_i) begin
                rst_sync <= 1'b0;
                reset_n <= 1'b0;
            end else if (!locked) begin 
                rst_sync <= 1'b0;
                reset_n <= 1'b0;
            end else if (locked & ddr_ready) begin 
                rst_sync <= rst_n_i;
                reset_n <= rst_sync;
            end 
        end 


    /* DDR reset syncronizer */
    logic ddr_reset_n, ddr_rst_sync;

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

    logic single_store_trx, load_instruction, load_trx_room;

    /* Interrupt nets */
    logic interrupt, nmsk_interrupt, timer_interrupt, interrupt_ackn;
    logic [7:0] int_vector;

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
        .clk_i   ( sys_clk ),
        .rst_n_i ( reset_n ),

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

        /* Interrupts */
        .gen_interrupt_i    ( interrupt       ),
        .nmsk_interrupt_i   ( nmsk_interrupt  ),
        .timer_interrupt_i  ( timer_interrupt ), 
        .interrupt_vector_i ( int_vector      ),
        .interrupt_ackn_o   ( interrupt_ackn  )
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

    assign timer_interrupt = (int_vector[$clog2(INTERRUPT_SOURCES) - 1:0] == 2) & interrupt;

    assign nmsk_interrupt = (int_vector[$clog2(INTERRUPT_SOURCES) - 1:0] == 3) & interrupt;

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

    assign interrupt_source[INTERRUPT_SOURCES - 1] = write_bus_error | read_bus_error;

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

    genvar i; logic [UART_DEVICE_NUMBER - 1:0] uart_interrupt;

    generate 
        for (i = 0; i < UART_DEVICE_NUMBER; ++i) begin
            uart #(
                .RX_BUFFER_SIZE ( UART_RX_BUFFER_SIZE ),
                .TX_BUFFER_SIZE ( UART_TX_BUFFER_SIZE )
            ) uart_device (
                .clk_i       ( sys_clk ),
                .rst_n_i     ( reset_n ),

                .interrupt_o ( uart_interrupt[i] ),

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

    assign interrupt_source[INTERRUPT_SOURCES - 3] = uart_interrupt != '0;


//====================================================================================
//      TIMER
//====================================================================================

    localparam _TIMER_ = _UART_ + UART_DEVICE_NUMBER;

    logic [TIMER_DEVICE_NUMBER - 1:0] timers_interrupt;

    generate 
        for (i = 0; i < TIMER_DEVICE_NUMBER; ++i) begin
            timer timer_device (
                .clk_i   ( sys_clk ),
                .rst_n_i ( reset_n ),

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

    assign interrupt_source[INTERRUPT_SOURCES - 2] = timers_interrupt != '0;


//====================================================================================
//      GPIO
//====================================================================================

    localparam _GPIO_ = _TIMER_ + TIMER_DEVICE_NUMBER;

    logic [GPIO_DEVICE_NUMBER - 1:0][7:0] gpio_interrupt;

    genvar j;

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

                    /* Read interface */
                    .read_address_i ( read_address[_GPIO_ + i] >> 2 ),
                    .read_data_o    ( read_data[_GPIO_ + i][j]      ),

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

    assign interrupt_source[INTERRUPT_SOURCES - 4] = gpio_interrupt != '0;


//====================================================================================
//      SPI
//====================================================================================

    localparam _SPI_ = _GPIO_ + GPIO_DEVICE_NUMBER;

    logic [SPI_DEVICE_NUMBER - 1:0] spi_interrupt;

    generate 
        for (i = 0; i < TIMER_DEVICE_NUMBER; ++i) begin
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
    endgenerate

    assign interrupt_source[INTERRUPT_SOURCES - 5] = spi_interrupt != '0;


//====================================================================================
//      ETHERNET
//====================================================================================

    localparam _ETHERNET_ = _SPI_ + SPI_DEVICE_NUMBER;

    logic ethernet_busy;

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

        .interrupt_o ( interrupt_source[INTERRUPT_SOURCES - 6] ),

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


//====================================================================================
//      PSEUDO RANDOM NUMBER GENERATOR
//====================================================================================

    localparam _PRNG_ = _ETHERNET_ + PRNG_NUMBER;

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
    

//====================================================================================
//      BOOT ROM
//====================================================================================

    localparam _BOOT_ = 0;

    on_chip_memory #(BOOT_SIZE, "/home/gabriele/Desktop/Projects/ZenithSoC/Software/Examples/Floating Point/output.hex") boot_memory (
        .clk_i      ( sys_clk ),
        .rst_n_i    ( reset_n ),

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

    assign write_busy[_BOOT_] = 1'b0;
    assign write_ready[_BOOT_] = 1'b1;
    assign write_error[_BOOT_] = 1'b0;

    assign read_busy[_BOOT_] = 1'b0;
    assign read_ready[_BOOT_] = 1'b1;
    assign read_error[_BOOT_] = 1'b0;


//====================================================================================
//      NON CACHABLE MEMORY
//====================================================================================

    localparam _NC_MEM_ = _PRNG_ + 1;

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


        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
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
//      DDR CONTROLLER
//====================================================================================

    logic [26:0] ddr_address; logic ddr_write, ddr_read, push_trx, pull_trx, ddr_data_valid, ddr_done; 
    logic [63:0] ddr_data_write, ddr_data_read; logic [7:0] ddr_mask; 

    cache_ddr_interface #(
        .DATA_MAX_BURST        ( DBLOCK_SIZE_BYTE / 4 ),
        .INSTRUCTION_MAX_BURST ( IBLOCK_SIZE_BYTE / 4 )
    ) ddr_controller_interface (
        .clk_i   ( sys_clk ),
        .rst_n_i ( reset_n ),

        /* Arbiter */
        .hold_i ( 1'b0 ),

        /* Memory interface */
        .load_channel  ( ddr_load_channel  ),
        .store_channel ( ddr_store_channel ),

        .single_trx_i ( single_store_trx ),
        .instr_req_i  ( load_instruction ),
        .load_empty_o ( load_trx_room    ),

        /* Common address */
        .address_o ( ddr_address ), 
        
        /* Command interface */
        .write_o ( ddr_write ), 
        .read_o  ( ddr_read  ), 

        /* Data interface */
        .push_o       ( push_trx       ), 
        .pull_o       ( pull_trx       ), 
        .write_data_o ( ddr_data_write ),
        .write_mask_o ( ddr_mask       ),
        .read_data_i  ( ddr_data_read  ),
        .read_valid_i ( ddr_data_valid ),

        /* Status */
        .done_o  ( ddr_done  ),
        .ready_i ( ddr_ready )
    );


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

        /* Common address */
        .address_i ( ddr_address ), 
        
        /* Command interface */
        .write_i ( ddr_write ), 
        .read_i  ( ddr_read  ), 

        /* Data interface */
        .push_i       ( push_trx       ), 
        .pull_i       ( pull_trx       ), 
        .write_data_i ( ddr_data_write ),
        .write_mask_i ( ddr_mask       ),
        .read_data_o  ( ddr_data_read  ),
        .read_valid_o ( ddr_data_valid ),

        /* Status */
        .done_i  ( ddr_done  ),
        .ready_o ( ddr_ready ),
        .start_o (           )
    );

endmodule : ZenithSoC

`endif 