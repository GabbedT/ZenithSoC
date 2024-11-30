`ifndef POST_PROCESSOR_SV
    `define POST_PROCESSOR_SV

module post_processor #(
    parameter WIDTH = 16
) (
    input logic clk_i,
    input logic rst_n_i,

    /* Code from the CIC filter */
    input logic [WIDTH - 1:0] code_i,
    input logic               valid_i,

    /* Decimation factor used to normalize the PCM signal */
    input logic [WIDTH - 1:0] normalizer_i,

    /* Signed or unsigned PCM */
    input logic signed_i,

    /* Pulse Code Modulation, either signed or unsigned */
    output logic [WIDTH - 1:0] pcm_o,
    output logic               valid_o,

    /* Invalid normalizer */
    output logic invalid_o
);

//==========================================================
//      NORMALIZATION STAGE
//==========================================================

    logic [WIDTH - 1:0] normalized_sample; logic normalized_valid, normalizer_idle;

    /* Perform fixed-point division to normalize the input sample into [1:0] range
     * the result will be a Q_0.16 fixed point number */
    non_restoring_divider #( 
        .DATA_WIDTH ( WIDTH * 2 )
    ) normalizer (
        .clk_i        ( clk_i   ),
        .rst_n_i      ( rst_n_i ),
        .clk_en_i     ( 1'b1    ),

        .dividend_i   ( {code_i, 16'b0      }     ),
        .divisor_i    ( {16'b0, normalizer_i}     ),
        .data_valid_i ( valid_i & normalizer_idle ),

        .quotient_o       ( normalized_sample ),
        .remainder_o      (                   ),
        .divide_by_zero_o ( invalid_o         ),
        .data_valid_o     ( normalized_valid  ),
        .idle_o           ( normalizer_idle   )
    );


//==========================================================
//      PCM CONVERSION STAGE
//==========================================================

    logic [31:0] full_sample;

    assign valid_o = normalized_valid;

        /* Map the normalized sample, represented in the [0:1] range,
         * to the appropriate range for 16-bit PCM output */
        always_comb begin
            if (signed_i) begin
                /* Convert the normalized sample to the [0:65535] range,
                 * then shift and center it to the [-32768:32767] range */
                full_sample = (normalized_sample << 16) - 16'h8000;
            end else begin
                /* Convert the normalized sample to the [0:65535] range without centering */
                full_sample = normalized_sample << 16;
            end
        end

    assign pcm_o = full_sample[31:16];

endmodule : post_processor

`endif