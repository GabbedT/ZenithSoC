`include "IO/PRNG/prng.sv"

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

    prng u_prng (
        .clk_i   ( clk   ),
        .rst_n_i ( rst_n ),

        .write_i         ( write_i               ),
        .write_data_i    ( write_data_i          ),
        .write_address_i ( write_address_i[0]    ),
        .write_done_o    ( write_done_o          ),

        .read_i         ( read_i               ),
        .read_address_i ( read_address_i[0]    ),
        .read_done_o    ( read_done_o          ),
        .read_data_o    ( read_data_o          )
    );

    assign write_error_o = 1'b0;
    assign read_error_o  = 1'b0;
    assign interrupt_o   = 1'b0;

endmodule
