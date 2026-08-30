`ifndef LOAD_CONTROLLER_SV
    `define LOAD_CONTROLLER_SV

module load_controller #(
    /* Cache block words */
    parameter BLOCK_WIDTH = 8,

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
    output logic busy_o,

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
//      ADDRESS SUPPLY STAGE
//====================================================================================

    typedef enum logic [2:0] {IDLE, WRITE_BACK, REFILL_REQ, REFILL} fsm_states_t;

    fsm_states_t state_CRT, state_NXT;

    /* One S0 request can already be accepted when the older S1 lookup is
     * discovered to be a miss. Preserve it until refill completion. */
    logic replay_valid;
    logic [31:0] replay_address;
    logic replay_valid_2;
    logic [31:0] replay_address_2;

    logic s0_request, s0_cache_issue;
    logic [31:0] s0_address;
    logic s1_hit, s1_miss;

    /* Cache outcome stage nets */
    logic [31:0] s1_address;
    logic s1_request;
    logic replay_push, replay_pop;


    /* Invalidate kills a queued request, if invalidation happends, take the
     * newest request */
    assign s0_request = invalidate_i ? request_i : (replay_valid | request_i);
    assign s0_address = (replay_valid & !invalidate_i) ? replay_address : address_i;
    assign replay_push = request_i & !invalidate_i &
                         ((state_CRT != IDLE) | s1_miss | lock_i |
                          lock_status_i | replay_valid);
    assign replay_pop = replay_valid & s0_cache_issue;

    /* When S1 returns an hit, S0 can pass its data to S1. When S1 returns a miss the pipeline and the
     * CPU load unit must not issue more request */
    assign s0_cache_issue = (state_CRT == IDLE) & s0_request &
                            !s1_miss & !lock_status_i;


    data_enable_t cache_read; logic [31:0] cache_read_address;

    assign cache_read_address = s0_address;
    assign cache_read = s0_cache_issue ? '1 : '0;


        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin
                s1_request <= 1'b0;
            end else if (s0_cache_issue) begin
                s1_request <= 1'b1;
            end else if (invalidate_i | ((state_CRT == IDLE) & s1_request)) begin
                /* Consume each S1 response exactly once. */
                s1_request <= 1'b0;
            end
        end

        always_ff @(posedge clk_i) begin
            if (s0_cache_issue) begin
                s1_address <= s0_address;
            end
        end

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin
                replay_valid <= 1'b0;
                replay_address <= '0;
                replay_valid_2 <= 1'b0;
                replay_address_2 <= '0;
            end else if (invalidate_i) begin
                replay_valid <= 1'b0;
                replay_valid_2 <= 1'b0;
            end else if (replay_pop) begin
                if (replay_valid_2) begin
                    replay_valid <= 1'b1;
                    replay_address <= replay_address_2;
                    replay_valid_2 <= replay_push;
                    if (replay_push) begin
                        replay_address_2 <= address_i;
                    end
                end else if (replay_push) begin
                    replay_valid <= 1'b1;
                    replay_address <= address_i;
                    replay_valid_2 <= 1'b0;
                end else begin
                    replay_valid <= 1'b0;
                    replay_valid_2 <= 1'b0;
                end
            end else if (replay_push) begin
                /* Save requests arriving while a miss or lock is serviced. */
                if (!replay_valid) begin
                    replay_valid <= 1'b1;
                    replay_address <= address_i;
                end else if (!replay_valid_2) begin
                    replay_valid_2 <= 1'b1;
                    replay_address_2 <= address_i;
                end
            end
        end


//====================================================================================
//      LOCK LOGIC
//====================================================================================

    logic wait_lock;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin
                wait_lock <= 1'b0;
            end else if (invalidate_i) begin
                wait_lock <= 1'b0;
            end else if (lock_i & request_i) begin
                wait_lock <= 1'b1;
            end else if (!lock_status_i) begin
                wait_lock <= 1'b0;
            end
        end

    
    assign lock_request_o = wait_lock & !lock_status_i;

        always_comb begin
            if (s1_miss | (state_CRT != IDLE)) begin
                lock_address_o = s1_address;
            end else if (s0_cache_issue) begin
                lock_address_o = s0_address;
            end else begin
                lock_address_o = s1_request ? s1_address : s0_address;
            end

            lock_index_o = lock_address_o[OFFSET + INDEX + 1:OFFSET + 2];
        end


//====================================================================================
//      CACHE OUTCOME STAGE
//====================================================================================

    logic cache_response_valid;

    assign s1_hit = (state_CRT == IDLE) & s1_request & cache_hit_i;
    assign s1_miss = (state_CRT == IDLE) & s1_request & !cache_hit_i & !invalidate_i;

    assign cache_response_valid = s1_hit & !invalidate_i;


        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin : state_register
            if (!rst_n_i) begin
                state_CRT <= IDLE;
            end else begin
                state_CRT <= state_NXT;
            end
        end : state_register


    logic [31:0] miss_address;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin
                miss_address <= '0;
            end else if (s1_miss) begin
                miss_address <= s1_address;
            end
        end


    logic [OFFSET:0] request_counter_CRT, request_counter_NXT;
    logic [OFFSET:0] response_counter_CRT, response_counter_NXT;
    logic miss_delivered_CRT, miss_delivered_NXT;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin
                request_counter_CRT <= '0;
                response_counter_CRT <= '0;
                miss_delivered_CRT <= 1'b0;
            end else begin
                request_counter_CRT <= request_counter_NXT;
                response_counter_CRT <= response_counter_NXT;
                miss_delivered_CRT <= miss_delivered_NXT;
            end
        end


    logic invalidate_pending;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                invalidate_pending <= 1'b0;
            end else begin 
                if (invalidate_i & (state_CRT != IDLE)) begin
                    invalidate_pending <= 1'b1;
                end

                if (state_CRT == IDLE) begin
                    invalidate_pending <= 1'b0;
                end
            end 
        end 


    typedef struct packed {
        logic [TAG - 1:0] tag; 
        logic [INDEX - 1:0] index; 
        logic [OFFSET - 1:0] offset; 
    } cache_address_t;

    cache_address_t cache_address; assign cache_address = miss_address[31:2];


    logic refill_data_valid, refill_complete;

        always_comb begin
            /* Default values */
            state_NXT = state_CRT;
            request_counter_NXT = request_counter_CRT;
            response_counter_NXT = response_counter_CRT;
            miss_delivered_NXT = miss_delivered_CRT;

            refill_data_valid = 1'b0;
            refill_complete = 1'b0;

            cache_write_o = '0;
            cache_read_o = cache_read;
            cache_address_o = cache_read_address;
            cache_data_o = '0;
            cache_status_o = '0;

            load_channel.request = 1'b0;
            load_channel.address = '0;
            store_channel.request = 1'b0;
            store_channel.address = '0;

            case (state_CRT)
                /* Pipeline hits continue without entering the refill FSM. */
                IDLE: begin
                    request_counter_NXT = '0;
                    response_counter_NXT = '0;
                    miss_delivered_NXT = 1'b0;

                    if (s1_miss) begin
                        state_NXT = cache_dirty_i ? WRITE_BACK : REFILL_REQ;

                        if (!stall_i) begin
                            if (cache_dirty_i) begin
                                /* Read only data */
                                cache_read_o.data = !stall_i;

                                /* Start from block base */
                                if (!invalidate_i) begin
                                    cache_address_o = {s1_address[31:OFFSET + 2], {OFFSET{1'b0}}, 2'b00};
                                end

                                /* Increment word counter */
                                request_counter_NXT = 'd1;
                            end else begin
                                /* With no ready signal, word zero is accepted only
                                 * when neither the DDR owner nor D-cache conflicts. */
                                load_channel.request = 1'b1;
                                load_channel.address = {s1_address[31:OFFSET + 2], {OFFSET{1'b0}}, 2'b00};

                                request_counter_NXT = 'd1;
                            end
                        end
                    end
                end

                /* The controller reads a block slice every cycle,
                 * starting from the base of the block. Once the
                 * entire block is transferred, the controller requests 
                 * a load from the new address */
                WRITE_BACK: begin
                    if (request_counter_CRT == '0) begin
                        /* A dirty miss may enter WRITE_BACK while the data
                         * port is stalled.  Start the victim read only when
                         * the port becomes available. */
                        cache_read_o.data = !stall_i;
                        cache_address_o = {cache_tag_i, cache_address.index, {OFFSET{1'b0}}, 2'b00};

                        if (!stall_i) begin
                            request_counter_NXT = 'd1;
                        end
                    end else if (!request_counter_CRT[OFFSET] & request_counter_CRT[OFFSET - 1:0] != '0) begin
                        /* Read only data sequentially */
                        cache_read_o.data = !stall_i;
                        cache_address_o = {cache_tag_i, cache_address.index, request_counter_CRT[OFFSET - 1:0], 2'b0}; 

                        /* Increment word counter */
                        if (!stall_i) begin
                            request_counter_NXT = request_counter_CRT + 1'b1;
                        end

                        /* Request a store to memory controller */
                        store_channel.request = !stall_i;
                    end else if (request_counter_CRT[OFFSET] & request_counter_CRT[OFFSET - 1:0] == '0) begin
                        /* Send store request for the last data. Don't read
                         * any more words after writing back all the block */
                        store_channel.request = !stall_i;

                        if (!stall_i) begin
                            state_NXT = (invalidate_i | invalidate_pending) ? IDLE : REFILL_REQ;
                        end
                        
                        load_channel.request = !(invalidate_i | invalidate_pending) & !stall_i;
                        load_channel.address = {cache_address.tag, cache_address.index, request_counter_NXT[OFFSET - 1:1], 3'b0};
                        
                        /* Reset the word counter only after the final victim
                         * word has actually been accepted. */
                        if (!stall_i) begin
                            request_counter_NXT = 'd1;
                        end
                    end

                    store_channel.address = {cache_tag_i, cache_address.index, request_counter_CRT[OFFSET - 1:0] - 1'b1, 2'b0}; 
                end 

                /* Send a series of load requests to the memory to allocate
                 * the new data block, incrementing the address sequentially */
                REFILL_REQ: begin
                    /* A D-cache refill has priority at the shared DDR port */
                    if (!request_counter_CRT[OFFSET] & !stall_i) begin
                        /* Request until the counter overflows */
                        load_channel.request = 1'b1;
                        request_counter_NXT = request_counter_CRT + 1'b1;
                    end

                    load_channel.address = {cache_address.tag, cache_address.index, request_counter_CRT[OFFSET - 1:0], 2'b00};

                    if (request_counter_NXT[OFFSET]) begin
                        state_NXT = REFILL;
                    end

                    /* No request has reached DDR yet, so this miss can be
                     * abandoned immediately. Started bursts must be drained. */
                    if (invalidate_i & (request_counter_CRT == '0)) begin
                        state_NXT = IDLE;

                        load_channel.request = 1'b0;
                    end
                end

                /* When the memory interface has data ready, write to *
                 * cache. Allocation has priority over simple stores  *
                 * so it happens in 1 clock cycle until the block is  *
                 * completely filled.                                 */
                REFILL: begin
                    if (load_channel.valid) begin
                        /* Increment word counter */
                        response_counter_NXT = response_counter_CRT + 1'b1; 

                        cache_data_o = load_channel.data; 
                        cache_write_o.data = !invalidate_i & !invalidate_pending;

                        if (response_counter_CRT[OFFSET - 1:0] == '0) begin
                            /* The first time allocate metadata */
                            cache_write_o = (invalidate_i | invalidate_pending) ? '0 : '1;
                        end else if (response_counter_CRT[OFFSET - 1:0] == '1) begin
                            /* Block has been allocated */
                            state_NXT = IDLE; 
                        end 
                    end 

                    /* Early return */
                    if (load_channel.valid & !response_counter_CRT[OFFSET]) begin
                        response_counter_NXT = response_counter_CRT + 1'b1;

                        if (!miss_delivered_CRT & (miss_address[OFFSET + 1:2] == response_counter_CRT[OFFSET - 1:0])) begin
                            /* Forward the instruction once it's the requested one */
                            refill_data_valid = !invalidate_i & !invalidate_pending;
                            miss_delivered_NXT = !invalidate_i & !invalidate_pending;
                        end
                    end

                    cache_address_o = {cache_address.tag, cache_address.index, response_counter_CRT[OFFSET - 1:0], 2'b0};

                    /* Set status to valid and clean */
                    cache_status_o.valid = 1'b1;
                    cache_status_o.dirty = 1'b0; 
                end

                default: begin
                    state_NXT = IDLE;
                end
            endcase
        end

    assign load_channel.invalidate = invalidate_i;

    assign store_channel.data = cache_data_i; 
    assign store_channel.width = WORD;


//====================================================================================
//      DATA MULTIPLEXING
//====================================================================================

    always_comb begin
        if (refill_data_valid) begin
            /* Instruction from miss logic has maximum priority */
            data_o = load_channel.data;
        end else begin
            data_o = cache_data_i;
        end
    end

    assign valid_o = !invalidate_i & (cache_response_valid | refill_data_valid);
    assign busy_o = (state_CRT != IDLE) | s1_request | replay_valid | replay_valid_2 | s0_cache_issue;

//====================================================================================
//      PROTOCOL ASSERTIONS
//====================================================================================

    `ifdef SV_ASSERTION
        initial begin
            assert (BLOCK_WIDTH >= 2);
            assert ((1 << OFFSET) == BLOCK_WIDTH);
            assert ((TAG + INDEX + OFFSET + 2) == 32);
        end

        /* Every cache response has exactly one saved S1 request, and every
         * read command captures the address on the same edge. */
        assert property (@(posedge clk_i) disable iff (!rst_n_i)
            s1_request |-> $past(cache_read_o.valid));

        assert property (@(posedge clk_i) disable iff (!rst_n_i)
            cache_read_o.valid |=>
            s1_request & (s1_address == $past(cache_read_address)));

        assert property (@(posedge clk_i) disable iff (!rst_n_i)
            valid_o |-> !invalidate_i);

        assert property (@(posedge clk_i) disable iff (!rst_n_i)
            $onehot0({cache_response_valid, refill_data_valid}));

        /* Allocation and eviction writes are only allowed while consuming a
         * response, and an invalidated refill must never make cache state
         * visible. */
        assert property (@(posedge clk_i) disable iff (!rst_n_i)
            cache_write_o != '0 |->
            ((state_CRT == REFILL) & load_channel.valid &
             !invalidate_i & !invalidate_pending));

        /* A refill request walks every word exactly once. */
        assert property (@(posedge clk_i) disable iff (!rst_n_i)
            (state_CRT == REFILL_REQ) & !stall_i &
            !request_counter_CRT[OFFSET] |-> load_channel.request);

        assert property (@(posedge clk_i) disable iff (!rst_n_i)
            (state_CRT == REFILL_REQ) & !stall_i &
            !request_counter_CRT[OFFSET] |->
            (load_channel.address ==
             {cache_address.tag, cache_address.index,
              request_counter_CRT[OFFSET - 1:0], 2'b00}));

        assert property (@(posedge clk_i) disable iff (!rst_n_i)
            (state_CRT == WRITE_BACK) & !stall_i & store_channel.request |->
            (store_channel.address ==
             {cache_tag_i, cache_address.index,
              request_counter_CRT[OFFSET - 1:0] - 1'b1, 2'b00}));

        assert property (@(posedge clk_i) disable iff (!rst_n_i)
            (state_CRT == REFILL) & load_channel.valid &
            !response_counter_CRT[OFFSET] |=>
            (response_counter_CRT == $past(response_counter_CRT) + 1'b1));

        /* Eviction and refill bursts must not advance while the shared cache
         * port is stalled. */
        assert property (@(posedge clk_i) disable iff (!rst_n_i)
            load_channel.request |-> !stall_i);

        assert property (@(posedge clk_i) disable iff (!rst_n_i)
            store_channel.request |-> !stall_i);

        /* The miss identity cannot change after allocation has started. */
        assert property (@(posedge clk_i) disable iff (!rst_n_i)
            ((state_CRT == REFILL_REQ) | (state_CRT == REFILL) |
             (state_CRT == WRITE_BACK)) &
            (($past(state_CRT) == REFILL_REQ) |
             ($past(state_CRT) == REFILL) |
             ($past(state_CRT) == WRITE_BACK)) |->
            $stable(miss_address));

        /* The second replay entry is meaningful only when the first exists;
         * a full queue can advance only by consuming its head. */
        assert property (@(posedge clk_i) disable iff (!rst_n_i)
            replay_valid_2 |-> replay_valid);

        assert property (@(posedge clk_i) disable iff (!rst_n_i)
            (replay_push & replay_valid & replay_valid_2) |-> replay_pop);


    `endif


endmodule : load_controller

`endif
