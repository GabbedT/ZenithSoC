`timescale 1ns/1ps

module cic_filter_tb;

    localparam NUMBER_OF_DUT = 5;
    localparam DECIMATION_FACTOR = 64;

    parameter WIDTH = 16;
    parameter FILTER_ORDER = 2;
    parameter COMB_DELAY = 1;

    logic clk_i = 0;
    logic rst_n_i = 0;

    logic reset_filter_i = 1;
    logic [31:0] decimator_factor_i = 0;
    logic pdm_i = 0;
    logic valid_i = 0;

    logic [1:0][NUMBER_OF_DUT - 1:0][15:0] filtered_pdm;
    logic [1:0][NUMBER_OF_DUT - 1:0]       valid;

    logic [15:0] pcm_o;
    logic valid_o, invalid_o;

    generate 
        for (genvar i = 0; i < NUMBER_OF_DUT; ++i) begin
            cic_filter #(
                .WIDTH        ( WIDTH          ),
                .FILTER_ORDER ( FILTER_ORDER   ),
                .COMB_DELAY   ( COMB_DELAY + i )
            ) uut_delay (
                .clk_i              ( clk_i              ),
                .rst_n_i            ( rst_n_i            ),
                .reset_filter_i     ( reset_filter_i     ),
                .decimator_factor_i ( decimator_factor_i ),
                .pdm_i              ( pdm_i              ),
                .valid_i            ( valid_i            ),
                .pcm_o              ( filtered_pdm[0][i] ),
                .valid_o            ( valid[0][i]        )
            );
        end
    endgenerate


    post_processor #(
        .WIDTH ( WIDTH )
    ) post_processing (
        .clk_i   ( clk_i   ),
        .rst_n_i ( rst_n_i ),

        /* Code from the CIC filter */
        .code_i  ( filtered_pdm[0][0] ),
        .valid_i ( valid[0][0]        ),

        /* Decimation factor used to normalize the PCM signal */
        .normalizer_i ( decimator_factor_i ),

        /* Signed or unsigned PCM */
        .signed_i ( 1'b0 ),

        /* Pulse Code Modulation, either signed or unsigned */
        .pcm_o   ( pcm_o   ),
        .valid_o ( valid_o ),

        /* Invalid normalizer */
        .invalid_o ( invalid_o )
    );


    /* Clock generation: 100 MHz */
    always #5 clk_i <= !clk_i;

    /* Frequency divider for valid_i to simulate 1 MHz */
    logic [6:0] clk_divider;

    always_ff @(posedge clk_i or negedge rst_n_i) begin
        if (!rst_n_i) begin
            clk_divider <= 0;
            valid_i <= 0;
        end else begin
            if (clk_divider == 31) begin
                clk_divider <= 0;

                valid_i <= 1;
            end else begin
                clk_divider <= clk_divider + 1;

                valid_i <= 0;
            end
        end
    end

    integer pdm_file;
    integer out_file;
    integer read_bit;
        
    /* Simulation */
    initial begin
        
        /* Open input PDM file */
        pdm_file = $fopen("/home/gabriele/Desktop/Projects/ZenithSoC/Testbench/PDM/pdm.txt", "r");

        if (pdm_file == 0) begin
            $display("Error: Unable to open PDM input file!");
            $finish;
        end

        /* Open output PCM file */
        out_file = $fopen("/home/gabriele/Desktop/Projects/ZenithSoC/Testbench/PDM/pcm.txt", "w");

        if (out_file == 0) begin
            $display("Error: Unable to create PCM output file!");
            $finish;
        end

        /* Initialize signals */
        rst_n_i <= 0;
        reset_filter_i <= 1;
        pdm_i <= 0;
        decimator_factor_i <= DECIMATION_FACTOR;  // Decimation factor for 48 kHz

        /* Apply reset */
        #100;  // Wait for 100 ns
        rst_n_i = 1;
        reset_filter_i = 0;
        #100;

        /* Feed PDM input from file */
        while (!$feof(pdm_file)) begin
            if (valid_i) begin
                read_bit = $fgetc(pdm_file);

                if (read_bit == "0") begin
                    pdm_i = 0;
                end else if (read_bit == "1") begin
                    pdm_i = 1;
                end else begin
                    continue;
                end 
            end

            @(posedge clk_i);
        end
        
        repeat (1000) @(posedge clk_i);

        /* End simulation */
        $fclose(pdm_file);
        $fclose(out_file);
        $stop;
    end

    logic [WIDTH - 1:0] sample;

    /* Write PCM output to file */
    always_ff @(posedge clk_i) begin
        if (valid_o) begin
            $fwrite(out_file, "%d\n", pcm_o);

            sample <= pcm_o;
        end
    end

endmodule
