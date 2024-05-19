`ifndef FETCH_CONTROLLER
    `define FETCH_CONTROLLER

`include "../../../Utility/Packages/cache_pkg.sv"

`include "../../../CPU/ApogeoRV/Hardware/Include/Interfaces/bus_interface.sv"

module fetch_controller #(
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

    /* Fetch unit interface */
    input logic invalidate_i,
    input logic fetch_i,
    input logic [31:0] program_counter_i,
    output logic [BLOCK_WIDTH - 1:0][31:0] instruction_o, 
    output logic valid_o,

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
//      DATAPATH
//====================================================================================

    typedef struct packed {
        logic [TAG - 1:0] tag; 
        logic [INDEX - 1:0] index; 
        logic [OFFSET - 1:0] offset; 
    } cache_address_t;

    cache_address_t program_counter; assign program_counter = program_counter_i[31:2];


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
            end else begin
                word_counter_CRT <= word_counter_NXT;
            end
        end : counter


//====================================================================================
//      FSM LOGIC
//====================================================================================

    typedef enum logic [2:0] {IDLE, OUTCOME, MISALIGNED, ALLOCATION_REQ, WAIT_BUNDLE, ALLOCATE} fsm_states_t;

    fsm_states_t state_CRT, state_NXT;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin : state_register
            if (!rst_n_i) begin
                state_CRT <= IDLE;
            end else if (invalidate_i) begin 
                state_CRT <= IDLE;
            end else if (!stall_i) begin
                state_CRT <= state_NXT;
            end
        end : state_register


        always_comb begin
            /* Default values */
            state_NXT = state_CRT;
            word_counter_NXT = word_counter_CRT;

            instruction_o = '0; 
            valid_o = 1'b0;

            cache_read_o = '0;
            cache_write_o = '0;
            cache_read_address_o = '0;
            cache_write_address_o = '0;

            load_channel.request = 1'b0;
            load_channel.address = '0;

            case (state_CRT)

                /* FSM waits for front end to request an     *
                 * instruction fetch. As soon as the request *
                 * arrives, a cache read is issued.          */
                IDLE: begin
                    if (fetch_i) begin
                        state_NXT = OUTCOME;

                        /* Read cache */
                        cache_read_o = '1; 
                    end

                    word_counter_NXT = '0; 

                    cache_read_address_o = program_counter_i; 
                end


                /* Once the outcome arrives, the FSM will *
                 * either output a valid instruction      *
                 * bundle or in case of misaligned access *
                 * it will read the next cache block. On  *
                 * cache miss a memory request is issued. */
                OUTCOME: begin
                    if (cache_hit_i) begin
                        state_NXT = IDLE; 
                        
                        /* Load in bundle */
                        valid_o = 1'b1;
                        instruction_o = cache_instruction_i;
                    end else begin
                        state_NXT = ALLOCATION_REQ;

                        word_counter_NXT = 'd1;

                        load_channel.request = 1'b1;
                        load_channel.address = {program_counter.tag, program_counter.index, word_counter_CRT, 2'b0};
                    end
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
                        load_channel.request = 1'b1; 
                    end else if (word_counter_CRT[OFFSET]) begin
                        /* Wait for response */
                        state_NXT = WAIT_BUNDLE;
                        
                        /* Reset word counter */ 
                        word_counter_NXT = '0; 
                    end

                    /* Load address */
                    load_channel.address = {program_counter.tag, program_counter.index, word_counter_CRT[OFFSET - 1:0], 2'b0}; 
                end


                WAIT_BUNDLE: begin
                    if (load_channel.valid) begin
                        /* Increment word counter */
                        word_counter_NXT = word_counter_CRT + 1'b1;
                    end

                    if (word_counter_CRT[OFFSET] & word_counter_CRT[OFFSET - 1:0] == '0) begin
                        /* Wait for response */
                        state_NXT = ALLOCATE;
                        
                        /* Reset word counter */ 
                        word_counter_NXT = '0; 
                    end
                end


                /* FSM waits for memory to supply instructions. *
                 * When the memory interface has data ready, it *
                 * allocate a new cache block by writing the    *
                 * incoming data to cache.                      */
                ALLOCATE: begin
                    state_NXT = IDLE; 

                    cache_write_o = '1;
                    cache_write_address_o = {program_counter.tag, program_counter.index, word_counter_CRT[OFFSET - 1:0], 2'b0};

                    /* Load in sequencer */
                    instruction_o = instruction_bundle;
                    valid_o = 1'b1;
                end
            endcase 
        end

    assign cache_instruction_o = instruction_bundle;

    assign load_channel.invalidate = invalidate_i;

endmodule : fetch_controller

`endif 