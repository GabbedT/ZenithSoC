`ifndef FETCH_CONTROLLER
    `define FETCH_CONTROLLER

module fetch_controller #(
    /* Cache block words */
    parameter BLOCK_WIDTH = 8,

    /* Bits selecting a word inside a cache block */
    parameter OFFSET = $clog2(BLOCK_WIDTH),

    /* Cache index size */
    parameter INDEX = 12,

    /* Cache tag size */
    parameter TAG = 32 - (2 + OFFSET + INDEX),

    /* Pending fetch requests. Deeper than the frontend instruction buffer
     * so the registered full flag can never race a fetch into a drop:
     * a dropped push would desync the frontend address/instruction streams */
    parameter REQUEST_FIFO_DEPTH = 16,

    /* Maximum number of requests allowed ahead of the responses */
    parameter FETCH_AHEAD_LIMIT = 6
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
//      SAVED CACHE LINE
//====================================================================================

    /* Cache outcome stage nets */
    logic [31:0] s1_address;
    logic s1_request;

    /* Last delivered block kept to serve its remaining words in one cycle */
    logic [BLOCK_WIDTH - 1:0][31:0] retained_bundle, retain_bundle;
    logic [TAG + INDEX - 1:0] retained_block, retain_block;
    logic retained_valid, retain_valid, retain;
    logic retain_hit;

    logic cache_response_valid;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin
                retained_valid <= 1'b0;
            end else if (invalidate_i) begin
                retained_valid <= 1'b0;
            end else if (cache_response_valid) begin
                retained_valid <= 1'b1;
            end else if (retain) begin
                retained_valid <= retain_valid;
            end
        end

        always_ff @(posedge clk_i) begin
            if (cache_response_valid) begin
                retained_block <= s1_address[31:OFFSET + 2];
                retained_bundle <= cache_instruction_i;
            end else if (retain) begin
                retained_block <= retain_block;
                retained_bundle <= retain_bundle;
            end
        end


//====================================================================================
//      ADDRESS SUPPLY STAGE
//====================================================================================

    typedef enum logic [2:0] {IDLE, REFILL_REQ, REFILL, WRITE} fsm_states_t;

    fsm_states_t state_CRT, state_NXT;

    /* One S0 request can already be accepted when the older S1 lookup is
     * discovered to be a miss. Preserve it until refill completion. */
    logic replay_valid;
    logic [31:0] replay_address;

    logic s0_request, s0_cache_issue, s0_retained_response;
    logic [31:0] s0_address;
    logic s1_hit, s1_miss;

    /* Invalidate kills a queued request, if invalidation happends, take the
     * newest request */
    assign s0_request = invalidate_i ? fetch_i : (replay_valid | fetch_i);
    assign s0_address = (replay_valid & !invalidate_i) ? replay_address : program_counter_i;

    /* When S1 returns an hit, S0 can pass its data to S1. When S1 returns a miss the pipeline and the
     * CPU frontend must not issue more instructions */
    assign s0_cache_issue = (state_CRT == IDLE) & s0_request & !s1_miss & (invalidate_i | s1_request | !retain_hit);


    /* A request hit in the saved cache line */
    assign retain_hit = retained_valid & s0_request & (retained_block == s0_address[31:OFFSET + 2]);

    /* Valid signal of a retain hit after a miss */
    assign s0_retained_response = retain_hit & !s1_request & !invalidate_i;


    assign cache_read_address_o = s0_address;
    assign cache_read_o = s0_cache_issue ? '1 : '0;


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
            end else if (invalidate_i) begin
                replay_valid <= 1'b0;
            end else if (s1_miss & fetch_i) begin
                /* Save to not lose after miss is serviced */
                replay_valid <= 1'b1;
                replay_address <= program_counter_i;
            end else if (replay_valid & (s0_cache_issue | s0_retained_response)) begin
                /* Clear this the cycle after the miss is serviced */
                replay_valid <= 1'b0;
            end
        end


//====================================================================================
//      CACHE OUTCOME STAGE
//====================================================================================

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
    logic [BLOCK_WIDTH - 1:0][31:0] refill_bundle_CRT, refill_bundle_NXT;
    logic miss_delivered_CRT, miss_delivered_NXT;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin
                request_counter_CRT <= '0;
                response_counter_CRT <= '0;
                refill_bundle_CRT <= '0;
                miss_delivered_CRT <= 1'b0;
            end else begin
                request_counter_CRT <= request_counter_NXT;
                response_counter_CRT <= response_counter_NXT;
                refill_bundle_CRT <= refill_bundle_NXT;
                miss_delivered_CRT <= miss_delivered_NXT;
            end
        end


    logic invalidate_pending;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                invalidate_pending <= 1'b0;
            end else begin 
                if (invalidate_i & (state_CRT != IDLE)) begin
                    /* A ROM-to-DDR handoff is a region transition, not a
                     * stale I-cache refill: preserve the new line. */
                    invalidate_pending <= !region_switch_i;
                end

                if (state_CRT == IDLE) begin
                    invalidate_pending <= 1'b0;
                end
            end 
        end 


    logic refill_instruction_valid, refill_complete;

        always_comb begin
            /* Default values */
            state_NXT = state_CRT;
            request_counter_NXT = request_counter_CRT;
            response_counter_NXT = response_counter_CRT;
            refill_bundle_NXT = refill_bundle_CRT;
            miss_delivered_NXT = miss_delivered_CRT;

            stall_fetch_o = 1'b1;

            retain = 1'b0;
            retain_valid = 1'b0;
            retain_block = miss_address[31:OFFSET + 2];
            refill_instruction_valid = 1'b0;
            refill_complete = 1'b0;
            retain_bundle = refill_bundle_CRT;

            cache_write_o = '0;
            cache_write_address_o = '0;
            cache_instruction_o = refill_bundle_CRT;

            load_channel.request = 1'b0;
            load_channel.address = '0;

            case (state_CRT)
                /* Pipeline hits continue without entering the refill FSM. */
                IDLE: begin
                    stall_fetch_o = replay_valid | s1_miss;

                    request_counter_NXT = '0;
                    response_counter_NXT = '0;
                    refill_bundle_NXT = '0;
                    miss_delivered_NXT = 1'b0;

                    if (s1_miss) begin
                        state_NXT = REFILL_REQ;

                        /* With no ready signal, word zero is accepted only
                         * when neither the DDR owner nor D-cache conflicts. */
                        if (!stall_i & !conflict_i) begin
                            load_channel.request = 1'b1;
                            load_channel.address = {s1_address[31:OFFSET + 2], {OFFSET{1'b0}}, 2'b00};

                            request_counter_NXT = 'd1;
                        end
                    end
                end

                /* Send a series of load requests to the memory to allocate
                 * the new data block, incrementing the address sequentially */
                REFILL_REQ: begin
                    /* A D-cache refill has priority at the shared DDR port */
                    if (!request_counter_CRT[OFFSET] & !stall_i & !conflict_i) begin
                        /* Request until the counter overflows */
                        load_channel.request = 1'b1;
                        request_counter_NXT = request_counter_CRT + 1'b1;
                    end

                    load_channel.address = {miss_address[31:OFFSET + 2], request_counter_CRT[OFFSET - 1:0], 2'b00};

                    if (request_counter_NXT[OFFSET]) begin
                        state_NXT = REFILL;
                    end

                    /* No request has reached DDR yet, so this miss can be
                     * abandoned immediately. Started bursts must be drained. */
                    if (invalidate_i & (request_counter_CRT == '0)) begin
                        state_NXT = IDLE;

                        load_channel.request = 1'b0;
                    end

                    /* Capture responses in case they arrive during refill requests */
                    if (load_channel.valid & !response_counter_CRT[OFFSET]) begin
                        refill_bundle_NXT[response_counter_CRT[OFFSET - 1:0]] = load_channel.data;
                        response_counter_NXT = response_counter_CRT + 1'b1;

                        if (!miss_delivered_CRT & (miss_address[OFFSET + 1:2] == response_counter_CRT[OFFSET - 1:0])) begin
                            /* Forward the instruction once it's the requested one */
                            refill_instruction_valid = !invalidate_i & !invalidate_pending;
                            miss_delivered_NXT = !invalidate_i & !invalidate_pending;
                        end
                    end
                end

                REFILL: begin
                    if (load_channel.valid & !response_counter_CRT[OFFSET]) begin
                        refill_bundle_NXT[response_counter_CRT[OFFSET - 1:0]] = load_channel.data;
                        response_counter_NXT = response_counter_CRT + 1'b1;

                        if (!miss_delivered_CRT & (miss_address[OFFSET + 1:2] == response_counter_CRT[OFFSET - 1:0])) begin
                            /* Forward the instruction once it's the requested one */
                            refill_instruction_valid = !invalidate_i & !invalidate_pending;
                            miss_delivered_NXT = !invalidate_i & !invalidate_pending;
                        end
                    end

                    if (response_counter_NXT[OFFSET]) begin
                        refill_complete = 1'b1;
                        state_NXT = WRITE;

                        retain = 1'b1;
                        retain_valid = !invalidate_i & !invalidate_pending;
                        retain_block = miss_address[31:OFFSET + 2];
                        retain_bundle = refill_bundle_NXT;
                    end
                end

                /* Store the received block in the cache */
                WRITE: begin
                    state_NXT = IDLE;

                    if (!invalidate_i & !invalidate_pending) begin
                        cache_write_o = '1;
                        cache_write_address_o = {
                            miss_address[31:OFFSET + 2],
                            {(OFFSET + 2){1'b0}}
                        };
                    end
                end

                default: begin
                    state_NXT = IDLE;
                end
            endcase
        end

    assign load_channel.invalidate = invalidate_i;


//====================================================================================
//      INSTRUCTION MULTIPLEXING
//====================================================================================

    always_comb begin
        if (refill_instruction_valid) begin
            /* Instruction from miss logic has maximum priority */
            instruction_o = load_channel.data;
        end else if (cache_response_valid) begin
            /* Instruction from cache hit is older than first stage,
             * using saved address select from the bundle */
            instruction_o = cache_instruction_i[s1_address[OFFSET + 1:2]];
        end else begin
            /* Return from bundle */
            instruction_o = retained_bundle[s0_address[OFFSET + 1:2]];
        end
    end

    assign valid_o = !invalidate_i & (s0_retained_response | cache_response_valid | refill_instruction_valid);

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
            s1_request & (s1_address == $past(cache_read_address_o)));

        assert property (@(posedge clk_i) disable iff (!rst_n_i)
            valid_o |-> !invalidate_i);

        assert property (@(posedge clk_i) disable iff (!rst_n_i)
            $onehot0({s0_retained_response, cache_response_valid,
                      refill_instruction_valid}));

        assert property (@(posedge clk_i) disable iff (!rst_n_i)
            load_channel.request |-> (!stall_i & !conflict_i));

        assert property (@(posedge clk_i) disable iff (!rst_n_i)
            ((state_CRT == REFILL_REQ) | (state_CRT == REFILL)
             | (state_CRT == WRITE))
            & (($past(state_CRT) == REFILL_REQ)
               | ($past(state_CRT) == REFILL)
               | ($past(state_CRT) == WRITE))
            |-> $stable(miss_address));

        /* Once the first request wins the burst, the arbiter must preserve
         * ownership so the DDR bridge sees gap-free request pairs. */
        assert property (@(posedge clk_i) disable iff (!rst_n_i)
            (state_CRT == REFILL_REQ)
            & (request_counter_CRT != '0)
            & !request_counter_CRT[OFFSET]
            |-> load_channel.request);

        assert property (@(posedge clk_i) disable iff (!rst_n_i)
            retain & retain_valid |-> refill_complete);
    `endif


endmodule : fetch_controller

`endif
