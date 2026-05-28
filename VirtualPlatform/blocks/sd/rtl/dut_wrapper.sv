`include "sd_top.v"

module dut_wrapper (
    input  logic        clk,
    input  logic        rst_n,

    input  logic        write_i,
    input  logic [31:0] write_address_i,
    input  logic [31:0] write_data_i,
    input  logic [3:0]  write_strobe_i,
    output logic        write_done_o,
    output logic        write_error_o,

    input  logic        read_i,
    input  logic [31:0] read_address_i,
    output logic [31:0] read_data_o,
    output logic        read_done_o,
    output logic        read_error_o,

    output logic        interrupt_o
);

    
    tri1 sd_cmd_io;
    tri1 [3:0] sd_data_io;

    logic sd_cd_n_i;
    logic sd_reset_n_o;
    logic sd_clk_o;

    sd dut (
        .clk_i   ( clk   ),
        .rst_n_i ( rst_n ),

        .interrupt_o ( interrupt_o ),

        .write_i         ( write_i         ),
        .write_address_i ( write_address_i ),
        .write_data_i    ( write_data_i    ),
        .write_strobe_i  ( write_strobe_i  ),
        .write_done_o    ( write_done_o    ),
        .write_error_o   ( write_error_o   ),

        .read_i         ( read_i         ),
        .read_address_i ( read_address_i ),
        .read_data_o    ( read_data_o    ),
        .read_done_o    ( read_done_o    ),
        .read_error_o   ( read_error_o   ),

        .sd_cd_n_i    ( sd_cd_n_i    ),
        .sd_cmd_io    ( sd_cmd_io    ),
        .sd_data_io   ( sd_data_io   ),
        .sd_reset_n_o ( sd_reset_n_o ),
        .sd_clk_o     ( sd_clk_o     ) 
    );

    assign sd_cd_n_i = 1'b0;
    

//====================================================================================
//      SD CARD MODEL
//====================================================================================

    /* Wishbone Interface */
    logic        wbm_clk_o;
    logic [31:0] wbm_adr_o;
    logic [31:0] wbm_dat_i = 32'h0;
    logic [31:0] wbm_dat_o;
    logic [3:0]  wbm_sel_o;
    logic        wbm_cyc_o;
    logic        wbm_stb_o;
    logic        wbm_we_o;
    logic        wbm_ack_i = 1'b0;
    logic [2:0]  wbm_cti_o;
    logic [1:0]  wbm_bte_o;
        
    bram_whishbone SD_memory (
        .clk ( wbm_clk_o ),
        .rst ( rst_n     ),

        .wbm_adr_i  ( wbm_adr_o[11:2] ),
        .wbm_dat_i  ( wbm_dat_o       ),
        .wbm_dat_o  ( wbm_dat_i       ),
        .wbm_we_i   ( wbm_we_o        ),
        .wbm_stb_i  ( wbm_stb_o       ),
        .wbm_cyc_i  ( wbm_cyc_o       ),
        .wbm_sel_i  ( wbm_sel_o       ),
        .wbm_ack_o  ( wbm_ack_i       ),
        .wbm_cti_i  ( wbm_cti_o       ),
        .wbm_bte_i  ( wbm_bte_o       )
    );


    /* Clocks for the SD Card */
    logic clk_50MHz = 0;
    logic clk_100MHz = 0;
    logic clk_200MHz = 0;
    
    /* Clock generation */
    always #10ns clk_50MHz  <= ~clk_50MHz;
    always #5ns  clk_100MHz <= ~clk_100MHz;

    /* Card SD Interface */
    logic sd_cmd_i, sd_cmd_o, sd_cmd_t;
    logic [3:0] sd_dat_i, sd_dat_o, sd_dat_t;

    wire sd_cmd_wire; wire [3:0] sd_dat_wire;

    sd_top sd_model (
        .clk_50      ( clk_50MHz  ),
        .clk_100     ( clk_100MHz ),
        .clk_200     ( 1'b0       ),
        .reset_n     ( rst_n      ),

        .sd_clk      ( sd_clk_o ),

        .sd_cmd_i    ( sd_cmd_i ),
        .sd_cmd_o    ( sd_cmd_o ),
        .sd_cmd_t    ( sd_cmd_t ),

        .sd_dat_i    ( sd_dat_i ),
        .sd_dat_o    ( sd_dat_o ),
        .sd_dat_t    ( sd_dat_t ),

        .wbm_clk_o ( wbm_clk_o ),
        .wbm_adr_o ( wbm_adr_o ),
        .wbm_dat_i ( wbm_dat_i ),
        .wbm_dat_o ( wbm_dat_o ),
        .wbm_sel_o ( wbm_sel_o ),
        .wbm_cyc_o ( wbm_cyc_o ),
        .wbm_stb_o ( wbm_stb_o ),
        .wbm_we_o  ( wbm_we_o  ),
        .wbm_ack_i ( wbm_ack_i ),
        .wbm_cti_o ( wbm_cti_o ),
        .wbm_bte_o ( wbm_bte_o ),

        .opt_enable_hs ( '0 )
    );


//====================================================================================
//      SD CMD/DAT shared lines with pull-up
//====================================================================================

    logic host_cmd_drive;
    logic host_cmd_value;

    logic card_cmd_drive;
    logic card_cmd_value;

    logic sd_cmd_line;

    assign host_cmd_drive = dut.command_controller.tristate_enable_flop;
    assign host_cmd_value = dut.command_controller.cmd_bit_flop;

    assign card_cmd_drive = (sd_cmd_t == 1'b0);
    assign card_cmd_value = sd_cmd_o;

    /* Bus resolution with pull-up */
    assign sd_cmd_line = card_cmd_drive ? card_cmd_value :
                        host_cmd_drive ? host_cmd_value :
                        1'b1;   // pull-up

    assign sd_cmd_i  = sd_cmd_line;
    assign sd_cmd_io = sd_cmd_line;


    logic card_force_busy;
    assign card_force_busy = (sd_model.isdl.data_state == 7'd24);

    logic host_dat_drive;
    logic [3:0] host_dat_value;

    logic card_dat_drive;
    logic [3:0] card_dat_value;

    logic [3:0] sd_dat_line;

    assign host_dat_drive = dut.data_controller.tristate_enable_flop;
    assign host_dat_value = dut.data_controller.data_line_flop;

    assign card_dat_drive = (sd_dat_t != 4'b1111);
    assign card_dat_value = sd_dat_o;

    /* Bus resolution with pull-up */
    genvar k;
    
    generate
        for (k = 0; k < 4; k++) begin : sd_dat_resolve
            assign sd_dat_line[k] =
                (k == 0 && card_force_busy) ? 1'b0 :
                (sd_dat_t[k] == 1'b0)       ? sd_dat_o[k] :
                (host_dat_drive)            ? host_dat_value[k] :
                                            1'b1;
        end
    endgenerate

    assign sd_dat_i   = sd_dat_line;
    assign sd_data_io = sd_dat_line;

    assign sd_dat_i    = sd_dat_line;
    assign sd_data_io  = sd_dat_line;

endmodule