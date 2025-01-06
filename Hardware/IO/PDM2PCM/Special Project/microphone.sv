`ifndef MICROPHONE_SV
    `define MICROPHONE_SV

`include "../pdm2pcm_processing_pipeline.sv"
`include "../pdm2pcm_interface.sv"

`include "ddr_interface.sv"
`include "playback_controller.sv"
`include "pcm2pwm_interface.sv"

module microphone (
    input logic clk_i,
    input logic rst_n_i,

    /* PDM Interface */
    input logic pdm_data_i,
    output logic pdm_clk_o,
    output logic pdm_lrsel_o,

    /* User Interface */
    input logic start_recording_i,
    input logic replay_audio_i,
    input logic stop_i,

    output logic [7:0] led_test_o,
    output logic is_recording_o,
    output logic is_replaying_o,
    output logic led_ground_o,

    /* PWM interface */
    output logic pwm_o,
    output logic audio_enable_o,

    /* DDR Interface */
    inout logic [15:0] ddr2_dq,
    inout logic [1:0] ddr2_dqs_n,
    inout logic [1:0] ddr2_dqs_p,
    output logic [1:0] ddr2_dm,
    output logic [12:0] ddr2_addr,
    output logic [2:0] ddr2_ba,
    output logic ddr2_ras_n,
    output logic ddr2_cas_n,
    output logic ddr2_we_n,
    output logic ddr2_cke,
    output logic ddr2_ck_p,
    output logic ddr2_ck_n,
    output logic ddr2_cs_n,
    output logic ddr2_odt
);

//====================================================================================
//      PARAMETERS
//====================================================================================

    /* 100 MHz */
    localparam SYSTEM_CLOCK = 100000000;

    /* PDM clock frequency */
    localparam PDM_CLOCK = 3000000;

    /* 48KHz Audio */
    localparam SAMPLING_RATE = 48000;

    /* To obtain the output frequency of the PDM clock */
    localparam CLOCK_DIVISOR = SYSTEM_CLOCK / (PDM_CLOCK  * 2);

    /* Decimate to the desired sampling rate */
    localparam DECIMATION_RATE = PDM_CLOCK / SAMPLING_RATE;

    /* Normalizer value for the pipeline */
    localparam NORMALIZER = DECIMATION_RATE ** (4);


//====================================================================================
//      CLOCKING AND RESET
//====================================================================================

    logic sys_clk, ddr_clk, locked, ddr_ready;

    clock_source clock_generator (
        /* External clock source */
        .ext_clk_i ( clk_i ),

        /* Generated clocks */
        .sys_clk_o ( sys_clk ),
        .mem_clk_o ( ddr_clk ),

        /* PLL is locked */
        .locked_o ( locked )
    );


    /* System reset syncronizer */
    logic reset_n, rst_sync;

        always_ff @(posedge sys_clk or negedge rst_n_i) begin 
            if (!rst_n_i) begin
                rst_sync <= 1'b0;
                reset_n <= 1'b0;
            end else if (!locked) begin 
                rst_sync <= 1'b0;
                reset_n <= 1'b0;
            end else if (locked & ddr_ready) begin 
                rst_sync <= rst_n_i;
                reset_n <= rst_sync;
            end 
        end 


    /* DDR reset syncronizer */
    logic ddr_reset_n, ddr_rst_sync;

        always_ff @(posedge ddr_clk or negedge rst_n_i) begin 
            if (!rst_n_i) begin
                ddr_rst_sync <= 1'b0;
                ddr_reset_n <= 1'b0;
            end else if (!locked) begin 
                ddr_rst_sync <= 1'b0;
                ddr_reset_n <= 1'b0;
            end else if (locked) begin 
                ddr_rst_sync <= rst_n_i;
                ddr_reset_n <= ddr_rst_sync;
            end 
        end 


//====================================================================================
//      INPUT PROCESSING
//====================================================================================

    logic start_recording, start_playback, trace_end;

        always_ff @(posedge sys_clk) begin
            if (!reset_n | stop_i | trace_end) begin
                start_recording <= 1'b0;
                start_playback <= 1'b0;
            end else begin
                case ({start_recording_i, replay_audio_i})
                    2'b01: begin
                        if (!start_recording) begin
                            start_recording <= 1'b0;
                            start_playback <= 1'b1;
                        end
                    end

                    2'b10: begin
                        if (!start_playback) begin
                            start_recording <= 1'b1;
                            start_playback <= 1'b0;
                        end
                    end

                    2'b11: begin
                        start_recording <= 1'b1;
                        start_playback <= 1'b0;
                    end
                endcase
            end
        end

    assign is_recording_o = start_recording;
    assign is_replaying_o = start_playback;

    assign led_ground_o = '0;


//====================================================================================
//      EXTERNAL INTERFACE
//====================================================================================

    logic pdm_sampled, pdm_valid;

    pdm2pcm_interface external_interface (
        /* Global signals */
        .clk_i        ( sys_clk          ),
        .rst_n_i      ( reset_n          ),
        .clk_en_i     ( start_recording  ),

        /* Clock frequency division */
        .clock_divisor_i ( CLOCK_DIVISOR ),

        /* Channel setup */
        .dual_channel_i ( 1'b0 ),
        .channel_i      ( 1'b0 ),

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
//      AUDIO INPUT PIPELINE
//====================================================================================
    
    logic sample_valid; logic [7:0] sample; 

    pdm2pcm_processing_pipeline #(
        .CIC_FILTER_ORDER ( 5 ),
        .CIC_COMB_DELAY   ( 1 )
    ) pdm2pcm_pipeline_inst (
        /* Global signals */
        .clk_i    ( sys_clk          ),
        .rst_n_i  ( reset_n          ),
        .clk_en_i ( start_recording  ),

        /* Input for CIC filter */
        .pdm_i     ( pdm_sampled ),
        .valid_i   ( pdm_valid   ),
        .channel_i ( pdm_channel ),

        .decimator_factor_i ( DECIMATION_RATE ),
        .normalizer_i       ( NORMALIZER      ),
        .gain_i             ( 16'h8000        ),

        /* PCM output */
        .pcm_o     ( sample         ),
        .valid_o   ( sample_valid   ),
        .channel_o (                ),
        .invalid_o (                )
    );

    // OUTPUT 16 BIT, THEN NOISE SHAPE AND FILTER SCALE 8 BIT

    /* Pack 16 bit PCM samples inside 64 bit word */
    logic [7:0][7:0] wr_sample_packet;

        always_ff @(posedge sys_clk) begin
            if (sample_valid) begin
                wr_sample_packet <= {sample, wr_sample_packet[7:1]};
            end
        end


    /* MOD 8 counter */
    logic [2:0] counter; logic write_sample_packet;

        always_ff @(posedge sys_clk) begin
            if (!reset_n) begin 
                counter <= '0;
            end else if (sample_valid) begin
                counter <= counter + 1;
            end
        end

        always_ff @(posedge sys_clk) begin
            if (!reset_n) begin 
                write_sample_packet <= 1'b0;
            end else begin
                write_sample_packet <= (counter == 'd7) & sample_valid;
            end
        end


//====================================================================================
//      AUDIO PLAYBACK CONTROLLER
//====================================================================================
    
    /* Packed PCM samples */
    logic [7:0][7:0] rd_sample_packet;

    /* DDR commands */
    logic ddr_read_done, read_sample_packet, pull_packet; 

    /* PWM Interface */
    logic next_out_sample, valid_out_sample; logic [7:0] out_sample;

    playback_controller playback (
        .clk_i   ( sys_clk     ),
        .rst_n_i ( reset_n     ),

        /* User interface */
        .start_playback_i ( start_playback & !trace_end ),

        /* DDR interface */
        .read_packet_o    ( read_sample_packet ),
        .extract_packet_o ( pull_packet        ),
        .packet_valid_i   ( ddr_read_done      ),
        .sample_packet_i  ( rd_sample_packet   ),

        /* PWM interface */
        .next_sample_i  ( next_out_sample  ),
        .sample_o       ( out_sample       ),
        .valid_sample_o ( valid_out_sample )
    );


//====================================================================================
//      DDR MEMORY INTERFACE
//====================================================================================

    logic [26:0] write_address, write_address_ff, read_address, read_address_ff;

        always_ff @(posedge sys_clk) begin
            if (!reset_n | (trace_end & stop_i)) begin
                write_address <= '0;
                write_address_ff <= '0;
            end else begin
                if (write_sample_packet) begin
                    write_address <= write_address + 8;
                end

                write_address_ff <= write_address;
            end
        end

        always_ff @(posedge sys_clk) begin
            if (!reset_n | stop_i | trace_end) begin
                read_address <= '0;
                read_address_ff <= '0;
            end else begin
                if (read_sample_packet) begin
                    read_address <= read_address + 16;
                end

                read_address_ff <= read_address;
            end
        end

    assign trace_end = (read_address >= write_address) & ({read_address, write_address} != '0);


    ddr_interface ddr_controller (
        .clk_i       ( sys_clk     ),
        .rst_n_i     ( reset_n     ),
        .mem_clk_i   ( ddr_clk     ),
        .mem_rst_n_i ( ddr_reset_n ),

        /* Data */
        .ddr2_dq_io ( ddr2_dq ),
        .ddr2_dm_o  ( ddr2_dm ),

        /* Data strobe pair */
        .ddr2_dqs_n_io ( ddr2_dqs_n ),
        .ddr2_dqs_p_io ( ddr2_dqs_p ),

        /* Addreses */
        .ddr2_addr_o ( ddr2_addr ),
        .ddr2_ba_o   ( ddr2_ba   ),

        /* Enable signals */
        .ddr2_ras_n_o ( ddr2_ras_n ),
        .ddr2_cas_n_o ( ddr2_cas_n ),
        .ddr2_we_n_o  ( ddr2_we_n  ),
        .ddr2_cke_o   ( ddr2_cke   ),

        /* Clock pair */
        .ddr2_ck_p_o ( ddr2_ck_p ),
        .ddr2_ck_n_o ( ddr2_ck_n ),

        /* Chip select */
        .ddr2_cs_n_o ( ddr2_cs_n ),
        .ddr2_odt_o  ( ddr2_odt  ),

        /* Addresses */
        .write_address_i ( write_address_ff ), 
        .read_address_i  ( read_address_ff  ), 
        
        /* Command interface */
        .write_i     ( write_sample_packet ), 
        .read_i      ( read_sample_packet  ), // ADJUST
        .pull_data_i ( pull_packet         ), // ADJUST 

        /* Data interface */
        .write_data_i ( wr_sample_packet  ),
        .read_data_o  ( rd_sample_packet  ),
        
        /* Read status */
        .read_valid_o ( ddr_read_done  ),

        /* Status */
        .ready_o ( ddr_ready )
    );


//====================================================================================
//      PWM INTERFACE
//====================================================================================

    logic [11:0] pwm_request_counter;

        always_ff @(posedge sys_clk) begin
            if (!reset_n | !start_playback) begin
                pwm_request_counter <= '0;
            end else begin
                if (pwm_request_counter == 'd2083) begin
                    pwm_request_counter <= '0;
                end else begin
                    pwm_request_counter <= pwm_request_counter + 1;
                end
            end
        end

    assign next_out_sample = pwm_request_counter == 'd2083;



    logic [7:0] sample2convert;

        always_ff @(posedge sys_clk) begin
            if (!reset_n | !start_playback) begin
                sample2convert <= '0;
            end else if (valid_out_sample) begin
                sample2convert <= out_sample;
            end
        end


    logic pwm;

    pcm2pwm_interface audio_interface (
        .clk_i    ( sys_clk        ),
        .rst_n_i  ( reset_n        ),
        .enable_i ( start_playback ),

        .pcm_i ( sample2convert[7:0] ),
        .pwm_o ( pwm                 )
    );

    assign pwm_o = pwm ? 1'bZ : 1'b0;

    assign audio_enable_o = start_playback;


//====================================================================================
//      TEST INTERFACE
//====================================================================================

    logic full, empty; logic [7:0] test_sample;
    
    synchronous_buffer #(2**15, 8) right_sample_buffer (
        .clk_i   ( sys_clk ),
        .rst_n_i ( reset_n ),

        .write_i ( sample_valid & !full    ),
        .read_i  ( start_playback & !empty ),

        .empty_o ( empty ),
        .full_o  ( full  ),

        .write_data_i ( sample[7:0] ),
        .read_data_o  ( test_sample )
    );
    
    always_ff @(posedge sys_clk) begin
        led_test_o <= test_sample;
    end

endmodule : microphone

`endif