`ifndef DDR_MEMORY_INTERFACE_SV
    `define DDR_MEMORY_INTERFACE_SV

module ddr_memory_interface #(
    parameter REQUEST_BUFFER_SIZE = 32,
    parameter RESPONSE_BUFFER_SIZE = 32
) (
    input logic clk_i,
    input logic rst_n_i,
    input logic mem_clk_i,
    input logic mem_rst_n_i,

    /* Data */
    inout logic [15:0] ddr2_dq_io,
    output logic [1:0] ddr2_dm_o,

    /* Data strobe pair */
    inout logic [1:0] ddr2_dqs_n_io,
    inout logic [1:0] ddr2_dqs_p_io,

    /* Addreses */
    output logic [12:0] ddr2_addr_o,
    output logic [2:0] ddr2_ba_o,

    /* Enable signals */
    output logic ddr2_ras_n_o,
    output logic ddr2_cas_n_o,
    output logic ddr2_we_n_o,
    output logic ddr2_cke_o,

    /* Clock pair */
    output logic ddr2_ck_p_o,
    output logic ddr2_ck_n_o,

    /* Chip select */
    output logic ddr2_cs_n_o,
    output logic ddr2_odt_o,

    dev2ddr_interface.slave ddr_channel,

    output logic start_o
);


//====================================================================================
//      CLOCK DOMAIN CROSSING LOGIC
//====================================================================================

    logic ui_clk, ui_rst;

    typedef struct packed {
        logic operation;
        logic [26:0] address;
        logic [127:0] data;
        logic [15:0] strobe;
    } request_packet_t;

    request_packet_t request_write_packet, request_read_packet;
    logic request_write, request_read, request_empty, request_full;

    /* Requests cross once and remain 128-bit transactions in the UI domain. */
    assign request_write_packet.operation = ddr_channel.trx_type;
    assign request_write_packet.address = ddr_channel.trx_addr;
    assign request_write_packet.data = ddr_channel.wdata;
    assign request_write_packet.strobe = ddr_channel.wstrobe;
    assign request_write = ddr_channel.trx_req & ddr_channel.ready;

    asynchronous_buffer #(
        .BUFFER_DEPTH           ( REQUEST_BUFFER_SIZE   ),
        .DATA_WIDTH             ( $bits(request_packet_t) ),
        .FIRST_WORD_FALL_TROUGH ( 1                     )
    ) request_buffer (
        .write_clk_i  ( clk_i   ),
        .write_rstn_i ( rst_n_i ),
        .read_clk_i   ( ui_clk  ),
        .read_rstn_i  ( !ui_rst ),

        .write_i ( request_write ),
        .read_i  ( request_read  ),

        .empty_o ( request_empty ),
        .full_o  ( request_full  ),

        .write_data_i ( request_write_packet ),
        .read_data_o  ( request_read_packet  )
    );

    typedef struct packed {
        logic error;
        logic [127:0] data;
    } read_response_t;

    read_response_t read_response_write_packet, read_response_read_packet;
    logic read_response_write, read_response_read;
    logic read_response_empty, read_response_full;

    asynchronous_buffer #(
        .BUFFER_DEPTH           ( RESPONSE_BUFFER_SIZE   ),
        .DATA_WIDTH             ( $bits(read_response_t) ),
        .FIRST_WORD_FALL_TROUGH ( 1                      )
    ) read_response_buffer (
        .write_clk_i  ( ui_clk  ),
        .write_rstn_i ( !ui_rst ),
        .read_clk_i   ( clk_i   ),
        .read_rstn_i  ( rst_n_i ),

        .write_i ( read_response_write ),
        .read_i  ( read_response_read  ),

        .empty_o ( read_response_empty ),
        .full_o  ( read_response_full  ),

        .write_data_i ( read_response_write_packet ),
        .read_data_o  ( read_response_read_packet  )
    );

    logic write_response_write, write_response_read;
    logic write_response_error;
    logic write_response_empty, write_response_full;

    asynchronous_buffer #(
        .BUFFER_DEPTH           ( RESPONSE_BUFFER_SIZE ),
        .DATA_WIDTH             ( 1                    ),
        .FIRST_WORD_FALL_TROUGH ( 1                    )
    ) write_response_buffer (
        .write_clk_i  ( ui_clk  ),
        .write_rstn_i ( !ui_rst ),
        .read_clk_i   ( clk_i   ),
        .read_rstn_i  ( rst_n_i ),

        .write_i ( write_response_write ),
        .read_i  ( write_response_read  ),

        .empty_o ( write_response_empty ),
        .full_o  ( write_response_full  ),

        .write_data_i ( write_response_error ),
        .read_data_o  ( ddr_channel.write_error )
    );

    assign read_response_read = !read_response_empty;
    assign write_response_read = !write_response_empty;

    /* Responses are consumed immediately after crossing to the system clock. */
    assign ddr_channel.read_valid = !read_response_empty;
    assign ddr_channel.read_error = read_response_read_packet.error;
    assign ddr_channel.rdata = read_response_read_packet.data;
    assign ddr_channel.write_valid = !write_response_empty;


//====================================================================================
//      RESPONSE CAPACITY CREDITS
//====================================================================================

    logic read_credit_write, read_credit_read;
    logic read_credit_empty, read_credit_full, read_credit_data;
    logic write_credit_write, write_credit_read;
    logic write_credit_empty, write_credit_full, write_credit_data;

    /* Returned credits release slots previously reserved in the UI domain. */
    assign read_credit_write = read_response_read;
    assign write_credit_write = write_response_read;
    assign read_credit_read = !read_credit_empty;
    assign write_credit_read = !write_credit_empty;

    asynchronous_buffer #(
        .BUFFER_DEPTH           ( RESPONSE_BUFFER_SIZE ),
        .DATA_WIDTH             ( 1                    ),
        .FIRST_WORD_FALL_TROUGH ( 1                    )
    ) read_credit_buffer (
        .write_clk_i  ( clk_i   ),
        .write_rstn_i ( rst_n_i ),
        .read_clk_i   ( ui_clk  ),
        .read_rstn_i  ( !ui_rst ),

        .write_i ( read_credit_write ),
        .read_i  ( read_credit_read  ),

        .empty_o ( read_credit_empty ),
        .full_o  ( read_credit_full  ),

        .write_data_i ( 1'b1             ),
        .read_data_o  ( read_credit_data )
    );

    asynchronous_buffer #(
        .BUFFER_DEPTH           ( RESPONSE_BUFFER_SIZE ),
        .DATA_WIDTH             ( 1                    ),
        .FIRST_WORD_FALL_TROUGH ( 1                    )
    ) write_credit_buffer (
        .write_clk_i  ( clk_i   ),
        .write_rstn_i ( rst_n_i ),
        .read_clk_i   ( ui_clk  ),
        .read_rstn_i  ( !ui_rst ),

        .write_i ( write_credit_write ),
        .read_i  ( write_credit_read  ),

        .empty_o ( write_credit_empty ),
        .full_o  ( write_credit_full  ),

        .write_data_i ( 1'b1              ),
        .read_data_o  ( write_credit_data )
    );

    localparam CREDIT_WIDTH = $clog2(RESPONSE_BUFFER_SIZE + 1);
    logic [CREDIT_WIDTH - 1:0] read_reserved, write_reserved;
    logic reserve_read, reserve_write;


//====================================================================================
//      MEMORY CONTROLLER
//====================================================================================

    logic [26:0] app_addr;
    logic [2:0] app_cmd;
    logic app_en, app_rdy;
    logic [127:0] app_wdf_data;
    logic [15:0] app_wdf_mask;
    logic app_wdf_end, app_wdf_rdy, app_wdf_wren;
    logic [127:0] app_rd_data;
    logic app_rd_data_end, app_rd_data_valid;
    logic init_calib_complete;

    ddr_controller ddr_memory_controller (
        .ddr2_addr  ( ddr2_addr_o   ),
        .ddr2_ba    ( ddr2_ba_o     ),
        .ddr2_cas_n ( ddr2_cas_n_o  ),
        .ddr2_ck_n  ( ddr2_ck_n_o   ),
        .ddr2_ck_p  ( ddr2_ck_p_o   ),
        .ddr2_cke   ( ddr2_cke_o    ),
        .ddr2_ras_n ( ddr2_ras_n_o  ),
        .ddr2_we_n  ( ddr2_we_n_o   ),
        .ddr2_dq    ( ddr2_dq_io    ),
        .ddr2_dqs_n ( ddr2_dqs_n_io ),
        .ddr2_dqs_p ( ddr2_dqs_p_io ),
        .ddr2_cs_n  ( ddr2_cs_n_o   ),
        .ddr2_dm    ( ddr2_dm_o     ),
        .ddr2_odt   ( ddr2_odt_o    ),

        .init_calib_complete ( init_calib_complete ),

        .app_addr          ( app_addr          ),
        .app_cmd           ( app_cmd           ),
        .app_en            ( app_en            ),
        .app_wdf_data      ( app_wdf_data      ),
        .app_wdf_end       ( app_wdf_end       ),
        .app_wdf_wren      ( app_wdf_wren      ),
        .app_wdf_mask      ( app_wdf_mask      ),
        .app_rd_data       ( app_rd_data       ),
        .app_rd_data_end   ( app_rd_data_end   ),
        .app_rd_data_valid ( app_rd_data_valid ),
        .app_rdy           ( app_rdy           ),
        .app_wdf_rdy       ( app_wdf_rdy       ),
        .app_sr_req        ( 1'b0              ),
        .app_ref_req       ( 1'b0              ),
        .app_zq_req        ( 1'b0              ),
        .app_sr_active     (                   ),
        .app_ref_ack       (                   ),
        .app_zq_ack        (                   ),

        .ui_clk          ( ui_clk ),
        .ui_clk_sync_rst ( ui_rst ),

        .sys_clk_i ( mem_clk_i   ),
        .sys_rst   ( mem_rst_n_i )
    );


//====================================================================================
//      STREAMED MIG USER INTERFACE
//====================================================================================

    logic write_command_accepted, write_data_accepted, write_head_reserved;
    logic command_accept, data_accept, write_complete;
    logic aligned_request, read_slot, write_slot;

    assign aligned_request = request_read_packet.address[3:0] == 4'b0;
    assign read_slot = read_reserved < RESPONSE_BUFFER_SIZE;
    assign write_slot = write_reserved < RESPONSE_BUFFER_SIZE;

    always_comb begin
        /* MIG addresses count 16-bit words, while requests use byte addresses. */
        app_addr = {1'b0, request_read_packet.address[26:4], 3'b0};
        app_cmd = request_read_packet.operation ? 3'b000 : 3'b001;
        app_en = 1'b0;

        app_wdf_data = request_read_packet.data;
        app_wdf_mask = ~request_read_packet.strobe;
        app_wdf_end = 1'b1;
        app_wdf_wren = 1'b0;

        request_read = 1'b0;
        reserve_read = 1'b0;
        reserve_write = 1'b0;

        read_response_write = app_rd_data_valid;
        read_response_write_packet.error = 1'b0;
        read_response_write_packet.data = app_rd_data;

        write_response_write = 1'b0;
        write_response_error = 1'b0;

        if (!request_empty & init_calib_complete) begin
            if (!request_read_packet.operation) begin
                if (!aligned_request) begin
                    if (read_slot & !app_rd_data_valid) begin
                        request_read = 1'b1;
                        reserve_read = 1'b1;
                        read_response_write = 1'b1;
                        read_response_write_packet.error = 1'b1;
                        read_response_write_packet.data = '0;
                    end
                end else if (read_slot) begin
                    app_en = 1'b1;

                    if (app_rdy) begin
                        request_read = 1'b1;
                        reserve_read = 1'b1;
                    end
                end
            end else if (!aligned_request) begin
                if (write_slot) begin
                    request_read = 1'b1;
                    reserve_write = 1'b1;
                    write_response_write = 1'b1;
                    write_response_error = 1'b1;
                end
            end else if (write_head_reserved | write_slot) begin
                /* MIG may accept the write command and data independently. */
                reserve_write = !write_head_reserved;
                app_en = !write_command_accepted;
                app_wdf_wren = !write_data_accepted;

                if (write_complete) begin
                    request_read = 1'b1;
                    write_response_write = 1'b1;
                end
            end
        end
    end

    assign command_accept = app_en & app_rdy;
    assign data_accept = app_wdf_wren & app_wdf_rdy;
    assign write_complete = (write_command_accepted | command_accept) & (write_data_accepted | data_accept);

        always_ff @(posedge ui_clk) begin
            if (ui_rst | !init_calib_complete) begin
                write_command_accepted <= 1'b0;
                write_data_accepted <= 1'b0;
                write_head_reserved <= 1'b0;
            end else if (request_read) begin
                write_command_accepted <= 1'b0;
                write_data_accepted <= 1'b0;
                write_head_reserved <= 1'b0;
            end else begin
                if (reserve_write) begin
                    write_head_reserved <= 1'b1;
                end

                if (command_accept) begin
                    write_command_accepted <= 1'b1;
                end

                if (data_accept) begin
                    write_data_accepted <= 1'b1;
                end
            end
        end

        always_ff @(posedge ui_clk) begin
            if (ui_rst | !init_calib_complete) begin
                read_reserved <= '0;
                write_reserved <= '0;
            end else begin
                case ({reserve_read, read_credit_read})
                    2'b10: read_reserved <= read_reserved + 1'b1;
                    2'b01: read_reserved <= read_reserved - 1'b1;
                    default: read_reserved <= read_reserved;
                endcase

                case ({reserve_write, write_credit_read})
                    2'b10: write_reserved <= write_reserved + 1'b1;
                    2'b01: write_reserved <= write_reserved - 1'b1;
                    default: write_reserved <= write_reserved;
                endcase
            end
        end

    logic memory_ready;

    synchronizer ready_synchronizer (
        .clk_i   ( clk_i   ),
        .rst_n_i ( rst_n_i ),

        .signal_i ( init_calib_complete ),
        .sync_o   ( memory_ready        )
    );

    /* Backpressure starts before the CDC request FIFO can overflow. */
    assign ddr_channel.ready = memory_ready & !request_full;
    assign start_o = ddr_channel.trx_req & ddr_channel.ready;


//====================================================================================
//      ASSERTIONS
//====================================================================================

`ifndef SYNTHESIS

    assert property (@(posedge ui_clk)
        disable iff (ui_rst)
        app_rd_data_valid |-> !read_response_full)
        else $error("DDR read response FIFO overflow");

    assert property (@(posedge ui_clk)
        disable iff (ui_rst)
        write_response_write |-> !write_response_full)
        else $error("DDR write response FIFO overflow");

    assert property (@(posedge ui_clk)
        disable iff (ui_rst)
        read_reserved <= RESPONSE_BUFFER_SIZE)
        else $error("DDR read response credits exceeded capacity");

    assert property (@(posedge ui_clk)
        disable iff (ui_rst)
        write_reserved <= RESPONSE_BUFFER_SIZE)
        else $error("DDR write response credits exceeded capacity");

`endif

    logic unused;
    assign unused = app_rd_data_end ^ read_credit_full ^ write_credit_full ^
                    read_credit_data ^ write_credit_data;

endmodule : ddr_memory_interface

`endif
