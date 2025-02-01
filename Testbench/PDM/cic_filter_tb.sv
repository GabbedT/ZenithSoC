`timescale 1ns/1ps

module cic_filter_tb;

    localparam NUMBER_OF_DUT = 5;
    localparam DECIMATION_FACTOR = 32;

    parameter WIDTH = 16;
    parameter FILTER_ORDER = 5;
    parameter COMB_DELAY = 1;

    logic clk_i = 0;
    logic clk_en_i = 1;
    logic rst_n_i = 0;

    logic reset_filter_i = 1;
    logic [7:0] decimator_factor_i = 0;
    logic pdm_i = 0;
    logic valid_i = 0;

    logic [15:0] gain_i;
    // logic [NUMBER_OF_DUT - 1:0][63:0] pcm_o;
    // logic [NUMBER_OF_DUT - 1:0] valid_out;
    logic [15:0] pcm_o, filtered_sample;
    logic valid_out;
    logic valid_o, invalid_o;

    pdm2pcm_processing_pipeline #(
        .CIC_FILTER_ORDER(5),
        .CIC_COMB_DELAY(1)
    ) dut (
        .clk_i              ( clk_i              ),
        .rst_n_i            ( rst_n_i            ),
        .clk_en_i           ( clk_en_i           ),
        .pdm_i              ( pdm_i              ),
        .valid_i            ( valid_i            ),
        .channel_i (1'b0),
        .decimator_factor_i ( decimator_factor_i ),
        .normalizer_i       ( 32 ** 4 ),
        .gain_i             ( gain_i             ),
        .pcm_o              ( pcm_o              ),
        .valid_o            ( valid_o            ),
        .invalid_o          ( invalid_o          )
    );

    
    // genvar i;

    // generate
    //     for (i = 0; i < NUMBER_OF_DUT; ++i) begin
    //         pdm2pcm_decimator #(
    //             .WIDTH(32),          // Imposta la larghezza di output
    //             .FILTER_ORDER(2 + i),    // Imposta l'ordine del filtro (numero di stadi integratori/comb)
    //             .COMB_DELAY(1)       // Imposta il ritardo del comb
    //         ) decimator_inst (
    //             .clk_i(clk_i),                      // Segnale di clock
    //             .rst_n_i(rst_n_i),                  // Segnale di reset attivo basso
    //             .reset_filter_i( 0 ),    // Reset del filtro
    //             .decimator_factor_i(decimator_factor_i), // Fattore di decimazione
    //             .pdm_i(pdm_i),                   // Ingresso PDM
    //             .valid_i(valid_i),               // Segnale di validità per il dato in ingresso
    //             .pcm_o(pcm_o[i]),                  // Output PCM
    //             .valid_o(valid_out[i])               // Segnale di validità per l'output PCM
    //         );
    //     end
    // endgenerate 

    logic [15:0] pcm_final, pcm_audio, normalized_pcm, filter_pcm; logic [7:0] pcm_high, pcm_low;

        always_ff @(posedge clk_i) begin
            if (valid_out) begin
                pcm_final <= filtered_sample;
                
                pcm_high <= pcm_o[15:8];
                pcm_low <= pcm_o[7:0];
            end
            
            if (!rst_n_i) begin
                pcm_audio <= '0;
            end else if (valid_o) begin 
                pcm_audio <= pcm_o;
            end

            if (dut.normalized_valid) begin
                normalized_pcm <= dut.normalized_sample;
            end

            if (dut.valid_filter) begin
                filter_pcm <= dut.filtered_sample;
            end
        end
        
        


    localparam BIAS = 2**(15);

    logic [15:0] sample_scaled; 

    assign sample_scaled = $signed(pcm_audio) - 16'sd32768;
    logic [16:0] sample_biased;
    logic [16:0] accumulator;

    assign sample_biased = {sample_scaled[15], sample_scaled} + BIAS;

        always_ff @(posedge clk_i) begin
            if (!rst_n_i) begin 
                accumulator <= '0;
            end else begin
                accumulator <= {1'b0, accumulator[15:0]} + sample_biased;
            end
        end

    assign pwm_o = accumulator[16];
    
    
    

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
        decimator_factor_i <= DECIMATION_FACTOR; 
        gain_i <= '0;
        gain_i <= 16'h4000;


        /* Apply reset */
        #100;  // Wait for 100 ns
        rst_n_i = 1;
        reset_filter_i = 0;
        #100;

        fork 
            begin
                gain_i <= 16'h8000;
                #5s;
            end

            begin
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
            end
        join
        
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
