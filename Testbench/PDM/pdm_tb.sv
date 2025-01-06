`include "/home/gabriele/Desktop/Projects/ZenithSoC/Hardware/Utility/Packages/pdm2pcm_pkg.sv"

module tb_pdm2pcm;

    /* Testbench nets */
    logic clk = 0;
    logic rst_n = 0;
    logic interrupt;

    logic pdm_data = 0;
    logic pdm_clk;
    logic pdm_lrsel;

    logic write = 0;
    pdm2pcm_registers_t write_address;
    logic [3:0][7:0] write_data;
    logic [3:0] write_strobe = '1;
    logic write_error;

    logic read = 0;
    pdm2pcm_registers_t read_address;
    logic [31:0] read_data;
    logic read_error;

    /* DUT instance */
    pdm2pcm #(
        .LEFT_BUFFER_SIZE  ( 512 ),
        .RIGHT_BUFFER_SIZE ( 512 )
    ) dut (
        .clk_i       (clk),
        .rst_n_i     (rst_n),
        .interrupt_o (interrupt),

        .pdm_data_i  (pdm_data),
        .pdm_clk_o   (pdm_clk),
        .pdm_lrsel_o (pdm_lrsel),

        .write_i         (write),
        .write_address_i (write_address),
        .write_data_i    (write_data),
        .write_strobe_i  (write_strobe),
        .write_error_o   (write_error),

        .read_i         (read),
        .read_address_i (read_address),
        .read_data_o    (read_data),
        .read_error_o   (read_error)
    );

    /* Clock generation */
    always #5 clk <= ~clk;

    /* Testbench tasks */

    /* Task to start audio processing */
    task start_audio(input logic [6:0] clk_div);
        automatic pdm2pcm_control_t reg_read;
        automatic logic [31:0] tmp;

        read_register(PDM2PCM_CONTROL, tmp);
        reg_read = tmp;
        reg_read.interface_enable = 1'b1;
        reg_read.buffer_enable = 1'b1;
        reg_read.clock_divisor = clk_div;
        tmp = reg_read;
        write_register(PDM2PCM_CONTROL, tmp);
    endtask

    /* Task to stop audio processing */
    task stop_audio();
        automatic pdm2pcm_control_t reg_read;
        automatic logic [31:0] tmp;

        read_register(PDM2PCM_CONTROL, tmp);
        reg_read = tmp;
        reg_read.interface_enable = 1'b0;
        tmp = reg_read;
        write_register(PDM2PCM_CONTROL, tmp);
    endtask

    /* Change decimation rate */
    task change_decimation(input logic [7:0] decimation_rate);
        automatic logic [7:0] reg_read;

        read_register(PDM2PCM_DECIMATION_FACTOR, reg_read);
        reg_read = decimation_rate;
        write_register(PDM2PCM_DECIMATION_FACTOR, reg_read);
    endtask
    
    /* Cange normalization rate */
    task change_normalizer(input logic [31:0] normalizer);
        write_register(PDM2PCM_NORMALIZER, normalizer);
    endtask

    /* Change gain */
    task change_gain(input logic [15:0] gain);
        automatic logic [15:0] reg_read;

        read_register(PDM2PCM_GAIN, reg_read);
        reg_read = gain;
        write_register(PDM2PCM_GAIN, reg_read);
    endtask

    /* Task to switch to the next audio mode */
    task channel(input logic channel_selection, input logic dual_channel);
        automatic pdm2pcm_control_t reg_read = '0;
        automatic logic [31:0] tmp = '0;

        read_register(PDM2PCM_CONTROL, tmp);
        reg_read = tmp;
        reg_read.interface_enable = 1'b0;
        tmp = reg_read;
        write_register(PDM2PCM_CONTROL, tmp);

        @(posedge clk);

        read_register(PDM2PCM_CONTROL, tmp);
        reg_read = tmp;
        reg_read.channel = channel_selection;
        reg_read.dual_channel = dual_channel;
        tmp = reg_read;
        write_register(PDM2PCM_CONTROL, tmp);
    endtask

    /* Task to read from a register */
    task read_register(input pdm2pcm_registers_t address, output logic [31:0] data);
        read_address <= address;

        read <= 1'b1;
        @(posedge clk);
        read <= 1'b0;

        data <= read_data;
    endtask

    /* Task to write to a register */
    task write_register(input pdm2pcm_registers_t address, input logic [31:0] data);
        write_address <= address;
        write_data <= data;
        
        write <= 1'b1;
        @(posedge clk);
        write <= 1'b0;
    endtask

    /* Reset logic */
    initial begin
        rst_n = 0;
        #20 rst_n = 1;
    end


    integer pdm_file; logic [15:0] sample; logic [31:0] temp; integer read_bit;

    /* Test sequences */
    initial begin
        /* Open input PDM file */
        pdm_file = $fopen("/home/gabriele/Desktop/Projects/ZenithSoC/Testbench/PDM/pdm.txt", "r");

        if (pdm_file == 0) begin
            $display("Error: Unable to open PDM input file!");
            $finish;
        end

        rst_n = 0;
        #20; 
        rst_n = 1;

        @(posedge clk);

        fork 
            begin
                // channel(LEFT, 1'b0);
                change_gain(16'h8000);
                change_decimation(8'd62);
                change_normalizer(62 ** 4);
                start_audio(7'd16);

                while (1) begin 
                    read_register(PDM2PCM_STATUS, temp);

                    if (!temp[3]) begin
                        read_register(PDM2PCM_SAMPLE_BUFFER, sample);
                    end

                    @(posedge clk);
                end
            end

            begin
                #100;

                /* Feed PDM input from file */
                while (!$feof(pdm_file)) begin
                    if (dut.pdm_valid) begin
                        read_bit = $fgetc(pdm_file);

                        if (read_bit == "0") begin
                            pdm_data = 0;
                        end else if (read_bit == "1") begin
                            pdm_data = 1;
                        end 
                    end

                    @(posedge clk);
                end
            end
        join

        $finish;
    end

endmodule