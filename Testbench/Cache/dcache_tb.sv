module dcache_tb ();

    logic clk_i = 0;
    logic rst_n_i = 0;

    /* Load unit interface */
    load_interface ldu_channel();

    /* Store unit interface */
    store_interface stu_channel();

    /* DDR Memory load interface */
    load_interface ddr_load_channel();

    /* DDR  Memory store interface */
    store_interface ddr_store_channel();

    /* I/O Memory load interface */
    load_interface io_load_channel();

    /* I/O  Memory store interface */
    store_interface io_store_channel();


    always #5 clk_i <= !clk_i;


    localparam BLOCK_WIDTH = 16;
    localparam CACHE_SIZE = 2 ** 10;

    data_cache_complex #(CACHE_SIZE, BLOCK_WIDTH) dcache (
        .clk_i   ( clk_i   ),
        .rst_n_i ( rst_n_i ),

        .ldu_channel       ( ldu_channel       ),
        .stu_channel       ( stu_channel       ),
        .ddr_load_channel  ( ddr_load_channel  ),
        .ddr_store_channel ( ddr_store_channel ),
        .io_load_channel   ( io_load_channel   ),
        .io_store_channel  ( io_store_channel  )
    );

    logic [3:0][7:0] memory[(CACHE_SIZE * 8) - 1:0];
    
    task io_load(input int address);
        $display("Starting IO load at address 0x%0h...", address);

        ldu_channel.request <= 1'b1;
        ldu_channel.address <= address;
        @(posedge clk_i);
        ldu_channel.request <= 1'b0;

        repeat(10) @(posedge clk_i);

        io_load_channel.data <= $random();
        io_load_channel.valid <= 1'b1;
        @(posedge clk_i);
        io_load_channel.valid <= 1'b0;

        $display("IO load done: 0x%0h\n", ldu_channel.data);
    endtask : io_load


    task io_store(input int address, input int data);
        $display("Starting IO store of 0x%0h at address 0x%0h...", data, address);

        stu_channel.request <= 1'b1;
        stu_channel.address <= address;
        stu_channel.data <= data;
        @(posedge clk_i);
        stu_channel.request <= 1'b0;

        repeat(10) @(posedge clk_i);

        io_store_channel.done <= 1'b1;
        @(posedge clk_i);
        io_store_channel.done <= 1'b0;

        $display("IO store done!\n");
    endtask : io_store


    task memory_load(input logic [31:0] address);
        automatic int new_address = address + `USER_MEMORY_REGION_START;

        $display("Starting memory load at address 0x%0h...", new_address);

        ldu_channel.request <= 1'b1;
        ldu_channel.address <= new_address;
        @(posedge clk_i);
        ldu_channel.request <= 1'b0;
        @(negedge clk_i);

        if (ldu_channel.valid) begin
            $display("Cache hit! Memory load done: 0x%h\n", ldu_channel.data);
            @(posedge clk_i);

            return;
        end

        $display("Cache miss!");
        wait(ddr_load_channel.request | ddr_store_channel.request);
        @(negedge clk_i);

        if (ddr_store_channel.request) begin
            for (int i = 0; i < BLOCK_WIDTH / 4; ++i) begin
                automatic logic [31:0] mem_address = {1'b0, ddr_store_channel.address} - {1'b0, `USER_MEMORY_REGION_START};

                memory[mem_address[31:2]] <= ddr_store_channel.data;
                $display("Written back 0x%h in 0x%0h", ddr_store_channel.data, mem_address);

                @(posedge clk_i);
                ddr_store_channel.done <= 1'b1;
                #1;
            end

            ddr_store_channel.done <= 1'b0;
        end

        repeat(50) @(posedge clk_i);

        for (int i = 0; i < BLOCK_WIDTH / 4; ++i) begin
            ddr_load_channel.data <= memory[{address[31:$clog2(BLOCK_WIDTH / 4) + 2], 2'b0} + i];
            ddr_load_channel.valid <= 1'b1;
            @(posedge clk_i);
        end 

        ddr_load_channel.valid <= 1'b0;

        wait(ldu_channel.valid);

        $display("Memory load done: 0x%h\n", ldu_channel.data);
    endtask : memory_load


    task memory_store(input logic [31:0] address, input int data, input store_width_t width);
        automatic int new_address = address + `USER_MEMORY_REGION_START;

        $display("Starting memory store of 0x%h at address 0x%0h...", data, new_address);

        stu_channel.request <= 1'b1;
        stu_channel.address <= new_address;
        stu_channel.data <= data;
        stu_channel.width <= width;
        @(posedge clk_i);
        stu_channel.request <= 1'b0;
        @(negedge clk_i);

        if (stu_channel.done) begin
            $display("Cache hit! Memory store done!\n");
            @(posedge clk_i);

            return;
        end

        $display("Cache miss!");
        wait(ddr_store_channel.request);

        case (width)
            BYTE: memory[address[31:2]][address[1:0]] <= data[address[1:0]];

            HALF_WORD: begin
                memory[address[31:2]][{address[1], 1'b0}] <= data[{address[1], 1'b0}];
                memory[address[31:2]][{address[1], 1'b1}] <= data[{address[1], 1'b1}];
            end

            WORD: memory[address] <= data;
        endcase 

        repeat(50) @(posedge clk_i);
        ddr_store_channel.done <= 1'b1;
        @(posedge clk_i);
        ddr_store_channel.done <= 1'b0;

        wait(stu_channel.done);

        $display("Memory store done!\n");
    endtask : memory_store


    initial begin
        rst_n_i <= 1'b0;

        for (int i = 0; i < CACHE_SIZE * 8; ++i) begin
            memory[i] = i;
        end

        ldu_channel.invalidate <= 1'b0; 
        stu_channel.width <= WORD;

        repeat(50) @(posedge clk_i);

        rst_n_i <= 1'b1;

        /* Check IO bus */
        io_load(0);
        io_load(1);
        io_load(2);
        io_load(3);

        /* Check IO bus */
        io_store(0, 32'hDEADBEEF);
        io_store(1, 32'hBEAFDEAD);
        io_store(2, 32'hFFFFFFFF);
        io_store(3, 32'h00000000);

        /* Load data, it should be a cache miss 
         * thus a block will be allocated */
        memory_load(0);

        /* Check for hit */
        memory_load(0);
        memory_load(4);
        memory_load(8);
        memory_load(12);

        /* It's a miss */
        memory_load(16);

        /* Hit */
        memory_load(16);
        memory_load(20);
        memory_load(24);
        memory_load(28);

        /* Miss */
        memory_load(40);

        /* Hit */
        memory_load(32);
        memory_load(36);
        memory_load(40);
        memory_load(44); 

        for (int i = 0; i < 48; i += 4) begin
            /* Hit */
            memory_load(i); 
        end

        /* Test store hit */
        memory_store(0, '1, WORD);
        memory_load(0); /* 0xFFFFFFFF */
        memory_store(0, 0, BYTE);
        memory_store(3, 0, BYTE);
        memory_load(0); /* 0x00FFFF00 */
        memory_store(0, 0, HALF_WORD);
        memory_load(0); /* 0x00FF0000 */

        /* Miss, address 0 replaced and written back to memory */
        memory_load(CACHE_SIZE);

        /* Address 0 allocated again */
        memory_load(0);

        $display("============= ALLOCATING NEW AREA =============");
        for (int i = 0; i < 256; ++i) begin
            /* Allocate a new memory area */
            memory_load(1024 + (i * 4));
        end

        $display("============= MODIFYING NEW AREA =============");
        for (int i = 0; i < 256; ++i) begin
            /* Store all 1 */
            memory_store(1024 + (i * 4), '1, WORD);
        end

        $display("============= ALLOCATING NEW AREA =============");
        for (int i = 0; i < 256; ++i) begin
            /* Write back all data and allocate a new memory area */
            memory_load(i * 4);
        end

        $finish;
    end

endmodule : dcache_tb