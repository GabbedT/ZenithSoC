`ifndef WAVEFORM_REGISTERS_SV
    `define WAVEFORM_REGISTERS_SV

`include "../../Utility/Packages/apu_pkg.sv"

module waveform_registers (
    input logic clk_i,
    input logic rst_n_i,

    /* Address interface */
    input logic write_i,
    input logic [3:0] write_strobe_i,
    input wave_synth_registers_t write_address_i,
    input logic read_i,
    input wave_synth_registers_t read_address_i,

    /* Data interface */
    input logic [31:0] write_data_i,
    output logic [31:0] read_data_o,

    /* Control interface */
    output logic wave_start_o,
    output logic wave_enable_o,
    output logic adsr_start_o,
    output logic adsr_enable_o,
    input logic adsr_idle_i,

    /* Gain interface */
    output logic [15:0] gain_o,

    /* Frequency interface */
    output logic [31:0] phase_increment_o,

    /* ADSR interface */
    output logic [31:0] attack_step_o,
    output logic [31:0] decay_step_o,
    output logic [31:0] release_step_o,
    output logic [31:0] sustain_duration_o,
    output logic [31:0] attack_level_o,
    output logic [31:0] sustain_level_o
);

//====================================================================================
//      MASK GENERATION
//====================================================================================

    logic [3:0][7:0] mask;

        always_comb begin
            for (int i = 0; i < 4; ++i) begin
                /* Mask certain bytes */
                mask[i] = write_strobe_i[i] ? '1 : '0;
            end
        end

//====================================================================================
//      CONTROL REGISTER
//====================================================================================

    wave_synth_control_t control_register;

        always_ff @(posedge clk_i) begin
            if (!rst_n_i) begin
                control_register <= '0;
            end else if ((write_address_i == WAVE_CONTROL) & write_i) begin
                control_register <= write_data_i | (control_register & mask);
            end else begin
                control_register.adsr_idle <= adsr_idle_i;

                /* Always reset */
                control_register.wave_start <= 1'b0;
            end
        end

    /* Wave Control */
    assign wave_start_o = control_register.wave_start;
    assign wave_enable_o = control_register.wave_enable;

    /* ADSR Control */
    assign adsr_start_o = control_register.adsr_start;
    assign adsr_enable_o = control_register.adsr_enable;

    /* Gain Control */
    assign gain_o = control_register.wave_gain;


//====================================================================================
//      PHASE INCREMENT REGISTER
//====================================================================================

    logic [31:0] phase_increment_register;

        always_ff @(posedge clk_i) begin
            if (!rst_n_i) begin
                phase_increment_register <= '0;
            end else if ((write_address_i == WAVE_INCREMENT) & write_i) begin
                phase_increment_register <= write_data_i | (phase_increment_register & mask);
            end
        end

    assign phase_increment_o = phase_increment_register;


//====================================================================================
//      ATTACK STEP REGISTER
//====================================================================================

    logic [31:0] attack_step;

        always_ff @(posedge clk_i) begin
            if (!rst_n_i) begin
                attack_step <= '0;
            end else if ((write_address_i == WAVE_ATTACK_STEP) & write_i) begin
                attack_step <= write_data_i | (attack_step & mask);
            end
        end

    assign attack_step_o = attack_step;


//====================================================================================
//      DECAY STEP REGISTER
//====================================================================================

    logic [31:0] decay_step;

        always_ff @(posedge clk_i) begin
            if (!rst_n_i) begin
                decay_step <= '0;
            end else if ((write_address_i == WAVE_DECAY_STEP) & write_i) begin
                decay_step <= write_data_i | (decay_step & mask);
            end
        end

    assign decay_step_o = decay_step;


//====================================================================================
//      RELEASE STEP REGISTER
//====================================================================================

    logic [31:0] release_step;

        always_ff @(posedge clk_i) begin
            if (!rst_n_i) begin
                release_step <= '0;
            end else if ((write_address_i == WAVE_RELEASE_STEP) & write_i) begin
                release_step <= write_data_i | (release_step & mask);
            end
        end

    assign release_step_o = release_step;


//====================================================================================
//      SUSTAIN DURATION REGISTER
//====================================================================================

    logic [31:0] sustain_duration;

        always_ff @(posedge clk_i) begin
            if (!rst_n_i) begin
                sustain_duration <= '0;
            end else if ((write_address_i == WAVE_SUSTAIN_TIME) & write_i) begin
                sustain_duration <= write_data_i | (sustain_duration & mask);
            end
        end

    assign sustain_duration_o = sustain_duration;


//====================================================================================
//      ATTACK LEVEL REGISTER
//====================================================================================

    logic [31:0] attack_level;

        always_ff @(posedge clk_i) begin
            if (!rst_n_i) begin
                attack_level <= '0;
            end else if ((write_address_i == WAVE_ATTACK_LEVEL) & write_i) begin
                attack_level <= write_data_i | (attack_level & mask);
            end
        end

    assign attack_level_o = attack_level;


//====================================================================================
//      SUSTAIN LEVEL REGISTER
//====================================================================================

    logic [31:0] sustain_level;

        always_ff @(posedge clk_i) begin
            if (!rst_n_i) begin
                sustain_level <= '0;
            end else if ((write_address_i == WAVE_SUSTAIN_LEVEL) & write_i) begin
                sustain_level <= write_data_i | (sustain_level & mask);
            end
        end

    assign sustain_level_o = sustain_level;


//====================================================================================
//      OUTPUT LOGIC
//====================================================================================

        always_comb begin
            if (read_i) begin
                case (read_address_i)
                    WAVE_CONTROL: read_data_o = control_register;

                    WAVE_INCREMENT: read_data_o = phase_increment_register;

                    WAVE_ATTACK_STEP: read_data_o = attack_step;

                    WAVE_DECAY_STEP: read_data_o = decay_step;

                    WAVE_RELEASE_STEP: read_data_o = release_step;

                    WAVE_SUSTAIN_TIME: read_data_o = sustain_duration;

                    WAVE_ATTACK_LEVEL: read_data_o = attack_level;

                    WAVE_SUSTAIN_LEVEL: read_data_o = sustain_level;

                    default: read_data_o = '0;
                endcase
            end else begin
                read_data_o = '0;
            end
        end

endmodule : waveform_registers

`endif 