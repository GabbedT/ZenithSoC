`ifndef SYNCHRONIZER_SV
    `define SYNCHRONIZER_SV 

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

    logic [FLOP_NUMBER - 1:0] sync_signal;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                sync_signal <= RESET_VALUE;
            end else begin 
                sync_signal <= {sync_signal[FLOP_NUMBER - 2:0], signal_i};
            end 
        end 

    assign sync_o = sync_signal[FLOP_NUMBER - 1];

endmodule : synchronizer 

`endif 