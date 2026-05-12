`ifndef SOC_TESTBENCH_SV
    `define SOC_TESTBENCH_SV

`timescale 1ns/1ps

`include "../../Hardware/Utility/Packages/soc_parameters.sv"

// `include "../SD/Model/sd_top.v"
// `include "../SD/bram_whishbone.sv"

`include "/home/gabriele/Desktop/Projects/ZenithSoC/Testbench/SD/Model/sd_top.v"
`include "/home/gabriele/Desktop/Projects/ZenithSoC/Testbench/SD/bram_whishbone.sv"

`define CPU dut.ApogeoRV.system_cpu


// ============================================
//      TESTBENCH SETTINGS 
// ============================================

/* Enable or disable cpu instruction tracing */
// `define TRACE_CPU

/* Enable or disable memory accesses tracing */
// `define TRACE_MEMORY

/* Enable or disable STDOUT output tracing */
// `define TRACE_OUTPUT

/* Enable or disable PDM file read */
// `define AUDIO_PDM

/* Enable or disable SD model, this is used to simulate the SD card interface */
`define SD_MODEL

/* Simulation time, this is the maximum simulation time, if the RUN_CONDITION is true
 * simulation will continue until this time is reached */
`define SIM_TIME 1s

/* While RUN_CONDITION is true, simulation will continue */
`define RUN_CONDITION ($time() < `SIM_TIME) & !`CPU.exception

/* The program can write into the Non Cachable Memory, each write is considered as an
 * output write, if this define is removed, the writes to UART device are considered
 * as output to be written into the output_trace.txt */
// `define FAST_OUTPUT


// ============================================
//      TESTBENCH FILE PATHS 
// ============================================

`define CPU_TRACE_FILE "/home/gabriele/Desktop/Projects/ZenithSoC/Testbench/System/cpu_trace.txt"
`define MEMORY_TRACE_FILE "/home/gabriele/Desktop/Projects/ZenithSoC/Testbench/System/memory_trace.txt"
`define OUTPUT_TRACE_FILE "/home/gabriele/Desktop/Projects/ZenithSoC/Testbench/System/output_trace.txt"
`define PDM_FILE "/home/gabriele/Desktop/Projects/ZenithSoC/Testbench/PDM/pdm.txt"


module soc_testbench;

    logic clk_i = 0;

    /* Clock instantiation */
    always #5ns clk_i <= !clk_i;


    logic rst_n_i;

    /* GPIO pins */
    wire [GPIO_DEVICE_NUMBER - 1:0][7:0] pin_io;

    /* UART pins */
    logic [UART_DEVICE_NUMBER - 1:0] uart_rx_i;
    logic [UART_DEVICE_NUMBER - 1:0] uart_cts_i;
    logic [UART_DEVICE_NUMBER - 1:0] uart_tx_o;
    logic [UART_DEVICE_NUMBER - 1:0] uart_rts_o;

    /* SPI pins */
    logic [SPI_DEVICE_NUMBER - 1:0]                   spi_sclk_o;
    logic [SPI_DEVICE_NUMBER - 1:0][SPI_SLAVES - 1:0] spi_cs_n_o;
    logic [SPI_DEVICE_NUMBER - 1:0]                   spi_mosi_o;
    logic [SPI_DEVICE_NUMBER - 1:0]                   spi_miso_i;

    /* RMII Interface */
    wire [1:0]  rmii_rxd_io;
    wire        rmii_crsdv_io;
    logic       rmii_rxer_i;
    logic [1:0] rmii_txd_o;
    logic       rmii_txen_o;
    logic       rmii_refclk_o;
    logic       rmii_rstn_o;
    
    /* PDM Interface */
    logic pdm_data_i = 0;
    logic pdm_clk_o;
    logic pdm_lrsel_o;
    
    /* PWM Interface */
    wire pwm_o;
    logic audio_enable_o;

    /* SMI interface */
    logic smi_mdc_o;
    wire  smi_mdio_io;

    /* DDR Interface */
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

    /* SD Interface */
    logic sd_cd_n_i = 1'b1; // Card Detect, active low
    wire sd_cmd_io;
    wire [3:0] sd_data_io;
    logic sd_reset_n_o;
    logic sd_clk_o;


    /* Top level SoC */
    ZenithSoC dut (.*);

    /* Ethernet SMI Interface */
    assign smi_mdio_io = dut.ethernet_mac.mac2phy.enable ? 1'bZ : 1'b0;


//====================================================================================
//      MEMORY DDR2 MODEL
//====================================================================================

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


//====================================================================================
//      SD CARD MODEL
//====================================================================================

    `ifdef SD_MODEL

    /* Wishbone Interface */
    logic        wbm_clk_o;
    logic [31:0] wbm_adr_o;
    logic [31:0] wbm_dat_i = 32'h0;
    logic [31:0] wbm_dat_o;
    logic [3:0]  wbm_sel_o;
    logic        wbm_cyc_o;
    logic        wbm_stb_o;
    logic        wbm_we_o;
    logic        wbm_ack_i = 1'b0;
    logic [2:0]  wbm_cti_o;
    logic [1:0]  wbm_bte_o;
        
    bram_whishbone SD_memory (
        .clk ( wbm_clk_o ),
        .rst ( rst_n_i   ),

        .wbm_adr_i  ( wbm_adr_o[11:2] ),
        .wbm_dat_i  ( wbm_dat_o       ),
        .wbm_dat_o  ( wbm_dat_i       ),
        .wbm_we_i   ( wbm_we_o        ),
        .wbm_stb_i  ( wbm_stb_o       ),
        .wbm_cyc_i  ( wbm_cyc_o       ),
        .wbm_sel_i  ( wbm_sel_o       ),
        .wbm_ack_o  ( wbm_ack_i       ),
        .wbm_cti_i  ( wbm_cti_o       ),
        .wbm_bte_i  ( wbm_bte_o       )
    );


    /* Clocks for the SD Card */
    logic clk_50MHz = 0;
    logic clk_100MHz = 0;
    logic clk_200MHz = 0;
    
    /* Clock generation */
    always #10ns clk_50MHz  <= ~clk_50MHz;
    always #5ns  clk_100MHz <= ~clk_100MHz;

    /* Card SD Interface */
    logic sd_cmd_i, sd_cmd_o, sd_cmd_t;
    logic [3:0] sd_dat_i, sd_dat_o, sd_dat_t;

    wire sd_cmd_wire; wire [3:0] sd_dat_wire;

    sd_top sd_model (
        .clk_50      ( clk_50MHz  ),
        .clk_100     ( clk_100MHz ),
        .clk_200     ( 1'b0       ),
        .reset_n     ( rst_n_i    ),

        .sd_clk      ( sd_clk_o ),

        .sd_cmd_i    ( sd_cmd_i ),
        .sd_cmd_o    ( sd_cmd_o ),
        .sd_cmd_t    ( sd_cmd_t ),

        .sd_dat_i    ( sd_dat_i ),
        .sd_dat_o    ( sd_dat_o ),
        .sd_dat_t    ( sd_dat_t ),

        .wbm_clk_o ( wbm_clk_o ),
        .wbm_adr_o ( wbm_adr_o ),
        .wbm_dat_i ( wbm_dat_i ),
        .wbm_dat_o ( wbm_dat_o ),
        .wbm_sel_o ( wbm_sel_o ),
        .wbm_cyc_o ( wbm_cyc_o ),
        .wbm_stb_o ( wbm_stb_o ),
        .wbm_we_o  ( wbm_we_o  ),
        .wbm_ack_i ( wbm_ack_i ),
        .wbm_cti_o ( wbm_cti_o ),
        .wbm_bte_o ( wbm_bte_o ),

        .opt_enable_hs ( '0 )
    );

    /* SD Connection Command to DUT  */
    assign sd_cmd = sd_cmd_t ? dut.sd_controller.command_controller.cmd_bit_flop : sd_cmd_o;
    assign sd_cmd_io = sd_cmd;

    /* SD Connection Data to DUT */
    assign sd_data = (sd_dat_t == '1) ? dut.sd_controller.data_controller.data_line_flop : sd_dat_o;
    assign sd_data_io = sd_data;

    assign sd_cmd_wire = (sd_cmd_t == 1'b0) ? sd_cmd_o :  
                         (dut.sd_controller.command_controller.tristate_enable_flop) ? dut.sd_controller.command_controller.cmd_bit_flop : 1'bz;
    
    assign sd_cmd_i = sd_cmd_wire;

    assign sd_dat_wire = (sd_dat_t == '0) ? sd_dat_o :  
                         (dut.sd_controller.data_controller.tristate_enable_flop) ? dut.sd_controller.data_controller.data_line_flop : 4'bz;

    assign sd_dat_i = sd_dat_wire;

    `endif // SD_MODEL

    
//====================================================================================
//      TRACE OBJECTS
//====================================================================================

    /* Trace */
    typedef struct packed {
        time timestamp; 
        logic [31:0] iaddress; 
        logic [31:0] result;
        logic [4:0] destination;
        logic store; 
        logic load;
    } writeback_packet_t;
    
    writeback_packet_t writeback_buffer [$]; writeback_packet_t data2write, data2read;

    typedef struct packed {
        logic [31:0] address; 
        logic [31:0] data; 
        logic io;
    } store_packet_t;

    typedef struct packed {
        logic [31:0] address; 
        logic io;
    } load_packet_t;

    typedef struct packed { 
        time timestamp; 
        logic [31:0] address; 
        logic [31:0] data; 
        logic store; 
        logic load;
    } cache2ddr_packet_t;

    /* Memory operation descriptors */
    store_packet_t store_buffer [$];
    load_packet_t load_buffer [$];
    cache2ddr_packet_t ddr_buffer [$];

    load_packet_t load_tmp_pkt;
    store_packet_t store_tmp_pkt;


    logic pdm_clk_prev, pdm_valid;

        always_ff @(posedge clk_i) begin
            pdm_clk_prev <= pdm_clk_o;
        end

    assign pdm_valid = !pdm_clk_prev & pdm_clk_o; 


    int cpuFile, memoryFile, outputFile, misprediction_number, branch_jump_number, pdm_file;


    logic stopCondition;

    logic fowardMatch, fowardMatch_prev; assign fowardMatch = `CPU.apogeo_backend.execute_stage.LSU.ldu.foward_match_i;

        always_ff @(posedge clk_i) begin
            fowardMatch_prev <= fowardMatch;
        end 


//====================================================================================
//      TESTBENCH
//====================================================================================

    logic [31:0] program_counter;

        always_ff @(posedge clk_i) begin
            if (`CPU.apogeo_backend.writeback_o) begin
                program_counter <= `CPU.apogeo_backend.trap_iaddress;
            end
        end 


    initial begin
        pdm_file = $fopen(`PDM_FILE, "r"); $display("%d", pdm_file);
        cpuFile = $fopen(`CPU_TRACE_FILE, "w"); $display("%d", cpuFile);
        memoryFile = $fopen(`MEMORY_TRACE_FILE, "w"); $display("%d", memoryFile);
        outputFile = $fopen(`OUTPUT_TRACE_FILE, "w"); $display("%d", outputFile);
        
        @(posedge clk_i);
        rst_n_i <= 1'b0;

        repeat(40) @(posedge clk_i);
        rst_n_i <= 1'b1;

        `ifdef SD_MODEL
            sd_cd_n_i = 1'b0; // Simulate SD card present
        `endif // SD_MODEL

        stopCondition = 0;

        wait(dut.locked);

        fork

            `ifdef AUDIO_PDM

            begin : pdm_interface
                /* Feed PDM input from file */
                while (!$feof(pdm_file)) begin
                    if (pdm_valid) begin
                        automatic integer read_bit = $fgetc(pdm_file);

                        if (read_bit == "0") begin
                            pdm_data_i = 0;
                        end else if (read_bit == "1") begin
                            pdm_data_i = 1;
                        end else begin
                            continue;
                        end 
                    end

                    @(posedge clk_i);
                end
            end : pdm_interface

            `endif // AUDIO_PDM


            `ifdef TRACE_MEMORY

            begin : io_watcher
                while (!stopCondition) begin
                    if (dut.io_load_channel.request) begin
                        $fdisplay(memoryFile, "[IO][%t] Load address: 0x%h", $time, dut.io_load_channel.address);
                    end

                    if (dut.io_load_channel.valid) begin
                        $fdisplay(memoryFile, "[IO][%t] Load data: 0x%h", $time, dut.io_load_channel.data);
                    end

                    if (dut.io_store_channel.request) begin
                        $fdisplay(memoryFile, "[IO][%t] Store 0x%h at address: 0x%h", $time, dut.io_store_channel.data, dut.io_store_channel.address);
                    end

                    if (dut.io_store_channel.done) begin
                        $fdisplay(memoryFile, "[IO][%t] Store done!", $time);
                    end

                    if (dut.io_load_channel.invalidate) begin
                        $fdisplay(memoryFile, "[IO][%t] Load invalidated!", $time);
                    end

                    @(posedge clk_i);
                end
            end : io_watcher


            begin : cache_watcher
                while (!stopCondition) begin
                    if (dut.ApogeoRV.cpu_load_channel.request) begin
                        $fdisplay(memoryFile, "[CACHE][%t] Load address: 0x%h", $time, dut.ApogeoRV.cpu_load_channel.address);

                        load_buffer.push_back({dut.ApogeoRV.cpu_load_channel.address, dut.ApogeoRV.cpu_load_channel.address < `USER_MEMORY_REGION_START});
                    end

                    if (dut.ApogeoRV.cpu_load_channel.valid) begin
                        $fdisplay(memoryFile, "[CACHE][%t] Load data: 0x%h", $time, dut.ApogeoRV.cpu_load_channel.data);
                    end

                    if (dut.ApogeoRV.cpu_store_channel.request) begin
                        $fdisplay(memoryFile, "[CACHE][%t] Store 0x%h at address: 0x%h", $time, dut.ApogeoRV.cpu_store_channel.data, dut.ApogeoRV.cpu_store_channel.address);

                        store_buffer.push_back({dut.ApogeoRV.cpu_store_channel.address, dut.ApogeoRV.cpu_store_channel.data, dut.ApogeoRV.cpu_store_channel.address < `USER_MEMORY_REGION_START});
                    end

                    if (dut.ApogeoRV.cpu_store_channel.done) begin
                        $fdisplay(memoryFile, "[CACHE][%t] Store done!", $time);
                    end

                    if (dut.ApogeoRV.cpu_load_channel.invalidate) begin
                        $fdisplay(memoryFile, "[CACHE][%t] Load invalidated!", $time);

                        if (!fowardMatch_prev) begin
                            load_buffer.pop_front();
                        end
                    end

                    @(posedge clk_i);
                end
            end : cache_watcher


            begin : ddr_watcher
                while (!stopCondition) begin
                    if (dut.ddr_load_channel.request) begin
                        $fdisplay(memoryFile, "[DDR][%t] Load address: 0x%h", $time, dut.ddr_load_channel.address);
                    end

                    if (dut.ddr_load_channel.valid) begin
                        $fdisplay(memoryFile, "[DDR][%t] Load data: 0x%h", $time, dut.ddr_load_channel.data);
                    end

                    if (dut.ddr_store_channel.request) begin
                        $fdisplay(memoryFile, "[DDR][%t] Store 0x%h at address: 0x%h", $time, dut.ddr_store_channel.data, dut.ddr_store_channel.address);
                    end

                    if (dut.ddr_store_channel.done) begin
                        $fdisplay(memoryFile, "[DDR][%t] Store done!", $time);
                    end

                    if (dut.ddr_load_channel.invalidate) begin
                        $fdisplay(memoryFile, "[DDR][%t] Load invalidated!", $time);
                    end

                    @(posedge clk_i);
                end
            end : ddr_watcher

            `endif // TRACE_MEMORY


            `ifdef TRACE_CPU

            begin : cpu_watcher
                while (!stopCondition) begin
                    branch_jump_number += `CPU.executed;
                    misprediction_number += `CPU.apogeo_frontend.mispredicted_o;

                    if (dut.ApogeoRV.cpu_load_channel.request) begin
                        load_buffer.push_back({dut.ApogeoRV.cpu_load_channel.address, dut.ApogeoRV.cpu_load_channel.address < `USER_MEMORY_REGION_START});
                    end

                    if (dut.ApogeoRV.cpu_store_channel.request) begin
                        store_buffer.push_back({dut.ApogeoRV.cpu_store_channel.address, dut.ApogeoRV.cpu_store_channel.data, dut.ApogeoRV.cpu_store_channel.address < `USER_MEMORY_REGION_START});
                    end

                    if (dut.ApogeoRV.cpu_load_channel.invalidate) begin
                        if (!fowardMatch_prev) begin
                            load_buffer.pop_front();
                        end
                    end

                    if (`CPU.apogeo_backend.writeback_o) begin
                        if (`CPU.apogeo_backend.exception_vector == 18) begin
                            data2write = {$time(), `CPU.apogeo_backend.trap_iaddress, `CPU.apogeo_backend.writeback_result_o, `CPU.apogeo_backend.reg_destination_o, 1'b1, 1'b0};
                        end else if (`CPU.apogeo_backend.exception_vector == 19) begin
                            data2write = {$time(), `CPU.apogeo_backend.trap_iaddress, `CPU.apogeo_backend.writeback_result_o, `CPU.apogeo_backend.reg_destination_o, 1'b0, 1'b1};
                        end else begin  
                            data2write = {$time(), `CPU.apogeo_backend.trap_iaddress, `CPU.apogeo_backend.writeback_result_o, `CPU.apogeo_backend.reg_destination_o, 1'b0, 1'b0};
                        end 

                        writeback_buffer.push_back(data2write); 
                    end

                    @(posedge clk_i);
                end
            end : cpu_watcher

            `endif // TRACE_CPU


            `ifdef TRACE_OUTPUT 

            begin: print_watcher
               while (!stopCondition) begin
                    `ifdef FAST_OUTPUT

                        if (dut.write_request[dut._NC_MEM_]) begin
                            $fwrite(outputFile, "%c", dut.write_data[dut._NC_MEM_][7:0]);
                        end

                    `else 

                        if (dut.genblk1[0].uart_device.write_i & dut.genblk1[0].uart_device.write_address_i == UART_TX_BUFFER) begin
                            case (dut.genblk1[0].uart_device.write_data_i)
                                8'h00: $fwrite(outputFile, "");

                                8'h20: $fwrite(outputFile, " ");

                                default: $fwrite(outputFile, "%c", dut.genblk1[0].uart_device.write_data_i[7:0]);
                            endcase 
                        end

                    `endif // FAST_OUTPUT

                   @(posedge clk_i);
               end
            end : print_watcher

            `endif // TRACE_OUTPUT

            begin
                while (`RUN_CONDITION) begin
                    @(posedge clk_i);
                end

                $display("OUT");

                stopCondition = 1;
            end
        join_any

        $display("OUT");

        `ifdef TRACE_CPU

        while (writeback_buffer.size() != '0) begin
            data2read = writeback_buffer.pop_front(); 

            if (data2read.store) begin
                automatic store_packet_t store2read = store_buffer.pop_front();

                if (store2read.io) begin
                    $fdisplay(cpuFile, "%0dns , 0x%0h , x%02d , 0x%h in IO 0x%h", data2read.timestamp, data2read.iaddress, data2read.destination, store2read.data, store2read.address);
                end else begin
                    $fdisplay(cpuFile, "%0dns , 0x%0h , x%02d , 0x%h in 0x%h", data2read.timestamp, data2read.iaddress, data2read.destination, store2read.data, store2read.address);
                end
            end else if (data2read.load) begin
                automatic load_packet_t load2read = load_buffer.pop_front();

                if (load2read.io) begin
                    $fdisplay(cpuFile, "%0dns , 0x%0h , x%02d , 0x%h from IO 0x%h", data2read.timestamp, data2read.iaddress, data2read.destination, data2read.result, load2read.address);
                end else begin
                    $fdisplay(cpuFile, "%0dns , 0x%0h , x%02d , 0x%h from 0x%h", data2read.timestamp, data2read.iaddress, data2read.destination, data2read.result, load2read.address);
                end
            end else begin
                $fdisplay(cpuFile, "%0dns , 0x%0h , x%02d , 0x%h", data2read.timestamp, data2read.iaddress, data2read.destination, data2read.result);
            end
        end

        `endif // TRACE_CPU

        $fclose(cpuFile);
        $fclose(memoryFile);
        $fclose(outputFile);

        $finish();
    end

endmodule : soc_testbench

`endif 