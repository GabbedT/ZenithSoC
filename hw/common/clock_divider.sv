`ifndef CLOCK_DIVIDER_SV
    `define CLOCK_DIVIDER_SV

module clock_divider #(
    parameter DIVIDER_VALUE = 1
) (
    /* Global signals */
    input logic clk_i,
    input logic rst_n_i,

    output logic tick_o
);

    logic [$clog2(DIVIDER_VALUE) - 1:0] counter;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                counter <= '0;
            end else begin 
                counter <= counter + 1'b1;
            end 
        end 

    assign tick_o = (counter == DIVIDER_VALUE);

endmodule : clock_divider 

`endif 