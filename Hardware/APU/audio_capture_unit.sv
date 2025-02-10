`ifndef AUDIO_CAPTURE_UNIT_SV
    `define AUDIO_CAPTURE_UNIT_SV

`include "Interface/ac_unit_registers.sv"
`include "Audio Capture Unit/recorder_audio_pipeline.sv"
`include "Interface/pdm2pcm_converter.sv"

module audio_capture_unit #(
    /* Buffer sizes */
    parameter BUFFER_SIZE = 512
) (
    /* Global signals */
    input logic clk_i,
    input logic rst_n_i,
    output logic interrupt_o,

    /* External interface */
    input logic pdm_data_i,
    output logic pdm_clk_o,
    output logic pdm_lrsel_o,

    /* Write interface */
    input logic write_i,
    input logic [2:0] write_address_i,
    input logic [3:0][7:0] write_data_i,
    input logic [3:0] write_strobe_i,
    output logic write_done_o,
    output logic write_error_o,

    /* Read interface */
    input logic read_i,
    input logic [2:0] read_address_i,
    output logic [31:0] read_data_o,
    output logic read_done_o,
    output logic read_error_o
);

//====================================================================================
//      REGISTER INTERFACE
//====================================================================================

    /* Sample interface */
    logic sample_valid, sample_channel, sample_invalid; logic [15:0] sample;

    /* Configuration interface */
    logic [6:0] clock_divisor; logic [6:0] decimation_rate; logic [15:0] sample_gain; logic [31:0] normalizer; logic enable_interface;

    /* Channel */
    logic dual_channel, channel_selection;

    ac_unit_registers #(
        .BUFFER_SIZE  ( BUFFER_SIZE  )
    ) register_interface (
        .clk_i         ( clk_i        ),
        .rst_n_i       ( rst_n_i      ),
        .interrupt_o   ( interrupt_o  ),

        /* Buffer interface */
        .pcm_sample_i  ( sample         ),
        .valid_i       ( sample_valid   ),
        .channel_i     ( sample_channel ),
        .invalid_i     ( sample_invalid ),

        /* Configuration */
        .divisor_o          ( clock_divisor    ),
        .decimation_rate_o  ( decimation_rate  ),
        .gain_o             ( sample_gain      ),
        .normalizer_o       ( normalizer       ),
        .enable_interface_o ( enable_interface ),

        /* Channel configuration */
        .dual_channel_o ( dual_channel      ),
        .channel_o      ( channel_selection ),

        /* Write interface */
        .write_i         ( write_i         ),
        .write_address_i ( write_address_i ),
        .write_data_i    ( write_data_i    ),
        .write_strobe_i  ( write_strobe_i  ),
        .write_error_o   ( write_error_o   ),

        /* Read interface */
        .read_i         ( read_i         ),
        .read_address_i ( read_address_i ),
        .read_data_o    ( read_data_o    ),
        .read_error_o   ( read_error_o   )
    );


    assign write_done_o = write_i;


    logic is_delayed;

    assign is_delayed = (capture_unit_registers_t'(read_address_i) == CAPTURE_UNIT_SAMPLE_BUFFER);


    logic read_done_delay; 

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                read_done_delay <= 1'b0; 
            end else begin 
                read_done_delay <= is_delayed & read_i;
            end 
        end 

    assign read_done_o = is_delayed ? read_done_delay : read_i;


//====================================================================================
//      EXTERNAL INTERFACE
//====================================================================================

    logic pdm_sampled, pdm_valid, pdm_direction;

    pdm2pcm_converter pdm_converter (
        /* Global signals */
        .clk_i        ( clk_i            ),
        .rst_n_i      ( rst_n_i          ),
        .clk_en_i     ( enable_interface ),

        /* Clock frequency division */
        .clock_divisor_i ( clock_divisor ),

        /* Channel setup */
        .dual_channel_i ( dual_channel      ),
        .channel_i      ( channel_selection ),

        /* Sampled PDM output */
        .pdm_data_o ( pdm_sampled ),
        .valid_o    ( pdm_valid   ),
        .channel_o  ( pdm_channel ),

        /* External interface */
        .pdm_data_i   ( pdm_data_i   ),
        .pdm_clk_o    ( pdm_clk_o    ),
        .pdm_lrsel_o  ( pdm_lrsel_o  )
    );


//====================================================================================
//      PROCESSING PIPELINE
//====================================================================================

    recorder_audio_pipeline #(
        .CIC_FILTER_ORDER ( 5 ),
        .CIC_COMB_DELAY   ( 1 )
    ) audio_pipeline (
        /* Global signals */
        .clk_i    ( clk_i            ),
        .rst_n_i  ( rst_n_i          ),
        .clk_en_i ( enable_interface ),

        /* Input for CIC filter */
        .pdm_i     ( pdm_sampled ),
        .valid_i   ( pdm_valid   ),
        .channel_i ( pdm_channel ),

        .decimator_factor_i ( decimation_rate ),
        .normalizer_i       ( normalizer      ),
        .gain_i             ( sample_gain     ),

        /* PCM output */
        .pcm_o     ( sample         ),
        .valid_o   ( sample_valid   ),
        .channel_o ( sample_channel ),
        .invalid_o ( sample_invalid )
    );

endmodule : audio_capture_unit 

`endif 