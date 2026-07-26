// SD card model used by the full-SoC Verilator testbench.
//
// The protocol/PHY model is the same one used by vp/blocks/sd. Its Wishbone
// storage port is connected to a C++ backing store so images larger than the
// small 4 KiB VP test BRAM can be loaded without making the waveform enormous.

module sd_card_model (
    input  logic       clk_100_i,
    input  logic       rst_n_i,
    input  logic       sd_clk_i,
    inout  tri         sd_cmd_io,
    inout  tri   [3:0] sd_data_io
);

    logic clk_50;

    always_ff @(posedge clk_100_i or negedge rst_n_i) begin
        if (!rst_n_i)
            clk_50 <= 1'b0;
        else
            clk_50 <= ~clk_50;
    end

    logic       sd_cmd_i;
    logic       sd_cmd_o;
    logic       sd_cmd_t;
    logic [3:0] sd_dat_i;
    logic [3:0] sd_dat_o;
    logic [3:0] sd_dat_t;

    logic        wbm_clk;
    logic [31:0] wbm_addr;
    logic [31:0] wbm_read_data;
    logic [31:0] wbm_write_data;
    logic [3:0]  wbm_select;
    logic        wbm_cycle;
    logic        wbm_strobe;
    logic        wbm_write;
    logic        wbm_ack;
    logic [2:0]  wbm_cycle_type;
    logic [1:0]  wbm_burst_type;

    sd_top sd_model (
        .clk_50        ( clk_50          ),
        .clk_100       ( clk_100_i       ),
        .clk_200       ( 1'b0            ),
        .reset_n       ( rst_n_i         ),
        .sd_clk        ( sd_clk_i        ),
        .sd_cmd_i      ( sd_cmd_i        ),
        .sd_cmd_o      ( sd_cmd_o        ),
        .sd_cmd_t      ( sd_cmd_t        ),
        .sd_dat_i      ( sd_dat_i        ),
        .sd_dat_o      ( sd_dat_o        ),
        .sd_dat_t      ( sd_dat_t        ),
        .wbm_clk_o     ( wbm_clk         ),
        .wbm_adr_o     ( wbm_addr        ),
        .wbm_dat_i     ( wbm_read_data   ),
        .wbm_dat_o     ( wbm_write_data  ),
        .wbm_sel_o     ( wbm_select      ),
        .wbm_cyc_o     ( wbm_cycle       ),
        .wbm_stb_o     ( wbm_strobe      ),
        .wbm_we_o      ( wbm_write       ),
        .wbm_ack_i     ( wbm_ack         ),
        .wbm_cti_o     ( wbm_cycle_type  ),
        .wbm_bte_o     ( wbm_burst_type  ),
        .opt_enable_hs ( 1'b0            )
    );

    assign sd_cmd_io = (sd_cmd_t == 1'b0) ? sd_cmd_o : 1'bz;
    assign sd_cmd_i  = sd_cmd_io;

    genvar data_bit;
    generate
        for (data_bit = 0; data_bit < 4; data_bit++) begin : sd_data_connection
            assign sd_data_io[data_bit] =
                (sd_dat_t[data_bit] == 1'b0) ? sd_dat_o[data_bit] : 1'bz;
        end
    endgenerate

    assign sd_dat_i = sd_data_io;

    import "DPI-C" function int unsigned zenith_sd_read_word(
        input int unsigned byte_addr
    );

    import "DPI-C" function void zenith_sd_write_word(
        input int unsigned byte_addr,
        input int unsigned data,
        input int unsigned strobe
    );

    always_ff @(posedge wbm_clk or negedge rst_n_i) begin
        if (!rst_n_i) begin
            wbm_ack       <= 1'b0;
            wbm_read_data <= 32'b0;
        end else begin
            wbm_ack <= 1'b0;

            if (wbm_cycle && wbm_strobe && !wbm_ack) begin
                wbm_ack <= 1'b1;

                if (wbm_write)
                    zenith_sd_write_word(wbm_addr, wbm_write_data,
                                         {28'b0, wbm_select});
                else
                    wbm_read_data <= zenith_sd_read_word(wbm_addr);
            end
        end
    end

endmodule : sd_card_model
