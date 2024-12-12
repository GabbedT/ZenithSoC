`ifndef PDM2PCM_INTERFACE
    `define PDM2PCM_INTERFACE

`include "../../Utility/Packages/pdm2pcm_pkg.sv"

module pdm2pcm_interface (
    /* Global signals */
    input logic clk_i,
    input logic rst_n_i,
    input logic clk_en_i,

    /* To divide the clock frequency */
    input logic [6:0] clock_divisor_i,

    /* Channel setup */
    input logic dual_channel_i,
    input logic channel_i,

    /* Sampled PDM bit */
    output logic pdm_data_o,
    output logic valid_o,
    output logic channel_o,

    /* External interface */
    input logic pdm_data_i,
    output logic pdm_clk_o,
    output logic pdm_lrsel_o
);

//==========================================================
//      CLOCK DIVISOR
//==========================================================

    logic [6:0] counter;

        /* Divide the clock to obtain the audio clock frequency */
        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin
                counter <= '0;
            end else if (clk_en_i) begin 
                if (counter == clock_divisor_i) begin
                    counter <= '0;
                end else begin
                    counter <= counter + 1'b1;
                end
            end else begin
                counter <= '0;
            end
        end

        /* Sample the clock to delete glitches */
        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin
                pdm_clk_o <= 1'b0;
            end else if (clk_en_i) begin 
                if (counter == clock_divisor_i) begin
                    pdm_clk_o <= !pdm_clk_o;
                end
            end else begin
                pdm_clk_o <= 1'b0;
            end
        end


//==========================================================
//      INPUT SYNCHRONIZER
//==========================================================

    synchronizer #(3, 0) input_sync (
        .clk_i   ( clk_i   ),
        .rst_n_i ( rst_n_i ),

        .signal_i ( pdm_data_i ),
        .sync_o   ( pdm_data_o )
    );


    logic pdm_clk_posedge, pdm_clk_negedge;

    /* Positive edge detector */
    edge_detector #(1, 0) pedge_detector (
        .clk_i   ( clk_i   ),
        .rst_n_i ( rst_n_i ),

        .signal_i ( pdm_clk_o       ),
        .edge_o   ( pdm_clk_posedge )
    );

    /* Negative edge detector */
    edge_detector #(0, 0) nedge_detector (
        .clk_i   ( clk_i   ),
        .rst_n_i ( rst_n_i ),

        .signal_i ( pdm_clk_o       ),
        .edge_o   ( pdm_clk_negedge )
    );

    logic valid_data;

        always_comb begin
            /* Default value */
            valid_data = 1'b0;
            
            if (dual_channel_i) begin
                valid_data = pdm_clk_posedge | pdm_clk_negedge; 
            end else begin
                if (channel_i == LEFT) begin
                    valid_data = pdm_clk_posedge;
                end else begin
                    valid_data = pdm_clk_negedge;
                end
            end
        end

    /* To match synchronizer latency */
    synchronizer #(3, 0) valid_latency_matcher (
        .clk_i   ( clk_i   ),
        .rst_n_i ( rst_n_i ),

        .signal_i ( valid_data ),
        .sync_o   ( valid_o    )
    );


//==========================================================
//      LEFT / RIGHT CHANNEL SELECTOR
//==========================================================

    logic [6:0] lr_counter;

        /* Divide the clock to obtain the audio clock frequency */
        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin
                lr_counter <= '0;
            end else if (clk_en_i) begin 
                if (lr_counter == clock_divisor_i) begin
                    lr_counter <= '0;
                end else begin
                    lr_counter <= lr_counter + 1'b1;
                end
            end else begin
                lr_counter <= clock_divisor_i >> 1;
            end
        end

        /* Sample the clock to delete glitches */
        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin
                pdm_lrsel_o <= LEFT;
            end else if (clk_en_i) begin
                if (dual_channel_i) begin 
                    if (lr_counter == clock_divisor_i) begin
                        pdm_lrsel_o <= !pdm_lrsel_o;
                    end
                end else begin
                    pdm_lrsel_o <= channel_i;
                end
            end else begin
                pdm_lrsel_o <= LEFT;
            end
        end

    assign channel_o = pdm_lrsel_o;
    
endmodule : pdm2pcm_interface

`endif