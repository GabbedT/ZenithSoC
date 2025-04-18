`ifndef LOAD_CONTROLLER_SV
    `define LOAD_CONTROLLER_SV

`include "../../../Utility/Packages/cache_pkg.sv"

`include "../../../CPU/ApogeoRV/Hardware/Include/Interfaces/bus_interface.sv"

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
//      DATAPATH
//====================================================================================

    typedef struct packed {
        logic [TAG - 1:0] tag; 
        logic [INDEX - 1:0] index; 
        logic [OFFSET - 1:0] offset; 
    } cache_address_t;

    cache_address_t cache_address; assign cache_address = address_i[31:2];


    data_word_t requested_data_NXT, requested_data_CRT;

        always_ff @(posedge clk_i) begin
            requested_data_CRT <= requested_data_NXT;
        end


    logic [OFFSET:0] word_counter_CRT, word_counter_NXT;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin : counter
            if (!rst_n_i) begin
                word_counter_CRT <= '0;
            end else if (!stall_i) begin
                word_counter_CRT <= word_counter_NXT;
            end
        end : counter


//====================================================================================
//      FSM LOGIC
//====================================================================================

    typedef enum logic [2:0] {IDLE, OUTCOME, ALLOCATION_REQ, ALLOCATE, WRITE_BACK} fsm_states_t;

    fsm_states_t state_CRT, state_NXT; logic force_state;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin : state_register
            if (!rst_n_i) begin
                state_CRT <= IDLE;
            end else if ((state_CRT == IDLE) | force_state) begin
                state_CRT <= state_NXT;
            end else if (!stall_i) begin
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
            word_counter_NXT = word_counter_CRT;
            state_NXT = state_CRT;

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

            force_state = 1'b0;

            case (state_CRT)

                /* FSM waits for LDU request, and sends a cache read *
                 * command as soon as the request arrives. Data,     *
                 * status bits and tag are requested from cache to   *
                 * determine if the read was an hit or a miss.       */
                IDLE: begin
                    if (request_i) begin
                        state_NXT = OUTCOME;

                        /* Read cache */
                        cache_read_o = '1; 
                    end

                    cache_address_o = address_i; 

                    /* Save address for later use */
                    word_counter_NXT = '0; 
                end

                /* Cache now has output the outcome of the previous    *
                 * request. On cache hit, the data is simply fowarded  *
                 * to the LDU. On cache miss, if the data is not       *
                 * dirty, a new block can be allocated, else the block *
                 * needs to be written back first.                     */
                OUTCOME: begin
                    if (cache_hit_i) begin
                        state_NXT = IDLE;

                        force_state = 1'b1;

                        data_o = cache_data_i;
                        valid_o = !invalidate_i;
                    end else begin
                        force_state = invalidate_i;
                        
                        if (cache_dirty_i) begin
                            state_NXT = invalidate_i ? IDLE : WRITE_BACK;

                            /* Read only data */
                            cache_read_o.data = !stall_i;

                            /* Start from block base */
                            cache_address_o = stall_i ? address_i : {cache_tag_i, cache_address.index, word_counter_CRT[OFFSET - 1:0], 2'b0}; 

                            /* Increment word counter */
                            word_counter_NXT = 'd1;
                        end else begin
                            state_NXT = invalidate_i ? IDLE : ALLOCATION_REQ;
                            
                            load_channel.request = !invalidate_i & !stall_i;
                            load_channel.address = {cache_address.tag, cache_address.index, word_counter_CRT[OFFSET - 1:0], 2'b0};

                            word_counter_NXT = 'd1;
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
                        cache_read_o.data = 1'b1;
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
                        
                        load_channel.request = !stall_i; 
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
                            state_NXT = IDLE; 
                            
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

                    cache_address_o = {cache_address.tag, cache_address.index, word_counter_CRT[OFFSET - 1:0], 2'b0}; 

                    /* Set status to valid and clean */
                    cache_status_o.valid = 1'b1;
                    cache_status_o.dirty = 1'b0; 
                end
            endcase 
        end

    assign store_channel.data = cache_data_i; 
    assign store_channel.width = WORD;

endmodule : load_controller 

`endif 