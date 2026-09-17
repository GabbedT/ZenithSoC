`ifndef DDR_ARBITER_SV
    `define DDR_ARBITER_SV

module ddr_arbiter #(
    parameter REQUEST_BUFFER_SIZE = 8,
    parameter OWNER_BUFFER_SIZE = 32,
    parameter VGA_MAX_CONSECUTIVE = 8
) (
    input logic clk_i,
    input logic rst_n_i,

    dev2ddr_interface.slave cache_channel,
    dev2ddr_interface.slave vga_channel,

    dev2ddr_interface.master memory_channel,

    input logic vga_urgent_i
);

    localparam logic CACHE = 1'b0;
    localparam logic VGA = 1'b1;

    typedef struct packed {
        logic operation;
        logic [26:0] address;
        logic [127:0] data;
        logic [15:0] strobe;
    } request_packet_t;


//====================================================================================
//      INDEPENDENT MASTER REQUEST BUFFERS
//====================================================================================

    request_packet_t cache_write_packet, cache_read_packet;
    request_packet_t vga_write_packet, vga_read_packet;
    logic cache_write, cache_read, cache_empty, cache_full;
    logic vga_write, vga_read, vga_empty, vga_full;

    /* Per-master queues allow both request sources to run independently. */
    assign cache_write_packet.operation = cache_channel.trx_type;
    assign cache_write_packet.address = cache_channel.trx_addr;
    assign cache_write_packet.data = cache_channel.wdata;
    assign cache_write_packet.strobe = cache_channel.wstrobe;

    assign vga_write_packet.operation = vga_channel.trx_type;
    assign vga_write_packet.address = vga_channel.trx_addr;
    assign vga_write_packet.data = vga_channel.wdata;
    assign vga_write_packet.strobe = vga_channel.wstrobe;

    assign cache_channel.ready = !cache_full;
    assign vga_channel.ready = !vga_full;
    assign cache_write = cache_channel.trx_req & cache_channel.ready;
    assign vga_write = vga_channel.trx_req & vga_channel.ready;

    synchronous_buffer #(
        .BUFFER_DEPTH           ( REQUEST_BUFFER_SIZE     ),
        .DATA_WIDTH             ( $bits(request_packet_t) ),
        .FIRST_WORD_FALL_TROUGH ( 1                       )
    ) cache_request_buffer (
        .clk_i   ( clk_i   ),
        .rst_n_i ( rst_n_i ),

        .write_i ( cache_write ),
        .read_i  ( cache_read  ),

        .empty_o ( cache_empty ),
        .full_o  ( cache_full  ),

        .write_data_i ( cache_write_packet ),
        .read_data_o  ( cache_read_packet  )
    );

    synchronous_buffer #(
        .BUFFER_DEPTH           ( REQUEST_BUFFER_SIZE     ),
        .DATA_WIDTH             ( $bits(request_packet_t) ),
        .FIRST_WORD_FALL_TROUGH ( 1                       )
    ) vga_request_buffer (
        .clk_i   ( clk_i   ),
        .rst_n_i ( rst_n_i ),

        .write_i ( vga_write ),
        .read_i  ( vga_read  ),

        .empty_o ( vga_empty ),
        .full_o  ( vga_full  ),

        .write_data_i ( vga_write_packet ),
        .read_data_o  ( vga_read_packet  )
    );


//====================================================================================
//      DYNAMIC REQUEST ARBITER
//====================================================================================

    logic select_vga, request_accept;
    logic read_owner_full, write_owner_full;
    logic [$clog2(VGA_MAX_CONSECUTIVE + 1) - 1:0] vga_consecutive;
    request_packet_t selected_packet;

    /* Urgent VGA traffic wins, with a bounded CPU starvation interval. */
    always_comb begin
        select_vga = 1'b0;

        if (!vga_empty) begin
            if (cache_empty) begin
                select_vga = 1'b1;
            end else if (vga_urgent_i & (vga_consecutive < VGA_MAX_CONSECUTIVE)) begin
                select_vga = 1'b1;
            end
        end
    end

    assign selected_packet = select_vga ? vga_read_packet : cache_read_packet;

    logic owner_room;
    assign owner_room = selected_packet.operation ? !write_owner_full : !read_owner_full;

    assign memory_channel.trx_req = (!cache_empty | !vga_empty) & owner_room;
    assign memory_channel.trx_type = selected_packet.operation;
    assign memory_channel.trx_addr = selected_packet.address;
    assign memory_channel.wdata = selected_packet.data;
    assign memory_channel.wstrobe = selected_packet.strobe;

    assign request_accept = memory_channel.trx_req & memory_channel.ready;
    assign cache_read = request_accept & !select_vga;
    assign vga_read = request_accept & select_vga;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin
                vga_consecutive <= '0;
            end else if (request_accept) begin
                if (!select_vga) begin
                    vga_consecutive <= '0;
                end else if (!cache_empty & vga_urgent_i &
                             (vga_consecutive < VGA_MAX_CONSECUTIVE)) begin
                    vga_consecutive <= vga_consecutive + 1'b1;
                end
            end
        end


//====================================================================================
//      RESPONSE OWNER TRACKING
//====================================================================================

    logic read_owner, read_owner_empty, read_owner_push, read_owner_pop;
    logic write_owner, write_owner_empty, write_owner_push, write_owner_pop;

    /* MIG responses are ordered, so an owner FIFO is sufficient. */
    assign read_owner_push = request_accept & !selected_packet.operation;
    assign write_owner_push = request_accept & selected_packet.operation;
    assign read_owner_pop = memory_channel.read_valid;
    assign write_owner_pop = memory_channel.write_valid;

    synchronous_buffer #(
        .BUFFER_DEPTH           ( OWNER_BUFFER_SIZE ),
        .DATA_WIDTH             ( 1                 ),
        .FIRST_WORD_FALL_TROUGH ( 1                 )
    ) read_owner_buffer (
        .clk_i   ( clk_i   ),
        .rst_n_i ( rst_n_i ),

        .write_i ( read_owner_push ),
        .read_i  ( read_owner_pop  ),

        .empty_o ( read_owner_empty ),
        .full_o  ( read_owner_full  ),

        .write_data_i ( select_vga ),
        .read_data_o  ( read_owner )
    );

    synchronous_buffer #(
        .BUFFER_DEPTH           ( OWNER_BUFFER_SIZE ),
        .DATA_WIDTH             ( 1                 ),
        .FIRST_WORD_FALL_TROUGH ( 1                 )
    ) write_owner_buffer (
        .clk_i   ( clk_i   ),
        .rst_n_i ( rst_n_i ),

        .write_i ( write_owner_push ),
        .read_i  ( write_owner_pop  ),

        .empty_o ( write_owner_empty ),
        .full_o  ( write_owner_full  ),

        .write_data_i ( select_vga  ),
        .read_data_o  ( write_owner )
    );

    assign cache_channel.read_valid = memory_channel.read_valid & (read_owner == CACHE) & !read_owner_empty;
    assign cache_channel.read_error = memory_channel.read_error;
    assign cache_channel.rdata = memory_channel.rdata;

    assign vga_channel.read_valid = memory_channel.read_valid & (read_owner == VGA) & !read_owner_empty;
    assign vga_channel.read_error = memory_channel.read_error;
    assign vga_channel.rdata = memory_channel.rdata;

    assign cache_channel.write_valid = memory_channel.write_valid & (write_owner == CACHE) & !write_owner_empty;
    assign cache_channel.write_error = memory_channel.write_error;
    
    assign vga_channel.write_valid = memory_channel.write_valid & (write_owner == VGA) & !write_owner_empty;
    assign vga_channel.write_error = memory_channel.write_error;


//====================================================================================
//      ASSERTIONS
//====================================================================================

`ifndef SYNTHESIS

    assert property (@(posedge clk_i)
        disable iff (!rst_n_i)
        memory_channel.read_valid |-> !read_owner_empty)
        else $error("DDR read response has no owner");

    assert property (@(posedge clk_i)
        disable iff (!rst_n_i)
        memory_channel.write_valid |-> !write_owner_empty)
        else $error("DDR write completion has no owner");

    assert property (@(posedge clk_i)
        disable iff (!rst_n_i)
        !cache_empty & !vga_empty & vga_urgent_i &
        (vga_consecutive >= VGA_MAX_CONSECUTIVE) & memory_channel.ready |->
        cache_read)
        else $error("DDR arbiter starved the CPU during VGA urgency");

`endif

endmodule : ddr_arbiter

`endif
