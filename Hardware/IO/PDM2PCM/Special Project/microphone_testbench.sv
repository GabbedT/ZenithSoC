module microphone_testbench;

    logic clk_i = 0;
    logic rst_n_i = 0;

    /* PDM Interface */
    logic pdm_data_i = 0;
    logic pdm_clk_o;
    logic pdm_lrsel_o;
    
    logic pwm_o;
    logic audio_enable_o;
    logic led_ground_o;

    /* User Interface */
    logic start_recording_i = 0;
    logic replay_audio_i = 0;
    logic stop_i = 0;

    logic is_recording_o;
    logic is_replaying_o;

    /* DDR Interface */
    wire [15:0] ddr2_dq;
    wire [1:0]  ddr2_dqs_n;
    wire [1:0]  ddr2_dqs_p;
    wire [1:0]  ddr2_dm;
    wire [12:0] ddr2_addr;
    wire [2:0]  ddr2_ba;
    wire        ddr2_ras_n;
    wire        ddr2_cas_n;
    wire        ddr2_we_n;
    wire        ddr2_cke;
    wire        ddr2_ck_p;
    wire        ddr2_ck_n;
    wire        ddr2_cs_n;
    wire        ddr2_odt;

    microphone dut (.*);

    // ddr2_model ddr2 (
    //     .ck      ( ddr2_ck_p  ),
    //     .ck_n    ( ddr2_ck_n  ),
    //     .cke     ( ddr2_cke   ),
    //     .cs_n    ( ddr2_cs_n  ),
    //     .ras_n   ( ddr2_ras_n ),
    //     .cas_n   ( ddr2_cas_n ),
    //     .we_n    ( ddr2_we_n  ),
    //     .dm_rdqs ( ddr2_dm    ),
    //     .ba      ( ddr2_ba    ),
    //     .addr    ( ddr2_addr  ),
    //     .dq      ( ddr2_dq    ),
    //     .dqs     ( ddr2_dqs_p ),
    //     .dqs_n   ( ddr2_dqs_n ),
    //     .rdqs_n  (            ),
    //     .odt     ( ddr2_odt   )
    // );

    /* Clock generation */
    always #5 clk_i <= ~clk_i;


    integer pdm_file, read_bit;

    initial begin
        pdm_file = $fopen("/home/gabriele/Desktop/Projects/ZenithSoC/Testbench/PDM/pdm.txt", "r");

        if (pdm_file == 0) begin
            $display("Error: Unable to open PDM input file!");
            $finish;
        end

        rst_n_i <= 0;
        #20; 
        rst_n_i <= 1;

        wait (dut.ddr_ready);
        wait (dut.locked);

        repeat(10) @(posedge clk_i);

        fork
            begin
                /* User input */
                start_recording_i <= 1'b1;
                @(posedge clk_i);
                start_recording_i <= 1'b0;

                #3ms;

                stop_i <= 1'b1;
                repeat(10) @(posedge clk_i);
                stop_i <= 1'b0;

                repeat(10) @(posedge clk_i);

                replay_audio_i <= 1'b1;
                @(posedge clk_i);
                replay_audio_i <= 1'b0;

                #800us;

                stop_i <= 1'b1;
                repeat(10) @(posedge clk_i);
                stop_i <= 1'b0;

                start_recording_i <= 1'b1;
                @(posedge clk_i);
                start_recording_i <= 1'b0;

                #800us;

                stop_i <= 1'b1;
                repeat(10) @(posedge clk_i);
                stop_i <= 1'b0;

                repeat(10) @(posedge clk_i);

                replay_audio_i <= 1'b1;
                @(posedge clk_i);
                replay_audio_i <= 1'b0;

                #800us;

                stop_i <= 1'b1;
                @(posedge clk_i);
                stop_i <= 1'b0;
            end

            begin
                #100;

                /* Feed PDM input from file */
                while (!$feof(pdm_file)) begin
                    if (dut.pdm_valid) begin
                        read_bit = $fgetc(pdm_file);

                        if (read_bit == "0") begin
                            pdm_data_i = 0;
                        end else if (read_bit == "1") begin
                            pdm_data_i = 1;
                        end 
                    end

                    @(posedge clk_i);
                end
            end
        join

        $finish;
    end

endmodule : microphone_testbench