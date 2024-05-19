module icache_tb;

    logic clk_i = 0; 
    logic rst_n_i = 0;
    logic stall_i = 0;

    /* Fetch unit interface */
    logic fetch_i = 0;
    logic invalidate_i = 0;
    logic [31:0] program_counter_i = 0;
    logic [31:0] instruction_o; 
    logic valid_o;

    /* Memory load interface */ 
    load_interface load_channel();

    always #5 clk_i <= !clk_i;


    localparam BLOCK_WIDTH = 32;
    localparam CACHE_SIZE = 2 ** 10;

    logic [31:0] memory [(CACHE_SIZE * 8) - 1:0];

    instruction_cache_complex #(CACHE_SIZE, BLOCK_WIDTH) icache (
        .clk_i   ( clk_i   ), 
        .rst_n_i ( rst_n_i ),
        .stall_i ( stall_i ),

        .fetch_i           ( fetch_i           ),
        .invalidate_i      ( invalidate_i      ),
        .program_counter_i ( program_counter_i ),
        .instruction_o     ( instruction_o     ),
        .valid_o           ( valid_o           ),

        .load_channel ( load_channel )
    ); 


    task fetch(input logic [31:0] address);
        $display("Starting fetch at address 0x%0h...", address);

        fetch_i <= 1'b1;
        program_counter_i <= address;
        @(negedge clk_i);

        if (valid_o) begin
            $display("Instruction 0x%h in the sequencer!\n", instruction_o);
            @(posedge clk_i); 

            return;
        end

        @(posedge clk_i); 
        fetch_i <= 1'b0;

        wait(valid_o | load_channel.request);
        @(negedge clk_i);

        if (valid_o) begin
            $display("Cache hit! Instruction 0x%h fetched!\n", instruction_o);
            @(posedge clk_i); 

            return;
        end else begin
            $display("Cache miss! Allocating a new block...");
            repeat(50) @(posedge clk_i);

            for (int i = 0; i < BLOCK_WIDTH / 4; ++i) begin
                load_channel.data <= memory[address[31:2] + i];
                load_channel.valid <= 1'b1;
                $display("Allocated instruction: 0x%0h", memory[address[31:2] + i]);

                @(posedge clk_i);
            end

            load_channel.valid <= 1'b0;
        end

        wait(valid_o);
        $display("Done, instruction: 0x%0h!\n", instruction_o);

        @(posedge clk_i); 
    endtask : fetch


    initial begin
        rst_n_i <= 1'b0;

        for (int i = 0; i < CACHE_SIZE * 8; ++i) begin
            memory[i] = i;    
        end

        @(posedge clk_i);
        rst_n_i <= 1'b1;

        /* Sequential fetch */
        for (int i = 0; i < 16; ++i) begin
            fetch(i * 4);
        end

        /* Fetch again, no misses */
        for (int i = 0; i < 16; ++i) begin
            fetch(i * 4);
        end

        /* Replace block */
        for (int i = 0; i < 16; ++i) begin
            fetch(CACHE_SIZE + (i * 4));
        end

        /* Fetch again, no misses */
        for (int i = 0; i < 16; ++i) begin
            fetch(CACHE_SIZE + (i * 4));
        end

        /* Sequential fetch */
        for (int i = 0; i < 16; ++i) begin
            fetch(i * 4);
        end

        /* Cache block misaligned access */
        fetch(8);
        fetch(12);
        
        $finish();
    end

endmodule : icache_tb 