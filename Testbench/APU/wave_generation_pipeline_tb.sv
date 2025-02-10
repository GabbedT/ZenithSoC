`timescale 1ns/1ps

module tb_wave_generation_pipeline;

    logic clk_i;
    logic rst_n_i;
    logic         write_table_i;
    logic [9:0]   table_addr_i;
    logic [15:0]  pcm_i;
    logic sine_wave_enable_i;
    logic sine_wave_start_i;
    logic square_wave_enable_i;
    logic square_wave_start_i;
    logic triangle_wave_enable_i;
    logic triangle_wave_start_i;
    logic custom_wave_enable_i;
    logic custom_wave_start_i;
    logic [31:0]   sine_wave_frequency_i;
    logic [31:0]  square_wave_frequency_i;
    logic [31:0]  triangle_wave_frequency_i;
    logic [26:0]  custom_wave_frequency_i;
    logic [31:0]  square_wave_duty_cycle_i;
    logic [15:0] sine_wave_gain_i;
    logic [15:0] square_wave_gain_i;
    logic [15:0] triangle_wave_gain_i;
    logic [15:0] custom_wave_gain_i;
    logic [15:0] mixed_wave_o;

    wave_generation_pipeline dut (.*);

    initial begin
        clk_i <= 0;
    end

    always #5 clk_i <= ~clk_i;


    initial begin
        // Initialize all signals
        rst_n_i                 <= 0;
        write_table_i           <= 0;
        table_addr_i            <= 10'd0;
        pcm_i                   <= 16'd0;
        sine_wave_enable_i      <= 0;
        sine_wave_start_i       <= 0;
        square_wave_enable_i    <= 0;
        square_wave_start_i     <= 0;
        triangle_wave_enable_i  <= 0;
        triangle_wave_start_i   <= 0;
        custom_wave_enable_i    <= 0;
        custom_wave_start_i     <= 0;
        sine_wave_frequency_i   <= 10'd0;
        square_wave_frequency_i <= 32'd0;
        sine_wave_gain_i        <= 16'd0;
        square_wave_gain_i      <= 16'd0;
        triangle_wave_gain_i    <= 16'd0;
        custom_wave_gain_i      <= 16'd0;
        triangle_wave_frequency_i <= 32'd0;
        custom_wave_frequency_i   <= 27'd0;
        square_wave_duty_cycle_i  <= 32'd0;

        // Apply Reset
        $display("Applying reset...");
        #20;
        rst_n_i <= 1;
        #20;

        //-------------------------------------------------------------------------
        // Write custom wave table: Create a sawtooth wave
        //-------------------------------------------------------------------------

        $display("Writing custom wave table...");

        // Loop through all 1024 addresses
        for (int i = 0; i < 1024; i++) begin
            @(posedge clk_i);
            write_table_i <= 1;
            table_addr_i  <= i;

            // Compute a sawtooth value from -32768 to +32767
            pcm_i <= (i * 16'd65535) / 1023 - 16'd32768;
        end

        @(posedge clk_i);
        write_table_i <= 0;
        $display("Custom wave table write complete.");

        //-------------------------------------------------------------------------
        // Test Case 1: Sine Wave Generation Only
        //-------------------------------------------------------------------------

        $display("Test 1: Sine wave generation only");
        sine_wave_enable_i      <= 1;
        sine_wave_start_i       <= 1;
        sine_wave_frequency_i   <= 100000;     // Example frequency step
        sine_wave_gain_i        <= 16'h4000;   // Example gain
        @(posedge clk_i);
        sine_wave_start_i       <= 0;
        #1ms;
        sine_wave_enable_i      <= 0;

        //-------------------------------------------------------------------------
        // Test Case 2: Square Wave Generation with Various Duty Cycles
        //-------------------------------------------------------------------------

        $display("Test 2: Square wave generation only, duty cycle 25%");
        sine_wave_enable_i      <= 0; // Disable sine wave
        square_wave_enable_i    <= 1;
        square_wave_start_i     <= 1;
        square_wave_frequency_i <= 32'd100000;    // Example frequency step
        square_wave_duty_cycle_i<= 32'h80000000;     // 50% duty cycle
        square_wave_gain_i      <= 16'h2000;   // Example gain
        @(posedge clk_i);
        square_wave_start_i     <= 0;
        #1ms;

        // Change duty cycle to 75%
        $display("Test 2b: Square wave generation only, duty cycle 25%");
        square_wave_start_i     <= 1;
        square_wave_frequency_i <= 32'd1000000;    // Example frequency step
        square_wave_duty_cycle_i <= 32'h40000000;      // 25% duty cycle
        @(posedge clk_i);
        square_wave_start_i     <= 0;
        #1ms;
        square_wave_enable_i    <= 0;

        //-------------------------------------------------------------------------
        // Test Case 3: Triangle Wave Generation Only
        //-------------------------------------------------------------------------

        $display("Test 3: Triangle wave generation only");
        square_wave_enable_i    <= 0; // Disable square wave
        triangle_wave_enable_i  <= 1;
        triangle_wave_start_i   <= 1;
        triangle_wave_frequency_i <= 32'd100000;   // Example frequency step
        triangle_wave_gain_i    <= 16'h3000; // Example gain
        @(posedge clk_i);
        triangle_wave_start_i   <= 0;
        #1ms;
        triangle_wave_enable_i  <= 0;

        //-------------------------------------------------------------------------
        // Test Case 4: Custom Wave Generation Only
        //-------------------------------------------------------------------------

        $display("Test 4: Custom wave generation only");

        triangle_wave_enable_i  <= 0; // Disable triangle wave
        custom_wave_enable_i    <= 1;
        custom_wave_start_i     <= 1;
        custom_wave_frequency_i <= 27'd200;  // Example custom frequency divisor
        custom_wave_gain_i      <= 16'h1000;    // Example gain

        @(posedge clk_i);
        custom_wave_start_i     <= 0;
        #1ms;
        custom_wave_enable_i    <= 0;

        //-------------------------------------------------------------------------
        // Test Case 5: All Generators Enabled Simultaneously
        //-------------------------------------------------------------------------

        $display("Test 5: All wave generators enabled");

        // Sine wave
        sine_wave_enable_i      <= 1;
        sine_wave_start_i       <= 1;
     

        // Square wave
        square_wave_enable_i    <= 1;
        square_wave_start_i     <= 1;
        square_wave_frequency_i <= 32'd120;
  

        // Triangle wave
        triangle_wave_enable_i  <= 1;
        triangle_wave_start_i   <= 1;
 

        // Custom wave
        custom_wave_enable_i    <= 1;
        custom_wave_start_i     <= 1;

        @(posedge clk_i);
        // Deassert start signals

        sine_wave_start_i       <= 0;
        square_wave_start_i     <= 0;
        triangle_wave_start_i   <= 0;
        custom_wave_start_i     <= 0;
        #1ms;

        for (int i = 0; i < 10; ++i) begin
            custom_wave_frequency_i <= $random();
            custom_wave_gain_i      <= $random();
            triangle_wave_frequency_i <= $random();
            triangle_wave_gain_i    <= $random();
            square_wave_duty_cycle_i<= $random();
            square_wave_gain_i      <= $random();
            sine_wave_frequency_i   <= $random();
            sine_wave_gain_i        <= $random();

            #20ms;
        end

        $display("Simulation finished.");
        $finish;
    end

endmodule
