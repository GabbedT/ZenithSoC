`ifndef ETHERNET_RESET_SV
    `define ETHERNET_RESET_SV

module ethernet_reset (
    input logic clk_i,
    input logic rst_n_i,

    output logic rst_n_o
);

    logic [14:0] reset_counter; logic start;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                start <= 1'b1;

                reset_counter <= '0;
            end else begin 
                if (start) begin
                    reset_counter <= reset_counter + 1'b1;
                end

                /* Count to 200us */
                start <= rst_n_o ? 1'b0 : start;
            end 
        end 

    assign rst_n_o = (reset_counter > 'd20000);

endmodule : ethernet_reset

`endif 