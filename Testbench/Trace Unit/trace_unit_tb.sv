`ifndef TRACE_UNIT_TB_SV
    `define TRACE_UNIT_TB_SV

module trace_unit_tb;

    localparam BUFFER_SIZE = 16;

    localparam STATUS = 0;
    localparam CONTROL = 1;
    localparam EVENT_ENABLE = 2;
    localparam TRIGGER_PC = 3;


    /* Global signals */
    logic clk_i = 0;
    logic rst_n_i = 0;

    /* CPU Signals */
    logic interrupt_o;
    logic halt_core_o;

    /* UART status */
    logic uart_tx_full_i = 0;

    /* Write interface */
    logic write_i = 0;
    logic [1:0] write_address_i = 0;
    logic [31:0] write_data_i = 0;
    logic [3:0] write_strobe_i = 0;
    logic write_error_o;
    logic write_done_o;

    /* Read interface */
    logic read_i = 0;
    logic [1:0] read_address_i = 0;
    logic [31:0] read_data_o;
    logic read_error_o;
    logic read_done_o;

    /* Trace channel from CPU */
    trace_interface trace_interface_i();

    /* Trace packets to UART */
    logic [7:0] trace_chunk_o;
    logic write_chunk_o;


    always #5 clk_i <= !clk_i;


    trace_unit #(BUFFER_SIZE) dut ( .* );


    bit [2047:0] branch_map;

    initial begin
        foreach (branch_map[i]) begin
            branch_map[i] = 0;
        end

        for (int i = 15; i < 2048; i += 16) begin
            branch_map[i] = 1;
        end
    end

//====================================================================================
//      TASKS
//====================================================================================

    localparam ENABLE = 1;
    localparam DISABLE = 0;

    task write(input logic [31:0] data, input logic [1:0] address);
        write_address_i <= address;
        write_data_i <= data;
        write_strobe_i <= '1;
        write_i <= 1'b1;
        @(posedge clk_i);
        write_i <= 1'b0;
        @(posedge clk_i);
    endtask : write

    task read(output logic [31:0] data, input logic [1:0] address);
        read_address_i <= address;
        read_i <= 1'b1;
        @(posedge clk_i);
        data <= read_data_o;
        read_i <= 1'b0;
        @(posedge clk_i);
    endtask : read


    task enable_event_tracing(input trace_unit_event_t events, input logic enable);
        trace_unit_control_t control;

        write(events, EVENT_ENABLE);

        read(control, CONTROL);
        control.enable_event_tracing = enable;
        @(posedge clk_i);

        write(control, CONTROL);
    endtask : enable_event_tracing

    task enable_branch_tracing(input logic enable);
        trace_unit_control_t control;

        read(control, CONTROL);
        control.enable_branch_tracing = enable;
        @(posedge clk_i);
        
        write(control, CONTROL);
    endtask : enable_branch_tracing

    task enable_timestamp_event(input logic enable);
        trace_unit_control_t control;

        read(control, CONTROL);
        control.enable_timestamp_event = enable;
        @(posedge clk_i);
        
        write(control, CONTROL);
    endtask : enable_timestamp_event

    task enable_timestamp_branch(input logic enable);
        trace_unit_control_t control;

        read(control, CONTROL);
        control.enable_timestamp_branch = enable;
        @(posedge clk_i);
        
        write(control, CONTROL);
    endtask : enable_timestamp_branch

    task enable_trigger(input logic enable);
        trace_unit_control_t control;

        read(control, CONTROL);
        control.enable_trigger = enable;
        @(posedge clk_i);
        
        write(control, CONTROL);
    endtask : enable_trigger

    task clear_timestamp();
        trace_unit_control_t control;

        read(control, CONTROL);
        control.clear_timestamp = 1'b1;
        @(posedge clk_i);
        
        write(control, CONTROL);
    endtask : clear_timestamp

    task set_trigger_pc(input logic [31:0] trigger_pc);
        write(trigger_pc, TRIGGER_PC);
    endtask : set_trigger_pc

    task enable_interrupts(input logic int_full, input logic int_empty);
        trace_unit_control_t control;

        read(control, CONTROL);
        control.enable_int_full = int_full;
        control.enable_int_empty = int_empty;
        @(posedge clk_i);
        
        write(control, CONTROL);
    endtask : enable_interrupts


    task automatic print_packet(input byte packet[$]);
        logic [31:0] start_pc;
        logic [31:0] end_pc;
        logic [23:0] timestamp;
        logic [5:0]  event_number;

        begin
            if (packet.size() == 0) begin
                $display("Empty packet");
            end else begin

                case (trace_unit_packet_type_t'(packet[0][7:6]))

                    EVENT_PACKET: begin
                        event_number = packet[0][5:0];

                        $display("========== EVENT PACKET ==========");
                        $write("Raw bytes: 0xAA ");

                        foreach (packet[i]) begin
                            $write("%02h ", packet[i]);
                        end
                        $write("\n");

                        $display("Type         : EVENT");
                        $display("Event number : %0d", event_number);

                        if (packet.size() == 4) begin
                            timestamp = {packet[1], packet[2], packet[3]};
                            $display("Timestamp    : 0x%06h", timestamp);
                        end

                        $display("==================================");
                    end

                    DIVERGENCE_PACKET: begin
                        $display("======= DIVERGENCE PACKET ========");
                        $write("Raw bytes: 0xAA ");

                        foreach (packet[i]) begin
                            $write("%02h ", packet[i]);
                        end
                        $write("\n");

                        start_pc = {packet[1], packet[2], packet[3], packet[4]};
                        end_pc   = {packet[5], packet[6], packet[7], packet[8]};

                        $display("Type     : DIVERGENCE");
                        $display("Start PC : 0x%08h", start_pc);
                        $display("End PC   : 0x%08h", end_pc);

                        if (packet.size() == 12) begin
                            timestamp = {packet[9], packet[10], packet[11]};
                            $display("Timestamp: 0x%06h", timestamp);
                        end

                        $display("==================================");
                    end

                    OVERFLOW_PACKET: begin
                        $display("======== OVERFLOW PACKET =========");
                        $write("Raw bytes: 0xAA ");

                        foreach (packet[i]) begin
                            $write("%02h ", packet[i]);
                        end
                        $write("\n");

                        $display("Type: OVERFLOW");
                        $display("==================================");
                    end

                    default: begin
                        $display("========== UNKNOWN PACKET =========");
                        $write("Raw bytes: 0xAA ");

                        foreach (packet[i]) begin
                            $write("%02h ", packet[i]);
                        end
                        $write("\n");

                        $display("Unknown type: %b", packet[0][7:6]);
                        $display("===================================");
                    end

                endcase
            end

            $display("\n");
        end
    endtask : print_packet


    function automatic logic [5:0] random_event();
        case ($urandom_range(0, 16))
            0:  return `INSTR_MISALIGNED;
            1:  return `INSTR_ACCESS_FAULT;
            2:  return `INSTR_ILLEGAL;
            3:  return `BREAKPOINT;
            4:  return `LOAD_OPERATION;
            5:  return `LOAD_MISALIGNED;
            6:  return `LOAD_ACCESS_FAULT;
            7:  return `STORE_OPERATION;
            8:  return `STORE_MISALIGNED;
            9:  return `STORE_ACCESS_FAULT;
            10: return `U_SYSTEM_CALL;
            11: return `M_SYSTEM_CALL;
            12: return `SLEEP;
            13: return `HANDLER_RETURN;
            14: return `BRANCH_OPERATION;
            15: return `JUMP_OPERATION;
            default: return `CSR_OPERATION;
        endcase
    endfunction

//====================================================================================
//      TASKS
//====================================================================================

    logic [31:0] program_counter; logic enable_pc, take_lock;


    /* Parser */
    typedef enum logic [1:0] {
        RX_WAIT_SYNC,
        RX_DATA
    } rx_state_t;

    rx_state_t rx_state;

    byte packet_buffer[$];

    trace_unit_packet_type_t rx_packet_type;
    int expected_len;
    int byte_count;


    initial begin
        repeat (2) @(posedge clk_i);
        rst_n_i <= 1'b1;
        take_lock <= 0;
        enable_pc <= 0;

        rx_state     = RX_WAIT_SYNC;
        packet_buffer.delete();
        program_counter = 0;
        expected_len = 0;
        byte_count   = 0;

        trace_interface_i.address = 0;
        trace_interface_i.info = 0;
        trace_interface_i.valid = 0;

        repeat (2) @(posedge clk_i);

        fork
            begin
                enable_interrupts(ENABLE, ENABLE);
                @(posedge clk_i);

                /* TEST 1: Enable branch tracing without timestamp */
                enable_branch_tracing(ENABLE);
                enable_pc <= 1'b1;
                repeat (2048) @(posedge clk_i);
                enable_pc <= 1'b0;

                enable_branch_tracing(DISABLE);
                repeat (100) @(posedge clk_i);


                /* TEST 2: Enable branch tracing and with timestamp */
                enable_branch_tracing(ENABLE);
                enable_timestamp_branch(ENABLE);
                enable_pc <= 1'b1;
                repeat (2048) @(posedge clk_i);
                enable_pc <= 1'b0;

                enable_branch_tracing(DISABLE);
                enable_timestamp_branch(DISABLE);
                repeat (100) @(posedge clk_i);

                /* TEST 3: Enable branch tracing with trigger PC 
                 * and with timestamp */
                set_trigger_pc(32'h00002000);
                enable_trigger(ENABLE);
                enable_branch_tracing(ENABLE);
                enable_timestamp_branch(ENABLE);
                enable_pc <= 1'b1;

                repeat (1024) @(posedge clk_i);
                @(negedge clk_i);
                trace_interface_i.address = 32'h00002000;
                trace_interface_i.info = random_event();
                trace_interface_i.valid = 1'b1;

                @(negedge clk_i);
                trace_interface_i.valid = 1'b0;
                take_lock = 1'b0;
                repeat (1023) @(posedge clk_i);
                enable_pc <= 1'b0;

                enable_branch_tracing(DISABLE);
                enable_timestamp_branch(DISABLE);
                enable_trigger(DISABLE);
                repeat (100) @(posedge clk_i);

                /* TEST 4: Enable event tracing without timestamp */
                enable_event_tracing('1, ENABLE);
                enable_pc <= 1'b1;
                repeat (2048) @(posedge clk_i);
                enable_pc <= 1'b0;

                enable_event_tracing('1, DISABLE);
                repeat (100) @(posedge clk_i);

                /* TEST 5: Enable event tracing with timestamp */
                enable_event_tracing('1, ENABLE);
                enable_timestamp_event(ENABLE);
                enable_pc <= 1'b1;
                repeat (2048) @(posedge clk_i);
                enable_pc <= 1'b0;

                enable_event_tracing('1, DISABLE);
                enable_timestamp_event(DISABLE);
                repeat (100) @(posedge clk_i);

                /* TEST 6: Enable event and branch tracing with timestamp */
                enable_event_tracing('1, ENABLE);
                enable_branch_tracing(ENABLE);
                enable_timestamp_event(ENABLE);
                enable_timestamp_branch(ENABLE);
                enable_pc <= 1'b1;
                repeat (2048) @(posedge clk_i);
                enable_pc <= 1'b0;

                enable_event_tracing('1, DISABLE);
                enable_branch_tracing(DISABLE);
                enable_timestamp_event(DISABLE);
                enable_timestamp_branch(DISABLE);
                repeat (100) @(posedge clk_i);

                /* TEST 7: Wait overflow */
                clear_timestamp();
                enable_timestamp_branch(ENABLE);
                force dut.packetizer.timestamp_counter = 24'hFFFFFE;
                release dut.packetizer.timestamp_counter;
                wait (dut.packetizer.timestamp_counter == '1);
                repeat (100) @(posedge clk_i);

                $display("\n\n\n============= Test completed =============");
                $finish;
            end

            begin
                forever begin
                    for (int i = 0; i < 2048; ) begin
                        if (enable_pc & $urandom_range(0, 1) & !take_lock & !halt_core_o) begin
                            if (branch_map[i]) begin
                                trace_interface_i.address <= $random();
                            end else begin
                                trace_interface_i.address <= trace_interface_i.address + 4;
                            end
                            
                            trace_interface_i.info <= random_event();
                            trace_interface_i.valid <= 1'b1;

                            ++i;
                        end else begin
                            trace_interface_i.valid <= 1'b0;
                        end

                        @(posedge clk_i);
                    end
                end
            end

            begin
                forever begin
                    @(posedge clk_i);

                    if (write_chunk_o) begin

                        case (rx_state)

                            RX_WAIT_SYNC: begin
                                if (trace_chunk_o == 8'hAA) begin
                                    packet_buffer.delete();
                                    expected_len = 0;
                                    byte_count   = 0;

                                    rx_state = RX_DATA;
                                end
                            end

                            RX_DATA: begin
                                packet_buffer.push_back(trace_chunk_o);
                                byte_count++;

                                /*
                                * First data byte contains:
                                * [7:6] packet type
                                * [5:0] event number / padding
                                */
                                if (byte_count == 1) begin
                                    rx_packet_type = trace_unit_packet_type_t'(trace_chunk_o[7:6]);

                                    case (rx_packet_type)

                                        OVERFLOW_PACKET: begin
                                            expected_len = 1;
                                        end

                                        EVENT_PACKET: begin
                                            if (dut.serializer.enable_timestamp_i) begin
                                                expected_len = 4;
                                            end else begin
                                                expected_len = 1;
                                            end
                                        end

                                        DIVERGENCE_PACKET: begin
                                            if (dut.serializer.enable_timestamp_i) begin
                                                expected_len = 12;
                                            end else begin
                                                expected_len = 9;
                                            end
                                        end

                                        default: begin
                                            expected_len = 1;
                                        end

                                    endcase
                                end

                                if ((expected_len != 0) && (byte_count == expected_len)) begin
                                    print_packet(packet_buffer);
                                    packet_buffer.delete();

                                    rx_state = RX_WAIT_SYNC;
                                end
                            end

                            default: begin
                                rx_state = RX_WAIT_SYNC;
                            end

                        endcase
                    end
                end
            end

            begin
                forever begin
                    @(negedge clk_i);
                    uart_tx_full_i = ($urandom_range(0, 9) == 0); // 10% full
                    repeat (5) @(negedge clk_i);
                end
            end
        join_any
    end

endmodule : trace_unit_tb

`endif