`ifndef SOC_TESTBENCH_SV
    `define SOC_TESTBENCH_SV

/* Enable or disable tracing */
`define TRACER

`define CPU_TRACE_FILE "/home/gabriele/Desktop/Projects/ZenithSoC/Testbench/System/cpu_trace.txt"
`define BUFFER_TRACE_FILE "/home/gabriele/Desktop/Projects/ZenithSoC/Testbench/System/buffer_trace.txt"
`define MEMORY_TRACE_FILE "/home/gabriele/Desktop/Projects/ZenithSoC/Testbench/System/memory_trace.txt"
`define DDR_TRACE_FILE "/home/gabriele/Desktop/Projects/ZenithSoC/Testbench/System/ddr_trace.txt"

`include "../../Hardware/Utility/Packages/soc_parameters.sv"

`define CPU dut.ApogeoRV.system_cpu

module soc_testbench;

    logic clk_i = 0;

    always #5ns clk_i <= !clk_i;


    logic rst_n_i = 0;

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

    /* SMI interface */
    logic smi_mdc_o;
    wire  smi_mdio_io;

    /* DDR Interface */
    wire [15:0] ddr2_dq_io;
    wire [1:0]  ddr2_dqs_n_io;
    wire [1:0]  ddr2_dqs_p_io;
    wire [1:0]  ddr2_dm_o;
    wire [12:0] ddr2_addr_o;
    wire [2:0]  ddr2_ba_o;
    wire        ddr2_ras_n_o;
    wire        ddr2_cas_n_o;
    wire        ddr2_we_n_o;
    wire        ddr2_cke_o;
    wire        ddr2_ck_p_o;
    wire        ddr2_ck_n_o;
    wire        ddr2_cs_n_o;
    wire        ddr2_odt_o;


    ZenithSoC dut (.*);

    ddr2_model ddr2 (
        .ck      ( ddr2_ck_p_o  ),
        .ck_n    ( ddr2_ck_n_o  ),
        .cke     ( ddr2_cke_o   ),
        .cs_n    ( ddr2_cs_n_o  ),
        .ras_n   ( ddr2_ras_n_o ),
        .cas_n   ( ddr2_cas_n_o ),
        .we_n    ( ddr2_we_n_o  ),
        .dm_rdqs ( ddr2_dm_o    ),
        .ba      ( ddr2_ba_o    ),
        .addr    ( ddr2_addr_o  ),
        .dq      ( ddr2_dq_io    ),
        .dqs     ( ddr2_dqs_p_io ),
        .dqs_n   ( ddr2_dqs_n_io ),
        .rdqs_n  (            ),
        .odt     ( ddr2_odt_o   )
    );

    assign smi_mdio_io = dut.ethernet_mac.mac2phy.enable ? 1'bZ : 1'b0;


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


    /* Char buffer logic */
    logic [7:0] char_buffer [$]; 
    
        always_ff @(posedge clk_i) begin 
            /* Writing to UART TX buffer */
            if (dut.write_request[dut._UART_] & dut.write_address[dut._UART_] == 'd1) begin  
                char_buffer.push_back(dut.write_data[dut._UART_][7:0]); 
            end
        end 


    int cpuFile, bufferFile, memoryFile, ddrFile, misprediction_number, branch_jump_number;


    // logic stopCondition; assign stopCondition = !`CPU.apogeo_backend.exception_generated;
    logic stopCondition; // assign stopCondition = $time() > 2000ns;

    logic fowardMatch, fowardMatch_prev; assign fowardMatch = `CPU.apogeo_backend.execute_stage.LSU.ldu.foward_match_i;

        always_ff @(posedge clk_i) begin
            fowardMatch_prev <= fowardMatch;
        end 

    initial begin
        `ifdef TRACER 
            cpuFile = $fopen(`CPU_TRACE_FILE, "w"); $display("%d", cpuFile);
            bufferFile = $fopen(`BUFFER_TRACE_FILE, "w"); $display("%d", bufferFile);
            memoryFile = $fopen(`MEMORY_TRACE_FILE, "w"); $display("%d", memoryFile);
            ddrFile = $fopen(`DDR_TRACE_FILE, "w"); $display("%d", ddrFile);
        `endif 
        
        @(posedge clk_i);
        rst_n_i <= 1'b0;

        repeat(40) @(posedge clk_i);
        rst_n_i <= 1'b1;

        stopCondition = 0;

        wait(dut.locked);

        fork
            begin : io_watcher
                while (!stopCondition) begin
                    if (dut.io_load_channel.request) begin
                        $fdisplay(memoryFile, "[IO][%t] Load address: 0x%h", $time, dut.io_load_channel.address);
                        $display("[IO][%t] Load address: 0x%h", $time, dut.io_load_channel.address);
                    end

                    if (dut.io_load_channel.valid) begin
                        $fdisplay(memoryFile, "[IO][%t] Load data: 0x%h", $time, dut.io_load_channel.data);
                        $display("[IO][%t] Load data: 0x%h", $time, dut.io_load_channel.data);
                    end

                    if (dut.io_store_channel.request) begin
                        $fdisplay(memoryFile, "[IO][%t] Store 0x%h at address: 0x%h", $time, dut.io_store_channel.data, dut.io_store_channel.address);
                        $display("[IO][%t] Store 0x%h at address: 0x%h", $time, dut.io_store_channel.data, dut.io_store_channel.address);
                    end

                    if (dut.io_store_channel.done) begin
                        $fdisplay(memoryFile, "[IO][%t] Store done!", $time);
                        $display("[IO][%t] Store done!", $time);
                    end

                    if (dut.io_load_channel.invalidate) begin
                        $fdisplay(memoryFile, "[IO][%t] Load invalidated!", $time);
                        $display("[IO][%t] Load invalidated!", $time);
                    end

                    @(posedge clk_i);
                end
            end : io_watcher

            begin : cache_watcher
                while (!stopCondition) begin
                    if (dut.ApogeoRV.cpu_load_channel.request) begin
                        $fdisplay(memoryFile, "[CACHE][%t] Load address: 0x%h", $time, dut.ApogeoRV.cpu_load_channel.address);
                        $display("[CACHE][%t] Load address: 0x%h", $time, dut.ApogeoRV.cpu_load_channel.address);

                        load_buffer.push_back({dut.ApogeoRV.cpu_load_channel.address, dut.ApogeoRV.cpu_load_channel.address < `USER_MEMORY_REGION_START});
                    end

                    if (dut.ApogeoRV.cpu_load_channel.valid) begin
                        $fdisplay(memoryFile, "[CACHE][%t] Load data: 0x%h", $time, dut.ApogeoRV.cpu_load_channel.data);
                        $display("[CACHE][%t] Load data: 0x%h", $time, dut.ApogeoRV.cpu_load_channel.data);
                    end

                    if (dut.ApogeoRV.cpu_store_channel.request) begin
                        $fdisplay(memoryFile, "[CACHE][%t] Store 0x%h at address: 0x%h", $time, dut.ApogeoRV.cpu_store_channel.data, dut.ApogeoRV.cpu_store_channel.address);
                        $display("[CACHE][%t] Store 0x%h at address: 0x%h", $time, dut.ApogeoRV.cpu_store_channel.data, dut.ApogeoRV.cpu_store_channel.address);

                        store_buffer.push_back({dut.ApogeoRV.cpu_store_channel.address, dut.ApogeoRV.cpu_store_channel.data, dut.ApogeoRV.cpu_store_channel.address < `USER_MEMORY_REGION_START});
                    end

                    if (dut.ApogeoRV.cpu_store_channel.done) begin
                        $fdisplay(memoryFile, "[CACHE][%t] Store done!", $time);
                        $display("[CACHE][%t] Store done!", $time);
                    end

                    if (dut.ApogeoRV.cpu_load_channel.invalidate) begin
                        $fdisplay(memoryFile, "[CACHE][%t] Load invalidated!", $time);
                        $display("[CACHE][%t] Load invalidated!", $time);

                        if (!fowardMatch_prev) begin
                            $display("Popping!");
                            
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
                        $display("[DDR][%t] Load address: 0x%h", $time, dut.ddr_load_channel.address);
                    end

                    if (dut.ddr_load_channel.valid) begin
                        $fdisplay(memoryFile, "[DDR][%t] Load data: 0x%h", $time, dut.ddr_load_channel.data);
                        $display("[DDR][%t] Load data: 0x%h", $time, dut.ddr_load_channel.data);
                    end

                    if (dut.ddr_store_channel.request) begin
                        $fdisplay(memoryFile, "[DDR][%t] Store 0x%h at address: 0x%h", $time, dut.ddr_store_channel.data, dut.ddr_store_channel.address);
                        $display("[DDR][%t] Store 0x%h at address: 0x%h", $time, dut.ddr_store_channel.data, dut.ddr_store_channel.address);
                    end

                    if (dut.ddr_store_channel.done) begin
                        $fdisplay(memoryFile, "[DDR][%t] Store done!", $time);
                        $display("[DDR][%t] Store done!", $time);
                    end

                    if (dut.ddr_load_channel.invalidate) begin
                        $fdisplay(memoryFile, "[DDR][%t] Load invalidated!", $time);
                        $display("[DDR][%t] Load invalidated!", $time);
                    end

                    @(posedge clk_i);
                end
            end : ddr_watcher

            begin : cpu_watcher
                while (!stopCondition) begin
                    branch_jump_number += `CPU.executed;
                    misprediction_number += `CPU.apogeo_frontend.mispredicted_o;

                    `ifdef TRACER 
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
                    `endif 

                    @(posedge clk_i);
                end
            end : cpu_watcher

            begin
                while ($time() < 257000ns) begin
                    @(posedge clk_i);
                end

                $display("OUT");

                stopCondition = 1;
            end
        join_any

        $display("OUT");

        `ifdef TRACER

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


        while (char_buffer.size != '0) begin
            automatic logic [7:0] temp = char_buffer.pop_front(); 
            $fwrite(bufferFile, "%c", temp);
        end

        $fclose(cpuFile);
        $fclose(bufferFile);
        $fclose(memoryFile);

        `endif 

        $finish();
    end

endmodule : soc_testbench

`endif 