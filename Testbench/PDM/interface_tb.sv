`timescale 1ns / 1ps

module pdm2pcm_interface_tb;

    /* Inputs */
    logic clk_i = 0;
    logic rst_n_i = 0;
    logic clk_en_i = 1;
    logic [6:0] clock_divisor_i = 0;
    logic dual_channel_i = 0;
    logic channel_i = 0;
    logic pdm_data_i = 0;

    /* Outputs */
    logic pdm_data_o;
    logic valid_o;
    logic pdm_clk_o;
    logic pdm_lrsel_o;

    /* Instantiate the DUT */
    pdm2pcm_interface dut (
        .clk_i           (clk_i),
        .rst_n_i         (rst_n_i),
        .clk_en_i        (clk_en_i),
        .clock_divisor_i (clock_divisor_i),
        .dual_channel_i  (dual_channel_i),
        .channel_i       (channel_i),
        .pdm_data_i      (pdm_data_i),
        .pdm_data_o      (pdm_data_o),
        .valid_o         (valid_o),
        .pdm_clk_o       (pdm_clk_o),
        .pdm_lrsel_o     (pdm_lrsel_o)
    );

    /* Clock generation */
    always #5 clk_i = !clk_i;

    /* Test sequence */
    initial begin
        /* Initialize inputs */
        rst_n_i = 0;
        clk_en_i = 1;
        clock_divisor_i = 7'd20; /* Divide clock by 4 */
        dual_channel_i = 0;
        channel_i = 0; /* LEFT */
        pdm_data_i = 0;

        /* Reset the design */
        #20 rst_n_i = 1;

        /* Test single-channel (LEFT) operation */
        @(posedge clk_i);
        channel_i = 0;
        repeat (1000) begin
            @(posedge pdm_clk_o);
            pdm_data_i = $random;
        end

        /* Test single-channel (RIGHT) operation */
        @(posedge clk_i);
        channel_i = 1;
        repeat (1000) begin
            @(posedge pdm_clk_o);
            pdm_data_i = $random;
        end

        /* Test dual-channel operation */
        @(posedge clk_i);
        dual_channel_i = 1;
        clk_en_i = 1'b0;
        @(posedge clk_i);
        clk_en_i = 1'b1;

        repeat (1000) begin
            @(posedge pdm_clk_o);
            pdm_data_i = $random;
        end

        /* Test clock division and glitches removal */
        @(posedge clk_i);
        clock_divisor_i = 7'd8; /* Change clock divisor */
        repeat (32) @(posedge clk_i);

        clk_en_i <= 1'b0;
        repeat (32) @(posedge clk_i);

        /* Finish simulation */
        $stop;
    end

endmodule
