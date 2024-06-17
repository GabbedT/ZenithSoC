`ifndef CACHE_DDR_INTERFACE_SV
    `define CACHE_DDR_INTERFACE_SV

`include "../../CPU/ApogeoRV/Hardware/Include/Interfaces/bus_interface.sv"
`include "../../CPU/ApogeoRV/Hardware/Include/Packages/Execution Unit/store_unit_pkg.sv"

`include "../../System/synchronous_buffer.sv"
`include "../../System/edge_detector.sv"

module cache_ddr_interface #(
    /* Number of 32 bits requests by cache */
    parameter MAX_BURST = 4
) (
    input logic clk_i,
    input logic rst_n_i,

    /* Arbiter */
    input logic hold_i,

    /* Memory interface */
    load_interface.slave load_channel,
    store_interface.slave store_channel,

    input logic single_trx_i,

    /* Common address */
    output logic [26:0] address_o, 
    
    /* Command interface */
    output logic write_o, 
    output logic read_o, 

    /* Data interface */
    output logic push_o, 
    output logic pull_o, 
    output logic [63:0] write_data_o,
    output logic [7:0] write_mask_o,
    input logic [1:0][31:0] read_data_i,
    input logic read_valid_i,

    /* Status */
    output logic done_o,
    output logic idle_o,
    input logic ready_i,
    input logic start_i
);

//====================================================================================
//      STORE TRANSACTIONS BUFFER
//====================================================================================

    logic str_select;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                str_select <= 1'b0;
            end else begin 
                if (store_channel.request) begin
                    str_select <= !str_select;
                end else begin
                    str_select <= 1'b0;
                end
            end 
        end 


    logic [31:0] str_saved_data; logic [26:0] str_saved_address;

        always_ff @(posedge clk_i) begin
            if (!str_select & store_channel.request) begin
                str_saved_data <= store_channel.data;
                str_saved_address <= store_channel.address;
            end
        end 

    /* FIFO packet structure */
    typedef struct packed {
        logic [26:0] address;
        logic [63:0] data;
        logic single_trx;
        store_width_t width;  
    } store_packet_t;


    store_packet_t str_wr_packet, str_rd_packet; logic str_read, str_write, str_empty;

    synchronous_buffer #(MAX_BURST / 2, $bits(store_packet_t)) store_info_buffer (
        .clk_i   ( clk_i   ),
        .rst_n_i ( rst_n_i ),

        .write_i ( str_write ),
        .read_i  ( str_read  ),

        .empty_o ( str_empty ),
        .full_o  (           ),

        .write_data_i ( str_wr_packet ),
        .read_data_o  ( str_rd_packet )
    );


    assign str_write = (str_select & store_channel.request) | (store_channel.request & single_trx_i);

        always_comb begin
            if (single_trx_i) begin
                str_wr_packet.address = store_channel.address;
                str_wr_packet.data = {32'b0, store_channel.data};
                str_wr_packet.width = store_channel.width;
                str_wr_packet.single_trx = 1'b1;
            end else begin
                /* Old address is used */
                str_wr_packet.address = str_saved_address;
                str_wr_packet.single_trx = 1'b0;

                /* Fuse the two data requests */
                str_wr_packet.data = {store_channel.data, str_saved_data};
                str_wr_packet.width = WORD;
            end
        end


    logic [$clog2(MAX_BURST / 2):0] str_count; logic str_ready, str_ackn;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                str_count <= '0;
                str_ready <= 1'b0;
            end else begin 
                if (str_write) begin
                    str_count <= str_count + 1'b1;
                end else if (str_read) begin
                    str_count <= '0;
                end

                if (str_ackn) begin
                    str_ready <= 1'b0;
                end else begin
                    /* FIFO is ready to be read */
                    str_ready <= (str_count == MAX_BURST / 2) | (str_write & single_trx_i);
                end
            end 
        end 


//====================================================================================
//      LOAD TRANSACTIONS BUFFER
//====================================================================================

    logic ldr_select;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                ldr_select <= 1'b0;
            end else begin 
                if (load_channel.request) begin
                    ldr_select <= !ldr_select;
                end else begin
                    ldr_select <= 1'b0;
                end
            end 
        end 


    logic [26:0] ldr_saved_address;

        always_ff @(posedge clk_i) begin
            if (!ldr_select & load_channel.request) begin
                ldr_saved_address <= load_channel.address;
            end
        end 


    logic [26:0] load_address; logic ldr_write, ldr_read, ldr_empty;

    synchronous_buffer #(MAX_BURST / 2, 27) load_info_buffer (
        .clk_i   ( clk_i   ),
        .rst_n_i ( rst_n_i ),

        .write_i ( ldr_write ),
        .read_i  ( ldr_read  ),

        .empty_o ( ldr_empty ),
        .full_o  (           ),

        .write_data_i ( ldr_saved_address ),
        .read_data_o  ( load_address      )
    );

    assign ldr_write = ldr_select & load_channel.request;


    logic [$clog2(MAX_BURST / 2):0] ldr_count; logic ldr_ready, ldr_ackn;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                ldr_count <= '0;
                ldr_ready <= 1'b0;
            end else begin 
                if (ldr_write) begin
                    ldr_count <= ldr_count + 1'b1;
                end else if (ldr_read) begin
                    ldr_count <= '0;
                end

                if (ldr_ackn) begin
                    ldr_ready <= 1'b0;
                end else begin
                    /* FIFO is ready to be read */
                    ldr_ready <= ldr_count == (MAX_BURST / 2);
                end
            end 
        end 

//====================================================================================
//      ARBITER
//====================================================================================

    typedef enum logic [2:0] { WAIT, STORE, LOAD, LOAD_DATA, WAIT_DATA, EXTRACT_DATA } fsm_states_t;

    fsm_states_t state_CRT, state_NXT;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                state_CRT <= WAIT;
            end else begin 
                state_CRT <= state_NXT;
            end 
        end 


    logic valid_address, valid_data;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                valid_address <= 1'b0;
            end else begin 
                if (state_CRT == WAIT) begin
                    valid_address <= 1'b0;
                end else if (str_read | ldr_read) begin
                    valid_address <= !valid_address;
                end

                if (state_CRT == WAIT) begin
                    valid_data <= 1'b0;
                end else if (state_CRT == LOAD_DATA) begin
                    valid_data <= !valid_data;
                end
            end 
        end 

    
    logic valid_negedge, write_shift_reg, read_shift_reg;

        always_comb begin
            state_NXT = state_CRT;

            store_channel.done = 1'b0;
            load_channel.valid = 1'b0;
            load_channel.data = '0;

            ldr_read = 1'b0;
            ldr_ackn = 1'b0;
            str_read = 1'b0;
            str_ackn = 1'b0;

            // write_shift_reg = 1'b0;
            // read_shift_reg = 1'b0;

            write_mask_o = '0;
            write_data_o = '0;
            address_o = '0;
            write_o = 1'b0;
            read_o = 1'b0;
            push_o = 1'b0;
            pull_o = 1'b0;
            done_o = 1'b0;

            case (state_CRT)
                WAIT: begin
                    if (ready_i & !hold_i) begin
                        case ({ldr_ready, str_ready})
                            2'b10, 2'b11: begin
                                state_NXT = LOAD;
                                
                                ldr_read = 1'b1;
                                ldr_ackn = 1'b1;
                            end

                            2'b01: begin
                                state_NXT = STORE;

                                str_read = 1'b1;
                                str_ackn = 1'b1;
                            end
                        endcase 
                    end 
                end

                STORE: begin
                    if (str_rd_packet.single_trx) begin
                        if (!hold_i) begin
                            state_NXT = WAIT;

                            push_o = 1'b1;
                            write_o = 1'b1;

                            store_channel.done = 1'b1;
                        end 

                        write_mask_o[7:0] = '0;

                        if (str_rd_packet.address[2]) begin 
                            case (str_rd_packet.width)
                                WORD: write_mask_o[7:4] = '1;

                                HALF_WORD: write_mask_o[7:4] = 2'b11 << {str_rd_packet.address[1], 1'b0};

                                BYTE: write_mask_o[7:4] = 1'b1 << str_rd_packet.address[1:0];
                            endcase 

                            write_data_o = {str_rd_packet.data[31:0], 32'b0};
                        end else begin
                            case (str_rd_packet.width)
                                WORD: write_mask_o[3:0] = '1;

                                HALF_WORD: write_mask_o[3:0] = 2'b11 << {str_rd_packet.address[1], 1'b0};

                                BYTE: write_mask_o[3:0] = 1'b1 << str_rd_packet.address[1:0];
                            endcase 

                            write_data_o = {32'b0, str_rd_packet.data[31:0]};
                        end
                    end else begin
                        if (str_empty) begin
                            state_NXT = WAIT;

                            /* Push last data */
                            push_o = 1'b1;
                        end

                        if (!str_empty & !hold_i) begin
                            str_read = 1'b1;
                            push_o = 1'b1;

                            store_channel.done = 1'b1;
                        end 

                        write_mask_o = '1;
                        write_data_o = str_rd_packet.data;
                    end

                    address_o = {1'b0, str_rd_packet.address[26:3], 2'b0};
                    write_o = !valid_address;
                end

                LOAD: begin
                    if (ldr_empty) begin
                        state_NXT = WAIT_DATA;

                        done_o = 1'b1;
                    end 

                    if (!ldr_empty & !hold_i) begin
                        ldr_read = 1'b1;
                    end 

                    address_o = {1'b0, load_address[26:3], 2'b0};
                    read_o = !valid_address;
                end

                WAIT_DATA: begin
                    if (read_valid_i) begin
                        state_NXT = LOAD_DATA;
                        
                        pull_o = 1'b1;
                    end
                end

                LOAD_DATA: begin
                    // write_shift_reg = 1'b1;
                    pull_o = valid_data;

                    load_channel.valid = 1'b1;
                    load_channel.data = read_data_i[valid_data];
                    
                    if (valid_negedge) begin
                        state_NXT = WAIT;

                        load_channel.valid = 1'b0;
                        pull_o = 1'b0;
                    end
                end

                EXTRACT_DATA: begin
                    load_channel.valid = 1'b1;
                end
            endcase 
        end


    // logic [(MAX_BURST / 2) - 1:0][63:0] read_data_register; 

    //     always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
    //         if (write_shift_reg) begin
    //             read_data_register <= {read_data_register[(MAX_BURST / 2) - 2:0], read_data_i}
    //         end else if (read_shift_reg) begin
    //             read_data_register <= read_data_register << 32;
    //         end
    //     end 

    // assign load_channel.data = read_data_register[(MAX_BURST / 2) - 1][63:32];


    edge_detector #(0, 0) valid_negedge_detector (
        .clk_i   ( clk_i ),
        .rst_n_i ( rst_n_i ),

        .signal_i ( read_valid_i  ),
        .edge_o   ( valid_negedge )
    );

endmodule : cache_ddr_interface

`endif