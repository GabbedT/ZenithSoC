`include "IO/GPIO/gpio.sv"

module dut_wrapper (
    input  logic        clk,
    input  logic        rst_n,

    input  logic        write_i,
    input  logic [31:0] write_address_i,
    input  logic [31:0] write_data_i,
    input  logic [3:0]  write_strobe_i,
    output logic        write_done_o,
    output logic        write_error_o,

    input  logic        read_i,
    input  logic [31:0] read_address_i,
    output logic [31:0] read_data_o,
    output logic        read_done_o,
    output logic        read_error_o,

    output logic        interrupt_o
);

    // GPIO in ZenithSoC: 8 single-bit instances sharing the bus.
    // Each instance handles 1 bit of the data word.
    // We replicate this arrangement here.

    logic [7:0] gpio_interrupt;
    wire [7:0] pin;

    genvar j;
    generate
        for (j = 0; j < 8; j++) begin : gpio_gen
            gpio gpio_bit (
                .clk_i      ( clk   ),
                .rst_n_i    ( rst_n ),

                .pin_io ( pin[j] ),

                .write_i         ( write_i                ),
                .write_address_i ( write_address_i[1:0]   ),
                .write_data_i    ( write_data_i[j]        ),

                .read_address_i ( read_address_i[1:0]   ),
                .read_data_o    ( read_data_o[j]        ),

                .interrupt_o ( gpio_interrupt[j] )
            );
        end
    endgenerate

    assign read_data_o[31:8] = '0;

    // GPIO has no done/error outputs in ZenithSoC; same-cycle response
    assign write_done_o  = write_i;
    assign write_error_o = 1'b0;
    assign read_done_o   = read_i;
    assign read_error_o  = 1'b0;
    assign interrupt_o   = (gpio_interrupt != '0);

endmodule
