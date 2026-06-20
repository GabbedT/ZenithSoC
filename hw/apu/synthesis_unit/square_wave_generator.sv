`ifndef SQUARE_WAVE_GENERATOR_SV
    `define SQUARE_WAVE_GENERATOR_SV

module square_wave_generator (
    input logic clk_i,
    input logic rst_n_i,
    
    /* Generator control */
    input logic enable_i,
    input logic start_i,

    /* Incremental step defines the frequency */
    input logic [31:0] inc_step_i,

    /* Square wave duty cycle */
    input logic [31:0] duty_cycle_i,

    /* PCM output */
    output logic [15:0] square_wave_o
);


    /* Take account for overflow */
    logic [32:0] counter;

    always_ff @(posedge clk_i) begin
        if (!rst_n_i | start_i) begin
            counter <= '0;
        end else if (enable_i) begin
            if (counter[32]) begin
                counter <= '0;
            end else begin
                counter <= counter + inc_step_i;
            end
        end
    end


    always_ff @(posedge clk_i) begin
        if (enable_i) begin
            square_wave_o <= (counter >= {1'b0, duty_cycle_i}) ? '0 : '1;
        end else begin
            square_wave_o <= '0;
        end
    end

endmodule : square_wave_generator

`endif
