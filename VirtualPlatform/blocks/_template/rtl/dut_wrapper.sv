// Template dut_wrapper: responds OKAY to all transactions
// Copy this block and replace the placeholder logic with your peripheral

module dut_wrapper (
    input  logic        clk,
    input  logic        rst_n,

    // Standard ZenithSoC peripheral write interface
    input  logic        write_i,
    input  logic [31:0] write_address_i,
    input  logic [31:0] write_data_i,
    input  logic [3:0]  write_strobe_i,
    output logic        write_done_o,
    output logic        write_error_o,

    // Standard ZenithSoC peripheral read interface
    input  logic        read_i,
    input  logic [31:0] read_address_i,
    output logic [31:0] read_data_o,
    output logic        read_done_o,
    output logic        read_error_o,

    // Interrupt
    output logic        interrupt_o
);

    // Placeholder: single-cycle response, no errors
    logic [31:0] scratch_reg;

    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            scratch_reg  <= 32'hDEADBEEF;
            write_done_o <= 1'b0;
            read_done_o  <= 1'b0;
            read_data_o  <= '0;
        end else begin
            write_done_o <= write_i;
            read_done_o  <= read_i;

            if (write_i)
                scratch_reg <= write_data_i;

            if (read_i)
                read_data_o <= scratch_reg;
        end
    end

    assign write_error_o = 1'b0;
    assign read_error_o  = 1'b0;
    assign interrupt_o   = 1'b0;

endmodule
