`ifndef TRIANGLE_WAVE_GENERATOR_SV
    `define TRIANGLE_WAVE_GENERATOR_SV

module triangle_wave_generator (
    input logic clk_i,
    input logic rst_n_i,
    
    /* Generator control */
    input logic enable_i,
    input logic start_i,

    /* Incremental step defines the frequency */
    input logic [31:0] inc_step_i,

    /* PCM output */
    output logic [15:0] triangle_wave_o
);

    logic up_down; logic [32:0] counter;

        always_ff @(posedge clk_i) begin
            if (!rst_n_i | start_i) begin
                up_down <= 1'b1;
                counter <= '0;
            end else if (enable_i) begin
                /* Invert the counter once the extremes are reached */
                if (up_down & counter[32]) begin
                    up_down <= 1'b0;
                    counter <= {1'b0, 32'hFFFFFFFF};
                end else if (!up_down & counter[32]) begin
                    up_down <= 1'b1;
                    counter <= '0;
                end

                if (counter[32]) begin
                    if (up_down) begin
                        counter <= {1'b0, 32'hFFFFFFFF} - inc_step_i;;
                    end else begin
                        counter <= inc_step_i;
                    end
                end else begin
                    if (up_down) begin
                        counter <= counter + inc_step_i;
                    end else begin
                        counter <= counter - inc_step_i;
                    end
                end
            end
        end


        always_ff @(posedge clk_i) begin
            if (enable_i) begin
                if (counter[32]) begin
                    if (up_down) begin
                        triangle_wave_o <= '1;
                    end else begin
                        triangle_wave_o <= '0;
                    end
                end else begin
                    triangle_wave_o <= counter[31:16];
                end
            end else begin
                triangle_wave_o <= '0;
            end
        end

    
endmodule : triangle_wave_generator

`endif 