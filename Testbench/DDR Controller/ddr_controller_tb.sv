`timescale 1ns/1ns

`include "ddr2_model.v"

module ddr_controller_tb;

    localparam WRITE = 3'b000;
    localparam READ = 3'b001;

    wire [12:0] ddr2_addr; // This input indicates the address for the current request.
    wire [2:0] ddr2_ba;
    wire ddr2_cas_n;
    wire ddr2_ck_n;
    wire ddr2_ck_p;
    wire ddr2_cke;
    wire ddr2_ras_n;
    wire ddr2_we_n;
    wire [15:0] ddr2_dq; 
    wire [1:0] ddr2_dqs_n; 
    wire [1:0] ddr2_dqs_p; 

    wire ddr2_cs_n; 
    wire [1:0] ddr2_dm;
    wire ddr2_odt;
      
    logic sys_clk;
    logic mem_clk;
    logic ext_clk = 0;
    logic sys_rstn = 0;
    logic locked;

    always #5ns ext_clk <= !ext_clk;

    system_clocking pll (
        .sys_clk_o ( sys_clk ),
        .mem_clk_o ( mem_clk ), 
        .ext_clk_i ( ext_clk ),
        .locked    ( locked  )  
    );


task reset();
    sys_rstn <= 1'b0;

    repeat(200) @(posedge sys_clk);
    wait(locked);
    repeat(200) @(posedge sys_clk);

    sys_rstn <= 1'b1;

    @(posedge sys_clk);
endtask : reset

// 4:1 MODE

// COMMAND PATH
// When the user logic app_en signal is asserted and the app_rdy signal is asserted from the
// UI, a command is accepted and written to the FIFO by the UI. The command is ignored by
// the UI whenever app_rdy is deasserted. The user logic needs to hold app_en High along
// with the valid command and address values until app_rdy is asserted.
//
// A non back-to-back write command can be issued. Three scenarios for the app_wdf_data, 
// app_wdf_wren, and app_wdf_end signals, as follows:
// 1. Write data is presented along with the corresponding write command (second half of BL8).
// 2. Write data is presented before the corresponding write command.
// 3. Write data is presented after the corresponding write command, but should not exceed the limitation of two clock cycles.

// WRITE PATH
// The write data is registered in the write FIFO when app_wdf_wren is asserted and
// app_wdf_rdy is High. If app_wdf_rdy is deasserted, the user logic needs to
// hold app_wdf_wren and app_wdf_end High along with the valid app_wdf_data value
// until app_wdf_rdy is asserted. app_wdf_data data can be pushed even before app_cmd
// "write command" is asserted. The only condition is that for every app_cmd "write
// command," the associated app_wdf_data "write data" must be present. The
// app_wdf_mask signal can be used to mask out the bytes to write to external memory.
// The app_wdf_end signal must be used to indicate the end of a memory write burst. For
// memory burst types of eight in 2:1 mode, the app_wdf_end signal must be asserted on the
// second write data word.

// Single write writes data to 8 address locations in 4:1 mode. Infact 128 bits = 16 bytes, the DDR interface is 16 bits (2 bytes) so 16 / 2 = 8!

// READ PATH
// The read data is returned by the UI in the requested order and is valid when
// app_rd_data_valid is asserted. The app_rd_data_end
// signal indicates the end of each read command burst and is not needed in user logic

logic [26:0] address; logic write = 0, read = 0, push = 0, pull = 0, ready, read_valid, done = 0; logic [63:0] write_data, read_data;

ddr_memory_interface mem_intf (
    .clk_i       ( sys_clk  ),
    .rst_n_i     ( sys_rstn ),
    .mem_clk_i   ( mem_clk  ),
    .mem_rst_n_i ( sys_rstn ),

    .ddr2_dq_io    ( ddr2_dq    ),
    .ddr2_dm_o     ( ddr2_dm    ),
    .ddr2_dqs_n_io ( ddr2_dqs_n ),
    .ddr2_dqs_p_io ( ddr2_dqs_p ),
    .ddr2_addr_o   ( ddr2_addr  ),
    .ddr2_ba_o     ( ddr2_ba    ),
    .ddr2_ras_n_o  ( ddr2_ras_n ),
    .ddr2_cas_n_o  ( ddr2_cas_n ),
    .ddr2_we_n_o   ( ddr2_we_n  ),
    .ddr2_cke_o    ( ddr2_cke   ),
    .ddr2_ck_p_o   ( ddr2_ck_p  ),
    .ddr2_ck_n_o   ( ddr2_ck_n  ),
    .ddr2_cs_n_o   ( ddr2_cs_n  ),
    .ddr2_odt_o    ( ddr2_odt   ),
      
    .address_i    ( address    ), 
    .write_i      ( write      ), 
    .push_i       ( push       ),
    .pull_i       ( pull       ),
    .read_i       ( read       ), 
    .write_data_i ( write_data ),
    .read_data_o  ( read_data  ),
    .read_valid_o ( read_valid ),

    .done_i  ( done  ),
    .ready_o ( ready )
);

task write_memory(input bit sequential, input bit random, input int length, input int address_in);
    automatic logic [26:0] address_gen = random ? $random() : address_in;

    wait(ready);
    @(posedge sys_clk);

    if (sequential) begin
        for (int i = 0; i < length; ++i) begin
            write = 1'b1;
            push = 1'b1;
            write_data = i;
            address = address_gen;

            @(posedge sys_clk);
            address_gen += 8;

            write = 1'b0;
            push = 1'b1;
            write_data = i;
            @(posedge sys_clk);
        end

        write = 1'b0;
        push = 1'b0;

        done = 1'b1;
        @(posedge sys_clk);
        done = 1'b0;

    end else begin
        for (int i = 0; i < length; ++i) begin
            address_gen = $random();

            write = 1'b1;
            push = 1'b1;
            write_data = i;
            address = address_gen;
            @(posedge sys_clk);
            write = 1'b0;
            push = 1'b1;
            write_data = i;
            @(posedge sys_clk);
        end

        write = 1'b0;
        push = 1'b0;
        
        done = 1'b1;
        @(posedge sys_clk);
        done = 1'b0;
    end

    wait(!ready);
endtask : write_memory


task read_memory(input bit sequential, input bit random, input int length, input int address_in);
automatic logic [26:0] address_gen = random ? $random() : address_in;

    wait(ready);
    @(posedge sys_clk);

    if (sequential) begin
        for (int i = 0; i < length; ++i) begin
            read = 1'b1;
            address = address_gen;
            @(posedge sys_clk);

            address_gen += 8;
        end

        read = 1'b0;

        done = 1'b1;
        @(posedge sys_clk);
        done = 1'b0;

    end else begin
        for (int i = 0; i < length; ++i) begin
            address_gen = $random();

            read = 1'b1;
            address = address_gen;
            @(posedge sys_clk);
        end

        read = 1'b0;
        
        done = 1'b1;
        @(posedge sys_clk);
        done = 1'b0;
    end

    wait(!ready);

    wait(read_valid);

    for (int i = 0; i < 2 * length; ++i) begin
        wait(read_valid);
        pull = 1'b1;
        @(posedge sys_clk);
    end

    pull = 1'b0;
endtask : read_memory


ddr2_model ddr2 (
    .ck      ( ddr2_ck_p  ),
    .ck_n    ( ddr2_ck_n  ),
    .cke     ( ddr2_cke   ),
    .cs_n    ( ddr2_cs_n  ),
    .ras_n   ( ddr2_ras_n ),
    .cas_n   ( ddr2_cas_n ),
    .we_n    ( ddr2_we_n  ),
    .dm_rdqs ( ddr2_dm    ),
    .ba      ( ddr2_ba    ),
    .addr    ( ddr2_addr  ),
    .dq      ( ddr2_dq    ),
    .dqs     ( ddr2_dqs_p ),
    .dqs_n   ( ddr2_dqs_n ),
    .rdqs_n  (            ),
    .odt     ( ddr2_odt   )
);


    initial begin
        reset();
        wait(ready);

        /* Random addresses */
        write_memory(1, 0, 8, 0);
        write_memory(0, 1, 8, 0);
        write_memory(0, 1, 8, 0);
        write_memory(1, 1, 8, 0);

        /* Specific addresses */
        write_memory(1, 0, 8, 0);
        read_memory(1, 0, 8, 0);

        read_memory(1, 0, 1, 0);
        read_memory(1, 0, 1, 8);
        read_memory(1, 0, 1, 16);
        read_memory(1, 0, 1, 4);

        $finish();
    end

endmodule : ddr_controller_tb