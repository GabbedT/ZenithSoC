`ifndef VGA_FLUSH_ENGINE_SV
    `define VGA_FLUSH_ENGINE_SV

module vga_flush_engine #(
    parameter BASE_WIDTH = 27,
    parameter OUTSTANDING_WIDTH = 4
) (
    input logic clk_i,
    input logic rst_n_i,
    input logic enable_video_i,
    input logic video_on_i,

    input logic [BASE_WIDTH - 1:0] frame_buffer_base_i,
    input logic early_frame_done_i,
    input logic [OUTSTANDING_WIDTH - 1:0] outstanding_count_i,

    output logic flush_sequencer_o,
    output logic flush_line_buffer_o
);

    typedef enum logic [1:0] {
        IDLE,
        WAIT_OUTSTANDING,
        FLUSH
    } state_t;

    state_t state_CRT, state_NXT;
    logic [BASE_WIDTH - 1:0] frame_buffer_base_delay;
    logic early_frame_seen;
    logic base_changed;

    assign base_changed = frame_buffer_base_i != frame_buffer_base_delay;

    /* Keep the early-frame notification until software writes the new base. */
    always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
        if (!rst_n_i) begin
            state_CRT <= IDLE;
            frame_buffer_base_delay <= '0;
            early_frame_seen <= 1'b0;
        end else if (!enable_video_i) begin
            /* Disabling the display already clears both downstream buffers. */
            state_CRT <= IDLE;
            frame_buffer_base_delay <= frame_buffer_base_i;
            early_frame_seen <= 1'b0;
        end else begin
            state_CRT <= state_NXT;

            if (state_CRT == FLUSH) begin
                frame_buffer_base_delay <= frame_buffer_base_i;
                early_frame_seen <= 1'b0;
            end else if (early_frame_done_i) begin
                early_frame_seen <= 1'b1;
            end else if (video_on_i && !base_changed) begin
                /* The blanking window passed without a base change. */
                early_frame_seen <= 1'b0;
            end
        end
    end

    always_comb begin
        state_NXT = state_CRT;

        case (state_CRT)
            IDLE: begin
                /* Accept a base change only after early-frame notification. */
                if (base_changed &&
                    (early_frame_done_i || early_frame_seen) &&
                    !video_on_i) begin
                    state_NXT = WAIT_OUTSTANDING;
                end
            end

            WAIT_OUTSTANDING: begin
                if (!base_changed) begin
                    state_NXT = IDLE;
                end else if (outstanding_count_i == '0) begin
                    state_NXT = FLUSH;
                end
            end

            FLUSH: begin
                /* One-cycle pulse. */
                state_NXT = IDLE;
            end

            default: state_NXT = IDLE;
        endcase
    end

    /* Empty data is not enough to skip the flush: control pointers can remain stale. */
    assign flush_sequencer_o = state_CRT == FLUSH;
    assign flush_line_buffer_o = state_CRT == FLUSH;

endmodule : vga_flush_engine

`endif
