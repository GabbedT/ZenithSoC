`ifndef DDR_MEMORY_INTERFACE_SV
    `define DDR_MEMORY_INTERFACE_SV

`include "../../System/asynchronous_buffer.sv"
`include "../../System/flag_synchronizer.sv"

// TODO: Continuous sending of command, external interface can write in fifo whenever it wants

module ddr_memory_interface (
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


    /* Common address */
    input logic [26:0] address_i, 
    
    /* Command interface */
    input logic write_i, 
    input logic read_i, 

    /* Data interface */
    input logic push_i, 
    input logic pull_i, 
    input logic [63:0] write_data_i,
    input logic [1:0] write_mask_i,
    output logic [63:0] read_data_o,
    output logic read_valid_o,

    /* Status */
    input logic done_i,
    output logic ready_o
);

//====================================================================================
//      CLOCK DOMAIN CROSSING LOGIC
//====================================================================================

    /* User interface timing signals */
    logic ui_clk, ui_rst;


    typedef struct packed {
        /* Command type: 0 = Write 1 = Read */
        logic command;

        /* DRAM Address */
        logic [26:0] address; 
    } dram_command_t;


    dram_command_t write_packet, read_packet; logic command_empty, command_full, read_fifo_command;

    asynchronous_buffer #(16, $bits(dram_command_t)) command_buffer (
        /* Global signals */
        .write_clk_i  ( clk_i   ),
        .write_rstn_i ( rst_n_i ),
        .read_clk_i   ( ui_clk  ),
        .read_rstn_i  ( !ui_rst ),

        /* Control signals */
        .write_i ( write_i | read_i  ),
        .read_i  ( read_fifo_command ),

        /* Status signals */
        .empty_o ( command_empty ),
        .full_o  ( command_full  ),

        /* Data */
        .write_data_i ( write_packet ),
        .read_data_o  ( read_packet  )
    );

    assign write_packet = {read_i, address_i};


    typedef struct packed {
        /* Data to write */
        logic [63:0] data;

        /* Data mask */
        logic [1:0] mask;
    } dram_data_t;

    dram_data_t write_data; logic write_data_empty, write_data_full, read_fifo;

    asynchronous_buffer #(16, $bits(dram_data_t)) write_data_buffer (
        /* Global signals */
        .write_clk_i  ( clk_i   ),
        .write_rstn_i ( rst_n_i ),
        .read_clk_i   ( ui_clk  ),
        .read_rstn_i  ( !ui_rst ),

        /* Control signals */
        .write_i ( push_i    ),
        .read_i  ( read_fifo ),

        /* Status signals */
        .empty_o ( write_data_empty ),
        .full_o  ( write_data_full  ),

        /* Data */
        .write_data_i ( {write_data_i, write_mask_i} ),
        .read_data_o  ( write_data                   )
    );


    logic [63:0] read_data; logic read_data_empty, read_data_full, write_fifo;

    asynchronous_buffer #(16, 64) read_data_buffer (
        /* Global signals */
        .write_clk_i  ( ui_clk  ),
        .write_rstn_i ( !ui_rst ),
        .read_clk_i   ( clk_i   ),
        .read_rstn_i  ( rst_n_i ),

        /* Control signals */
        .write_i ( write_fifo ),
        .read_i  ( pull_i     ),

        /* Status signals */
        .empty_o ( read_data_empty ),
        .full_o  ( read_data_full  ),

        /* Data */
        .write_data_i ( read_data   ),
        .read_data_o  ( read_data_o )
    );

    assign read_valid_o = !read_data_empty;


    logic done_sync;

    flag_synchronizer done_synchronizer (
        /* Global signals */
        .clk_A_i  ( clk_i   ),
        .rstn_A_i ( rst_n_i ),
        .clk_B_i  ( ui_clk  ),
        .rstn_B_i ( !ui_rst ),

        /* Clock domain A signals */
        .flag_A_i ( done_i    ),
        .flag_B_o ( done_sync )
    );


//====================================================================================
//      MEMORY CONTROLLER
//====================================================================================

    logic [26:0] app_addr; logic [3:0] app_cmd; logic app_en, app_rdy; 
    logic [63:0] app_wdf_data; logic [7:0] app_wdf_mask; logic app_wdf_end, app_wdf_rdy, app_wdf_wren;
    logic [63:0] app_rd_data; logic app_rd_data_end, app_rd_data_valid;
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

        .ui_clk            ( ui_clk ),
        .ui_clk_sync_rst   ( ui_rst ),
        
        .sys_clk_i ( mem_clk_i   ),
        .sys_rst   ( mem_rst_n_i ) 
    );


//====================================================================================
//      FSM LOGIC
//====================================================================================

    typedef enum logic [2:0] {CMD_IDLE, CMD_TYPE, CMD_SEND} command_fsm_states_t;
    typedef enum logic [1:0] {DAT_IDLE, DAT_WRITE} data_fsm_states_t;

    command_fsm_states_t cmd_state_CRT, cmd_state_NXT;
    data_fsm_states_t dat_state_CRT, dat_state_NXT;

        always_ff @(posedge ui_clk) begin
            if (ui_rst | !init_calib_complete) begin 
                cmd_state_CRT <= CMD_IDLE;
                dat_state_CRT <= DAT_IDLE;
            end else begin 
                cmd_state_CRT <= cmd_state_NXT;
                dat_state_CRT <= dat_state_NXT;
            end 
        end 


        always_comb begin : command_fsm_logic
            /* Default Values */
            cmd_state_NXT = cmd_state_CRT;

            app_en = 1'b0;
            app_cmd = {2'b0, read_packet.command};
            app_addr = read_packet.address;
            read_fifo_command = 1'b0;

            case (cmd_state_CRT)
                CMD_IDLE: begin
                    if (!command_empty & done_sync) begin
                        /* Start memory request if the FIFO is filled
                         * and the external module issue a done signal */
                        read_fifo_command = 1'b1;

                        cmd_state_NXT = CMD_TYPE;
                    end
                end

                CMD_TYPE: begin
                    app_en = 1'b1;
                    app_addr = read_packet.address;

                    if (app_rdy) begin
                        /* Go immediately IDLE if the transaction requested is 1 */
                        cmd_state_NXT = command_empty ? CMD_IDLE : CMD_SEND;

                        read_fifo_command = !command_empty;
                    end
                end

                CMD_SEND: begin
                    app_en = 1'b1;
                    app_addr = read_packet.address;

                    if (app_rdy) begin
                        if (command_empty) begin
                            /* Once finished writing all the data, go to idle */
                            cmd_state_NXT = CMD_IDLE;
                        end

                        /* Read only if FIFO is not empty */
                        read_fifo_command = !command_empty;
                    end
                end
            endcase
        end : command_fsm_logic


    logic rd_data_end_CRT, rd_data_end_NXT, wr_data_end_CRT, wr_data_end_NXT;

        always_ff @(posedge ui_clk) begin
            if (ui_rst) begin 
                rd_data_end_CRT <= '0;
                wr_data_end_CRT <= '0;
            end else begin 
                rd_data_end_CRT <= rd_data_end_NXT;
                wr_data_end_CRT <= wr_data_end_NXT;
            end 
        end 


        always_comb begin : data_write_logic 
            /* Default Values */
            dat_state_NXT = dat_state_CRT;
            wr_data_end_NXT = wr_data_end_CRT;

            app_wdf_wren = 1'b0;
            app_wdf_end = 1'b0;
            app_wdf_data = '0;

            read_fifo = 1'b0;

            case (dat_state_CRT)
                DAT_IDLE: begin
                    if (cmd_state_CRT == CMD_TYPE) begin
                        if (!read_packet.command) begin
                            dat_state_NXT = DAT_WRITE;

                            read_fifo = 1'b1;
                        end
                    end

                    wr_data_end_NXT = 1'b0;
                end

                DAT_WRITE: begin
                    app_wdf_wren = 1'b1;
                    app_wdf_end = wr_data_end_CRT;
                    app_wdf_data = write_data.data;

                    case (write_data.mask)
                        2'b00: app_wdf_mask = '0;

                        2'b01: app_wdf_mask = {4'h0, 4'hF};

                        2'b10: app_wdf_mask = {4'hF, 4'h0};
                    endcase 

                    if (app_wdf_rdy) begin
                        /* Read only if FIFO is not empty */
                        read_fifo = !write_data_empty;

                        /* Data end must alterate between 1 and 0 */
                        wr_data_end_NXT = !wr_data_end_CRT;

                        if (write_data_empty) begin
                            /* Once finished writing all the data, go to idle */
                            dat_state_NXT = DAT_IDLE;
                        end
                    end
                end
            endcase 
        end : data_write_logic

        always_comb begin : data_read_logic
            /* Default Values */
            rd_data_end_NXT = rd_data_end_CRT;

            write_fifo = 1'b0;
            read_data = '0;

            if (app_rd_data_valid) begin
                rd_data_end_NXT = app_rd_data_end;

                write_fifo = rd_data_end_CRT != app_rd_data_end;
                read_data = app_rd_data;
            end
        end : data_read_logic


    logic ready;

    assign ready = (dat_state_CRT == DAT_IDLE) & (cmd_state_CRT == CMD_IDLE) & init_calib_complete;
    

    synchronizer ready_synchronizer (
        /* Global signals */
        .clk_i   ( clk_i   ),
        .rst_n_i ( rst_n_i ),

        .signal_i ( ready   ),
        .sync_o   ( ready_o )
    );

endmodule : ddr_memory_interface

`endif 