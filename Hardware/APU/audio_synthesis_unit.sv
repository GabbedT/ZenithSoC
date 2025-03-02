`ifndef AUDIO_SYNTHESIS_UNIT_SV
    `define AUDIO_SYNTHESIS_UNIT_SV

`include "Audio Synthesis Unit/audio_synthesis_pipeline.sv"
`include "Interface/as_unit_registers.sv"
`include "Interface/pcm2pdm_converter.sv"

module audio_synthesis_unit (
    input logic clk_i,
    input logic rst_n_i,

    /* Write interface */
    input logic write_i,
    input logic [10:0] write_address_i,
    input logic [3:0][7:0] write_data_i,
    input logic [3:0] write_strobe_i,
    output logic write_error_o,

    /* Read interface */
    input logic read_i,
    input logic [10:0] read_address_i,
    output logic [31:0] read_data_o,
    output logic read_error_o,

    /* PWM interface */
    output logic pwm_o,
    output logic audio_enable_o
);

//====================================================================================
//      REGISTER INTERFACE
//====================================================================================

    /* Start ADSR cycle */
    logic [3:0] adsr_start, adsr_enable;
    
    /* Increment of each state */
    logic [3:0][31:0] attack_step, decay_step, release_step;

    /* Time duration */
    logic [3:0][31:0] sustain_duration;

    /* Gain level  */
    logic [3:0][31:0] attack_level, sustain_level;

    /* Status */
    logic [3:0] adsr_idle;

    /* Custom table control */
    logic write_table; logic [9:0] table_addr; logic [15:0] pcm;

    /* Control */
    logic [3:0] wave_enable, wave_start;

    /* Frequency control */
    logic [3:0][31:0] wave_frequency;

    /* Duty cycle control */
    logic [31:0] square_wave_duty_cycle;

    /* Gains Q1.15 */
    logic [3:0][15:0] wave_gain;

    /* Output wave */
    logic [15:0] mixed_wave;

    as_unit_registers registers (
        .clk_i   ( clk_i   ),
        .rst_n_i ( rst_n_i ),

        /* Write interface */
        .write_i        ( write_i         ),
        .write_address_i( write_address_i ),
        .write_data_i   ( write_data_i    ),
        .write_strobe_i ( write_strobe_i  ),

        /* Read interface */
        .read_i         ( read_i         ),
        .read_address_i ( read_address_i ),
        .read_data_o    ( read_data_o    ),

        /* Start ADSR cycle */
        .adsr_start_o  ( adsr_start  ),
        .adsr_enable_o ( adsr_enable ),
        
        /* Increment of each state */
        .attack_step_o  ( attack_step  ),
        .decay_step_o   ( decay_step   ),
        .release_step_o ( release_step ),

        /* Time duration */
        .sustain_duration_o ( sustain_duration ),

        /* Gain level  */
        .attack_level_o  ( attack_level  ),
        .sustain_level_o ( sustain_level ),

        /* Status */
        .adsr_idle_i ( adsr_idle ),

        /* Custom table control */
        .write_table_o ( write_table ),
        .table_addr_o  ( table_addr  ),
        .pcm_o         ( pcm         ),

        /* Control */
        .wave_enable_o ( wave_enable ),
        .wave_start_o  ( wave_start  ),

        /* Frequency control */
        .wave_frequency_o ( wave_frequency ),

        /* Duty cycle control */
        .square_wave_duty_cycle_o ( square_wave_duty_cycle ),

        /* Gains Q1.15 */
        .wave_gain_o ( wave_gain )
    );

    assign write_error_o = 1'b0;
    assign read_error_o = 1'b0;


//====================================================================================
//      REGISTER INTERFACE
//====================================================================================

    audio_synthesis_pipeline waveform_synthesizer (
        .clk_i   ( clk_i   ),
        .rst_n_i ( rst_n_i ),

        .adsr_start_i  ( adsr_start  ),
        .adsr_enable_i ( adsr_enable ),

        .attack_step_i  ( attack_step  ),
        .decay_step_i   ( decay_step   ),
        .release_step_i ( release_step ),

        .sustain_duration_i ( sustain_duration ),

        .attack_level_i  ( attack_level  ),
        .sustain_level_i ( sustain_level ),

        .adsr_idle_o ( adsr_idle ),

        .write_table_i ( write_table ),
        .table_addr_i  ( table_addr  ),
        .pcm_i         ( pcm         ),

        .wave_enable_i            ( wave_enable            ),
        .wave_start_i             ( wave_start             ),
        .wave_frequency_i         ( wave_frequency         ),
        .wave_gain_i              ( wave_gain              ),
        .square_wave_duty_cycle_i ( square_wave_duty_cycle ),

        .mixed_wave_o ( mixed_wave )
    );


//====================================================================================
//      OUTPUT LOGIC
//====================================================================================

    logic pdm_signal;

    pcm2pdm_converter audio_interface (
        .clk_i    ( clk_i          ),
        .rst_n_i  ( rst_n_i        ),
        .enable_i ( audio_enable_o ),

        .pcm_i ( mixed_wave ),
        .pdm_o ( pdm_signal )
    );

    assign pwm_o = pdm_signal ? 1'bZ : 1'b0;

    assign audio_enable_o = wave_enable != '0;

endmodule : audio_synthesis_unit

`endif 