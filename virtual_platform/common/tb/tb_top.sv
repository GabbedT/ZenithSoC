module tb_top (
    input  logic        clk,
    input  logic        rst_n,

    // Command interface (exposed to C++ Verilator testbench)
    input  logic        cmd_valid,
    input  logic        cmd_write,
    input  logic [31:0] cmd_addr,
    input  logic [31:0] cmd_wdata,
    input  logic [3:0]  cmd_wstrb,
    output logic        cmd_ready,
    output logic [31:0] cmd_rdata,
    output logic        cmd_done,
    output logic        cmd_error,

    // DUT interrupt output
    output logic        interrupt
);

    // Internal wires: BFM <-> DUT
    logic        write_req;
    logic [31:0] write_addr;
    logic [31:0] write_data;
    logic [3:0]  write_strb;
    logic        write_done;
    logic        write_error;

    logic        read_req;
    logic [31:0] read_addr;
    logic [31:0] read_data;
    logic        read_done;
    logic        read_error;

    // BFM instance
    io_master_bfm u_bfm (
        .clk             (clk),
        .rst_n           (rst_n),
        .cmd_valid       (cmd_valid),
        .cmd_write       (cmd_write),
        .cmd_addr        (cmd_addr),
        .cmd_wdata       (cmd_wdata),
        .cmd_wstrb       (cmd_wstrb),
        .cmd_ready       (cmd_ready),
        .cmd_rdata       (cmd_rdata),
        .cmd_done        (cmd_done),
        .cmd_error       (cmd_error),
        .write_o         (write_req),
        .write_address_o (write_addr),
        .write_data_o    (write_data),
        .write_strobe_o  (write_strb),
        .write_done_i    (write_done),
        .write_error_i   (write_error),
        .read_o          (read_req),
        .read_address_o  (read_addr),
        .read_data_i     (read_data),
        .read_done_i     (read_done),
        .read_error_i    (read_error)
    );

    // DUT instance (resolved at compile-time via file selection)
    dut_wrapper u_dut (
        .clk             (clk),
        .rst_n           (rst_n),
        .write_i         (write_req),
        .write_address_i (write_addr),
        .write_data_i    (write_data),
        .write_strobe_i  (write_strb),
        .write_done_o    (write_done),
        .write_error_o   (write_error),
        .read_i          (read_req),
        .read_address_i  (read_addr),
        .read_data_o     (read_data),
        .read_done_o     (read_done),
        .read_error_o    (read_error),
        .interrupt_o     (interrupt)
    );

endmodule
