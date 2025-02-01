`ifndef RECORDER_AUDIO_PIPELINE_SV
    `define RECORDER_AUDIO_PIPELINE_SV

`include "cic_filter.sv"

`include "../../Utility/Packages/pdm2pcm_pkg.sv"
`include "../../CPU/ApogeoRV/Hardware/Back End/Execution Unit/Arithmetic Circuits/Integer/Dividers/non_restoring_divider.sv"

`define _VIVADO_

module recorder_audio_pipeline #(
    /* Int - Comb stages */
    parameter CIC_FILTER_ORDER = 2,

    /* M delay */
    parameter CIC_COMB_DELAY = 1
) (
    input logic clk_i,
    input logic rst_n_i,
    input logic clk_en_i,

    /* Code from the CIC filter */
    input logic pdm_i,
    input logic valid_i,
    input logic channel_i,

    /* Decimation factor to bring down sample rate */
    input logic [6:0] decimator_factor_i,

    /* Used to bring the PCM signal into the [1:0] range */
    input logic [31:0] normalizer_i,

    /* Final gain */
    input logic [15:0] gain_i,

    /* Pulse Code Modulation, either signed or unsigned */
    output logic [15:0] pcm_o,
    output logic valid_o,
    output logic channel_o,

    /* Invalid normalizer */
    output logic invalid_o
);

//==========================================================
//      FILTERING STAGE
//==========================================================

    logic [31:0] filtered_sample; logic valid_filter;

    cic_filter #(32, CIC_FILTER_ORDER, CIC_COMB_DELAY) low_pass_filter (
        .clk_i   ( clk_i   ),
        .rst_n_i ( rst_n_i ),
        
        .reset_filter_i     ( !clk_en_i          ),
        .decimator_factor_i ( decimator_factor_i ),

        /* PDM input */
        .pdm_i   ( pdm_i   ),
        .valid_i ( valid_i ),

        /* PCM output */
        .pcm_o   ( filtered_sample ),
        .valid_o ( valid_filter    )
    );

    /* Direction pipeline filter stage */
    logic channel_filter_stg;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                channel_filter_stg <= 1'b0;
            end else if (valid_i) begin 
                channel_filter_stg <= channel_i;
            end
        end


//==========================================================
//      NORMALIZATION STAGE
//==========================================================

    logic [32:0] normalized_sample; logic normalized_valid, normalizer_idle;

    /* Perform fixed-point division to normalize the input sample into [1:0] range
     * the result will be a Q_0.32 fixed point number */
    non_restoring_divider #( 
        .DATA_WIDTH ( 64 )
    ) normalizer (
        .clk_i        ( clk_i    ),
        .rst_n_i      ( rst_n_i  ),
        .clk_en_i     ( clk_en_i ),

        .dividend_i   ( {filtered_sample, 32'b0}       ),
        .divisor_i    ( {32'b0, normalizer_i}          ), /* DECIMATION FACTOR ^ (FILTER_ORDER - 1) */
        .data_valid_i ( valid_filter & normalizer_idle ),

        .quotient_o       ( normalized_sample ),
        .remainder_o      (                   ),
        .divide_by_zero_o ( invalid_o         ),
        .data_valid_o     ( normalized_valid  ),
        .idle_o           ( normalizer_idle   )
    );

    /* Direction pipeline normalization stage */
    logic channel_normalization_stg;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                channel_normalization_stg <= 1'b0;
            end else if (valid_filter) begin 
                channel_normalization_stg <= channel_filter_stg;
            end
        end


//==========================================================
//      PCM CONVERSION STAGE
//==========================================================

    logic [63:0] full_sample; logic [15:0] pcm_sample;

        /* Map the normalized sample, represented in the [0:1] range,
         * to the appropriate range for 32-bit PCM output */
        always_comb begin
            /* Convert the normalized sample to the [0:2^32 - 1] range without centering */
            if (normalized_sample[32]) begin 
                /* Max value saturate */
                full_sample = 64'hFFFFFFFF_00000000;
            end else begin
                full_sample = normalized_sample << 32;
            end
        end

    /* Take the upper 32 bits to convert into 16 bit PCM (losing precision ) */
    assign pcm_sample = full_sample[63:48];


//==========================================================
//      SAMPLE GAIN STAGE
//==========================================================
    
    /* Fixed point product */
    logic [31:0] fxp_product;

    `ifdef _VIVADO_

        /* Fixed point product: Q16.0 * Q1.15 = Q17.15 */
        gain_multiplier gain_stage (
            .CLK ( clk_i       ),
            .A   ( pcm_sample  ),
            .B   ( gain_i      ),
            .CE  ( clk_en_i    ),
            .P   ( fxp_product )
        );

        assign pcm_o = fxp_product[30:15];

        /* To match multiplier latency */
        logic [1:0] valid_pipe;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                valid_pipe <= '0;
            end else begin 
                valid_pipe <= {valid_pipe[0], normalized_valid};
            end 
        end 

        assign valid_o = valid_pipe[1];


        /* Direction pipeline gain stage */
        logic [1:0] channel_gain_stg;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                channel_gain_stg <= 1'b0;
            end else if (normalized_valid) begin 
                channel_gain_stg <= {channel_gain_stg[0], channel_normalization_stg};
            end
        end

        assign channel_o = channel_gain_stg[1];

    `else 

        // ADD MULTIPLIER DEFINITION

    `endif 

endmodule : recorder_audio_pipeline

`endif