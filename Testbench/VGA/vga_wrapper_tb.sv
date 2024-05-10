module vga_wrapper_tb;
    
    logic clk_i = 0;
    logic rst_n_i = 0;
    logic start_i = 0;
    logic on_o;

    logic [9:0] hcount_o;
    logic [9:0] vcount_o;
    logic video_on_o;

    /* VGA interface */
    logic hsync_o;
    logic vsync_o;
    logic [3:0] red_o;
    logic [3:0] green_o;
    logic [3:0] blue_o;

    always #5 clk_i <= !clk_i;

    vga_wrapper dut (.*);

    logic [11:0] data; int file;

    assign data = {red_o, green_o, blue_o};

    logic [11:0] display_640x480 [480][640]; int col, row;

    initial begin 
        col = 0;
        row = 0;
        rst_n_i = 0;
        repeat(2) @(posedge clk_i);
        rst_n_i = 1;
        start_i = 1;
        @(posedge clk_i);
        start_i = 0;

        file = $fopen("/home/gabbed/Projects/ZenithSoC/Testbench/VGA/vga_frames.txt", "w"); $display("%d", file);

                repeat(2000000) begin 
                    @(posedge clk_i);

                    if (dut.vga_controller.registers.status_register.enable_video) begin
                        if (dut.vga_controller.pixel_pulse & dut.vga_controller.video_on) begin
                            if (col == 0 & row <= 8)
                                $display("[%t] #%h%h%h", $time, red_o, green_o, blue_o);

                            $fwrite(file, "#%h%h%h ", red_o, green_o, blue_o);

                            col = col + 1'b1;

                            if (col == 640) begin
                                col = 0;
                                row = row + 1'b1;

                                $fwrite(file, "\n");
                            end 
                                
                            if (row == 480) begin
                                row = 0;
                                col = 0;

                                $fdisplay(file, "\n\n\n");
                            end
                        end
                    end else begin
                        row = 0;
                        col = 0;
                    end
                end

        $fclose(file);

        $finish;
    end 

endmodule : vga_wrapper_tb