`ifndef ADSR_MODULATOR_SV
    `define ADSR_MODULATOR_SV

module adsr_modulator (
    input logic clk_i,
    input logic rst_n_i,

    /* Start ADSR cycle */
    input logic start_i,
    input logic enable_i,
    
    /* Increment of each state */
    input logic [31:0] attack_step_i,
    input logic [31:0] decay_step_i,
    input logic [31:0] release_step_i,

    /* Time duration */
    input logic [31:0] sustain_duration_i,

    /* Gain level  */
    input logic [31:0] attack_level_i,
    input logic [31:0] sustain_level_i,

    /* Status */
    output logic idle_o,

    /* Envelope */
    output logic [15:0] modulator_o
);

//====================================================================================
//      TEMPORARY REGISTERS 
//====================================================================================

    /* Count the time and rapresent the modulator */
    logic [31:0] modulator_CRT, modulator_NXT; 
    logic [31:0] sus_time_CRT, sus_time_NXT; 

        always_ff @(posedge clk_i) begin
            if (!rst_n_i | start_i) begin
                modulator_CRT <= '0;
                sus_time_CRT <= '0;
            end else begin
                modulator_CRT <= modulator_NXT;
                sus_time_CRT <= sus_time_NXT;
            end
        end

    assign modulator_o = modulator_CRT[31:16];


//====================================================================================
//      FSM LOGIC
//====================================================================================

    typedef enum logic [2:0] { IDLE, ATTACK, DECAY, SUSTAIN, RELEASE } fsm_states_t;
    
    fsm_states_t state_CRT, state_NXT;
    
        always_ff @(posedge clk_i) begin
            if (!rst_n_i | !enable_i) begin 
                state_CRT <= IDLE; 
            end else if (enable_i) begin
                if (start_i) begin
                    state_CRT <= ATTACK; 
                end else begin
                    state_CRT <= state_NXT;
                end
            end 
        end 


        always_comb begin
            /* Default Values */
            modulator_NXT = modulator_CRT;
            sus_time_NXT = sus_time_CRT;
            state_NXT = state_CRT;

            case (state_CRT)
                IDLE: begin
                    if (start_i) begin
                        state_NXT = ATTACK;

                        sus_time_NXT = '0;
                        modulator_NXT = '0;
                    end
                end

                /* In this state, the envelope starts increasing until
                 * the final envelope level of attack */
                ATTACK: begin
                    modulator_NXT = modulator_CRT + attack_step_i;

                    /* Go to DECAY to decrease the modulator */
                    if (modulator_CRT >= attack_level_i) begin
                        state_NXT = DECAY;

                        modulator_NXT = attack_level_i;
                    end
                end

                /* The envelope decrease until it reaches the level
                 * choosen for the sustain period */
                DECAY: begin
                    modulator_NXT = modulator_CRT - decay_step_i;

                    if (modulator_CRT <= sustain_level_i) begin
                        state_NXT = SUSTAIN;
                        
                        modulator_NXT = sustain_level_i;
                        sus_time_NXT = '0;
                    end
                end

                /* The envelope is stable, time is calculated
                 * through a counter, once the time expires,
                 * go to RELEASE state */
                SUSTAIN: begin
                    sus_time_NXT = sus_time_CRT + 1'b1;

                    if (sus_time_CRT == sustain_duration_i) begin
                        state_NXT = RELEASE;
                    end
                end

                /* During release, the envelope starts decreasing
                 * unti is 0 */
                RELEASE: begin
                    modulator_NXT = modulator_CRT - release_step_i;

                    /* To avoid wrap around and glitches */
                    if (modulator_CRT <= release_step_i) begin
                        state_NXT = IDLE;

                        modulator_NXT = '0;
                    end
                end
            endcase
        end

    assign idle_o = state_CRT == IDLE;

endmodule : adsr_modulator

`endif 