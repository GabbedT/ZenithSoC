`ifndef UART_BAUD_GENERATOR_SV
    `define UART_BAUD_GENERATOR_SV

module uart_baud_generator (
    /* Global signals */
    input logic clk_i,
    input logic rst_n_i,

    /* Clock divider value */
    input logic [15:0] divider_i,

    /* Sample pulse */
    output logic sample_o
);

    logic [15:0] counter;

    always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin 
        if (!rst_n_i) begin 
            counter <= '0;
        end else begin
            counter <= (counter >= divider_i) ? '0 : (counter + 1);
        end 
    end  

    assign sample_o = counter == divider_i;

endmodule : uart_baud_generator

`endif 