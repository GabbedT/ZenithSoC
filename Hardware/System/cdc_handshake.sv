`ifndef CDC_HANDSHAKE_SV
    `define CDC_HANDSHAKE_SV

`include "synchronizer.sv"

module cdc_handshake (
    /* Global signals */
    input logic clk_A_i,
    input logic rstn_A_i,
    input logic clk_B_i,
    input logic rstn_B_i,

    /* Clock domain A signals */
    input logic request_A_i,
    output logic ackn_A_o,

    /* Clock domain B signals */
    input logic ackn_B_i,
    output logic request_B_o
);

//====================================================================================
//      REQUEST LOGIC
//====================================================================================

    logic request_A, request_A_sync;

        always_ff @(posedge clk_A_i `ifdef ASYNC or negedge rstn_A_i `endif) begin
            if (!rstn_A_i) begin 
                request_A <= 1'b0;
            end else begin 
                request_A <= request_A_i;
            end 
        end 

    
    synchronizer #(2, '0) request_synchronizer (
        .clk_i   ( clk_B_i  ),
        .rst_n_i ( rstn_B_i ),

        .signal_i ( request_A      ),
        .sync_o   ( request_A_sync )
    );

    assign request_B_o = request_A_sync;


//====================================================================================
//      ACKNOWLEDGE LOGIC
//====================================================================================

    logic ackn_B, ackn_B_sync;

        always_ff @(posedge clk_B_i `ifdef ASYNC or negedge rstn_B_i `endif) begin
            if (!rstn_B_i) begin 
                ackn_B <= 1'b0;
            end else begin 
                ackn_B <= ackn_B_i;
            end 
        end


    synchronizer #(2, '0) acknowledge_synchronizer (
        .clk_i   ( clk_A_i  ),
        .rst_n_i ( rstn_A_i ),

        .signal_i ( ackn_B      ),
        .sync_o   ( ackn_B_sync )
    ); 

    assign ackn_A_o = ackn_B_sync;

endmodule : cdc_handshake

`endif 