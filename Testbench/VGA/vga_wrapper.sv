`ifndef VGA_WRAPPER_SV
    `define VGA_WRAPPER_SV

`define _VERILATOR_

`include "../../Hardware/Utility/Packages/vga_pkg.sv"

`include "../../Hardware/IO/VGA/vga.sv"

module vga_wrapper (
    input logic clk_i,
    input logic rst_n_i,
    input logic start_i,
    output logic on_o,

    /* VGA interface */
    output logic hsync_o,
    output logic vsync_o,
    output logic [3:0] red_o,
    output logic [3:0] green_o,
    output logic [3:0] blue_o
);

    // logic [9:0] divider; logic enable;

    // always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
    //     if (!rst_n_i) begin 
    //         divider <= '0;
    //     end else begin 
    //         divider <= divider + 1;
    //     end 
    // end 

    // assign enable = divider == '1;

    logic [3:0] red, green, blue, up, down;

    logic [10:0] hcount_CRT, vcount_CRT, y_CRT;
    logic [10:0] hcount_NXT, vcount_NXT, y_NXT;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                hcount_CRT <= '0;
                vcount_CRT <= '0;

                y_CRT <= '0;
            end else begin 
                hcount_CRT <= hcount_NXT;
                vcount_CRT <= vcount_NXT;

                y_CRT <= y_NXT;
            end 
        end 

    assign up = hcount_CRT[6:3];
    assign down = ~hcount_CRT[6:3];

        always_comb begin
            red = '0;
            green = '0;
            blue = '0;
            
            if (vcount_CRT < 128) begin
                red = hcount_CRT[8:5];
                green = hcount_CRT[8:5];
                blue = hcount_CRT[8:5];
            end else if (vcount_CRT < 256) begin
                red = {hcount_CRT[8], hcount_CRT[8], 2'b00};
                green = {hcount_CRT[7], hcount_CRT[7], 2'b00};
                blue = {hcount_CRT[6], hcount_CRT[6], 2'b00};
            end else begin
                case (hcount_CRT[9:7])
                    3'b000: begin
                        red = '1;
                        green = up;
                        blue = '0;
                    end

                    3'b001: begin
                        red = down;
                        green = '1;
                        blue = '0;
                    end

                    3'b010: begin
                        red = '0;
                        green = '1;
                        blue = up;
                    end

                    3'b011: begin
                        red = '0;
                        green = down;
                        blue = '1;
                    end

                    3'b100: begin
                        red = up;
                        green = '0;
                        blue = '1;
                    end

                    3'b101: begin
                        red = '1;
                        green = '0;
                        blue = down;
                    end

                    default: begin
                        red = '1;
                        green = '1;
                        blue = '1;
                    end
                endcase
            end
        end


    typedef enum logic [1:0] {IDLE, WRITE_BUFFER, WAIT} states_t;

    logic [15:0] buffer_address; logic [31:0] write_data, read_data; logic write, read; logic [16:0] write_address, read_address;

    states_t state_CRT, state_NXT; 

    assign on_o = state_CRT != IDLE;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                state_CRT <= IDLE;
            end else begin 
                state_CRT <= state_NXT;
            end 
        end 


    assign read_address[16] = 1;
    assign read_address[15:0] = 0;

        always_comb begin
            y_NXT = y_CRT;
            state_NXT = state_CRT;
            hcount_NXT = hcount_CRT;
            vcount_NXT = vcount_CRT;

            write_data = '0;
            write_address = '0;
            write = '0;

            read = '0;

            case (state_CRT)
                IDLE: begin
                    if (start_i) begin
                        state_NXT = WRITE_BUFFER;

                        write_data = {'0, 9'b000000001};
                        write = 1;

                        write_address[16] = 1'b1;
                        write_address[15:0] = '0;
                    end
                end

                WRITE_BUFFER: begin
                    if (y_CRT == 96) begin
                        state_NXT = WAIT;
                    end else begin
                        write = 1; write_data = {'0, red, green, blue};

                        write_address[16:0] = '0;


                        // if (enable) begin
                            hcount_NXT = (hcount_CRT == 639) ? '0 : hcount_CRT + 1;

                            if (hcount_CRT == 639) begin
                                vcount_NXT = (vcount_CRT == 479) ? '0 : vcount_CRT + 1;
                                y_NXT = y_CRT + 1;
                            end
                        // end
                    end
                end

                WAIT: begin
                    read = 1;

                    write_data = {'0, 9'b000100001};
                    write = 1;

                    write_address[16] = 1'b1;
                    write_address[15:0] = '0;

                    if (read_data[3]) begin
                        state_NXT = WRITE_BUFFER;

                        y_NXT = '0;
                    end
                end
            endcase 
        end

    logic [4:0] thrash_value;

    vga vga_controller (
        .clk_i       ( clk_i ),
        .rst_n_i     ( rst_n_i ),
        .interrupt_o ( thrash_value[0] ),

        /* Write interface */
        .write_i         ( write ),
        .write_address_i ( write_address ),
        .write_data_i    ( write_data ),
        .write_error_o   ( thrash_value[1] ),
        .write_done_o    ( thrash_value[2] ),

        /* Read interface */
        .read_i         ( read ),
        .read_address_i ( read_address ),
        .read_data_o    ( read_data ),
        .read_error_o   ( thrash_value[3] ),
        .read_done_o    ( thrash_value[4] ),

        /* VGA interface */
        .hsync_o  ( hsync_o ),
        .vsync_o  ( vsync_o ),
        .red_o    ( red_o ),
        .green_o  ( green_o ),
        .blue_o   ( blue_o )
    );

endmodule 

`endif 