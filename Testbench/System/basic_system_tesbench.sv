`ifndef BASIC_SYSTEM_TESTBENCH_SV
    `define BASIC_SYSTEM_TESTBENCH_SV

/* Enable or disable tracing */
`define TRACER

`define TRACE_FILE "/home/gabbed/Projects/ZenithSoC/Testbench/System/trace.txt"

`define CPU dut.system_cpu

module basic_system_testbench();

    localparam PREDICTOR_SIZE = 1024;
    localparam BTB_SIZE = 1024; 
    localparam STORE_BUFFER_SIZE = 4;
    localparam INSTRUCTION_BUFFER_SIZE = 8;
    localparam SYSTEM_MEMORY_SIZE = 2 ** 14;
    localparam BOOT_MEMORY_SIZE = 2 ** 11;
    localparam ROB_DEPTH = 32;
    localparam SLAVES = 1;
    localparam logic [4:0] CHIP_PHY_ADDRESS = 5'b00001;
    localparam logic [5:0][7:0] MAC_ADDRESS = 48'h00_00_00_00_00_00;

    logic clk_i = 1;
    logic rst_n_i = 0;

    /* External interrupts */
    logic [1:0] ext_int_i = 0;

    /* GPIO pins */
    wire [7:0] pin_io;

    /* UART pins */
    logic uart_rx_i;
    logic uart_cts_i = 1;
    logic uart_tx_o;
    logic uart_rts_o;

    assign uart_rx_i = uart_tx_o;
    
    /* SPI pins */    
    logic spi_miso_i;
    logic spi_mosi_o;
    logic spi_sclk_o;
    logic spi_cs_n_o;

    /* RMII Interface */
    wire [1:0] rmii_rxd_io;
    wire rmii_crsdv_io;
    logic rmii_rxer_i = 0;
    logic [1:0] rmii_txd_o;
    logic rmii_txen_o;
    logic rmii_refclk_o;
    logic rmii_rstn_o;

    /* SMII interface */
    logic smi_mdc_o;
    wire smi_mdio_io;

    assign spi_miso_i = spi_mosi_o;

    assign rmii_rxd_io = rmii_txd_o;
    assign rmii_crsdv_io = rmii_txen_o;


    basic_system #(
        PREDICTOR_SIZE, 
        BTB_SIZE, 
        STORE_BUFFER_SIZE, 
        INSTRUCTION_BUFFER_SIZE, 
        SYSTEM_MEMORY_SIZE, 
        CHIP_PHY_ADDRESS,
        MAC_ADDRESS,
        BOOT_MEMORY_SIZE,
        ROB_DEPTH,
        SLAVES
    ) dut ( .* );

    always #5 clk_i <= !clk_i; 


    assign smi_mdio_io = dut.ethernet_mac.mac2phy.enable ? 1'bZ : 1'b1;

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
    } store_packet_t;

    store_packet_t store_buffer [$]; store_packet_t store2read;

    logic [31:0] load_address_buffer [$];


    /* Char buffer logic */
    logic [7:0] char_buffer [$]; 
    
    localparam _BOOT_ = 0;
    localparam _MEMORY_ = 5;

        always_ff @(posedge clk_i) begin 
            if (dut.write_address[_MEMORY_] == '1 & dut.write_request[_MEMORY_]) begin  
                char_buffer.push_back(dut.write_data[_MEMORY_][7:0]); 
            end
        end 


    task dump_memory(input int file);
        automatic int idx = 0;

        for (int i = 0; i < BOOT_MEMORY_SIZE / 4; ++i) begin
            $fwrite(file, "%h", dut.boot_memory.mem_bank[3].bank.memory[idx + 3]);
            $fwrite(file, "%h", dut.boot_memory.mem_bank[2].bank.memory[idx + 2]);
            $fwrite(file, "%h", dut.boot_memory.mem_bank[1].bank.memory[idx + 1]);
            $fwrite(file, "%h\n", dut.boot_memory.mem_bank[0].bank.memory[idx]);

            idx += 4;
        end

        idx = 0;

        for (int i = 0; i < SYSTEM_MEMORY_SIZE / 4; ++i) begin
            $fwrite(file, "%h", dut.system_memory.mem_bank[3].bank.memory[idx + 3]);
            $fwrite(file, "%h", dut.system_memory.mem_bank[3].bank.memory[idx + 2]);
            $fwrite(file, "%h", dut.system_memory.mem_bank[3].bank.memory[idx + 1]);
            $fwrite(file, "%h\n", dut.system_memory.mem_bank[3].bank.memory[idx]);

            idx += 4;
        end
    endtask : dump_memory


    logic [31:0] registers[32]; int file, misprediction_number, branch_jump_number;

    logic [3:0] pin; assign pin_io[7:4] = pin;

    int count; 

    always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
        if (!rst_n_i) begin 
            count <= '0;
            pin <= '0;
        end else begin 
            count <= count + 1;

            if (count == 3000) begin 
                pin <= '1;
            end
        end 
    end 

    initial begin
        registers = '{default: 0}; 

        `ifdef TRACER 
            file = $fopen(`TRACE_FILE, "w"); $display("%d", file);
        `endif 
        
        @(posedge clk_i);
        rst_n_i <= 1'b0;

        repeat(40) @(posedge clk_i);
        rst_n_i <= 1'b1;

        while (!(`CPU.apogeo_backend.exception_generated & `CPU.apogeo_backend.exception_vector == 2) & ($time() < 245000)) begin
            /* Write the registers */
            if (`CPU.apogeo_backend.writeback_o) begin
                registers[`CPU.apogeo_backend.reg_destination_o] <= `CPU.apogeo_backend.writeback_result_o;
            end

            if (dut.load_channel.request) begin
                load_address_buffer.push_back(dut.load_channel.address); 
            end
            if (dut.store_channel.request) begin
                store_buffer.push_back({dut.store_channel.address, dut.store_channel.data}); 
            end


            branch_jump_number += `CPU.executed;
            misprediction_number += `CPU.apogeo_frontend.mispredicted_o;

            `ifdef TRACER 
                if (`CPU.apogeo_backend.writeback_o) begin
                    if (`CPU.apogeo_backend.exception_vector == 18) begin
                        data2write = {$time, `CPU.apogeo_backend.trap_iaddress, `CPU.apogeo_backend.writeback_result_o, `CPU.apogeo_backend.reg_destination_o, 1'b1, 1'b0};
                    end else if (`CPU.apogeo_backend.exception_vector == 19) begin
                        data2write = {$time, `CPU.apogeo_backend.trap_iaddress, `CPU.apogeo_backend.writeback_result_o, `CPU.apogeo_backend.reg_destination_o, 1'b0, 1'b1};
                    end else begin  
                        data2write = {$time, `CPU.apogeo_backend.trap_iaddress, `CPU.apogeo_backend.writeback_result_o, `CPU.apogeo_backend.reg_destination_o, 1'b0, 1'b0};
                    end 

                    writeback_buffer.push_back(data2write); 
                end
            `endif 

            @(posedge clk_i);
        end 

        `ifdef TRACER

        while (writeback_buffer.size() != '0) begin
            data2read = writeback_buffer.pop_front(); 

            if (data2read.store) begin
                store2read = store_buffer.pop_front();

                $fdisplay(file, "%0dns , 0x%0h , x%02d , 0x%h in 0x%h", data2read.timestamp, data2read.iaddress, data2read.destination, store2read.data, store2read.address);
            end else if (data2read.load) begin
                $fdisplay(file, "%0dns , 0x%0h , x%02d , 0x%h from 0x%h", data2read.timestamp, data2read.iaddress, data2read.destination, data2read.result, load_address_buffer.pop_front());
            end else begin
                $fdisplay(file, "%0dns , 0x%0h , x%02d , 0x%h", data2read.timestamp, data2read.iaddress, data2read.destination, data2read.result);
            end
        end


        $fdisplay(file, "\n");

        $fdisplay(file, "============== REGISTER FILE ==============");

        for (int i = 0; i < 32; ++i) begin
            $fdisplay(file, "%02d | 0x%h", i, registers[i]); 
        end

        $fdisplay(file, "============== MEMORY DUMP =============="); 
        
        // dump_memory(file);

        $fdisplay(file, "\n\n============== BUFFER DATA ==============");

        while (char_buffer.size != '0) begin
            automatic logic [7:0] temp = char_buffer.pop_front(); 
            $fwrite(file, "%02h", temp);
        end

        $fclose(file);

        `endif 

        $finish();
    end

endmodule : basic_system_testbench

`endif