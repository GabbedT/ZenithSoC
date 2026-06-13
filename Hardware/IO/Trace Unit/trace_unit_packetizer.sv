`ifndef TRACE_UNIT_PACKETIZER_SV
    `define TRACE_UNIT_PACKETIZER_SV

`include "../../CPU/ApogeoRV/Hardware/Include/Headers/apogeo_exception_vectors.svh"
`include "../../CPU/ApogeoRV/Hardware/Include/Interfaces/trace_interface.sv"

`include "../../System/synchronous_buffer.sv"

`include "../../Utility/Packages/trace_unit_pkg.sv"


module trace_unit_packetizer #(
    /* Buffer depth */
    parameter PACKET_BUFFER_SIZE = 512
) (
    /* Global signals */
    input logic clk_i,
    input logic rst_n_i,
    output logic halt_core_o,

    /* Configuration Tracing */
    input logic enable_event_tracing_i,
    input logic enable_branch_tracing_i,

    /* Configuration Timestamp */
    input logic enable_timestamp_event_i,
    input logic enable_timestamp_branch_i,

    /* Configuration Trigger PC */
    input logic enable_trigger_i,
    input logic [31:0] trigger_pc_i,

    /* Timestamp control */
    input logic clear_timestamp_i,

    /* Enable events */
    input trace_unit_event_t enable_i,

    /* Trace channel from CPU */
    trace_interface.slave trace_interface_i,

    /* Trace packets to Serializer */
    input logic trace_buffer_read_i,
    output logic trace_buffer_empty_o,
    output logic trace_buffer_full_o,
    output trace_unit_packet_t trace_packet_o
);

//====================================================================================
//      TIMESTAMP GENERATION
//====================================================================================

    logic [23:0] timestamp_counter;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin
                timestamp_counter <= 24'b0;
            end else if (clear_timestamp_i) begin
                timestamp_counter <= 24'b0;
            end else if (enable_timestamp_event_i | enable_timestamp_branch_i) begin
                /* Start counting only when timestamping is enabled, otherwise the counter will 
                 * just consume energy */
                timestamp_counter <= timestamp_counter + 1;
            end
        end

    logic overflow; 

    assign overflow = (timestamp_counter == 24'hFFFFFF) & (enable_timestamp_event_i | enable_timestamp_branch_i);


//====================================================================================
//      EVENT SIGNAL GENERATION
//====================================================================================

    /* Event arrives from the trace interface, we need to decide if 
     * it should generate a trace packet or not based on the enable signals */
    logic event_generated;

        always_comb begin
            event_generated = 1'b0;

            if (trace_interface_i.valid & enable_event_tracing_i) begin
                case (trace_interface_i.info)
                    `INSTR_MISALIGNED: event_generated = enable_i.instr_misaligned;

                    `INSTR_ACCESS_FAULT: event_generated = enable_i.instr_access_fault;

                    `INSTR_ILLEGAL: event_generated = enable_i.instr_illegal;

                    `BREAKPOINT: event_generated = enable_i.breakpoint;

                    `LOAD_OPERATION: event_generated = enable_i.load_operation;

                    `LOAD_MISALIGNED: event_generated = enable_i.load_misaligned;

                    `LOAD_ACCESS_FAULT: event_generated = enable_i.load_access_fault;

                    `STORE_OPERATION: event_generated = enable_i.store_operation;

                    `STORE_MISALIGNED: event_generated = enable_i.store_misaligned;

                    `STORE_ACCESS_FAULT: event_generated = enable_i.store_access_fault; 

                    `U_SYSTEM_CALL: event_generated = enable_i.u_system_call;

                    `M_SYSTEM_CALL: event_generated = enable_i.m_system_call;

                    `SLEEP: event_generated = enable_i.sleep;

                    `HANDLER_RETURN: event_generated = enable_i.handler_return;

                    `BRANCH_OPERATION: event_generated = enable_i.branch;

                    `JUMP_OPERATION: event_generated = enable_i.jump;

                    `CSR_OPERATION: event_generated = enable_i.csr;

                    `NO_EVENT: event_generated = 1'b0;

                    default: event_generated = 1'b0;
                endcase
            end
        end


//====================================================================================
//      DIVERGENCE DETECTION
//====================================================================================

    /* Memorize the last PC value received from the trace interface, if the new one is different
     * than the classic PC+4 or PC+2 (for compressed instructions), it means that a branch or jump has been taken, 
     * so we need to generate a packet if branch tracing is enabled */
    logic [31:0] last_pc;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin
                last_pc <= 32'b0;
            end else if (trace_interface_i.valid & enable_branch_tracing_i) begin
                last_pc <= trace_interface_i.address;
            end
        end


    /* The first time we receive a PC, we don't have a reference to compare it with, so we can consider it as non-divergent
     * we delay the enable signal so that the first comparision is not considered valid. 
     *
     * EXAMPLE: PC is at 0x1000, but the last PC memorized was at 0x0800. If we enable branch tracing immediately it will be recognized as a divergence,
     * which is not the case. Instead we delay the enable signal so the new PC can be memorized so last PC is 0x0800 -> 0x1000 and the next time a valid PC
     * arrives 0x1004 for example it does not generate a divergence packet */
    logic branch_trace_ready;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin
                branch_trace_ready <= 1'b0;
            end else begin
                if (enable_branch_tracing_i) begin
                    if (enable_trigger_i) begin
                        /* If trigger is enabled, we need to wait for the trigger PC to be hit to start generating packets, otherwise we can have false positives in the divergence detection because the core is executing code before reaching the trigger PC that can cause the PC to jump around and generate divergence packets even if there is no branch taken */
                        if (trace_interface_i.valid & (trace_interface_i.address == trigger_pc_i)) begin
                            branch_trace_ready <= 1'b1;
                        end
                    end else begin
                        /* If trigger is not enabled, we can start generating packets immediately */
                        if (trace_interface_i.valid) begin
                            branch_trace_ready <= 1'b1;
                        end
                    end
                end else begin
                    branch_trace_ready <= 1'b0;
                end
            end
        end


    /* Trigger signal */
    logic divergence; 

        always_comb begin
            if (branch_trace_ready & trace_interface_i.valid) begin
                divergence = (trace_interface_i.address != last_pc + 4) & (trace_interface_i.address != last_pc + 2) & (trace_interface_i.address != last_pc);
            end else begin
                divergence = 1'b0;
            end
        end


//====================================================================================
//      PACKET BUILDER
//====================================================================================

    logic pending_overflow, overflow_serviced; 

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin
                pending_overflow <= 1'b0;
            end else begin
                if (overflow & (divergence | event_generated)) begin
                    /* If there is an overflow and we are generating a divergence or event packet, 
                     * we dont't need to prioritize the overflow packet, as we'll lose the PC/Event 
                     * information, which is more useful for debugging than the overflow information */
                    pending_overflow <= 1'b1;
                end else if (overflow_serviced) begin
                    pending_overflow <= 1'b0;
                end
            end
        end


    logic write_packet; trace_unit_packet_t trace_packet;

        always_comb begin
            /* Default values */
            trace_packet = '0;
            write_packet = 1'b0;
            overflow_serviced = 1'b0;
            
            /* Priority: Divergence > Event > Overflow */

            /* Don't generate packets if the core is halted, otherwise it can cause
             * deadlocks because the core is waiting for the packet to be consumed 
             * to be able to proceed and generate new packets, but the packet cannot
             * be consumed because the core is halted */
            if (divergence & enable_branch_tracing_i) begin
                /* Generate a divergence packet */
                trace_packet.divergence_packet.type_ = DIVERGENCE_PACKET;
                trace_packet.divergence_packet.delta_pc = trace_interface_i.address - last_pc;
                trace_packet.divergence_packet.timestamp = timestamp_counter;

                write_packet = !halt_core_o;
            end else if (event_generated) begin
                /* Generate an event packet */
                trace_packet.event_packet.type_ = EVENT_PACKET;
                trace_packet.event_packet.event_number = trace_interface_i.info;
                trace_packet.event_packet.timestamp = timestamp_counter;

                write_packet = !halt_core_o; 
            end else if (overflow | pending_overflow) begin
                /* Generate an overflow packet */
                trace_packet.overflow_packet.type_ = OVERFLOW_PACKET;

                write_packet = !halt_core_o;

                overflow_serviced = !halt_core_o;
            end
        end


//====================================================================================
//      PACKET BUFFER
//====================================================================================

    logic buffer_full;

    synchronous_buffer #(
        .DATA_WIDTH   ( $bits(trace_unit_packet_t) ),
        .BUFFER_DEPTH ( PACKET_BUFFER_SIZE         )
    ) packet_buffer (
        .clk_i   ( clk_i   ),
        .rst_n_i ( rst_n_i ),

        .write_i      ( write_packet ),
        .write_data_i ( trace_packet ),

        .read_i      ( trace_buffer_read_i ),
        .read_data_o ( trace_packet_o      ),

        .full_o  ( buffer_full          ),
        .empty_o ( trace_buffer_empty_o )
    );


        always_ff @(posedge clk_i) begin
            if (!rst_n_i) begin
                halt_core_o <= 1'b0;
            end else begin
                if (buffer_full) begin
                    halt_core_o <= 1'b1;
                end

                if (trace_buffer_empty_o) begin
                    halt_core_o <= 1'b0;
                end
            end
        end

    assign trace_buffer_full_o = buffer_full;

endmodule : trace_unit_packetizer

`endif