`ifndef TRACE_UNIT_SV
    `define TRACE_UNIT_SV

module trace_unit #(
    /* Buffer depth */
    parameter PACKET_BUFFER_SIZE = 512
) (
    /* Global signals */
    input logic clk_i,
    input logic rst_n_i,

    /* CPU Signals */
    output logic interrupt_o,
    output logic halt_core_o,

    /* UART status */
    input logic uart_tx_full_i,

    /* Write interface */
    input logic write_i,
    input logic [1:0] write_address_i,
    input logic [31:0] write_data_i,
    input logic [3:0] write_strobe_i,
    output logic write_error_o,
    output logic write_done_o,

    /* Read interface */
    input logic read_i,
    input logic [1:0] read_address_i,
    output logic [31:0] read_data_o,
    output logic read_error_o,
    output logic read_done_o,

    /* Trace channel from CPU */
    trace_interface.slave trace_interface_i,

    /* Trace packets to UART */
    output logic [7:0] trace_chunk_o,
    output logic write_chunk_o
);

//====================================================================================
//      REGISTERS
//====================================================================================

    logic enable_event_tracing, enable_branch_tracing, enable_timestamp_event, enable_timestamp_branch, enable_trigger;
    logic clear_timestamp, trace_buffer_empty, trace_buffer_full; logic [31:0] trigger_pc;
    trace_unit_event_t enable_events;

    trace_unit_registers registers (
        /* Global signals */
        .clk_i       ( clk_i       ),
        .rst_n_i     ( rst_n_i     ),
        .interrupt_o ( interrupt_o ),

        /* Configuration Tracing */
        .enable_event_tracing_o  ( enable_event_tracing  ),
        .enable_branch_tracing_o ( enable_branch_tracing ),

        /* Configuration Timestamp */
        .enable_timestamp_event_o  ( enable_timestamp_event  ),
        .enable_timestamp_branch_o ( enable_timestamp_branch ),

        /* Configuration Trigger PC */
        .enable_trigger_o ( enable_trigger ),
        .trigger_pc_o     ( trigger_pc     ),

        /* Timer control */
        .clear_timestamp_o ( clear_timestamp ),

        /* From Packetizer buffer */
        .trace_buffer_empty_i ( trace_buffer_empty ),
        .trace_buffer_full_i  ( trace_buffer_full  ),

        /* Enable interrupts */
        .enable_event_o ( enable_events ),

        /* Write interface */
        .write_i         ( write_i         ),
        .write_address_i ( write_address_i ),
        .write_data_i    ( write_data_i    ),
        .write_strobe_i  ( write_strobe_i  ),
        .write_error_o   ( write_error_o   ),

        /* Read interface */
        .read_i         ( read_i         ),
        .read_address_i ( read_address_i ),
        .read_data_o    ( read_data_o    ),
        .read_error_o   ( read_error_o   )
    );


        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin
                write_done_o <= 1'b0;
                read_done_o <= 1'b0;
            end else begin
                write_done_o <= write_i;
                read_done_o <= read_i;
            end
        end


//====================================================================================
//      PACKETIZER
//====================================================================================

    logic trace_buffer_read; trace_unit_packet_t trace_packet;

    trace_unit_packetizer #(
        /* Buffer depth */
        .PACKET_BUFFER_SIZE ( PACKET_BUFFER_SIZE )
    ) packetizer (
        /* Global signals */
        .clk_i       ( clk_i       ),
        .rst_n_i     ( rst_n_i     ),
        .halt_core_o ( halt_core_o ),

        /* Configuration Tracing */
        .enable_event_tracing_i  ( enable_event_tracing  ),
        .enable_branch_tracing_i ( enable_branch_tracing ),

        /* Configuration Timestamp */
        .enable_timestamp_event_i  ( enable_timestamp_event  ),
        .enable_timestamp_branch_i ( enable_timestamp_branch ),

        /* Configuration Trigger PC */
        .enable_trigger_i ( enable_trigger ),
        .trigger_pc_i     ( trigger_pc     ),

        /* Timestamp control */
        .clear_timestamp_i ( clear_timestamp ),

        /* Enable events */
        .enable_i ( enable_events ),

        /* Trace channel from CPU */
        .trace_interface_i ( trace_interface_i ),

        /* To Registers */
        .trace_buffer_empty_o ( trace_buffer_empty ),
        .trace_buffer_full_o  ( trace_buffer_full  ),

        /* Trace packets to Serializer */
        .trace_buffer_read_i  ( trace_buffer_read  ),
        .trace_packet_o       ( trace_packet       )
    );


//====================================================================================
//      SERIALIZER
//====================================================================================

    trace_unit_serializer serializer (
        /* Global signals */
        .clk_i   ( clk_i       ),
        .rst_n_i ( rst_n_i     ),
    
        /* From UART */
        .uart_tx_full_i ( uart_tx_full_i ),

        /* Configuartion */
        .enable_timestamp_i ( enable_timestamp_event | enable_timestamp_branch ),

        /* Trace packets from Packetizer */
        .trace_buffer_empty_i ( trace_buffer_empty ),
        .trace_packet_i       ( trace_packet       ),
        .trace_buffer_read_o  ( trace_buffer_read  ),

        /* Trace chunks to UART */
        .trace_chunk_o ( trace_chunk_o ),
        .write_chunk_o ( write_chunk_o )
    );

endmodule : trace_unit

`endif 