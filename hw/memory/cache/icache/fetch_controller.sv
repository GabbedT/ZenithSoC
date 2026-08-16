`ifndef FETCH_CONTROLLER
    `define FETCH_CONTROLLER

module fetch_controller #(
    /* Cache block words */
    parameter BLOCK_WIDTH = 8,

    /* Cache block */
    parameter OFFSET = 2,

    /* Cache tag size */
    parameter TAG = 16,

    /* Cache index size */
    parameter INDEX = 12,

    /* Pending fetch requests. Deeper than the frontend instruction buffer
     * (8) so the registered full flag can never race a fetch into a drop:
     * a dropped push would desync the frontend address/instruction streams */
    parameter REQUEST_FIFO_DEPTH = 16
) (
    input logic clk_i,
    input logic rst_n_i,
    input logic stall_i,
    input logic region_switch_i,
    input logic conflict_i,

    /* Fetch unit interface */
    input logic invalidate_i,
    input logic fetch_i,
    input logic [31:0] program_counter_i,
    output logic [31:0] instruction_o,
    output logic valid_o,
    output logic request_fifo_full_o,
    output logic stall_fetch_o,

    /* Cache write interface */
    output data_word_t cache_write_address_o,
    output instruction_enable_t cache_write_o,
    output logic [BLOCK_WIDTH - 1:0][31:0] cache_instruction_o,

    /* Cache read interface */
    input logic [BLOCK_WIDTH - 1:0][31:0] cache_instruction_i,
    input logic cache_hit_i,
    output logic [31:0] cache_read_address_o,
    output instruction_enable_t cache_read_o,

    /* Memory load interface */
    load_interface.master load_channel
);

//====================================================================================
//      REQUEST FIFO
//====================================================================================

    /* THIS LOGIC NEEDS AN AD-HOC FIFO */

    /* Requests are queued so the frontend keeps fetching while a refill
     * is in flight */
    logic [REQUEST_FIFO_DEPTH - 1:0][31:0] request_buffer;
    logic [$clog2(REQUEST_FIFO_DEPTH) - 1:0] write_ptr, read_ptr;
    logic fifo_full, fifo_empty, push, pop;

    /* Number of queued requests (mod-16 subtraction of the ring pointers).
     * The frontend is allowed to run at most FETCH_AHEAD_LIMIT fetches ahead
     * of the serves: the CPU-side instruction buffer has 8 cells and pairs,
     * so more run-ahead would overwrite */
    localparam FETCH_AHEAD_LIMIT = 4'd6;

    logic [3:0] fifo_occupancy;
    assign fifo_occupancy = write_ptr - read_ptr;
    assign request_fifo_full_o = fifo_full;

    /* The fetch issued together with an invalidate is the redirect target:
     * it must be kept while the stale entries are cleared */
    assign push = fetch_i & (!fifo_full | invalidate_i);

        always_ff @(posedge clk_i) begin
            if (push) begin
                request_buffer[invalidate_i ? '0 : write_ptr] <= program_counter_i;
            end
        end

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin : request_pointers
            if (!rst_n_i) begin
                write_ptr <= '0;
                read_ptr <= '0;
            end else if (invalidate_i) begin
                write_ptr <= push ? 1'b1 : '0;
                read_ptr <= '0;
            end else begin
                if (push) begin
                    write_ptr <= write_ptr + 1'b1;
                end

                if (pop) begin
                    read_ptr <= read_ptr + 1'b1;
                end
            end
        end : request_pointers

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin : request_status
            if (!rst_n_i) begin
                fifo_full <= 1'b0;
                fifo_empty <= 1'b1;
            end else if (invalidate_i) begin
                fifo_full <= 1'b0;
                fifo_empty <= !push;
            end else begin
                case ({push, pop})
                    2'b01: begin
                        fifo_full <= 1'b0;
                        fifo_empty <= (write_ptr == read_ptr + 1'b1);
                    end

                    2'b10: begin
                        fifo_full <= (read_ptr == write_ptr + 1'b1);
                        fifo_empty <= 1'b0;
                    end
                endcase
            end
        end : request_status

    logic [31:0] head;

    assign head = request_buffer[read_ptr];


//====================================================================================
//      DATAPATH
//====================================================================================

    logic [BLOCK_WIDTH - 1:0][31:0] instruction_bundle;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (load_channel.valid) begin
                instruction_bundle <= {load_channel.data, instruction_bundle[BLOCK_WIDTH - 1:1]}; 
            end 
        end 


    logic [OFFSET:0] word_counter_CRT, word_counter_NXT;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin : counter
            if (!rst_n_i) begin
                word_counter_CRT <= '0;
            end else if (!stall_i) begin
                word_counter_CRT <= word_counter_NXT;
            end
        end : counter


    logic [TAG + INDEX - 1:0] refill_block;

    /* Block whose read is being validated in OUTCOME: an invalidate can
     * replace the head while the read is in flight, and the hit would
     * otherwise deliver the old line to the new head */
    logic [TAG + INDEX - 1:0] read_block;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin
                read_block <= '0;
            end else if (state_CRT == IDLE) begin
                read_block <= head[31:4];
            end
        end


        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin
                refill_block <= '0;
            end else if ((state_CRT == OUTCOME) & !cache_hit_i & !stall_i & !conflict_i & !(invalidate_i | invalidate_pending)) begin
                refill_block <= head[31:4];
            end
        end


    /* Last delivered block kept to serve its remaining words in one cycle */
    logic [BLOCK_WIDTH - 1:0][31:0] retained_bundle;
    logic [TAG + INDEX - 1:0] retained_block;
    logic retained_valid, retain;
    logic [TAG + INDEX - 1:0] retain_block;
    logic [BLOCK_WIDTH - 1:0][31:0] retain_bundle;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin
                retained_valid <= 1'b0;
                retained_block <= '0;
                retained_bundle <= '0;
            end else if (invalidate_i) begin
                retained_valid <= 1'b0;
            end else if (retain) begin
                retained_valid <= 1'b1;
                retained_block <= retain_block;
                retained_bundle <= retain_bundle;
            end
        end


    /* Speculative read of the next block issued while the current
     * one is consumed, so sequential block hits cost zero cycles */
    logic [31:0] pre_read_address;
    logic [BLOCK_WIDTH - 1:0][31:0] pre_read_bundle;
    logic pre_read_valid, pre_read_pending, pre_read_issue, pre_read_consume;
    logic [31:0] pre_read_target;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin
                pre_read_address <= '0;
                pre_read_bundle <= '0;
                pre_read_valid <= 1'b0;
                pre_read_pending <= 1'b0;
            end else if (invalidate_i | (state_CRT == WRITE)) begin
                /* A refill write can replace the line the speculative read
                 * returned: drop it rather than serve stale data */
                pre_read_valid <= 1'b0;
                pre_read_pending <= 1'b0;
            end else begin
                if (pre_read_consume) begin
                    pre_read_valid <= 1'b0;
                end

                if (pre_read_issue) begin
                    pre_read_address <= pre_read_target;
                    pre_read_pending <= 1'b1;
                end else if (pre_read_pending) begin
                    pre_read_bundle <= cache_instruction_i;
                    pre_read_valid <= cache_hit_i;
                    pre_read_pending <= 1'b0;
                end
            end
        end


//====================================================================================
//      LOAD PERFORMANCE (SIMULATION ONLY)
//====================================================================================

    logic [31:0] fetch_access_CRT, fetch_access_NXT, fetch_hit_CRT, fetch_hit_NXT;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin
                fetch_access_CRT <= '0;
                fetch_hit_CRT <= '0;
            end else begin
                fetch_access_CRT <= fetch_access_NXT;
                fetch_hit_CRT <= fetch_hit_NXT;
            end
        end


//====================================================================================
//      FSM LOGIC
//====================================================================================

    typedef enum logic [2:0] {IDLE, OUTCOME, REFILL_REQ, REFILL, WRITE} fsm_states_t;

    fsm_states_t state_CRT, state_NXT;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin : state_register
            if (!rst_n_i) begin
                state_CRT <= IDLE;
            end else if ((state_CRT == IDLE) | (cache_hit_i & (state_CRT == OUTCOME))) begin
                state_CRT <= state_NXT;
            end else if (!stall_i) begin
                state_CRT <= state_NXT;
            end
        end : state_register


    logic invalidate_pending, invalidate_done;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                invalidate_pending <= 1'b0;
            end else begin 
                if (invalidate_i & (state_CRT != IDLE)) begin
                    invalidate_pending <= !region_switch_i;
                end

                if (invalidate_done) begin
                    invalidate_pending <= 1'b0;
                end
            end 
        end 

    assign invalidate_done = state_CRT == IDLE;


    logic pre_read; 
    
    assign pre_read = (head[OFFSET + 1:2] >= 1'b1) & !pre_read_pending 
                    & ((retained_valid & (head[31:4] == retained_block)) 
                    | (pre_read_valid & (head == pre_read_address)));

        always_comb begin
            /* Default values */
            state_NXT = state_CRT;
            word_counter_NXT = word_counter_CRT;

            fetch_access_NXT = fetch_access_CRT;
            fetch_hit_NXT = fetch_hit_CRT;

            instruction_o = '0; 
            valid_o = 1'b0;
            stall_fetch_o = 1'b1;

            pop = 1'b0;
            retain = 1'b0;
            retain_block = '0;
            retain_bundle = '0;

            pre_read_issue = 1'b0;
            pre_read_consume = 1'b0;
            pre_read_target = '0;

            cache_read_o = '0;
            cache_write_o = '0;
            cache_read_address_o = '0;
            cache_write_address_o = '0;
            cache_instruction_o = '0;

            load_channel.request = 1'b0;
            load_channel.address = '0;

            case (state_CRT)

                /* A request is served either from the retained block, from
                 * the speculative read, or through a cache access */
                IDLE: begin
                    /* Stop accepting fetches once the queue is deep enough */
                    stall_fetch_o = (fifo_occupancy >= FETCH_AHEAD_LIMIT);

                    /* MUX the instruction to CPU */
                    if (retained_valid & (head[31:4] == retained_block)) begin
                        instruction_o = retained_bundle[head[OFFSET + 1:2]];
                    end else begin
                        instruction_o = pre_read_bundle[head[OFFSET + 1:2]];
                    end

                    /* Coming from request buffer */
                    cache_read_address_o = head;

                    /* If a request needs to be serviced */
                    if (!fifo_empty) begin
                        if (retained_valid & (head[31:4] == retained_block)) begin
                            /* Block address to be serviced is the address of the 
                             * saved block, just multiplex the instruction from the 
                             * block itself */
                            valid_o = !invalidate_i;

                            pop = valid_o;
                        end else if (pre_read_valid & (head == pre_read_address) & !pre_read_pending) begin
                            /* Read from prefetched block */
                            valid_o = !invalidate_i;

                            pop = valid_o;
                            pre_read_consume = valid_o;

                            /* Move prefetched block into retained block */
                            retain = valid_o;
                            retain_block = head[31:4];
                            retain_bundle = pre_read_bundle;
                        end else begin
                            /* Simple cache request */
                            cache_read_o = '1;

                            state_NXT = OUTCOME;
                        end

                        /* Read the next block ahead of time once the current
                         * one is consumed word by word */
                        if (pre_read) begin
                            pre_read_issue = 1'b1;
                            pre_read_target = (head & 32'hFFFF_FFF0) + 32'd16;

                            cache_read_o = '1;
                            cache_read_address_o = pre_read_target;
                        end
                    end
                end


                /* Cache outcome: the requested word is served on a hit,
                 * otherwise the block refill starts with word zero */
                OUTCOME: begin
                    if (cache_hit_i & (head[31:4] == read_block)) begin
                        state_NXT = IDLE;

                        valid_o = !invalidate_i & !invalidate_pending & !fifo_empty;
                        instruction_o = cache_instruction_i[head[OFFSET + 1:2]];

                        /* Write bundle into retain block */
                        pop = valid_o;
                        retain = valid_o;
                        retain_block = head[31:4];
                        retain_bundle = cache_instruction_i;
                    end else if (cache_hit_i) begin
                        /* The head changed while the read was in flight:
                         * re-serve it from the new address */
                        state_NXT = IDLE;
                    end else begin
                        if (!stall_i & !conflict_i) begin
                            state_NXT = (invalidate_i | invalidate_pending | fifo_empty) ? IDLE : REFILL_REQ;
                        end

                        word_counter_NXT = 'd1;

                        load_channel.request = !stall_i & !(invalidate_i | invalidate_pending) & !conflict_i & !fifo_empty;
                        load_channel.address = {head[31:4], 2'b00, 2'b00};
                    end
                end


                /* Send a series of load requests to the memory to allocate
                 * the new data block, incrementing the address sequentially */
                REFILL_REQ: begin
                    /* A D-cache refill has priority at the shared DDR port.
                     * Hold both the request and word counter while that
                     * conflict exists */
                    if (!stall_i & !conflict_i) begin
                        if (!word_counter_CRT[OFFSET] & word_counter_CRT[OFFSET - 1:0] != '0) begin
                            /* Increment word counter */
                            word_counter_NXT = word_counter_CRT + 1'b1;

                            /* Request a load to memory controller */
                            load_channel.request = 1'b1;
                        end else if (word_counter_CRT[OFFSET]) begin
                            /* Wait for response */
                            state_NXT = REFILL;

                            /* Reset word counter */
                            word_counter_NXT = '0;
                        end
                    end

                    /* Load address */
                    load_channel.address = {refill_block, word_counter_CRT[OFFSET - 1:0], 2'b00};
                end


                /* Data words stream back from memory: each beat is delivered
                 * directly if it matches the next queued request, otherwise it
                 * is collected in the bundle. The whole block is stored in the
                 * cache at the end so later requests hit */
                REFILL: begin
                    if (load_channel.valid) begin
                        /* Increment word counter */
                        word_counter_NXT = word_counter_CRT + 1'b1;
                    end

                    if (word_counter_CRT[OFFSET] & word_counter_CRT[OFFSET - 1:0] == '0) begin
                        /* The whole block has been received */
                        state_NXT = WRITE;

                        /* Reset word counter */
                        word_counter_NXT = '0;
                    end

                    /* Deliver early if the incoming word matches the next
                     * request or was already received in the bundle */
                    if (!fifo_empty & !(invalidate_i | invalidate_pending) & (head[31:4] == refill_block)) begin
                        if ((head[OFFSET + 1:2] == word_counter_CRT[OFFSET - 1:0]) & load_channel.valid) begin
                            valid_o = 1'b1;
                            instruction_o = load_channel.data;

                            pop = 1'b1;
                        end else if (head[OFFSET + 1:2] < word_counter_CRT[OFFSET - 1:0]) begin
                            /* Beats fill the bundle newest-first: with word_counter
                             * beats already received, word j sits at slot
                             * j + BLOCK_WIDTH - word_counter */
                            valid_o = 1'b1;
                            instruction_o = instruction_bundle[head[OFFSET + 1:2] + BLOCK_WIDTH - word_counter_CRT[OFFSET - 1:0]];

                            pop = 1'b1;
                        end
                    end
                end


                /* Store the received block in the cache */
                WRITE: begin
                    state_NXT = IDLE;

                    if (!(invalidate_i | invalidate_pending)) begin
                        cache_write_o = '1;
                        cache_write_address_o = {refill_block, 2'b00, 2'b00};
                        cache_instruction_o = instruction_bundle;

                        retain = 1'b1;
                        retain_block = refill_block;
                        retain_bundle = instruction_bundle;
                    end
                end
            endcase

            if (pop) begin
                fetch_access_NXT = fetch_access_CRT + 1'b1;
            end

            if (valid_o & !((state_CRT == REFILL))) begin
                fetch_hit_NXT = fetch_hit_CRT + 1'b1;
            end
        end

    assign load_channel.invalidate = invalidate_i;

endmodule : fetch_controller

`endif
