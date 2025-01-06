`ifndef PLAYBACK_CONTROLLER_SV
    `define PLAYBACK_CONTROLLER_SV

module playback_controller (
    input logic clk_i,
    input logic rst_n_i,

    /* User interface */
    input logic start_playback_i,

    /* DDR interface */
    output logic read_packet_o,
    output logic extract_packet_o,
    input logic packet_valid_i,
    input logic [7:0][7:0] sample_packet_i,

    /* PWM interface */
    input logic next_sample_i,
    output logic [7:0] sample_o,
    output logic valid_sample_o
);

//====================================================================================
//      SAVED VARIABLES
//====================================================================================

    logic [2:0] sample_counter_CRT, sample_counter_NXT;
    logic [7:0][7:0] sample_packet_CRT, sample_packet_NXT;

        always_ff @(posedge clk_i) begin
            sample_counter_CRT <= sample_counter_NXT;
            sample_packet_CRT <= sample_packet_NXT;
        end


//====================================================================================
//      FSM LOGIC
//====================================================================================

    typedef enum logic [2:0] { IDLE, READ, WAIT, INSERT, PLAY } playback_fsm_t;

    playback_fsm_t state_CRT, state_NXT;

        always_ff @(posedge clk_i) begin
            if (!rst_n_i | !start_playback_i) begin
                state_CRT <= IDLE;
            end else begin
                state_CRT <= state_NXT;
            end
        end

        always_comb begin
            /* Default values */
            sample_packet_NXT = sample_packet_CRT;
            sample_counter_NXT = sample_counter_CRT;
            state_NXT = state_CRT;

            read_packet_o = 1'b0;
            extract_packet_o = 1'b0;
            valid_sample_o = 1'b0;
            sample_o = 1'b0;

            case (state_CRT)
                IDLE: begin
                    if (start_playback_i) begin
                        /* Issue a read command to DDR memory */
                        state_NXT = READ; 
                    end

                    sample_counter_NXT = '0;
                end

                READ: begin
                    read_packet_o = 1'b1;

                    state_NXT = WAIT;
                end

                WAIT: begin
                    if (packet_valid_i) begin
                        extract_packet_o = 1'b1;

                        state_NXT = INSERT;
                    end
                end

                INSERT: begin
                    sample_packet_NXT = sample_packet_i;

                    state_NXT = PLAY;
                end

                PLAY: begin
                    if (next_sample_i) begin
                        valid_sample_o = 1'b1;

                        sample_o = sample_packet_CRT[sample_counter_CRT];

                        sample_counter_NXT = sample_counter_CRT + 1'b1;
                    end

                    if (sample_counter_CRT == 7 & next_sample_i) begin
                        /* Hopefully the sample comes before the next PWM reqeust */
                        state_NXT = packet_valid_i ? WAIT : READ;
                    end
                end
            endcase
        end

endmodule : playback_controller

`endif 