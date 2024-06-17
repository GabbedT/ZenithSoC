`include "/home/gabbed/Projects/ZenithSoC/Hardware/CPU/ApogeoRV/Hardware/Include/Headers/apogeo_memory_map.svh"

`include "/home/gabbed/Projects/ZenithSoC/Hardware/CPU/ApogeoRV/Hardware/Include/Packages/Execution Unit/store_unit_pkg.sv"

module cache2ddr_tb ();

    /* Cache DDR Signals */
    logic clk_i = 1'b0;

    always #5ns clk_i <= !clk_i;

    
    logic sys_clk;
    logic mem_clk;
    logic locked;

    logic rst_n_i = 1'b0;

    system_clocking pll (
        .ext_clk_i ( clk_i   ),
        .sys_clk_o ( sys_clk ),
        .mem_clk_o ( mem_clk ), 
        .locked    ( locked  )  
    );

    /* Arbiter */
    logic hold_i = 1'b0;

    /* Memory interface */
    load_interface ddr_load_channel();
    store_interface ddr_store_channel();
    logic single_trx_i = 1'b0;

    /* Common address */
    logic [26:0] address_o; 
    
    /* Command interface */
    logic write_o; 
    logic read_o; 

    /* Data interface */
    logic push_o; 
    logic pull_o; 
    logic [63:0] write_data_o;
    logic [7:0] write_mask_o;
    logic [63:0] read_data_i;
    logic read_valid_i = 1'b0;

    /* Status */
    logic done_o;
    logic idle_o;
    logic ready_i = 1'b1;
    logic start_i;

    cache_ddr_interface #(4) ddr_interface (
        .clk_i   ( sys_clk ),
        .rst_n_i ( rst_n_i ),

        /* Arbiter */
        .hold_i ( hold_i ),

        /* Memory interface */
        .load_channel  ( ddr_load_channel  ),
        .store_channel ( ddr_store_channel ),
        .single_trx_i  ( single_trx_i      ),

        /* Common address */
        .address_o ( address_o ), 
        
        /* Command interface */
        .write_o ( write_o ),  
        .read_o  ( read_o  ), 

        /* Data interface */
        .push_o       ( push_o       ), 
        .pull_o       ( pull_o       ), 
        .write_data_o ( write_data_o ),
        .write_mask_o ( write_mask_o ),
        .read_data_i  ( read_data_i  ),
        .read_valid_i ( read_valid_i ),

        /* Status */
        .done_o  ( done_o  ),
        .idle_o  ( idle_o  ),
        .ready_i ( ready_i ),
        .start_i ( start_i )
    );


    /* I/O Memory load interface */
    load_interface io_load_channel();

    /* I/O  Memory store interface */
    store_interface io_store_channel();

    /* Load unit interface */
    load_interface ldu_channel();

    /* Store unit interface */
    store_interface stu_channel();

    localparam BLOCK_WIDTH = 16;
    localparam CACHE_SIZE = 2 ** 10;

    data_cache_complex #(CACHE_SIZE, BLOCK_WIDTH) dcache (
        .clk_i   ( sys_clk ),
        .rst_n_i ( rst_n_i ),

        .single_trx_o      ( single_trx_i      ),
        .ldu_channel       ( ldu_channel       ),
        .stu_channel       ( stu_channel       ),
        .ddr_load_channel  ( ddr_load_channel  ),
        .ddr_store_channel ( ddr_store_channel ),
        .io_load_channel   ( io_load_channel   ),
        .io_store_channel  ( io_store_channel  )
    );


    localparam WRITE = 3'b000;
    localparam READ = 3'b001;

    wire [12:0] ddr2_addr;
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

    ddr_memory_interface mem_intf (
        .clk_i       ( sys_clk  ),
        .rst_n_i     ( rst_n_i  ),
        .mem_clk_i   ( mem_clk  ),
        .mem_rst_n_i ( rst_n_i  ),

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
        
        .address_i    ( address_o    ), 
        .write_i      ( write_o      ), 
        .read_i       ( read_o       ), 
        .push_i       ( push_o       ),
        .pull_i       ( pull_o       ),
        .write_data_i ( write_data_o ),
        .write_mask_i ( write_mask_o ),
        .read_data_o  ( read_data_i  ),
        .read_valid_o ( read_valid_i ),

        .done_i  ( done_o  ),
        .ready_o ( ready_i ),
        .start_o ( start_i )
    );

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


    logic [3:0][7:0] memory[(CACHE_SIZE * 8) - 1:0];

    task memory_load(input logic [31:0] address);
        automatic int new_address = address + `USER_MEMORY_REGION_START;

        $display("[TESTBENCH] Starting memory load at address 0x%0h...", new_address);

        ldu_channel.request <= 1'b1;
        ldu_channel.address <= new_address;
        @(posedge sys_clk);
        ldu_channel.request <= 1'b0;
        @(negedge sys_clk);

        if (ldu_channel.valid) begin
            $display("[TESTBENCH] Cache hit! Memory load done: 0x%h\n", ldu_channel.data);
            @(posedge sys_clk);

            return;
        end

        $display("[TESTBENCH] Cache miss!");
        wait(ldu_channel.valid);
        @(posedge sys_clk);
        $display("[TESTBENCH] Memory load done: 0x%h\n", ldu_channel.data);
    endtask : memory_load


    task memory_store(input logic [31:0] address, input int data, input store_width_t width);
        automatic int new_address = address + `USER_MEMORY_REGION_START;

        $display("[TESTBENCH] Starting memory store of 0x%h at address 0x%0h...", data, new_address);

        single_trx_i <= 1'b1;
        stu_channel.request <= 1'b1;
        stu_channel.address <= new_address;
        stu_channel.data <= data;
        stu_channel.width <= width;
        @(posedge sys_clk);
        stu_channel.request <= 1'b0;
        single_trx_i <= 1'b0;
        @(negedge sys_clk);

        if (stu_channel.done) begin
            $display("[TESTBENCH] Cache hit! Memory store done!\n");
            @(posedge sys_clk);

            return;
        end

        $display("[TESTBENCH] Cache miss!");
        wait(stu_channel.done);
        @(posedge sys_clk);
        $display("[TESTBENCH] Memory store done!\n");
    endtask : memory_store



    initial begin
        rst_n_i <= 1'b0;

        ldu_channel.request <= 1'b0;
        stu_channel.request <= 1'b0;

        for (int i = 0; i < CACHE_SIZE * 8; ++i) begin
            memory[i] = i;
        end

        ldu_channel.invalidate <= 1'b0; 
        stu_channel.width <= WORD;

        repeat(200) @(posedge sys_clk);
        wait(locked);
        repeat(200) @(posedge sys_clk);

        rst_n_i <= 1'b1;

        wait(ready_i);

        for (int i = 0; i < 4096; i += 4) begin
            memory_store(i, i, WORD);
        end

        $display("[TESTBENCH] ============= ALLOCATING NEW AREA =============");
        for (int i = 0; i < 64; i += 4) begin
            memory_load(i);
        end

        $display("[TESTBENCH] ============= TEST CACHE HIT =============");
        for (int i = 0; i < 64; i += 4) begin
            memory_load(i);
        end

        $display("[TESTBENCH] ============= ALLOCATING NEW AREA =============");
        for (int i = 0; i < 64; ++i) begin
            /* Allocate a new memory area */
            memory_load(1024 + (i * 4));
        end

        $display("[TESTBENCH] ============= MODIFYING NEW AREA =============");
        for (int i = 0; i < 64; ++i) begin
            /* Store all 1 */
            memory_store(1024 + (i * 4), '1, WORD);
        end

        $display("[TESTBENCH] ============= ALLOCATING NEW AREA =============");
        for (int i = 0; i < 64; ++i) begin
            /* Write back all data and allocate a new memory area */
            memory_load(i * 4);
        end

        $display("[TESTBENCH] ============= ALLOCATING NEW AREA =============");
        for (int i = 0; i < 64; ++i) begin
            /* Allocate a new memory area */
            memory_load(1024 + (i * 4));
        end

        $finish;
    end

endmodule : cache2ddr_tb