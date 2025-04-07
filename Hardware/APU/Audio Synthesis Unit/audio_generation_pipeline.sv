`ifndef AUDIO_GENERATION_PIPELINE_SV
    `define AUDIO_GENERATION_PIPELINE_SV

module audio_generation_pipeline (
    input logic clk_i,
    input logic rst_n_i,

    /* Custom table control */
    input logic write_table_i,
    input logic [10:0] table_addr_i,
    input logic [15:0] pcm_i,
    output logic active_table_o,

    /* Control */
    input logic sine_wave_enable_i,
    input logic sine_wave_start_i,
    input logic square_wave_enable_i,
    input logic square_wave_start_i,
    input logic triangle_wave_enable_i,
    input logic triangle_wave_start_i,
    input logic custom_wave_enable_i,
    input logic custom_wave_start_i,

    /* Frequency control */
    input logic [31:0] sine_wave_frequency_i,
    input logic [31:0] square_wave_frequency_i,
    input logic [31:0] triangle_wave_frequency_i,
    input logic [31:0] custom_wave_frequency_i,

    /* Duty cycle control */
    input logic [31:0] square_wave_duty_cycle_i,

    /* Gains Q1.15 */
    input logic [15:0] sine_wave_gain_i,
    input logic [15:0] square_wave_gain_i,
    input logic [15:0] triangle_wave_gain_i,
    input logic [15:0] custom_wave_gain_i,

    /* Output Wave */
    output logic [15:0] mixed_wave_o
);

//====================================================================================
//      GENERATOR
//====================================================================================

    logic [15:0] triangle_wave_out;

    triangle_wave_generator triangle_generator (
        .clk_i           ( clk_i                     ),
        .rst_n_i         ( rst_n_i                   ),
        .enable_i        ( triangle_wave_enable_i    ),
        .start_i         ( triangle_wave_start_i     ),
        .inc_step_i      ( triangle_wave_frequency_i ),
        .triangle_wave_o ( triangle_wave_out         )
    );


    logic [15:0] square_wave_out;

    square_wave_generator square_generator (
        .clk_i         ( clk_i                    ),
        .rst_n_i       ( rst_n_i                  ),
        .enable_i      ( square_wave_enable_i     ),
        .start_i       ( square_wave_start_i      ),
        .inc_step_i    ( square_wave_frequency_i  ),
        .duty_cycle_i  ( square_wave_duty_cycle_i ),
        .square_wave_o ( square_wave_out          )
    );


    logic [15:0] sine_wave_out;

    sine_wave_generator sine_generator (
        .clk_i      ( clk_i                 ),
        .rst_n_i    ( rst_n_i               ),
        .enable_i   ( sine_wave_enable_i    ),
        .start_i    ( sine_wave_start_i     ),
        .inc_step_i ( sine_wave_frequency_i ),
        .sin_wave_o ( sine_wave_out         )
    );


    logic [15:0] custom_wave_out;

    custom_wave_generator custom_generator (
        .clk_i          ( clk_i                   ),
        .rst_n_i        ( rst_n_i                 ),
        .write_table_i  ( write_table_i           ),
        .table_addr_i   ( table_addr_i            ),
        .pcm_i          ( pcm_i                   ),
        .active_table_o ( active_table_o          ),
        .enable_i       ( custom_wave_enable_i    ),
        .start_i        ( custom_wave_start_i     ),
        .inc_step_i     ( custom_wave_frequency_i ),
        .wave_o         ( custom_wave_out         )
    );


//====================================================================================
//      MIXER
//====================================================================================

    audio_mixer audio_wave_mixer (
        .clk_i   ( clk_i   ),

        .sine_wave_enable_i     ( sine_wave_enable_i    ),
        .square_wave_enable_i   ( square_wave_enable_i  ),
        .triangle_wave_enable_i ( triangle_wave_enable_i),
        .custom_wave_enable_i   ( custom_wave_enable_i  ),

        .sine_wave_gain_i     ( sine_wave_gain_i      ),
        .square_wave_gain_i   ( square_wave_gain_i    ),
        .triangle_wave_gain_i ( triangle_wave_gain_i  ),
        .custom_wave_gain_i   ( custom_wave_gain_i    ),

        .sine_wave_i     ( sine_wave_out     ),
        .square_wave_i   ( square_wave_out   ),
        .triangle_wave_i ( triangle_wave_out ),
        .custom_wave_i   ( custom_wave_out   ),

        .mixed_wave_o ( mixed_wave_o )
    );

endmodule : audio_generation_pipeline

`endif 