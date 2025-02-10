`ifndef SINE_WAVE_GENERATOR_SV
    `define SINE_WAVE_GENERATOR_SV

module sine_wave_generator (
    input logic clk_i,
    input logic rst_n_i,

    /* Generator control */
    input logic enable_i,
    input logic start_i,

    /* Increment value for phase register */
    input logic [31:0] inc_step_i,

    /* PCM output */
    output logic [15:0] sin_wave_o
);

//====================================================================================
//      SINE TABLE
//====================================================================================

    logic [15:0] sine_table [511:0];

    initial begin
        $readmemh("/home/gabriele/Desktop/Projects/ZenithSoC/Hardware/APU/Audio Synthesis Unit/Sine Generation/sine_quarter.hex", sine_table);
    end


    logic [8:0] table_index; logic [15:0] offset;

        always_ff @(posedge clk_i) begin
            if (enable_i) begin
                if (offset == '0) begin
                    sin_wave_o <= sine_table[table_index];
                end else begin 
                    sin_wave_o <= offset - sine_table[table_index];
                end
            end else begin
                sin_wave_o <= '0;
            end
        end


//====================================================================================
//      PHASE REGISTER FSM
//====================================================================================

    logic [32:0] phase_register_CRT, phase_register_NXT;

        always_ff @(posedge clk_i) begin
            if (!rst_n_i | start_i) begin
                phase_register_CRT <= '0;
            end else if (enable_i) begin
                phase_register_CRT <= phase_register_NXT;
            end
        end

    assign table_index = phase_register_CRT[31:23];


    
    typedef enum logic [1:0] { FIRST_PI4, SECOND_PI4, THIRD_PI4, FOURTH_PI4 } fsm_states_t;

    fsm_states_t state_CRT, state_NXT;

        always_ff @(posedge clk_i) begin
            if (!rst_n_i | start_i) begin 
                state_CRT <= FIRST_PI4; 
            end else if (enable_i) begin 
                state_CRT <= state_NXT;
            end 
        end 


        /* The table contains only a fourth of the sine period, so the phase
         * register should switch between incrementing and decrementing each
         * change of 1/4 period */
        always_comb begin
            /* Default Values */
            phase_register_NXT = phase_register_CRT;
            state_NXT = state_CRT;

            offset = '0;

            case (state_CRT)
                FIRST_PI4: begin
                    /* Increment */
                    phase_register_NXT = phase_register_CRT + inc_step_i;

                    if (phase_register_NXT[32]) begin
                        state_NXT = SECOND_PI4;

                        phase_register_NXT = phase_register_CRT - inc_step_i;
                    end

                    offset = '0;
                end

                SECOND_PI4: begin
                    /* Decrement */
                    phase_register_NXT = phase_register_CRT - inc_step_i;

                    if (phase_register_NXT[32]) begin
                        state_NXT = THIRD_PI4;

                        phase_register_NXT = phase_register_CRT + inc_step_i;
                    end

                    offset = '0;
                end

                THIRD_PI4: begin
                    /* Increment */
                    phase_register_NXT = phase_register_CRT + inc_step_i;

                    if (phase_register_NXT[32]) begin
                        state_NXT = FOURTH_PI4;

                        phase_register_NXT = phase_register_CRT - inc_step_i;
                    end

                    offset = '1;
                end

                FOURTH_PI4: begin
                    /* Decrement */
                    phase_register_NXT = phase_register_CRT - inc_step_i;

                    if (phase_register_NXT[32]) begin
                        state_NXT = FIRST_PI4;

                        phase_register_NXT = phase_register_CRT + inc_step_i;
                    end

                    offset = '1;
                end
            endcase
        end

endmodule : sine_wave_generator

`endif