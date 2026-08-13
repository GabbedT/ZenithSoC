`ifndef LOAD_CONTROLLER_SV
    `define LOAD_CONTROLLER_SV

module load_controller #(
    /* Cache block */
    parameter OFFSET = 2, 

    /* Cache tag size */
    parameter TAG = 16, 

    /* Cache index size */
    parameter INDEX = 12
) (
    input logic clk_i,
    input logic rst_n_i, 
    input logic stall_i, 

    /* Lock logic */
    input logic lock_status_i,
    input logic lock_i,
    output logic lock_request_o,
    output logic [31:0] lock_address_o,
    output logic [INDEX - 1:0] lock_index_o,

    /* Load unit interface */
    input logic invalidate_i,
    input logic request_i,
    input logic [31:0] address_i, 
    output logic [31:0] data_o,
    output logic valid_o,

    /* Memory load interface */ 
    load_interface.master load_channel,

    /* Memory store interface */
    store_interface.master store_channel,

    /* Cache hit */ 
    input logic cache_hit_i,

    /* Block tag */
    input logic [TAG - 1:0] cache_tag_i,

    /* Status bits of a block */
    input logic cache_dirty_i,
    output status_packet_t cache_status_o,

    /* Cache address shared between ports */
    output logic [31:0] cache_address_o,

    /* Data to cache */
    input logic [31:0] cache_data_i,
    output logic [31:0] cache_data_o,

    /* Enable operation on cache data */
    output data_enable_t cache_read_o,
    output data_enable_t cache_write_o
); 

//====================================================================================
//      LOAD QUEUE
//====================================================================================

    /* The CPU load FIFO has two entries. One address is active in this
     * controller and, while that address misses, one younger address can wait
     * here. Hits promote the next request directly without using the queue. */
    logic push_load, pop_load, valid_load;
    logic [31:0] queued_load_address;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin
                valid_load <= 1'b0;
                queued_load_address <= '0;
            end else if (invalidate_i) begin
                valid_load <= 1'b0;
            end else begin
                if (pop_load) begin
                    valid_load <= 1'b0;
                end

                if (push_load) begin
                    valid_load <= 1'b1;
                    queued_load_address <= address_i;
                end
            end
        end

//====================================================================================
//      DATAPATH
//====================================================================================

    typedef struct packed {
        logic [TAG - 1:0] tag; 
        logic [INDEX - 1:0] index; 
        logic [OFFSET - 1:0] offset; 
    } cache_address_t;

    data_word_t active_address_CRT, active_address_NXT;
    cache_address_t cache_address; assign cache_address = active_address_CRT[31:2];

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin
                active_address_CRT <= '0;
            end else if ((state_CRT == IDLE) | force_state | !stall_i |
                         ((state_CRT == ALLOCATE) & load_channel.valid)) begin
                active_address_CRT <= active_address_NXT;
            end
        end


    data_word_t requested_data_NXT, requested_data_CRT;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin
                requested_data_CRT <= '0;
            end else if (!stall_i | ((state_CRT == ALLOCATE) & load_channel.valid)) begin
                requested_data_CRT <= requested_data_NXT;
            end
        end


    logic [OFFSET:0] word_counter_CRT, word_counter_NXT;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin : counter
            if (!rst_n_i) begin
                word_counter_CRT <= '0;
            end else if (!stall_i | ((state_CRT == ALLOCATE) & load_channel.valid)) begin
                word_counter_CRT <= word_counter_NXT;
            end
        end : counter

//====================================================================================
//      LOAD PERFORMANCE (SIMULATION ONLY)
//====================================================================================

    logic [31:0] load_access_CRT, load_access_NXT, load_hit_CRT, load_hit_NXT;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin
                load_access_CRT <= '0;
                load_hit_CRT <= '0;
            end else if (!stall_i) begin
                load_access_CRT <= load_access_NXT;
                load_hit_CRT <= load_hit_NXT;
            end
        end

//====================================================================================
//      FSM LOGIC
//====================================================================================

    typedef enum logic [2:0] {IDLE, WAIT_LOCK, OUTCOME, ALLOCATION_REQ, ALLOCATE, WRITE_BACK} fsm_states_t;

    fsm_states_t state_CRT, state_NXT; logic force_state;

    /* A cache response is meaningful to the CPU only after this controller
     * issued a CPU lookup.  Data-only reads used by write-back must never
     * create or refresh this valid bit. */
    logic cpu_lookup_issue, cpu_lookup_consume, lookup_response_valid;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin
                lookup_response_valid <= 1'b0;
            end else if (invalidate_i) begin
                lookup_response_valid <= 1'b0;
            end else begin
                case ({cpu_lookup_issue, cpu_lookup_consume})
                    2'b10, 2'b11: lookup_response_valid <= 1'b1;
                    2'b01:        lookup_response_valid <= 1'b0;
                    default:      lookup_response_valid <= lookup_response_valid;
                endcase
            end
        end

        /* Separate the transaction address used by the lock arbiter from the
         * cache-port address, which walks refill/writeback words.  This output
         * is deliberately independent of lock_status_i. */
        always_comb begin
            lock_address_o = active_address_CRT;

            case (state_CRT)
                IDLE: lock_address_o = address_i;

                OUTCOME: begin
                    if (cache_hit_i & request_i) begin
                        lock_address_o = address_i;
                    end else if (cache_hit_i & valid_load) begin
                        lock_address_o = queued_load_address;
                    end
                end

                ALLOCATE: begin
                    if (load_channel.valid &
                        (word_counter_CRT[OFFSET - 1:0] == '1) &
                        !invalidate_pending) begin
                        if (valid_load) begin
                            lock_address_o = queued_load_address;
                        end else if (request_i & !valid_load) begin
                            lock_address_o = address_i;
                        end
                    end
                end

                default: lock_address_o = active_address_CRT;
            endcase
        end

        /* The lock arbiter only compares cache indices.  Compute that index
         * without the late cache-hit result; in cycles where the controller
         * requests a lock it is identical to lock_address_o's index. */
        always_comb begin
            lock_index_o = active_address_CRT[OFFSET + INDEX + 1:OFFSET + 2];

            case (state_CRT)
                IDLE: lock_index_o = address_i[OFFSET + INDEX + 1:OFFSET + 2];

                OUTCOME: begin
                    if (request_i) begin
                        lock_index_o = address_i[OFFSET + INDEX + 1:OFFSET + 2];
                    end else if (valid_load) begin
                        lock_index_o = queued_load_address[OFFSET + INDEX + 1:OFFSET + 2];
                    end
                end

                ALLOCATE: begin
                    if (load_channel.valid &
                        (word_counter_CRT[OFFSET - 1:0] == '1) &
                        !invalidate_pending) begin
                        if (valid_load) begin
                            lock_index_o = queued_load_address[OFFSET + INDEX + 1:OFFSET + 2];
                        end else if (request_i & !valid_load) begin
                            lock_index_o = address_i[OFFSET + INDEX + 1:OFFSET + 2];
                        end
                    end
                end

                default: lock_index_o = active_address_CRT[OFFSET + INDEX + 1:OFFSET + 2];
            endcase
        end

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin : state_register
            if (!rst_n_i) begin
                state_CRT <= IDLE;
            end else if ((state_CRT == IDLE) | force_state) begin
                state_CRT <= state_NXT;
            end else if (!stall_i | ((state_CRT == ALLOCATE) & load_channel.valid)) begin
                state_CRT <= state_NXT;
            end
        end : state_register

    assign load_channel.invalidate = 1'b0;


    logic invalidate_pending, invalidate_done;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                invalidate_pending <= 1'b0;
            end else begin 
                if (invalidate_i & (state_CRT != IDLE)) begin
                    invalidate_pending <= 1'b1;
                end

                if (invalidate_done) begin
                    invalidate_pending <= 1'b0;
                end
            end 
        end 

    assign invalidate_done = state_CRT == IDLE;

        always_comb begin
            /* Default values */
            requested_data_NXT = requested_data_CRT;
            active_address_NXT = active_address_CRT;
            word_counter_NXT = word_counter_CRT;
            state_NXT = state_CRT;

            load_access_NXT = load_access_CRT;
            load_hit_NXT = load_hit_CRT;

            load_channel.address = '0;
            load_channel.request = 1'b0; 
            store_channel.address = '0;
            store_channel.request = 1'b0;

            cache_write_o = '0;
            cache_read_o = '0;

            cache_data_o  = '0;
            cache_address_o = '0;
            cache_status_o = '0; 

            data_o = '0;
            valid_o = '0;

            lock_request_o = 1'b0;

            force_state = 1'b0;
            push_load = 1'b0;
            pop_load = 1'b0;
            cpu_lookup_issue = 1'b0;
            cpu_lookup_consume = 1'b0;

            /* Once a miss or lock wait has started, the second CPU load may
             * arrive in any later cycle, including during the refill itself.
             * Preserve it until the active request completes. */
            if (request_i & !invalidate_i & !valid_load &
                ((state_CRT == WAIT_LOCK) | (state_CRT == WRITE_BACK) |
                 (state_CRT == ALLOCATION_REQ) | (state_CRT == ALLOCATE))) begin
                push_load = 1'b1;
            end

            case (state_CRT)

                /* FSM waits for LDU request, and sends a cache read  *
                 * command as soon as the request arrives. Data,      *
                 * status bits and tag are requested from cache to    *
                 * determine if the read was an hit or a miss.        *
                 * If the other FSM is currently executing operations *
                 * on the same cache address there is a lock, wait    *
                 * until the lock is released                         */
                IDLE: begin
                    if (request_i) begin
                        active_address_NXT = address_i;
                    end

                    if (request_i & lock_i) begin
                        state_NXT = WAIT_LOCK;
                    end else if (request_i) begin
                        state_NXT = OUTCOME;
                        cpu_lookup_issue = 1'b1;

                        load_access_NXT = load_access_CRT + 1'b1;

                        /* Read cache */
                        cache_read_o = '1; 
                    end

                    cache_address_o = address_i;

                    /* Save address for later use */
                    word_counter_NXT = '0; 
                end

                /* Wait until the other FSM releases the lock, then  *
                 * read from cache                                   */
                WAIT_LOCK: begin
                    /* Exit lock state if an invalidate is requested */
                    force_state = invalidate_i;

                    if (invalidate_i) begin
                        state_NXT = IDLE;
                    end else if (!lock_status_i) begin
                        state_NXT = OUTCOME;
                        cpu_lookup_issue = 1'b1;

                        /* Request lock on that address to avoid other
                         * operations on the same block */
                        lock_request_o = 1'b1;

                        /* Read cache */
                        cache_read_o = '1;
                    end

                    cache_address_o = active_address_CRT;
                end

                /* Cache now has output the outcome of the previous    *
                 * request. On cache hit, the data is simply fowarded  *
                 * to the LDU. On cache miss, if the data is not       *
                 * dirty, a new block can be allocated, else the block *
                 * needs to be written back first.                     */
                OUTCOME: begin
                    if (lookup_response_valid & cache_hit_i) begin
                        cpu_lookup_consume = 1'b1;
                        state_NXT = IDLE;

                        load_hit_NXT = load_hit_CRT + 1'b1;

                        force_state = 1'b1;

                        data_o = cache_data_i;
                        valid_o = !invalidate_i;

                        /* Promote the oldest pending load before accepting a
                         * same-cycle younger request.  The load unit can
                         * retire the active hit and enqueue another load in
                         * the same cycle, so a full pending slot must support
                         * pop and push without reordering the two requests. */
                        if (valid_load) begin
                            /* A request captured while the active load waited
                             * on a store lock must also be promoted here. */
                            active_address_NXT = queued_load_address;
                            pop_load = 1'b1;
                            push_load = request_i & !invalidate_i;
                            lock_request_o = !lock_status_i;
                            cache_address_o = queued_load_address;

                            if (lock_status_i) begin
                                state_NXT = WAIT_LOCK;
                            end else begin
                                state_NXT = OUTCOME;
                                cpu_lookup_issue = 1'b1;
                                cache_read_o = '1;
                                load_access_NXT = load_access_CRT + 1'b1;
                            end
                        end else if (request_i & lock_i) begin
                            active_address_NXT = address_i;
                            state_NXT = WAIT_LOCK;
                        end else if (request_i) begin
                            active_address_NXT = address_i;
                            state_NXT = OUTCOME;
                            cpu_lookup_issue = 1'b1;

                            load_access_NXT = load_access_CRT + 1'b1;

                            /* Read cache */
                            cache_read_o = '1;
                        end

                        if (!valid_load) begin
                            cache_address_o = request_i ? address_i : active_address_CRT;
                        end
                    end else if (lookup_response_valid) begin
                        force_state = invalidate_i;

                        if (request_i & !invalidate_i) begin
                            /* The active miss must retain its address until the
                             * refill completes. Queue the one younger load. */
                            push_load = 1'b1;
                        end

                        /* A miss cannot be consumed until the DDR channel is
                         * available.  Until then, leave the synchronous cache
                         * response and its address association untouched. */
                        if (!stall_i | invalidate_i) begin
                            cpu_lookup_consume = 1'b1;

                            if (cache_dirty_i) begin
                                state_NXT = invalidate_i ? IDLE : WRITE_BACK;

                                /* Read only data */
                                cache_read_o.data = !stall_i;

                                /* Start from block base */
                                if (!invalidate_i) begin
                                    cache_address_o = {cache_tag_i, cache_address.index, word_counter_CRT[OFFSET - 1:0], 2'b0};
                                end

                                /* Increment word counter */
                                word_counter_NXT = 'd1;
                            end else begin
                                state_NXT = invalidate_i ? IDLE : ALLOCATION_REQ;

                                load_channel.request = !invalidate_i & !stall_i;
                                load_channel.address = {cache_address.tag, cache_address.index, word_counter_CRT[OFFSET - 1:0], 2'b0};

                                word_counter_NXT = 'd1;
                            end
                        end else begin
                            cache_address_o = active_address_CRT;
                        end
                    end
                end

                /* The controller reads a block slice every cycle,     * 
                 * starting from the base of the block. By pipelining  * 
                 * the read requests it can feed the memory controller *
                 * with a word every cycle. Once the entire block is   * 
                 * transferred, the controller requests a load from    *
                 * the new address                                     */
                WRITE_BACK: begin
                    if (!word_counter_CRT[OFFSET] & word_counter_CRT[OFFSET - 1:0] != '0) begin
                        /* Read only data sequentially */
                        cache_read_o.data = !stall_i;
                        cache_address_o = {cache_tag_i, cache_address.index, word_counter_CRT[OFFSET - 1:0], 2'b0}; 

                        /* Increment word counter */
                        word_counter_NXT = word_counter_CRT + 1'b1;

                        /* Request a store to memory controller */
                        store_channel.request = !stall_i;
                    end else if (word_counter_CRT[OFFSET] & word_counter_CRT[OFFSET - 1:0] == '0) begin
                        /* Send store request for the last data. Don't read
                         * any more words after writing back all the block */
                        store_channel.request = !invalidate_i & !invalidate_pending & !stall_i;

                        state_NXT = (invalidate_i | invalidate_pending) ? IDLE : ALLOCATION_REQ;
                        
                        load_channel.request = !(invalidate_i | invalidate_pending) & !stall_i;
                        load_channel.address = {cache_address.tag, cache_address.index, word_counter_NXT[OFFSET - 1:1], 3'b0};
                        
                        /* Reset word counter */ 
                        word_counter_NXT = 'd1;
                    end

                    store_channel.address = {cache_tag_i, cache_address.index, word_counter_CRT[OFFSET - 1:0] - 1'b1, 2'b0}; 
                end 


                /* Send a series of load requests to the memory to allocate 
                 * the new data block, increment the address sequentially.
                 * Once the requests have been issued, wait for a response,
                 * then allocate */
                ALLOCATION_REQ: begin
                    if (!word_counter_CRT[OFFSET] & word_counter_CRT[OFFSET - 1:0] != '0) begin
                        /* Increment word counter */
                        word_counter_NXT = word_counter_CRT + 1'b1;

                        /* Request a load to memory controller */
                        load_channel.request = !stall_i; 
                    end else begin
                        /* Wait for response */
                        state_NXT = ALLOCATE;
                        
                        /* Reset word counter */ 
                        word_counter_NXT = '0; 
                    end

                    /* Load address */
                    load_channel.address = {cache_address.tag, cache_address.index, word_counter_CRT[OFFSET - 1:0], 2'b0}; 
                end


                /* When the memory interface has data ready, write to *
                 * cache. Allocation has priority over simple stores  *
                 * so it happens in 1 clock cycle until the block is  *
                 * completely filled.                                 */
                ALLOCATE: begin
                    if (load_channel.valid) begin
                        /* Increment word counter */
                        word_counter_NXT = word_counter_CRT + 1'b1; 

                        cache_data_o = load_channel.data; 
                        cache_write_o.data = !invalidate_pending;

                        if (word_counter_CRT[OFFSET - 1:0] == '0) begin
                            /* The first time allocate metadata */
                            cache_write_o = invalidate_pending ? '0 : '1;
                        end else if (word_counter_CRT[OFFSET - 1:0] == '1) begin
                            /* Block has been allocated */
                            if ((valid_load | push_load) & !invalidate_pending) begin
                                /* If the younger request arrives on the last
                                 * refill beat, promote it directly rather than
                                 * enqueueing it for an IDLE state that would
                                 * otherwise never consume it. */
                                active_address_NXT = valid_load ? queued_load_address : address_i;
                                pop_load = valid_load;
                                /* Replace a consumed queued load with a
                                 * younger request accepted on this final
                                 * refill beat. */
                                push_load = valid_load ? (request_i & !invalidate_i) : 1'b0;

                                /* The cache write port still uses
                                 * cache_address_o for the final refill beat.
                                 * Defer the younger read to WAIT_LOCK so the
                                 * refill data cannot be written into the
                                 * younger request's bank. */
                                state_NXT = WAIT_LOCK;
                                lock_request_o = !lock_status_i;

                                load_access_NXT = load_access_CRT + 1'b1;

                                /* Save address for later use */
                                word_counter_NXT = '0; 
                            end else begin
                                state_NXT = IDLE; 
                            end
                            
                            /* If the requested data is the last word of the cache block, foward it immediately */
                            data_o = cache_address.offset == '1 ? load_channel.data : requested_data_CRT;
                            valid_o = !invalidate_pending; 
                        end 

                        if (cache_address.offset == word_counter_CRT) begin 
                            /* Save requested data to foward once the allocation
                             * is finished */
                            requested_data_NXT = load_channel.data;
                        end
                    end 

                    cache_address_o = {cache_address.tag, cache_address.index,
                                       word_counter_CRT[OFFSET - 1:0], 2'b0};

                    /* Set status to valid and clean */
                    cache_status_o.valid = 1'b1;
                    cache_status_o.dirty = 1'b0; 
                end
            endcase 
        end

    assign store_channel.data = cache_data_i; 
    assign store_channel.width = WORD;

    `ifdef SV_ASSERTION
        assert property (@(posedge clk_i) disable iff (!rst_n_i)
            !(push_load & valid_load & !pop_load));

        assert property (@(posedge clk_i) disable iff (!rst_n_i)
            (state_CRT != IDLE) |-> !$isunknown(active_address_CRT));

        assert property (@(posedge clk_i) disable iff (!rst_n_i)
            valid_o |-> ((state_CRT == OUTCOME) | (state_CRT == ALLOCATE)));

        assert property (@(posedge clk_i) disable iff (!rst_n_i)
            ((state_CRT == OUTCOME) & valid_o) |-> lookup_response_valid);

        assert property (@(posedge clk_i) disable iff (!rst_n_i)
            (state_CRT == WRITE_BACK) |-> !valid_o);

        assert property (@(posedge clk_i) disable iff (!rst_n_i)
            (state_CRT == WAIT_LOCK) |-> (lock_address_o == active_address_CRT));

        assert property (@(posedge clk_i) disable iff (!rst_n_i)
            lock_request_o |->
            (lock_index_o == lock_address_o[OFFSET + INDEX + 1:OFFSET + 2]));

        assert property (@(posedge clk_i) disable iff (!rst_n_i)
            (request_i & !invalidate_i & !valid_load &
             ((state_CRT == WAIT_LOCK) | (state_CRT == WRITE_BACK) |
              (state_CRT == ALLOCATION_REQ) | (state_CRT == ALLOCATE)))
            |-> (push_load | ((state_CRT == ALLOCATE) & load_channel.valid & valid_o)));
    `endif

endmodule : load_controller 

`endif
