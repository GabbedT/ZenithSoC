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

    logic pwm;

    assign write_done_o = write_i;
    assign read_done_o = read_i;

    timer u_timer (
        .clk_i       ( clk   ),
        .rst_n_i     ( rst_n ),
        .pwm_o       ( pwm   ),
        .interrupt_o ( interrupt_o ),

        .write_i         ( write_i              ),
        .write_address_i ( write_address_i[2:0] ),
        .write_data_i    ( write_data_i         ),
        .write_strobe_i  ( write_strobe_i       ),
        .write_error_o   ( write_error_o        ),

        .read_i         ( read_i                ),
        .read_address_i ( read_address_i[2:0]   ),
        .read_data_o    ( read_data_o           ),
        .read_error_o   ( read_error_o          )
    );

endmodule
