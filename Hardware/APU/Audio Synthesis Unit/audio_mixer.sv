`ifndef AUDIO_MIXER_SV
    `define AUDIO_MIXER_SV

module audio_mixer (
    input logic clk_i,

    /* Control */
    input logic sine_wave_enable_i,
    input logic square_wave_enable_i,
    input logic triangle_wave_enable_i,
    input logic custom_wave_enable_i,

    /* Gains Q1.15 */
    input logic [15:0] sine_wave_gain_i,
    input logic [15:0] square_wave_gain_i,
    input logic [15:0] triangle_wave_gain_i,
    input logic [15:0] custom_wave_gain_i,

    /* Audio Waves */
    input logic [15:0] sine_wave_i,
    input logic [15:0] square_wave_i,
    input logic [15:0] triangle_wave_i,
    input logic [15:0] custom_wave_i,

    /* Output Wave */
    output logic [15:0] mixed_wave_o
);

//====================================================================================
//      GAIN STAGE
//====================================================================================

    /* Audio Waves */
    logic [32:0] sine_wave, square_wave, triangle_wave, custom_wave;

    /* Fixed point multiplication */
    assign sine_wave = (sine_wave_enable_i ? sine_wave_i : '0) * sine_wave_gain_i;
    assign square_wave = (square_wave_enable_i ? square_wave_i : '0) * square_wave_gain_i;
    assign triangle_wave = (triangle_wave_enable_i ? triangle_wave_i : '0) * triangle_wave_gain_i;
    assign custom_wave = (custom_wave_enable_i ? custom_wave_i : '0) * custom_wave_gain_i;


//====================================================================================
//      MIXING STAGE
//====================================================================================

    /* First mixing stage */
    logic [16:0] intermediate_1, intermediate_2;

    assign intermediate_1 = sine_wave[30:15] + square_wave[30:15];
    assign intermediate_2 = triangle_wave[30:15] + custom_wave[30:15];

    /* Final mixing stage */
    logic [17:0] mixed_audio_wave, mixed_audio_wave_ff;

    assign mixed_audio_wave = intermediate_1 + intermediate_2;

        always_ff @(posedge clk_i) begin
            mixed_audio_wave_ff <= mixed_audio_wave;
        end


//====================================================================================
//      NORMALIZATION STAGE
//====================================================================================
    
    logic [31:0] temp_mult; assign temp_mult = mixed_audio_wave_ff * 16'hAAAB;
    /* Divide by 4 signals to be in range for PCM16 */
    always_ff @(posedge clk_i) begin
        case ({sine_wave_enable_i, square_wave_enable_i, triangle_wave_enable_i, custom_wave_enable_i})
            4'b0000: begin
                mixed_wave_o <= '0;
            end

            4'b0001, 4'b0010, 4'b0100, 4'b1000: begin
                /* Divide by 1 */
                mixed_wave_o <= mixed_audio_wave_ff;
            end

            4'b0011, 4'b0101, 4'b1001, 4'b1010, 4'b1100, 4'b0110: begin
                /* Divide by 2 */
                mixed_wave_o <= mixed_audio_wave_ff >> 1;
            end

            4'b0111, 4'b1110, 4'b1101, 4'b1011: begin
                /* Divide by 3 */
                mixed_wave_o <= temp_mult >> 17;
            end

            4'b1111: begin
                /* Divide by 4 */
                mixed_wave_o <= mixed_audio_wave_ff >> 2;
            end
        endcase
    end

endmodule : audio_mixer

`endif 