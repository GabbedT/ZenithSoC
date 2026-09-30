`ifndef TRACE_UNIT_SERIALIZER_SV
    `define TRACE_UNIT_SERIALIZER_SV

module trace_unit_serializer (
    /* Global signals */
    input logic clk_i,
    input logic rst_n_i,

    /* From UART */
    input logic uart_tx_full_i,

    /* Configuration */
    input logic enable_timestamp_i,

    /* Trace packets from Packetizer */
    input logic trace_buffer_empty_i,
    input trace_unit_packet_t trace_packet_i,
    output logic trace_buffer_read_o,

    /* Trace chunks to UART */
    output logic [7:0] trace_chunk_o,
    output logic busy_o,
    output logic write_chunk_o
);

//====================================================================================
//      FSM DATA
//====================================================================================

    /* Temporary data used by the FSM datapath */
    trace_unit_packet_t packet_CRT, packet_NXT;
    logic [3:0] byte_counter_CRT, byte_counter_NXT;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin
                packet_CRT <= '0;
                byte_counter_CRT <= '0;
            end else begin
                packet_CRT <= packet_NXT;
                byte_counter_CRT <= byte_counter_NXT;
            end
        end


    typedef enum logic [1:0] { IDLE, SYNC, DATA, ESCAPED } fsm_state_t;

    fsm_state_t state_CRT, state_NXT;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin
                state_CRT <= IDLE;
            end else begin
                state_CRT <= state_NXT;
            end
        end


//====================================================================================
//      FSM DATAPATH
//====================================================================================

    logic reserved_byte;

    assign busy_o = state_CRT != IDLE;
    assign reserved_byte = (packet_CRT.raw[7] == TRACE_SYNC) | (packet_CRT.raw[7] == TRACE_ESCAPE);

        always_comb begin
            byte_counter_NXT = byte_counter_CRT;
            packet_NXT = packet_CRT;
            state_NXT = state_CRT;

            trace_chunk_o = '0;
            write_chunk_o = 1'b0;
            trace_buffer_read_o = 1'b0;

            case (state_CRT)
                IDLE: begin
                    /* Start if there is a packet to read and the UART is not full */
                    if (!trace_buffer_empty_i & !uart_tx_full_i) begin
                        trace_buffer_read_o = 1'b1;
                        state_NXT = SYNC;
                    end
                end

                SYNC: begin
                    trace_chunk_o = TRACE_SYNC;
                    write_chunk_o = !uart_tx_full_i;

                    if (!uart_tx_full_i) begin
                        /* The synchronous packet buffer is valid after the IDLE read */
                        packet_NXT = trace_packet_i;
                        state_NXT = DATA;

                        /* Latch the number of body bytes before serialization */
                        case (trace_packet_i.raw[7][7:6])
                            EVENT_PACKET: byte_counter_NXT = enable_timestamp_i ? 4'd4 : 4'd1;

                            DIVERGENCE_PACKET: byte_counter_NXT = enable_timestamp_i ? 4'd8 : 4'd5;

                            START_PACKET: byte_counter_NXT = 4'd5;
                            
                            default: byte_counter_NXT = 4'd1;
                        endcase
                    end
                end

                DATA: begin
                    trace_chunk_o = reserved_byte ? TRACE_ESCAPE : packet_CRT.raw[7];
                    write_chunk_o = !uart_tx_full_i;

                    if (!uart_tx_full_i) begin
                        if (reserved_byte) begin
                            /* Keep the body byte until its escaped value is sent */
                            state_NXT = ESCAPED;
                        end else begin
                            /* Shift by 8 bits and count the completed body byte */
                            packet_NXT.raw = { packet_CRT.raw[6:0], 8'h00 };
                            byte_counter_NXT = byte_counter_CRT - 1'b1;

                            if (byte_counter_CRT == 1) begin
                                state_NXT = IDLE;
                            end
                        end
                    end
                end

                ESCAPED: begin
                    trace_chunk_o = packet_CRT.raw[7] ^ TRACE_ESCAPE_XOR;
                    write_chunk_o = !uart_tx_full_i;

                    if (!uart_tx_full_i) begin
                        /* The escape prefix does not count as a body byte */
                        packet_NXT.raw = { packet_CRT.raw[6:0], 8'h00 };
                        byte_counter_NXT = byte_counter_CRT - 1'b1;
                        state_NXT = DATA;

                        if (byte_counter_CRT == 1) begin
                            state_NXT = IDLE;
                        end
                    end
                end

                default: state_NXT = IDLE;
            endcase
        end

endmodule : trace_unit_serializer

`endif
