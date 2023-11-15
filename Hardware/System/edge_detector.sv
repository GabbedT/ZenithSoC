`ifndef EDGE_DETECTOR_SV
    `define EDGE_DETECTOR_SV

module edge_detector #(
    /* Detect positive edge or negative edge */
    parameter EDGE = 1,

    /* Startup value */
    parameter RESET_VALUE = 0
) (
    input logic clk_i,
    input logic rst_n_i,

    input logic signal_i,
    output logic edge_o
);

    logic previous_signal;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                previous_signal <= RESET_VALUE;
            end else begin 
                previous_signal <= signal_i;
            end 
        end 


    generate if (EDGE == 1) begin

        assign edge_o = !previous_signal & signal_i;

    end else begin
        
        assign edge_o = previous_signal & !signal_i;

    end endgenerate

endmodule : edge_detector

`endif 