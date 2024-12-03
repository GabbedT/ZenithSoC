`ifndef SYNCHRONIZER_SV
    `define SYNCHRONIZER_SV 

`define _VIVADO_

module synchronizer #(
    /* Number of sync flops */
    parameter FLOP_NUMBER = 2,

    /* Reset value */
    parameter logic [FLOP_NUMBER - 1:0] RESET_VALUE = '0
) (
    /* Global signals */
    input logic clk_i,
    input logic rst_n_i,

    /* Sync signal */
    input logic signal_i,
    output logic sync_o
);

    `ifndef _VIVADO_ /* IF VIVADO MACRO IS NOT DEFINED */

    logic [FLOP_NUMBER - 1:0] sync_signal;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                sync_signal <= RESET_VALUE;
            end else begin 
                sync_signal <= {sync_signal[FLOP_NUMBER - 2:0], signal_i};
            end 
        end 

    assign sync_o = sync_signal[FLOP_NUMBER - 1];

    `else /* IF VIVADO MACRO IS DEFINED */

    xpm_cdc_single #(
        .DEST_SYNC_FF   ( FLOP_NUMBER ),
        .INIT_SYNC_FF   ( 0           ),
        .SIM_ASSERT_CHK ( 0           ),
        .SRC_INPUT_REG  ( 0           )
    ) vivado_synchronizer (
        .dest_out ( sync_o   ),
        .dest_clk ( clk_i    ),
        .src_clk  (          ),
        .src_in   ( signal_i )
    );

    `endif /* _VIVADO_ */

endmodule : synchronizer 

`endif 