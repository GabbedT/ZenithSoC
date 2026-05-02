`ifndef TRACE_UNIT_SERIALIZER_SV
    `define TRACE_UNIT_SERIALIZER_SV

`include "../../Utility/Packages/trace_unit_pkg.sv"

module trace_unit_serializer (
    /* Global signals */
    input logic clk_i,
    input logic rst_n_i,

    /* From UART */
    input logic uart_tx_full_i,

    /* Configuartion */
    input logic enable_timestamp_i,

    /* Trace packets from Packetizer */
    input logic trace_buffer_empty_i,
    input trace_unit_packet_t trace_packet_i,
    output logic trace_buffer_read_o,

    /* Trace chunks to UART */
    output logic [7:0] trace_chunk_o,
    output logic write_chunk_o
);

//====================================================================================
//      FSM DATA
//====================================================================================

    /* Temporary data used by the FSM datapath */
    trace_unit_packet_type_t packet_type_CRT, packet_type_NXT;
    trace_unit_packet_t packet_CRT, packet_NXT;
    logic [3:0] byte_counter_CRT, byte_counter_NXT;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            packet_type_CRT <= packet_type_NXT;
            packet_CRT <= packet_NXT;
            byte_counter_CRT <= byte_counter_NXT;
        end


    typedef enum logic [1:0] { IDLE, SYNC, DATA } fsm_state_t;

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

        always_comb begin
            byte_counter_NXT = byte_counter_CRT;
            packet_type_NXT = packet_type_CRT;
            packet_NXT = packet_CRT;
            state_NXT = state_CRT;

            trace_chunk_o = '0;
            write_chunk_o = 1'b0;
            trace_buffer_read_o = 1'b0;

            case (state_CRT)
                IDLE: begin
                    /* Start if there is a packet to read and the UART is not full */
                    if (!trace_buffer_empty_i & !uart_tx_full_i) begin
                        /* Read packet from packetizer buffer */
                        trace_buffer_read_o = 1'b1;

                        /* Reset byte counter */
                        byte_counter_NXT = '0;
                        
                        state_NXT = SYNC;
                    end
                end

                SYNC: begin
                    if (!uart_tx_full_i) begin
                        state_NXT = DATA;

                        /* Extract packet type for the DATA state from last two bits */
                        packet_type_NXT = trace_unit_packet_type_t'(trace_packet_i.raw[11][7:6]);
                        packet_NXT = trace_packet_i;

                        /* Send SYNC byte */
                        write_chunk_o = 1'b1;
                        trace_chunk_o = 8'hAA;
                    end
                end

                DATA: begin
                    if (!uart_tx_full_i) begin
                        trace_chunk_o = packet_CRT.raw[11];
                        write_chunk_o = 1'b1;

                        /* Shift by 8 bits */
                        packet_NXT.raw = { packet_CRT.raw[10:0], 8'h00 };

                        byte_counter_NXT = byte_counter_CRT + 1;

                        case (packet_type_CRT)
                            OVERFLOW_PACKET: begin
                                /* Overflow packet payload is 1 bytes long so go straight to IDLE */
                                state_NXT = IDLE;
                            end

                            DIVERGENCE_PACKET: begin
                                if (enable_timestamp_i) begin
                                    /* Divergence packet payload is 12 bytes long */
                                    if (byte_counter_CRT == 11) begin
                                        state_NXT = IDLE;
                                    end
                                end else begin
                                    /* Divergence packet payload is 9 bytes long */
                                    if (byte_counter_CRT == 8) begin
                                        state_NXT = IDLE;
                                    end
                                end
                            end

                            EVENT_PACKET: begin
                                if (enable_timestamp_i) begin
                                    /* Event packet payload is 4 bytes long */
                                    if (byte_counter_CRT == 3) begin
                                        state_NXT = IDLE;
                                    end
                                end else begin
                                    /* Event packet payload is 1 bytes long */
                                    state_NXT = IDLE;
                                end
                            end

                            default: state_NXT = IDLE;
                        endcase
                    end
                end

                default: state_NXT = IDLE;
            endcase
        end

endmodule : trace_unit_serializer

`endif  