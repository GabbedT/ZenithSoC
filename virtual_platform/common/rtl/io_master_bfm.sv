module io_master_bfm (
    input  logic        clk,
    input  logic        rst_n,

    // Command interface (driven by C++ testbench)
    input  logic        cmd_valid,
    input  logic        cmd_write,
    input  logic [31:0] cmd_addr,
    input  logic [31:0] cmd_wdata,
    input  logic [3:0]  cmd_wstrb,
    output logic        cmd_ready,
    output logic [31:0] cmd_rdata,
    output logic        cmd_done,
    output logic        cmd_error,

    // ZenithSoC peripheral write interface
    output logic        write_o,
    output logic [31:0] write_address_o,
    output logic [31:0] write_data_o,
    output logic [3:0]  write_strobe_o,
    input  logic        write_done_i,
    input  logic        write_error_i,

    // ZenithSoC peripheral read interface
    output logic        read_o,
    output logic [31:0] read_address_o,
    input  logic [31:0] read_data_i,
    input  logic        read_done_i,
    input  logic        read_error_i
);

    typedef enum logic [1:0] {
        IDLE,
        WRITE_WAIT,
        READ_WAIT
    } state_t;

    state_t state;

    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            state           <= IDLE;
            cmd_ready       <= 1'b1;
            cmd_done        <= 1'b0;
            cmd_error       <= 1'b0;
            cmd_rdata       <= '0;
            write_o         <= 1'b0;
            write_address_o <= '0;
            write_data_o    <= '0;
            write_strobe_o  <= '0;
            read_o          <= 1'b0;
            read_address_o  <= '0;
        end else begin
            cmd_done  <= 1'b0;
            cmd_error <= 1'b0;

            case (state)
                IDLE: begin
                    if (cmd_valid && cmd_ready) begin
                        cmd_ready <= 1'b0;
                        if (cmd_write) begin
                            write_o         <= 1'b1;
                            write_address_o <= cmd_addr;
                            write_data_o    <= cmd_wdata;
                            write_strobe_o  <= cmd_wstrb;
                            state           <= WRITE_WAIT;
                        end else begin
                            read_o          <= 1'b1;
                            read_address_o  <= cmd_addr;
                            state           <= READ_WAIT;
                        end
                    end
                end

                WRITE_WAIT: begin
                    write_o <= 1'b0;
                    if (write_done_i) begin
                        cmd_done  <= 1'b1;
                        cmd_error <= write_error_i;
                        cmd_ready <= 1'b1;
                        state     <= IDLE;
                    end
                end

                READ_WAIT: begin
                    read_o <= 1'b0;
                    if (read_done_i) begin
                        cmd_rdata <= read_data_i;
                        cmd_done  <= 1'b1;
                        cmd_error <= read_error_i;
                        cmd_ready <= 1'b1;
                        state     <= IDLE;
                    end
                end

                default: state <= IDLE;
            endcase
        end
    end

endmodule
