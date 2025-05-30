`ifndef FLAG_SYNCHRONIZER_SV
    `define FLAG_SYNCHRONIZER_SV

`define _VIVADO_

module flag_synchronizer #(
    /* Number of sync flops */
    parameter FLOP_NUMBER = 3,

    /* Reset value */
    parameter logic [FLOP_NUMBER - 1:0] RESET_VALUE = '0
) (
    /* Global signals */
    input logic clk_A_i,
    input logic rstn_A_i,
    input logic clk_B_i,
    input logic rstn_B_i,

    /* Clock domain A signals */
    input logic flag_A_i,
    output logic flag_B_o
);

    `ifndef _VIVADO_ /* IF VIVADO MACRO IS NOT DEFINED */

    logic flag;

        always_ff @(posedge clk_A_i or negedge rstn_A_i) begin
            if (!rstn_A_i) begin 
                flag <= 1'b0;
            end else begin 
                flag <= flag ^ flag_A_i;
            end 
        end 


    logic [FLOP_NUMBER - 1:0] flag_sync;

        always_ff @(posedge clk_B_i or negedge rstn_B_i) begin
            if (!rstn_B_i) begin 
                flag_sync <= RESET_VALUE;
            end else begin 
                flag_sync <= {flag_sync[FLOP_NUMBER - 2:0], flag};
            end 
        end

    assign flag_B_o = flag_sync[FLOP_NUMBER - 1] ^ flag_sync[FLOP_NUMBER - 2];

    `else /* IF VIVADO MACRO IS DEFINED */

    xpm_cdc_pulse #(
        .DEST_SYNC_FF   ( FLOP_NUMBER ),
        .INIT_SYNC_FF   ( 0           ),
        .REG_OUTPUT     ( 0           ),
        .RST_USED       ( 1           ),
        .SIM_ASSERT_CHK ( 0           )
    ) vivado_pulse_synchronizer (
        .dest_pulse ( flag_B_o ),
        .dest_clk   ( clk_B_i  ),
        .dest_rst   ( rstn_B_i ),
        .src_clk    ( clk_A_i  ),
        .src_pulse  ( flag_A_i ),
        .src_rst    ( rstn_A_i )
    );

    `endif /* _VIVADO_ */

endmodule : flag_synchronizer

`endif 