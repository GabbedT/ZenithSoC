`ifndef PCM2PWM_INTERFACE_SV
    `define PCM2PWM_INTERFACE_SV

module pcm2pwm_interface (
    input logic clk_i,
    input logic rst_n_i,
    input logic enable_i,

    /* Scaled PCM value rapresent duty cycle */
    input logic [7:0] pcm_i,

    /* PWM pin */
    output logic pwm_o
);

    // localparam DIVISOR = 100000000 / 500000;

    // /* Divide the clock to obtain the PWM frequency */
    // logic [$clog2(DIVISOR) - 1:0] clock_divisor; logic enable_clock;

    //     always_ff @(posedge clk_i) begin
    //         if (!rst_n_i | !enable_i) begin
    //             clock_divisor <= '0;
    //         end else begin
    //             clock_divisor <= clock_divisor + 1'b1;
    //         end
    //     end

    // assign enable_clock = clock_divisor == (DIVISOR - 1);


    logic [7:0] duty_cycle_counter;

        always_ff @(posedge clk_i) begin
            if (!rst_n_i | !enable_i) begin
                duty_cycle_counter <= '0;
            end else begin
                duty_cycle_counter <= duty_cycle_counter + 1'b1;
            end
        end

    
        /* PWM output logic */
        always_ff @(posedge clk_i) begin
            if (!rst_n_i | !enable_i) begin
                pwm_o <= '0;
            end else begin
                pwm_o <= duty_cycle_counter < pcm_i;
            end
        end

endmodule : pcm2pwm_interface

`endif 