`ifndef DATA_CACHE_COMPLEX_SV
    `define DATA_CACHE_COMPLEX_SV

module data_cache_complex #(
    /* Total cache size in bytes */
    parameter CACHE_SIZE = 2 ** 13,

    /* Total block size in bytes */
    parameter BLOCK_SIZE = 16
) (
    input logic clk_i,
    input logic rst_n_i,
    input logic stall_i,

    input logic flush_i,
    input logic flush_ready_i,
    output logic flush_busy_o,
    output logic flush_done_o,

    output logic single_trx_o,

    /* Load unit interface */
    load_interface.slave ldu_channel,

    /* Store unit interface */
    store_interface.slave stu_channel,

    /* DDR Memory load interface */
    load_interface.master ddr_load_channel,

    /* DDR  Memory store interface */
    store_interface.master ddr_store_channel,

    /* I/O Memory load interface */
    load_interface.master io_load_channel,

    /* I/O  Memory store interface */
    store_interface.master io_store_channel
);

//====================================================================================
//      PARAMETERS
//====================================================================================

    /* Bits to select a word inside a cache block */
    localparam OFFSET = $clog2(BLOCK_SIZE / 4);

    /* Bits to select a cache block */
    localparam INDEX = $clog2(CACHE_SIZE / BLOCK_SIZE);

    localparam TAG = 32 - (2 + OFFSET + INDEX);
    localparam BLOCK_WORDS = BLOCK_SIZE / 4;
    localparam FLUSH_STORE_RESPONSES = BLOCK_SIZE / 8;


    typedef struct packed {
        logic [TAG - 1:0] tag; 
        logic [INDEX - 1:0] index; 
        logic [OFFSET - 1:0] bank_select; 
        logic [1:0] byte_offset;
    } cache_address_t;
    

//====================================================================================
//      MEMORY REGION DECODING
//====================================================================================

    logic io_load_request, io_store_request;

    assign io_load_request = (ldu_channel.address >= `PRIVATE_REGION_START) & (ldu_channel.address <= `PRIVATE_REGION_END);

    assign io_store_request = (stu_channel.address >= `PRIVATE_REGION_START) & (stu_channel.address <= `PRIVATE_REGION_END);


    assign io_load_channel.address = ldu_channel.address;
    assign io_load_channel.request = ldu_channel.request & io_load_request;
    assign io_load_channel.invalidate = ldu_channel.invalidate;

    assign io_store_channel.address = stu_channel.address;
    assign io_store_channel.data = stu_channel.data;
    assign io_store_channel.width = stu_channel.width;
    assign io_store_channel.request = stu_channel.request & io_store_request;


    logic io_store, io_load;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                io_store <= 1'b0;
                io_load <= 1'b0;
            end else begin 
                if (stu_channel.request) begin
                    io_store <= io_store_request;
                end

                if (ldu_channel.request) begin
                    io_load <= io_load_request;
                end
            end 
        end 


//====================================================================================
//      CACHE
//====================================================================================

    /* R/W Port nets, the two controllers contend the same port on the write side */
    data_word_t [1:0] cache_address; 
    data_enable_t cache_store; 
    data_word_t cache_store_data; logic [3:0] cache_byte_write;
    status_packet_t cache_store_status;

    /* R Port nets */
    data_word_t cache_load_data; logic [TAG - 1:0] cache_load_tag;

    /* Shared nets */
    data_enable_t [1:0] cache_load; logic [1:0] cache_hit, cache_valid, cache_dirty;

    data_cache #(CACHE_SIZE, BLOCK_SIZE, TAG) dcache (
        .clk_i ( clk_i ),

        .read_write_address_i ( cache_address[0]   ),
        .write_i              ( cache_store        ),
        .byte_write_i         ( cache_byte_write   ),
        .write_data_i         ( cache_store_data   ),
        .status_i             ( cache_store_status ),

        .read_address_i ( cache_address[1] ),
        .read_data_o    ( cache_load_data  ),
        .read_tag_o     ( cache_load_tag   ),

        .read_i  ( cache_load  ),
        .valid_o ( cache_valid ),
        .dirty_o ( cache_dirty ),
        .hit_o   ( cache_hit   )
    );


//====================================================================================
//      LOAD CONTROLLER
//====================================================================================

    status_packet_t lctrl_status_packet;
    logic [31:0] lctrl_store_data, lctrl_cache_address, lctrl_load_data; logic lctrl_valid_data, lctrl_stall, ld_lock, ld_lock_request;
    data_enable_t lctrl_cache_store, lctrl_cache_read;

    store_interface lctrl_store_channel(); assign lctrl_store_channel.done = ddr_store_channel.done;

    load_controller #(OFFSET, TAG, INDEX) load_cache_controller (
        .clk_i   ( clk_i                  ),
        .rst_n_i ( rst_n_i                ), 
        .stall_i ( stall_i | flush_busy_o ),

        .lock_i         ( ld_lock         ),
        .lock_status_i  ( lctrl_stall     ),
        .lock_request_o ( ld_lock_request ),

        .invalidate_i ( ldu_channel.invalidate                 ),
        .request_i    ( ldu_channel.request & !io_load_request ),
        .address_i    ( ldu_channel.address                    ), 
        .data_o       ( lctrl_load_data                        ),
        .valid_o      ( lctrl_valid_data                       ),

        .load_channel  ( ddr_load_channel    ),
        .store_channel ( lctrl_store_channel ),

        .cache_hit_i     ( cache_hit[1]        ),
        .cache_tag_i     ( cache_load_tag      ),
        .cache_dirty_i   ( cache_dirty[1]      ),
        .cache_status_o  ( lctrl_status_packet ),
        .cache_address_o ( lctrl_cache_address ),
        .cache_data_i    ( cache_load_data     ),
        .cache_data_o    ( lctrl_store_data    ),
        .cache_read_o    ( lctrl_cache_read    ),
        .cache_write_o   ( lctrl_cache_store   )
    ); 

    assign cache_address[1] = flush_busy_o ? flush_cache_address : lctrl_cache_address;
    assign cache_load[1] = flush_busy_o ? flush_cache_read : lctrl_cache_read;


//====================================================================================
//      STORE CONTROLLER
//====================================================================================

    logic sctrl_halt, sctrl_port_halt, sctrl_memory_halt, sctrl_store_done, sctrl_idle, sctrl_stall, st_lock, st_lock_request;
    status_packet_t sctrl_status_packet;
    data_word_t sctrl_store_data, sctrl_cache_address;
    data_enable_t sctrl_cache_store, sctrl_cache_read; logic [3:0] store_byte_write;

    store_interface sctrl_store_channel(); assign sctrl_store_channel.done = ddr_store_channel.done;

    assign sctrl_halt = sctrl_port_halt | sctrl_memory_halt;
    assign sctrl_memory_halt = flush_busy_o | lctrl_store_channel.request;

    store_controller store_cache_controller (
        .clk_i   ( clk_i       ),
        .rst_n_i ( rst_n_i     ), 
        .halt_i  ( sctrl_halt | flush_busy_o ),
        .stall_i ( stall_i | flush_busy_o     ),

        .lock_i         ( st_lock         ),
        .lock_status_i  ( sctrl_stall     ),
        .lock_request_o ( st_lock_request ),

        .request_i      ( stu_channel.request & !io_store_request                    ),
        .buffer_entry_i ( {stu_channel.data, stu_channel.address, stu_channel.width} ),
        .valid_o        ( sctrl_store_done                                           ),
        .idle_o         ( sctrl_idle                                                 ),

        .store_channel ( sctrl_store_channel ),

        .cache_address_o ( sctrl_cache_address ),
        .cache_data_o    ( sctrl_store_data    ),
        .cache_hit_i     ( cache_hit[0]        ),
        .cache_dirty_i   ( cache_dirty[0]      ),
        .cache_status_o  ( sctrl_status_packet ),
        .cache_read_o    ( sctrl_cache_read    ),
        .cache_write_o   ( sctrl_cache_store   ),
        .cache_byte_o    ( store_byte_write    )
    );

    assign cache_load[0] = flush_busy_o ? '0 : sctrl_cache_read;


//====================================================================================
//      FULL CACHE FLUSH
//====================================================================================

    typedef enum logic [2:0] {
        FLUSH_IDLE,
        FLUSH_WAIT_DRAIN,
        FLUSH_READ_META,
        FLUSH_CHECK_META,
        FLUSH_WRITEBACK,
        FLUSH_WAIT_STORE,
        FLUSH_INVALIDATE
    } flush_state_t;

    flush_state_t flush_state_CRT, flush_state_NXT;
    logic [INDEX - 1:0] flush_index_CRT, flush_index_NXT;
    logic [OFFSET - 1:0] flush_word_CRT, flush_word_NXT;
    logic [TAG - 1:0] flush_tag_CRT, flush_tag_NXT;
    logic [$clog2(FLUSH_STORE_RESPONSES + 1) - 1:0] flush_store_responses_CRT, flush_store_responses_NXT;

    data_enable_t flush_cache_read, flush_cache_write;
    status_packet_t flush_cache_status;
    logic [31:0] flush_cache_address, flush_store_address, flush_store_data;
    logic flush_store_request, flush_done_NXT;

    assign flush_busy_o = flush_state_CRT != FLUSH_IDLE;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin : flush_registers
            if (!rst_n_i) begin
                flush_state_CRT <= FLUSH_IDLE;
                flush_index_CRT <= '0;
                flush_word_CRT <= '0;
                flush_tag_CRT <= '0;
                flush_store_responses_CRT <= '0;
                flush_done_o <= 1'b0;
            end else begin
                flush_state_CRT <= flush_state_NXT;
                flush_index_CRT <= flush_index_NXT;
                flush_word_CRT <= flush_word_NXT;
                flush_tag_CRT <= flush_tag_NXT;
                flush_store_responses_CRT <= flush_store_responses_NXT;
                flush_done_o <= flush_done_NXT;
            end
        end : flush_registers

        always_comb begin : flush_control
            /* Default values */
            flush_state_NXT = flush_state_CRT;
            flush_index_NXT = flush_index_CRT;
            flush_word_NXT = flush_word_CRT;
            flush_tag_NXT = flush_tag_CRT;
            flush_store_responses_NXT = flush_store_responses_CRT;
            flush_done_NXT = 1'b0;

            flush_cache_read = '0;
            flush_cache_write = '0;
            flush_cache_status = '0;
            flush_cache_address = '0;

            flush_store_request = 1'b0;
            flush_store_address = '0;
            flush_store_data = cache_load_data;

            case (flush_state_CRT)
                FLUSH_IDLE: begin
                    if (flush_i) begin
                        /* Start from cache index 0 */
                        flush_index_NXT = '0;
                        flush_state_NXT = FLUSH_WAIT_DRAIN;
                    end
                end

                FLUSH_WAIT_DRAIN: begin
                    /* Older write-throughs and eviction writebacks can still
                     * be buffered below the cache controller. */
                    if (flush_ready_i & sctrl_idle) begin
                        flush_state_NXT = FLUSH_READ_META;
                    end
                end

                FLUSH_READ_META: begin
                    /* Read i-th cache line */
                    flush_cache_read = '1;
                    flush_cache_address = {{TAG{1'b0}}, flush_index_CRT, {OFFSET{1'b0}}, 2'b0};
                    flush_state_NXT = FLUSH_CHECK_META;
                end

                FLUSH_CHECK_META: begin
                    /* Word zero and its metadata are available from
                     * FLUSH_READ_META.  A dirty line must remain valid until
                     * all of its data has reached DDR. */
                    if (cache_valid[1] & cache_dirty[1]) begin
                        flush_tag_NXT = cache_load_tag;
                        flush_word_NXT = 'd1;
                        flush_state_NXT = FLUSH_WRITEBACK;

                        /* Request DDR store, for first word. Go to FLUSH_WRITEBACK
                         * flush other words */
                        flush_store_request = 1'b1;
                        flush_store_address = {cache_load_tag, flush_index_CRT, {OFFSET{1'b0}}, 2'b0};

                        flush_cache_read.data = 1'b1;
                        flush_cache_address = {cache_load_tag, flush_index_CRT, {{(OFFSET - 1){1'b0}}, 1'b1}, 2'b0};
                    end else begin
                        flush_state_NXT = FLUSH_INVALIDATE;
                    end
                end

                FLUSH_WRITEBACK: begin
                    /* Store in DDR each line wrod */
                    flush_store_request = 1'b1;
                    flush_store_address = {flush_tag_CRT, flush_index_CRT, flush_word_CRT, 2'b0};

                    if (flush_word_CRT == BLOCK_WORDS - 1) begin
                        flush_store_responses_NXT = '0;
                        flush_state_NXT = FLUSH_WAIT_STORE;
                    end else begin
                        flush_word_NXT = flush_word_CRT + 1'b1;

                        flush_cache_read.data = 1'b1;
                        flush_cache_address = {flush_tag_CRT, flush_index_CRT, flush_word_CRT + 1'b1, 2'b0};
                    end
                end

                FLUSH_WAIT_STORE: begin
                    if (ddr_store_channel.done) begin
                        if (flush_store_responses_CRT == FLUSH_STORE_RESPONSES - 1) begin
                            flush_state_NXT = FLUSH_INVALIDATE;
                        end else begin
                            flush_store_responses_NXT = flush_store_responses_CRT + 1'b1;
                        end
                    end
                end

                FLUSH_INVALIDATE: begin
                    /* Invalidate only after a dirty line's complete DDR
                     * writeback has been acknowledged. */
                    flush_cache_write.valid = 1'b1;
                    flush_cache_write.dirty = 1'b1;
                    flush_cache_status.valid = 1'b0;
                    flush_cache_status.dirty = 1'b0;
                    flush_cache_address = {flush_tag_CRT, flush_index_CRT, {OFFSET{1'b0}}, 2'b0};

                    if (flush_index_CRT == '1) begin
                        flush_state_NXT = FLUSH_IDLE;
                        flush_done_NXT = 1'b1;
                    end else begin
                        flush_index_NXT = flush_index_CRT + 1'b1;
                        flush_state_NXT = FLUSH_READ_META;
                    end
                end

                default: begin
                    flush_state_NXT = FLUSH_IDLE;
                end
            endcase
        end


//====================================================================================
//      ARBITER
//====================================================================================

        always_comb begin : arbiter
            /* Default values */ 
            sctrl_port_halt = 1'b0;
            cache_address[0] = '0;
            cache_store = '0; 
            cache_store_data = '0;
            cache_store_status = '0;
            cache_byte_write = '0;

            if (flush_busy_o) begin
                cache_store = flush_cache_write;
                cache_address[0] = flush_cache_address;
                cache_store_status = flush_cache_status;
                cache_byte_write = '0;
                sctrl_port_halt = 1'b1;
            end else begin
                case ({sctrl_cache_store != '0, lctrl_cache_store != '0})
                    2'b11, 2'b01: begin
                        cache_store = lctrl_cache_store;
                        cache_address[0] = lctrl_cache_address;
                        cache_store_data = lctrl_store_data;
                        cache_store_status = lctrl_status_packet;
                        cache_byte_write = '1;

                        sctrl_port_halt = 1'b1;
                    end

                    2'b00, 2'b10: begin
                        cache_store = sctrl_cache_store;
                        cache_address[0] = sctrl_cache_address;
                        cache_store_data = sctrl_store_data;
                        cache_store_status = sctrl_status_packet;
                        cache_byte_write = store_byte_write;

                        sctrl_port_halt = 1'b0;
                    end
                endcase
            end


            /* Default values */ 
            ddr_store_channel.data = '0;
            ddr_store_channel.address = '0;
            ddr_store_channel.width = WORD; 

            single_trx_o = 1'b0;

            if (flush_busy_o) begin
                ddr_store_channel.data = flush_store_data;
                ddr_store_channel.address = flush_store_address;
                ddr_store_channel.width = WORD;
            end else begin
                case ({sctrl_store_channel.request, lctrl_store_channel.request})
                    2'b11, 2'b01: begin
                        ddr_store_channel.data = lctrl_store_channel.data;
                        ddr_store_channel.address = lctrl_store_channel.address;
                        ddr_store_channel.width = lctrl_store_channel.width;  
                    end

                    2'b10: begin
                        ddr_store_channel.data = sctrl_store_channel.data;
                        ddr_store_channel.address = sctrl_store_channel.address;
                        ddr_store_channel.width = sctrl_store_channel.width;  

                        single_trx_o = 1'b1;
                    end
                endcase
            end
        end : arbiter

    assign ddr_store_channel.request = flush_busy_o
                                     ? flush_store_request
                                     : (sctrl_store_channel.request | lctrl_store_channel.request);


//====================================================================================
//      LOCK LOGIC
//====================================================================================

    /* Lock acquisition on the address to eliminate the possibility of out of order transactions on addresses
     * that maps on the same cache address. Example:
     *  1) Load to address A (Miss, request to main memory)
     *  2) Store to address B while data from A is being retrived (Hit, write on cache)
     *  3) Data from address A arrives and write on cache, but address B is now on the same cache line,
     *     so data from A overwrites data from B.
     *  4) Subsequent load to address B retrieves data from A instead of B.
     */
    logic ld_lock_acquired, st_lock_acquired; cache_address_t ld_lock_address, st_lock_address;

        always_ff @(posedge clk_i) begin
            if (!rst_n_i) begin
                ld_lock_acquired <= 1'b0;
                st_lock_acquired <= 1'b0;
                
                ld_lock_address <= '0;
                st_lock_address <= '0;
            end else begin
                /* A response normally releases the load lock. A back-to-back
                 * hit, or promotion of the queued load after a refill, hands
                 * ownership directly to the next address without an unlocked
                 * cycle. Requests queued behind a miss must not overwrite the
                 * active miss address. */
                if (ldu_channel.invalidate) begin
                    ld_lock_acquired <= 1'b0;
                end else if (ldu_channel.valid) begin
                    if (ld_lock_request) begin
                        ld_lock_acquired <= 1'b1;
                        ld_lock_address <= lctrl_cache_address;
                    end else if (ldu_channel.request & !io_load_request) begin
                        ld_lock_acquired <= 1'b1;
                        ld_lock_address <= ldu_channel.address;
                    end else begin
                        ld_lock_acquired <= 1'b0;
                    end
                end else if (ld_lock_request) begin
                    ld_lock_acquired <= 1'b1;
                    ld_lock_address <= lctrl_cache_address;
                end else if (!ld_lock_acquired & ldu_channel.request & !io_load_request) begin
                    ld_lock_acquired <= 1'b1;
                    ld_lock_address <= ldu_channel.address;
                end

                if (stu_channel.done) begin
                    if (stu_channel.request & !io_store_request & !st_lock) begin
                        st_lock_acquired <= 1'b1;
                        st_lock_address <= stu_channel.address;
                    end else begin
                        st_lock_acquired <= 1'b0;
                    end
                end else if (st_lock_request) begin
                    st_lock_acquired <= 1'b1;
                    st_lock_address <= sctrl_cache_address;
                end else if (!st_lock_acquired & stu_channel.request & !io_store_request & !st_lock) begin
                    st_lock_acquired <= 1'b1;
                    st_lock_address <= stu_channel.address;
                end
            end
        end


    /* Generate lock signals to hold one FSM when the other is using the cache on the same cache address */
    cache_address_t ldu_address_check, stu_address_check;
    cache_address_t lctrl_address_check, sctrl_address_check;

    assign ldu_address_check = ldu_channel.address;
    assign stu_address_check = stu_channel.address;
    assign lctrl_address_check = lctrl_cache_address;
    assign sctrl_address_check = sctrl_cache_address;
        
    assign ld_lock = (ldu_channel.request & !io_load_request)
                   & (ldu_address_check.index == st_lock_address.index)
                   & st_lock_acquired;

    /* Load requests have priority when both channels start a transaction in the
     * same cycle. Without this check both controllers can observe the same
     * direct-mapped line, allowing a later load refill to overwrite the store. */
    assign st_lock = (stu_channel.request & !io_store_request)
                    & (((stu_address_check.index == ld_lock_address.index) & ld_lock_acquired)
                    | ((stu_address_check.index == ldu_address_check.index)
                    & ldu_channel.request & !io_load_request));

    /* Controllers in WAIT_LOCK compare their captured request against the
     * current opposite owner. Combinational status naturally handles a
     * back-to-back lock hand-off to a different index. */
    assign lctrl_stall = st_lock_acquired
                       & (lctrl_address_check.index == st_lock_address.index);

    assign sctrl_stall = ld_lock_acquired
                       & (sctrl_address_check.index == ld_lock_address.index);

    `ifdef SV_ASSERTION
        assert property (@(posedge clk_i) disable iff (!rst_n_i)
            !(ld_lock_acquired & st_lock_acquired &
              (ld_lock_address.index == st_lock_address.index) &
              !lctrl_stall & !sctrl_stall));
    `endif


//====================================================================================
//      ROUTING LOGIC
//====================================================================================

        /* Route signals correctly */
        always_comb begin
            if (io_load) begin
                ldu_channel.valid = io_load_channel.valid;
                ldu_channel.data = io_load_channel.data;
            end else begin
                ldu_channel.valid = lctrl_valid_data;
                ldu_channel.data = lctrl_load_data;
            end

            if (io_store) begin
                stu_channel.done = io_store_channel.done;
            end else begin
                stu_channel.done = sctrl_store_done;
            end
        end

endmodule : data_cache_complex 

`endif
