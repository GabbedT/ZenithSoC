`ifndef INSTRUCTION_CACHE_COMPLEX
    `define INSTRUCTION_CACHE_COMPLEX 

module instruction_cache_complex #(
    /* Total cache size in bytes */
    parameter CACHE_SIZE = 2 ** 14,

    /* Total block size in bytes */
    parameter BLOCK_SIZE = 32
) (
    input logic clk_i, 
    input logic rst_n_i,
    input logic stall_i,
    input logic region_switch_i,
    input logic conflict_i,

    input logic flush_i,
    output logic flush_busy_o,
    output logic flush_done_o,

    /* Fetch unit interface */
    fetch_interface.slave fetch_channel,

    /* Memory load interface */ 
    load_interface.master load_channel 
);

//====================================================================================
//      PARAMETERS
//====================================================================================

    /* Bits to select a word inside a cache block */
    localparam OFFSET = $clog2(BLOCK_SIZE / 4);

    /* Bits to select a cache block */
    localparam INDEX = $clog2(CACHE_SIZE / BLOCK_SIZE);

    localparam TAG = 32 - (2 + OFFSET + INDEX);

    /* Number of words in a cache block */
    localparam BLOCK_WORDS = BLOCK_SIZE / 4;


//====================================================================================
//      INSTRUCTION CACHE
//====================================================================================

    logic [31:0] cache_write_address, controller_cache_write_address, cache_read_address;
    logic [BLOCK_WORDS - 1:0][31:0] cache_write_instruction, cache_read_bundle;
    instruction_enable_t cache_write, controller_cache_write, cache_read;
    logic cache_hit, cache_write_valid;

    logic [INDEX - 1:0] flush_index;
    logic flush_active, flush_invalidating;
    logic controller_fetch_stall;

    assign flush_busy_o = flush_active | flush_i;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin
                flush_index <= '0;
                flush_active <= 1'b0;
                flush_invalidating <= 1'b0;
                flush_done_o <= 1'b0;
            end else begin
                flush_done_o <= 1'b0;

                if (flush_i & !flush_active) begin
                    /* Keep the CPU halted, but let an older I$ refill finish
                     * before taking over the cache write port. */
                    flush_active <= 1'b1;
                    flush_invalidating <= 1'b0;
                    flush_index <= '0;
                end else if (flush_active & !flush_invalidating) begin
                    if (!controller_fetch_stall) begin
                        flush_invalidating <= 1'b1;
                        flush_index <= '0;
                    end
                end else if (flush_invalidating) begin
                    if (&flush_index) begin
                        flush_index <= '0;
                        flush_active <= 1'b0;
                        flush_invalidating <= 1'b0;
                        flush_done_o <= 1'b1;
                    end else begin
                        flush_index <= flush_index + 1'b1;
                    end
                end
            end
        end

    always_comb begin
        cache_write_address = controller_cache_write_address;
        cache_write = controller_cache_write;
        cache_write_valid = 1'b1;

        if (flush_invalidating) begin
            cache_write_address = {{TAG{1'b0}}, flush_index, {OFFSET{1'b0}}, 2'b0};
            cache_write = '0;
            cache_write.valid = 1'b1;
            cache_write_valid = 1'b0;
        end
    end

    instruction_cache #(CACHE_SIZE, BLOCK_SIZE, TAG) icache (
        .clk_i ( clk_i ),

        .write_address_i ( cache_write_address     ),
        .write_i         ( cache_write             ),
        .instruction_i   ( cache_write_instruction ),
        .valid_i         ( cache_write_valid       ),

        .read_address_i  ( cache_read_address ),
        .read_i          ( cache_read         ),
        .instruction_o   ( cache_read_bundle  ),
        .hit_o           ( cache_hit          )
    );


//====================================================================================
//      FETCH CONTROLLER
//====================================================================================

    logic [BLOCK_WORDS - 1:0][31:0] bundle; logic valid_bundle, request_bundle;

    fetch_controller #(BLOCK_WORDS, OFFSET, TAG, INDEX) controller (
        .clk_i           ( clk_i           ),
        .rst_n_i         ( rst_n_i         ),
        .stall_i         ( stall_i | flush_invalidating ),
        .region_switch_i ( region_switch_i ),
        .conflict_i      ( conflict_i      ),

        .invalidate_i      ( fetch_channel.invalidate             ),
        .stall_fetch_o     ( controller_fetch_stall               ),
        .fetch_i           ( request_bundle & fetch_channel.fetch ),
        .program_counter_i ( fetch_channel.address                ),
        .instruction_o     ( bundle                               ),  
        .valid_o           ( valid_bundle                         ),

        .cache_write_address_o ( controller_cache_write_address ),
        .cache_write_o         ( controller_cache_write         ),
        .cache_instruction_o   ( cache_write_instruction ),

        .cache_read_address_o ( cache_read_address ),
        .cache_read_o         ( cache_read         ),
        .cache_instruction_i  ( cache_read_bundle  ),
        .cache_hit_i          ( cache_hit          ),

        .load_channel ( load_channel )
    );

    assign fetch_channel.stall = controller_fetch_stall | flush_busy_o;


//====================================================================================
//      SEQUENCER
//====================================================================================

    logic [31:0] previous_pc;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                previous_pc <= '0;
            end else if (fetch_channel.fetch) begin 
                previous_pc <= fetch_channel.address;
            end 
        end 


    /* Index of the instruction requested */
    logic [OFFSET - 1:0] block_index; 

    assign block_index = fetch_channel.address[OFFSET + 1:2];

    
    /* Shift register that holds instruction in sequence */
    logic [BLOCK_WORDS - 2:0][31:0] sequencer;

        always_ff @(posedge clk_i) begin
            if (fetch_channel.fetch) begin
                /* Shift right */
                sequencer <= {32'b0, sequencer[BLOCK_WORDS - 2:1]}; 
            end else if (valid_bundle) begin
                /* Shift until the next instruction requested */
                sequencer <= bundle[BLOCK_WORDS - 1:0] >> ((block_index + 1'b1) * 32);
            end
        end 


    /* Instruction count inside sequencer */
    logic [BLOCK_WORDS - 1:0] sequencer_size;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                sequencer_size <= '0;
            end else if (fetch_channel.invalidate | flush_busy_o) begin
                sequencer_size <= '0;
            end else if (fetch_channel.fetch) begin
                if (request_bundle) begin
                    /* Empty the sequencer if a new bundle is requested */
                    sequencer_size <= '0;
                end else begin
                    /* Decrement size */
                    sequencer_size <= sequencer_size - 1'b1;
                end
            end else if (valid_bundle) begin 
                sequencer_size <= (BLOCK_WORDS - 1) - block_index;
            end 
        end 

    assign fetch_channel.instruction = valid_bundle ? bundle[block_index] : sequencer[0];

    /* Request if the sequencer is empty or if there is a jump */
    assign request_bundle = (sequencer_size == '0) | (fetch_channel.address != (previous_pc + 'd4));

    assign fetch_channel.valid = !flush_busy_o
                               & ((!request_bundle & fetch_channel.fetch) | valid_bundle);

endmodule : instruction_cache_complex 

`endif
