`ifndef CACHE_DDR_INTERFACE_SV
    `define CACHE_DDR_INTERFACE_SV

module cache_ddr_interface #(
    parameter DATA_MAX_BURST = 4,
    parameter INSTRUCTION_MAX_BURST = 8,
    parameter REQUEST_BUFFER_SIZE = 8,
    parameter RESPONSE_BUFFER_SIZE = 4
) (
    input logic clk_i,
    input logic rst_n_i,

    load_interface.slave load_channel,
    store_interface.slave store_channel,

    input logic single_trx_i,
    input logic instr_req_i,
    output logic load_empty_o,
    output logic store_idle_o,

    dev2ddr_interface.master ddr_channel
);

    localparam logic READ = 1'b0;
    localparam logic WRITE = 1'b1;


//====================================================================================
//      STORE TRANSACTIONS BUFFER
//====================================================================================

    typedef struct packed {
        logic [26:0] address;
        logic [127:0] data;
        logic [15:0] strobe;
    } store_packet_t;

    store_packet_t store_write_packet, store_read_packet;
    logic store_write, store_read, store_empty, store_full;

    synchronous_buffer #(
        .BUFFER_DEPTH           ( REQUEST_BUFFER_SIZE   ),
        .DATA_WIDTH             ( $bits(store_packet_t) ),
        .FIRST_WORD_FALL_TROUGH ( 1                     )
    ) store_request_buffer (
        .clk_i   ( clk_i   ),
        .rst_n_i ( rst_n_i ),

        .write_i ( store_write ),
        .read_i  ( store_read  ),

        .empty_o ( store_empty ),
        .full_o  ( store_full  ),

        .write_data_i ( store_write_packet ),
        .read_data_o  ( store_read_packet  )
    );

    logic [1:0] store_word_count;
    logic [26:0] store_base_address;
    logic [3:0][31:0] store_bundle;

    /* Cache-line stores are packed into one 128-bit DDR request. */
    assign store_write = store_channel.request & (single_trx_i | (store_word_count == 2'd3));

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin
                store_word_count <= '0;
                store_base_address <= '0;
                store_bundle <= '0;
            end else if (store_channel.request) begin
                if (single_trx_i) begin
                    store_word_count <= '0;
                end else begin
                    store_bundle[store_word_count] <= store_channel.data;
                    store_word_count <= store_word_count + 1'b1;

                    if (store_word_count == '0) begin
                        store_base_address <= store_channel.address;
                    end
                end
            end
        end

        always_comb begin
            store_write_packet = '0;

            if (single_trx_i) begin
                /* Place an uncached store in its addressed byte lanes. */
                store_write_packet.address = {store_channel.address[26:4], 4'b0};
                store_write_packet.data[store_channel.address[3:2] * 32 +: 32] = store_channel.data;

                case (store_channel.width)
                    WORD: begin
                        store_write_packet.strobe = 16'h000F << {store_channel.address[3:2], 2'b0};
                    end

                    HALF_WORD: begin
                        store_write_packet.strobe = 16'h0003 << {store_channel.address[3:1], 1'b0};
                    end

                    BYTE: begin
                        store_write_packet.strobe = 16'h0001 << store_channel.address[3:0];
                    end

                    default: store_write_packet.strobe = '0;
                endcase
            end else begin
                store_write_packet.address = {store_base_address[26:4], 4'b0};
                store_write_packet.data = store_bundle;
                store_write_packet.data[96 +: 32] = store_channel.data;
                store_write_packet.strobe = '1;
            end
        end


//====================================================================================
//      LOAD TRANSACTIONS BUFFER
//====================================================================================

    logic [26:0] load_write_address, load_read_address;
    logic load_write, load_read, load_empty, load_full;
    logic [1:0] load_word_count;
    logic [26:0] load_base_address;

    synchronous_buffer #(
        .BUFFER_DEPTH           ( REQUEST_BUFFER_SIZE ),
        .DATA_WIDTH             ( 27                  ),
        .FIRST_WORD_FALL_TROUGH ( 1                   )
    ) load_request_buffer (
        .clk_i   ( clk_i   ),
        .rst_n_i ( rst_n_i ),

        .write_i ( load_write ),
        .read_i  ( load_read  ),

        .empty_o ( load_empty ),
        .full_o  ( load_full  ),

        .write_data_i ( load_write_address ),
        .read_data_o  ( load_read_address  )
    );

    /* Four 32-bit cache requests share one aligned DDR read. */
    assign load_write = load_channel.request & (load_word_count == 2'd3);
    assign load_write_address = {load_base_address[26:4], 4'b0};

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin
                load_word_count <= '0;
                load_base_address <= '0;
            end else if (load_channel.request) begin
                load_word_count <= load_word_count + 1'b1;

                if (load_word_count == '0) begin
                    load_base_address <= load_channel.address;
                end
            end
        end


//====================================================================================
//      DDR REQUEST ARBITER
//====================================================================================

    logic request_accept;

    /* Reads have priority over buffered writes. */
    assign ddr_channel.trx_req = !load_empty | !store_empty;
    assign ddr_channel.trx_type = load_empty ? WRITE : READ;
    assign ddr_channel.trx_addr = load_empty ? store_read_packet.address : load_read_address;
    assign ddr_channel.wdata = store_read_packet.data;
    assign ddr_channel.wstrobe = store_read_packet.strobe;

    assign request_accept = ddr_channel.trx_req & ddr_channel.ready;
    assign load_read = request_accept & !ddr_channel.trx_type;
    assign store_read = request_accept & ddr_channel.trx_type;

    logic [$clog2(REQUEST_BUFFER_SIZE + 1) - 1:0] store_outstanding;

    /* A flush waits for every accepted DDR write to complete. */
        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin
                store_outstanding <= '0;
            end else begin
                case ({store_read, ddr_channel.write_valid})
                    2'b10: store_outstanding <= store_outstanding + 1'b1;
                    2'b01: store_outstanding <= store_outstanding - 1'b1;
                    default: store_outstanding <= store_outstanding;
                endcase
            end
        end

    assign store_channel.done = ddr_channel.write_valid;
    assign store_idle_o = store_empty & (store_word_count == '0) & (store_outstanding == '0);
    assign load_empty_o = load_empty & (load_word_count == '0);


//====================================================================================
//      READ RESPONSE BUFFER
//====================================================================================

    logic [127:0] response_data;
    logic response_read, response_empty, response_full;
    logic [1:0] response_word;

    synchronous_buffer #(
        .BUFFER_DEPTH           ( RESPONSE_BUFFER_SIZE ),
        .DATA_WIDTH             ( 128                  ),
        .FIRST_WORD_FALL_TROUGH ( 1                    )
    ) read_response_buffer (
        .clk_i   ( clk_i   ),
        .rst_n_i ( rst_n_i ),

        .write_i ( ddr_channel.read_valid ),
        .read_i  ( response_read         ),

        .empty_o ( response_empty ),
        .full_o  ( response_full  ),

        .write_data_i ( ddr_channel.rdata ),
        .read_data_o  ( response_data     )
    );

    assign load_channel.valid = !response_empty;
    assign load_channel.data = response_data[response_word * 32 +: 32];
    
    /* Return one 32-bit cache word per cycle. */
    assign response_read = !response_empty & (response_word == 2'd3);

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin
                response_word <= '0;
            end else if (!response_empty) begin
                response_word <= response_word + 1'b1;
            end
        end


//====================================================================================
//      ASSERTIONS
//====================================================================================

`ifndef SYNTHESIS

    initial begin
        assert ((DATA_MAX_BURST % 4) == 0)
            else $error("Data-cache burst must contain complete 128-bit transactions");

        assert ((INSTRUCTION_MAX_BURST % 4) == 0)
            else $error("Instruction-cache burst must contain complete 128-bit transactions");
    end

    assert property (@(posedge clk_i)
        disable iff (!rst_n_i)
        store_write |-> !store_full)
        else $error("Cache DDR store request buffer overflow");

    assert property (@(posedge clk_i)
        disable iff (!rst_n_i)
        load_write |-> !load_full)
        else $error("Cache DDR load request buffer overflow");

    assert property (@(posedge clk_i)
        disable iff (!rst_n_i)
        ddr_channel.read_valid |-> !response_full)
        else $error("Cache DDR response buffer overflow");

    assert property (@(posedge clk_i)
        disable iff (!rst_n_i)
        ddr_channel.trx_req & !ddr_channel.ready |=>
        $stable({ddr_channel.trx_type, ddr_channel.trx_addr,
                 ddr_channel.wdata, ddr_channel.wstrobe}))
        else $error("Cache DDR request changed while stalled");

`endif

    logic unused;
    assign unused = instr_req_i ^ ddr_channel.read_error ^ ddr_channel.write_error;

endmodule : cache_ddr_interface

`endif
