module vga_testbench;

    localparam BUFFER_SIZE = 2**16;
    
    logic clk_i = 0;
    logic rst_n_i = 0;
    logic interrupt_o;

    /* Write interface */
    logic write_i = 0;
    logic [$clog2(BUFFER_SIZE):0] write_address_i = 0;
    logic [3:0][7:0] write_data_i = 0;
    logic write_error_o;
    logic write_done_o;

    /* Read interface */
    logic read_i = 0;
    logic [$clog2(BUFFER_SIZE):0] read_address_i = 0;
    logic [31:0] read_data_o;
    logic read_error_o;
    logic read_done_o;

    /* VGA interface */
    logic hsync_o;
    logic vsync_o;
    logic [3:0] red_o;
    logic [3:0] green_o;
    logic [3:0] blue_o;

    always #5 clk_i <= !clk_i;

    vga #(BUFFER_SIZE) dut (.*);


    task write_register(input int addr, input logic [31:0] data);
        write_address_i <= addr;
        write_data_i <= data;

        write_i <= 1'b1;
        @(posedge clk_i);
        write_i <= 1'b0;
    endtask : write_register

    task read_register(input int addr);
        read_address_i <= addr;

        read_i <= 1'b1;
        @(posedge clk_i);
        read_i <= 1'b0;
    endtask : read_register


    logic [11:0] display_640x480 [480][640];
    logic [11:0] display_800x600 [800][600];

    int col, row;

    int colour, index;

    initial begin
        rst_n_i <= 1'b0;
        @(posedge clk_i);
        rst_n_i <= 1'b1;

        index <= '0;
        colour <= 12'hFFF;
        col <= 0;
        row <= 0;

        fork 
            begin
                write_register(1 << $clog2(BUFFER_SIZE), 9'b000100001);

                repeat(5) begin
                    /* Write 96 lines */
                    repeat(96) begin
                        /* Write 640 pixels */
                        repeat(640) begin
                            if (index % 2 == 0)
                                write_register(0, 12'hFFF);
                            else 
                                write_register(0, 12'h000);

                            index += 1'b1;
                        end
                    end

                    wait (dut.registers.status_register.buffer_empty);
                end
                
                wait (dut.registers.status_register.frame_done);
                
                index <= '0;

                write_register(1 << $clog2(BUFFER_SIZE), 9'b000010000);
                repeat(2) @(posedge clk_i);
                write_register(1 << $clog2(BUFFER_SIZE), 9'b000110001);

                repeat(8) begin
                    /* Write 75 lines */
                    repeat(75) begin
                        /* Write 640 pixels */
                        repeat(800) begin
                            if (index % 2 == 0)
                                write_register(0, 12'hFFF);
                            else 
                                write_register(0, 12'h000);

                            index += 1'b1;
                        end
                    end

                    wait (dut.registers.status_register.buffer_empty);
                end
                
                wait (dut.registers.status_register.frame_done);
            end

            begin
                while (1) begin
                    if (dut.registers.status_register.enable_video) begin
                        if (dut.registers.status_register.resolution == _640x480_) begin
                            if (dut.pixel_pulse & dut.video_on) begin
                                display_640x480[col][row] <= {red_o, green_o, blue_o};

                                col <= col + 1'b1;

                                if (col == 640) begin
                                    col <= 0;
                                    row <= row + 1'b1;
                                end 
                                
                                if (row == 480) begin
                                    row <= 0;
                                    col <= 0;
                                end
                            end
                        end else begin
                            if (dut.pixel_pulse & dut.video_on) begin
                                display_800x600[col][row] <= {red_o, green_o, blue_o};

                                col <= col + 1'b1;

                                if (col == 800) begin
                                    col <= 0;
                                    row <= row + 1'b1;
                                end 
                                
                                if (row == 480) begin
                                    row <= 0;
                                    col <= 0;
                                end
                            end
                        end
                    end else begin
                        row <= 0;
                        col <= 0;
                    end

                    @(posedge clk_i);
                end
            end
        join_any

        $finish;
    end

endmodule : vga_testbench