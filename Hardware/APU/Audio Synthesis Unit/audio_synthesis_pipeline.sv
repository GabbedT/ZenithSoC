`ifndef AUDIO_SYNTHESIS_PIPELINE_SV
    `define AUDIO_SYNTHESIS_PIPELINE_SV

`include "audio_generation_pipeline.sv"
`include "adsr_modulator.sv"

module audio_synthesis_pipeline (
    input logic clk_i,
    input logic rst_n_i,

    /* Start ADSR cycle */
    input logic [3:0] adsr_start_i,
    input logic [3:0] adsr_enable_i,
    
    /* Increment of each state */
    input logic [3:0][31:0] attack_step_i,
    input logic [3:0][31:0] decay_step_i,
    input logic [3:0][31:0] release_step_i,

    /* Time duration */
    input logic [3:0][31:0] sustain_duration_i,

    /* Gain level  */
    input logic [3:0][31:0] attack_level_i,
    input logic [3:0][31:0] sustain_level_i,

    /* Status */
    output logic [3:0] adsr_idle_o,

    /* Custom table control */
    input logic write_table_i,
    input logic [9:0] table_addr_i,
    input logic [15:0] pcm_i,

    /* Control */
    input logic [3:0] wave_enable_i,
    input logic [3:0] wave_start_i,

    /* Frequency control */
    input logic [3:0][31:0] wave_frequency_i,

    /* Duty cycle control */
    input logic [31:0] square_wave_duty_cycle_i,

    /* Gains Q1.15 */
    input logic [3:0][15:0] wave_gain_i,

    /* Output Wave */
    output logic [15:0] mixed_wave_o
);

//====================================================================================
//      PARAMETERS
//====================================================================================

    localparam SINE_WAVE = 0;

    localparam SQUARE_WAVE = 1;

    localparam TRIANGLE_WAVE = 2;

    localparam CUSTOM_WAVE = 3;


//====================================================================================
//      ADSR FSMs
//====================================================================================

    /* Envelope modulator from ADSR */
    logic [3:0][15:0] adsr_modulator, wave_gain;

    genvar i;

    generate
        for (i = 0; i < 4; ++i) begin
            adsr_modulator wave_modulator (
                .clk_i              ( clk_i                 ),
                .rst_n_i            ( rst_n_i               ),
                .start_i            ( adsr_start_i[i]       ),
                .enable_i           ( adsr_enable_i[i]      ),
                .attack_step_i      ( attack_step_i[i]      ),
                .decay_step_i       ( decay_step_i[i]       ),
                .release_step_i     ( release_step_i[i]     ),
                .sustain_duration_i ( sustain_duration_i[i] ),
                .attack_level_i     ( attack_level_i[i]     ),
                .sustain_level_i    ( sustain_level_i[i]    ),
                .idle_o             ( adsr_idle_o[i]        ),
                .modulator_o        ( adsr_modulator[i]     )
            );

            /* Select between static and dynamic gain */
            assign wave_gain[i] = adsr_enable_i[i] ? adsr_modulator[i] : wave_gain_i[i];
        end
    endgenerate


//====================================================================================
//      WAVE GENERATION
//====================================================================================


    audio_generation_pipeline audio_wave_generator (
        .clk_i   ( clk_i   ),
        .rst_n_i ( rst_n_i ),

        /* Custom waveform */
        .write_table_i ( write_table_i ),
        .table_addr_i  ( table_addr_i  ),
        .pcm_i         ( pcm_i         ),

        /* Control signals for the different waves */
        .sine_wave_enable_i     ( wave_enable_i[SINE_WAVE]     ),
        .sine_wave_start_i      ( wave_start_i[SINE_WAVE]      ),
        .square_wave_enable_i   ( wave_enable_i[SQUARE_WAVE]   ),
        .square_wave_start_i    ( wave_start_i[SQUARE_WAVE]    ),
        .triangle_wave_enable_i ( wave_enable_i[TRIANGLE_WAVE] ),
        .triangle_wave_start_i  ( wave_start_i[TRIANGLE_WAVE]  ),
        .custom_wave_enable_i   ( wave_enable_i[CUSTOM_WAVE]   ),
        .custom_wave_start_i    ( wave_start_i[CUSTOM_WAVE]    ),

        /* Frequency control signals */
        .sine_wave_frequency_i     ( wave_frequency_i[SINE_WAVE]     ),
        .square_wave_frequency_i   ( wave_frequency_i[SQUARE_WAVE]   ),
        .triangle_wave_frequency_i ( wave_frequency_i[TRIANGLE_WAVE] ),
        .custom_wave_frequency_i   ( wave_frequency_i[CUSTOM_WAVE]   ),

        /* Duty cycle for square wave */
        .square_wave_duty_cycle_i ( square_wave_duty_cycle_i ),

        /* Gain control signals */
        .sine_wave_gain_i       ( wave_gain[SINE_WAVE]     ),
        .square_wave_gain_i     ( wave_gain[SQUARE_WAVE]   ),
        .triangle_wave_gain_i   ( wave_gain[TRIANGLE_WAVE] ),
        .custom_wave_gain_i     ( wave_gain[CUSTOM_WAVE]   ),
        
        /* Output mixed wave */
        .mixed_wave_o ( mixed_wave_o )
    );

endmodule : audio_synthesis_pipeline

`endif 