`ifndef DDR_INTERFACE_SV 
    `define DDR_INTERFACE_SV

`include "../../../System/asynchronous_buffer.sv"
`include "../../../System/flag_synchronizer.sv"
`include "../../../System/synchronizer.sv"

module ddr_interface (
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


    /* Addresses */
    input logic [26:0] write_address_i, 
    input logic [26:0] read_address_i, 
    
    /* Commands */
    input logic write_i, 
    input logic read_i, 
    input logic pull_data_i,

    /* Status */
    output logic read_valid_o,
    output logic ready_o,

    /* PCM data */
    input logic [63:0] write_data_i,
    output logic [63:0] read_data_o
);


//====================================================================================
//      READ ADDRESS LOGIC
//====================================================================================

    /* Address generation, DDR logic read 64 bits per command */
    logic [26:0] read_address, write_address;

    /* Align to 64 bit word and to DDR memory configuration */
    assign read_address = {1'b0, read_address_i[26:3], 2'b0};

    assign write_address = {1'b0, write_address_i[26:3], 2'b0};



//====================================================================================
//      CLOCK DOMAIN CROSSING LOGIC
//====================================================================================

    /* System clock is different from the user interface of the DDR controller! */

    /* User interface timing signals */
    logic ui_clk, ui_rst;


    typedef struct packed {
        /* Command type: 0 = Write 1 = Read */
        logic command;

        /* DRAM Address */
        logic [26:0] address; 
    } dram_command_t;


    dram_command_t write_packet, read_packet; logic command_empty, read_fifo_command; 

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
        .full_o  (               ),

        /* Data */
        .write_data_i ( write_packet ),
        .read_data_o  ( read_packet  )
    );

        /* Build the command packet */
        always_comb begin
            if (read_i) begin
                write_packet.command = 1'b1;
                write_packet.address = read_address;
            end else begin
                write_packet.command = 1'b0;
                write_packet.address = write_address;
            end
        end



    logic [63:0] ddr_write_data; logic write_data_empty, read_wr_data_fifo;

    /* Asynchronous buffer to pass the data samples from SYSTEM_CLOCK domain to UI_CLOCK domain */
    asynchronous_buffer #(16, 64) write_data_buffer (
        /* Global signals */
        .write_clk_i  ( clk_i   ),
        .write_rstn_i ( rst_n_i ),
        .read_clk_i   ( ui_clk  ),
        .read_rstn_i  ( !ui_rst ),

        /* Control signals */
        .write_i ( write_i           ),
        .read_i  ( read_wr_data_fifo ),

        /* Status signals */
        .empty_o ( write_data_empty ),
        .full_o  (                  ),

        /* Data */
        .write_data_i ( write_data_i   ),
        .read_data_o  ( ddr_write_data )
    );



    logic [63:0] ddr_read_data; logic write_fifo;

    /* Buffer to pass read data from UI_CLOCK domain to SYSTEM_CLOCK domain */
    asynchronous_buffer #(16, 64) read_data_buffer (
        /* Global signals */
        .write_clk_i  ( ui_clk  ),
        .write_rstn_i ( !ui_rst ),
        .read_clk_i   ( clk_i   ),
        .read_rstn_i  ( rst_n_i ),

        /* Control signals */
        .write_i ( write_fifo  ),
        .read_i  ( pull_data_i ),

        /* Status signals */
        .empty_o ( read_valid ),
        .full_o  (            ),

        /* Data */
        .write_data_i ( ddr_read_data ),
        .read_data_o  ( read_data_o   )
    );

    synchronizer valid_synchronizer (
        /* Global signals */
        .clk_i   ( clk_i   ),
        .rst_n_i ( rst_n_i ),

        .signal_i ( !read_valid  ),
        .sync_o   ( read_valid_o )
    );


//====================================================================================
//      MEMORY CONTROLLER
//====================================================================================

    logic [26:0] app_addr; logic [2:0] app_cmd; logic app_en, app_rdy; 
    logic [63:0] app_wdf_data; logic [7:0] app_wdf_mask; logic app_wdf_end, app_wdf_rdy, app_wdf_wren;
    logic [63:0] app_rd_data; logic app_rd_data_end, app_rd_data_valid;
    logic init_calib_complete;

    /* Vivado IP */
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

    typedef enum logic {CMD_IDLE, CMD_SEND} command_fsm_states_t;
    typedef enum logic [1:0] {DAT_IDLE, DAT_WRITE, DAT_LAST} data_fsm_states_t;

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


    assign app_cmd = {2'b0, read_packet.command};
    assign app_addr = read_packet.address;

        always_comb begin : command_fsm_logic
            /* Default Values */
            cmd_state_NXT = cmd_state_CRT;

            app_en = 1'b0;
            read_fifo_command = 1'b0;

            case (cmd_state_CRT)
                CMD_IDLE: begin
                    if (!command_empty) begin
                        /* Start memory request if the FIFO is filled
                         * and the external module issue a done signal */
                        read_fifo_command = 1'b1;

                        cmd_state_NXT = CMD_SEND;
                    end
                end

                CMD_SEND: begin
                    app_en = 1'b1;

                    if (app_rdy) begin
                        /* Go immediately IDLE if controller accepts the command */
                        cmd_state_NXT = CMD_IDLE;
                    end
                end
            endcase
        end : command_fsm_logic



    logic wr_data_end_CRT, wr_data_end_NXT;

        always_ff @(posedge ui_clk) begin
            if (ui_rst) begin 
                wr_data_end_CRT <= '0;
            end else begin 
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
            app_wdf_mask = '0;

            read_wr_data_fifo = 1'b0;

            case (dat_state_CRT)
                DAT_IDLE: begin
                    if (!write_data_empty) begin
                        if (!read_packet.command) begin
                            dat_state_NXT = DAT_WRITE;

                            /* If command is a write start reading the data fifo buffer */
                            read_wr_data_fifo = 1'b1;
                        end
                    end

                    wr_data_end_NXT = 1'b0;
                end

                DAT_WRITE: begin
                    app_wdf_wren = 1'b1;
                    app_wdf_end = wr_data_end_CRT;
                    app_wdf_data = ddr_write_data;

                    /* Every byte is valid */
                    app_wdf_mask = '0;

                    if (app_wdf_rdy) begin
                        /* Read only if FIFO is not empty */
                        read_wr_data_fifo = !write_data_empty;

                        /* Data end must alterate between 1 and 0 */
                        wr_data_end_NXT = !wr_data_end_CRT;

                        if (write_data_empty) begin
                            if (wr_data_end_NXT) begin
                                /* Last transaction on UI interface since it is 128 bits */
                                dat_state_NXT = DAT_LAST;
                            end else begin
                                /* Once finished writing all the data, go to idle */
                                dat_state_NXT = DAT_IDLE;
                            end
                        end
                    end
                end

                DAT_LAST: begin
                    /* Last transaction is invalid, mask all the bytes */
                    app_wdf_wren = 1'b1;
                    app_wdf_end = 1'b1;
                    app_wdf_data = '0;
                    app_wdf_mask = '1;

                    if (app_wdf_rdy) begin
                        dat_state_NXT = DAT_IDLE;
                    end
                end
            endcase 
        end : data_write_logic

        always_comb begin : data_read_logic
            /* Default Values */
            write_fifo = 1'b0;
            ddr_read_data = '0;

            if (app_rd_data_valid) begin
                write_fifo = 1'b1;
                ddr_read_data = app_rd_data;
            end
        end : data_read_logic


    logic ready;

    assign ready = init_calib_complete;
    

    synchronizer ready_synchronizer (
        /* Global signals */
        .clk_i   ( clk_i ),
        .rst_n_i ( 1'b1  ),

        .signal_i ( ready   ),
        .sync_o   ( ready_o )
    );
    
endmodule : ddr_interface

`endif