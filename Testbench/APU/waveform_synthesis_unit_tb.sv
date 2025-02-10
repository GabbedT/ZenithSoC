`timescale 1ns / 1ps

module tb_waveform_synthesis_unit;

    localparam SINE_WAVE = 0;
    localparam SQUARE_WAVE = 1;
    localparam TRIANGLE_WAVE = 2;
    localparam CUSTOM_WAVE = 3;

    localparam logic [31:0] MAX_32 = '1;

    localparam SYS_FREQUENCY = 100000000;
    localparam MILLISECONDS = 100 * 1000;

    logic clk_i = '0;
    logic rst_n_i = '0;
    
    logic [3:0] adsr_start_i = '0;
    logic [3:0] adsr_enable_i = '0;
    logic [3:0][31:0] attack_step_i = '0;
    logic [3:0][31:0] decay_step_i = '0;
    logic [3:0][31:0] release_step_i = '0;
    logic [3:0][31:0] sustain_duration_i = '0;
    logic [3:0][31:0] attack_level_i = '0;
    logic [3:0][31:0] sustain_level_i = '0;

    logic write_table_i = '0;
    logic [9:0] table_addr_i = '0;
    logic [15:0] pcm_i = '0;

    logic [3:0] wave_enable_i = '0;
    logic [3:0] wave_start_i = '0;
    logic [3:0][31:0] wave_frequency_i = '0;
    logic [31:0] square_wave_duty_cycle_i = '0;
    logic [3:0][15:0] wave_gain_i = {4{16'h8000}};

    logic table_empty_o;
    logic [3:0] adsr_idle_o;
    logic [15:0] mixed_wave_o;


    waveform_synthesis_unit dut (.*);

    always #5 clk_i <= !clk_i;


    task start_wave(input int wave, input logic [15:0] gain);
        wave_enable_i[wave] <= 1'b1;
        wave_start_i[wave] <= 1'b1;
        wave_gain_i[wave] <= gain;
        @(posedge clk_i);
        wave_start_i[wave] <= 1'b0;
    endtask : start_wave

    task gain_wave(input int wave, input logic [15:0] gain);
        wave_gain_i[wave] <= gain;
        @(posedge clk_i);
    endtask : gain_wave

    task sine_frequency(input integer frequency);
        automatic logic [63:0] num = (frequency * (MAX_32) * 4);

        wave_frequency_i[SINE_WAVE] <= num / SYS_FREQUENCY;
        @(posedge clk_i);
    endtask : sine_frequency

    task square_frequency(input int frequency);
        automatic logic [63:0] num = (frequency * (MAX_32));

        wave_frequency_i[SQUARE_WAVE] <= num / SYS_FREQUENCY;
        @(posedge clk_i);
    endtask : square_frequency

    task triangle_frequency(input int frequency);
        automatic logic [63:0] num = (frequency * (MAX_32) * 2);

        wave_frequency_i[TRIANGLE_WAVE] <= num / SYS_FREQUENCY;
        @(posedge clk_i);
    endtask : triangle_frequency

    task custom_frequency(input int frequency);
        automatic logic [63:0] num = (frequency * (MAX_32));

        wave_frequency_i[CUSTOM_WAVE] <= num / SYS_FREQUENCY;
        @(posedge clk_i);
    endtask : custom_frequency

    task square_duty_cycle(input int duty_cycle_i);
        square_wave_duty_cycle_i <= duty_cycle_i;
        @(posedge clk_i);
    endtask : square_duty_cycle

    task enable_adsr(input int wave);
        adsr_enable_i[wave] <= 1'b1;
        adsr_start_i[wave] <= 1'b1;
        @(posedge clk_i);
        adsr_start_i[wave] <= 1'b0;
    endtask : enable_adsr

    task modify_adsr(
        /* Time in ms */
        input int wave, 
        input logic [31:0] attack_time, input logic [31:0] decay_time, input logic [31:0] sustain_time, input logic [31:0] release_time,
        input logic [31:0] attack_level, input logic [31:0] sustain_level
    );
        attack_level_i[wave] <= attack_level;
        sustain_level_i[wave] <= sustain_level;

        attack_step_i[wave] <= attack_level / (MILLISECONDS * attack_time);
        decay_step_i[wave] <= (attack_level - sustain_level) / (MILLISECONDS * decay_time); // Absolute value of attack - sustain
        release_step_i[wave] <= sustain_level / (MILLISECONDS * release_time);

        sustain_duration_i[wave] <= sustain_time * (SYS_FREQUENCY / 1000);
        @(posedge clk_i);
    endtask : modify_adsr


    initial begin
        repeat(2) @(posedge clk_i);
        rst_n_i <= 1'b1;

        for (int i = 0; i < 1024; ++i) begin
            pcm_i <= i * ((2**16 - 1) / 1024);
            table_addr_i <= i;
            write_table_i <= 1'b1;
            @(posedge clk_i);
        end

        write_table_i <= 1'b0;

        // start_wave(SINE_WAVE, 16'h8000);

        // for (logic [4:0] i = 1; i < 20; ++i) begin
        //     sine_frequency(i % 2 ? (10000 * i) : (10000 / i));
        //     gain_wave(SINE_WAVE, i % 2 ? 16'h8000 : 16'h4000);
        //     repeat(123490) @(posedge clk_i);
        // end

        // wave_enable_i[SINE_WAVE] <= 1'b0;



        // start_wave(SQUARE_WAVE, 16'h8000);

        // for (logic [4:0] i = 1; i < 100; ++i) begin
        //     square_frequency(i % 2 ? (1000000 * i) : (1000 / i));
        //     square_duty_cycle('1 / i);
        //     gain_wave(SQUARE_WAVE, i % 2 ? 16'h8000 : 16'h4000);
        //     repeat(123490) @(posedge clk_i);
        // end

        // wave_enable_i[SQUARE_WAVE] <= 1'b0;


        // start_wave(TRIANGLE_WAVE, 16'h8000);

        // for (logic [4:0] i = 1; i < 100; ++i) begin
        //     triangle_frequency(i % 2 ? (1000000 * i) : (1000 / i));
        //     gain_wave(TRIANGLE_WAVE, i % 2 ? 16'h8000 : 16'h4000);
        //     repeat(123490) @(posedge clk_i);
        // end

        // wave_enable_i[TRIANGLE_WAVE] <= 1'b0;

        start_wave(CUSTOM_WAVE, 16'h8000);

        custom_frequency(48000);
        #3ms;
        custom_frequency(42000);
        #3ms;
        custom_frequency(24000);
        #3ms;

        // start_wave(SINE_WAVE, 16'h4000);
        // start_wave(TRIANGLE_WAVE, 16'h8000);

        // sine_frequency(10000);
        // triangle_frequency(1000);

        // #10ms;

        // modify_adsr(SINE_WAVE, 2, 1, 10, 3, 32'h80000000, 32'h20000000);
        // modify_adsr(TRIANGLE_WAVE, 2, 1, 10, 3, 32'h80000000, 32'h60000000);
        // enable_adsr(SINE_WAVE);
        // enable_adsr(TRIANGLE_WAVE);

        #50ms;
    end

endmodule
