`ifndef STORE_CONTROLLER_SV
    `define STORE_CONTROLLER_SV 

module store_controller (
    input logic clk_i,
    input logic rst_n_i, 
    input logic halt_i,
    input logic stall_i,

    /* Lock logic */
    input logic lock_status_i,
    input logic lock_i,
    output logic lock_request_o,

    /* Store unit interface */
    input logic request_i,
    input store_buffer_entry_t buffer_entry_i,
    output logic valid_o,

    /* Memory store interface */ 
    store_interface.master store_channel,

    /* Cache hit */ 
    input logic cache_hit_i,

    /* Store data */
    output data_word_t cache_address_o,
    output data_word_t cache_data_o,

    /* Status bits of a block */
    input logic cache_dirty_i,
    output status_packet_t cache_status_o,

    /* Enable operation on cache data */
    output data_enable_t cache_read_o,
    output data_enable_t cache_write_o,
    output logic [3:0] cache_byte_o
);

//====================================================================================
//      STORE PERFORMANCE (SIMULATION ONLY)
//====================================================================================

    logic [31:0] store_access_CRT, store_access_NXT, store_hit_CRT, store_hit_NXT;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin
                store_access_CRT <= '0;
                store_hit_CRT <= '0;
            end else begin
                store_access_CRT <= store_access_NXT;
                store_hit_CRT <= store_hit_NXT;
            end
        end


//====================================================================================
//      FSM LOGIC
//====================================================================================

    store_buffer_entry_t buffer_entry; 

        always_ff @(posedge clk_i) begin
            if (request_i) begin
                buffer_entry <= buffer_entry_i;
            end
        end


    typedef enum logic [2:0] {IDLE, WAIT_LOCK, WAIT_PORT, OUTCOME, WRITE_THROUGH, WAIT} fsm_states_t;

    fsm_states_t state_CRT, state_NXT;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin : state_register
            if (!rst_n_i) begin
                state_CRT <= IDLE;
            end else begin
                state_CRT <= state_NXT;
            end
        end : state_register


        always_comb begin
            /* Default values */
            state_NXT = state_CRT;

            store_access_NXT = store_access_CRT;
            store_hit_NXT = store_hit_CRT;

            store_channel.request = 1'b0;

            cache_read_o = '0;
            cache_write_o = '0;

            cache_status_o = '0;
            cache_byte_o = '0; 
            cache_data_o = '0;

            lock_request_o = 1'b0;

            valid_o = '0; 

            case (state_CRT)

                /* FSM waits for STU request, and sends a cache read  *
                 * command as soon as the request arrives. Data,      *
                 * status bits and tag are requested from cache to    *
                 * determine if the read was an hit or a miss.        *
                 * If the other FSM is currently executing operations *
                 * on the same cache address there is a lock, wait    *
                 * until the lock is released                         */
                IDLE: begin
                    if (lock_i) begin
                        state_NXT = WAIT_LOCK;

                        store_access_NXT = store_access_CRT + 1'b1;
                    end else if (request_i) begin
                        state_NXT = halt_i ? WAIT_PORT : OUTCOME;

                        store_access_NXT = store_access_CRT + 1'b1;

                        /* Read cache */
                        cache_read_o = halt_i ? '0 : '1; 
                    end
                end

                /* Wait until the lock is released, then execute operations */
                WAIT_LOCK: begin
                    if (!lock_status_i) begin
                        state_NXT = OUTCOME;

                        /* Request lock on that address to avoid other
                         * operations on the same block */
                        lock_request_o = 1'b1;

                        /* Read cache */
                        cache_read_o = '1; 
                    end
                end

                /* The request was accepted (data already latched) but the cache
                 * port was busy. Re-issue the read as soon as the port is free */
                WAIT_PORT: begin
                    if (!halt_i & !stall_i) begin
                        state_NXT = OUTCOME;

                        cache_read_o = '1;
                    end
                end

                /* Cache now has output the outcome of the previous *
                 * request. On cache hit, the data is simply stored *
                 * in cache. On cache miss, the corresponding block *
                 * is invalidated and a store operation to memory   *
                 * is initiated.                                    */
                OUTCOME: begin
                    if (halt_i | stall_i) begin
                        state_NXT = WAIT_PORT; 
                    end else if (cache_hit_i) begin
                        state_NXT = IDLE; 
                        valid_o = 1'b1;

                        store_hit_NXT = store_hit_CRT + 1'b1;

                        /* Write data and update status bits */
                        cache_write_o.data = 1'b1;
                        cache_write_o.dirty = 1'b1;
                        cache_write_o.valid = 1'b1;

                        /* Set dirty and valid bit */
                        cache_status_o = '1;
                    end else begin
                        if (!halt_i & !stall_i) begin
                            state_NXT = WRITE_THROUGH;
                            store_channel.request = !stall_i; 
                        end

                        if (!cache_dirty_i) begin 
                            /* Invalidate cache block */
                            cache_status_o.valid = 1'b0; 
                            cache_write_o.valid = 1'b1;
                        end
                    end

                    /* Select data to write */
                    case (buffer_entry.store_width)
                        BYTE: cache_byte_o = 1'b1 << buffer_entry.address[1:0]; 

                        HALF_WORD: cache_byte_o = 2'b11 << {buffer_entry.address[1], 1'b0};

                        WORD: cache_byte_o = '1;
                    endcase 

                    cache_data_o = buffer_entry.data;
                end

                /* The FSM waits for memory to complete the *
                 * store operation                          */
                WRITE_THROUGH: begin
                    if (store_channel.done) begin
                        if (!halt_i & !stall_i) begin
                            state_NXT = IDLE;
                            valid_o = 1'b1;
                        end else begin
                            state_NXT = WAIT;
                        end
                    end
                end

                /* Wait until the stall is resolved */
                WAIT: begin
                    if (!halt_i & !stall_i) begin
                        state_NXT = IDLE;
                        valid_o = 1'b1;
                    end
                end

            endcase 
        end

    assign store_channel.address = buffer_entry.address;
    assign store_channel.width = buffer_entry.store_width;
    assign store_channel.data = cache_data_o;

    assign cache_address_o = (state_CRT == IDLE) ? buffer_entry_i.address : buffer_entry.address; 

endmodule : store_controller

`endif 