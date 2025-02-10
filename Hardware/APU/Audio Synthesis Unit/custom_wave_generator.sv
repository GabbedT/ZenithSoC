`ifndef CUSTOM_WAVE_GENERATOR_SV
    `define CUSTOM_WAVE_GENERATOR_SV

module custom_wave_generator (
    input logic clk_i,
    input logic rst_n_i,

    /* Table control */
    input logic write_table_i,
    input logic [9:0] table_addr_i,
    input logic [15:0] pcm_i,

    /* Generator control */
    input logic enable_i,
    input logic start_i,

    /* Clock divisor to reach custom sampling frequencies */
    input logic [31:0] inc_step_i,

    /* PCM output */
    output logic [15:0] wave_o
);

//====================================================================================
//      PHASE REGISTER
//====================================================================================

    logic [32:0] phase_register;

        always_ff @(posedge clk_i) begin
            if (!rst_n_i | start_i) begin
                phase_register <= '0;
            end else if (enable_i) begin
                phase_register <= phase_register + inc_step_i;
            end
        end


//====================================================================================
//      SINE TABLE
//====================================================================================

    /* Custom audio PCM samples */
    logic [15:0] audio_table [1023:0]; logic [9:0] table_index;

    assign table_index = phase_register[31:22];


        always_ff @(posedge clk_i) begin
            if (write_table_i) begin
                audio_table[table_addr_i] <= pcm_i;
            end

            if (enable_i) begin
                wave_o <= audio_table[table_index];
            end else begin
                wave_o <= '0;
            end
        end

endmodule : custom_wave_generator

`endif