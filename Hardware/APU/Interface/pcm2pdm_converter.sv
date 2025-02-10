`ifndef PCM2PDM_CONVERTER_SV
    `define PCM2PDM_CONVERTER_SV

module pcm2pdm_converter (
    input logic clk_i,
    input logic rst_n_i,
    input logic enable_i,

    /* Scaled PCM value rapresent duty cycle */
    input logic [15:0] pcm_i,

    /* PDM pin */
    output logic pdm_o
);

    localparam BIAS = 2**(15);

    /* Scale to signed PCM */
    logic [15:0] sample_scaled; 

    assign sample_scaled = $signed(pcm_i) - 16'sd32768;


    logic [16:0] sample_biased, accumulator;

    assign sample_biased = {sample_scaled[15], sample_scaled} + BIAS;

        always_ff @(posedge clk_i) begin
            if (!rst_n_i) begin 
                accumulator <= '0;
            end else if (enable_i) begin
                accumulator <= {1'b0, accumulator[15:0]} + sample_biased;
            end
        end

    assign pdm_o = accumulator[16];

endmodule : pcm2pdm_converter

`endif 