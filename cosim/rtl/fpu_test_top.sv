module fpu_test_top (
    input logic clk_i, rst_n_i, stall_i, flush_i, valid_i,
    input logic [2:0] operation_i, rounding_i,
    input logic [31:0] operand_a_i, operand_b_i,
    output logic [31:0] result_o,
    output logic valid_o
);
    fpu_uop_t operation;
    fpu_valid_t valid;

        always_comb begin
            operation = '0;
            valid = '0;
            case (operation_i)
                3'd0, 3'd1: begin
                    valid.FPADD = valid_i;
                    operation.FPADD.opcode = operation_i[0] ? FSUB : FADD;
                end
                3'd2: valid.FPMUL = valid_i;
                default: begin
                    valid.FPCVT = valid_i;
                    operation.FPCVT.opcode = (operation_i >= 3'd5) ? INT2FLOAT : FLOAT2INT;
                    operation.FPCVT.is_signed = operation_i[0];
                end
            endcase
            operation.FPADD.rounding_mode = rounding_mode_t'(rounding_i);
        end

    floating_point_unit dut (
        .clk_i ( clk_i ),
        .rst_n_i ( rst_n_i ),
        .stall_i ( stall_i ),
        .flush_i ( flush_i ),
        .ipacket_i ( '0 ),
        .operand_A_i ( operand_a_i ),
        .operand_B_i ( operand_b_i ),
        .operation_i ( operation ),
        .valid_i ( valid ),
        .result_o ( result_o ),
        .valid_o ( valid_o ),
        .ipacket_o (),
        .overflow_o (),
        .underflow_o (),
        .invalid_o (),
        .inexact_o ()
    );
endmodule : fpu_test_top
