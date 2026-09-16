`timescale 1ns/1ns

module framebuffer_memory #(
    parameter logic [26:0] BASE_ADDRESS = 27'h01000000,
    parameter int unsigned DEPTH_BYTES = 27'h00E1000
) (
    input logic clk_i,
    input logic rst_n_i,

    input logic        cpu_write_i,
    input logic [31:0] cpu_write_word_address_i,
    input logic [31:0] cpu_write_data_i,
    input logic [3:0]  cpu_write_strobe_i,
    output logic        cpu_write_done_o,
    output logic        cpu_write_error_o,

    input logic        cpu_read_i,
    input logic [31:0] cpu_read_word_address_i,
    output logic [31:0] cpu_read_data_o,
    output logic        cpu_read_done_o,
    output logic        cpu_read_error_o,

    dev2ddr_interface.slave ddr_channel
);

    localparam logic [31:0] END_ADDRESS = BASE_ADDRESS + DEPTH_BYTES;

    logic [7:0] memory [0:DEPTH_BYTES - 1];

    logic [31:0] cpu_write_byte_address;
    logic [31:0] cpu_read_byte_address;
    logic cpu_write_in_range;
    logic cpu_read_in_range;

    assign cpu_write_byte_address = cpu_write_word_address_i << 2;
    assign cpu_read_byte_address = cpu_read_word_address_i << 2;

    assign cpu_write_in_range = (cpu_write_byte_address >= BASE_ADDRESS) && (cpu_write_byte_address < END_ADDRESS);
    assign cpu_read_in_range = (cpu_read_byte_address >= BASE_ADDRESS) && (cpu_read_byte_address < END_ADDRESS);

    assign cpu_write_done_o = cpu_write_i;
    assign cpu_read_done_o = cpu_read_i;
    assign cpu_write_error_o = cpu_write_i && !cpu_write_in_range;
    assign cpu_read_error_o = cpu_read_i && !cpu_read_in_range;

    always_ff @(posedge clk_i) begin
        if (cpu_write_i && cpu_write_in_range) begin
            for (int lane = 0; lane < 4; ++lane) begin
                if (cpu_write_strobe_i[lane] && (cpu_write_byte_address + lane < END_ADDRESS)) begin
                    memory[cpu_write_byte_address - BASE_ADDRESS + lane] <= cpu_write_data_i[8 * lane +: 8];
                end
            end
        end
    end

    always_comb begin
        cpu_read_data_o = '0;

        if (cpu_read_in_range) begin
            for (int lane = 0; lane < 4; ++lane) begin
                if (cpu_read_byte_address + lane < END_ADDRESS) begin
                    cpu_read_data_o[8 * lane +: 8] = memory[cpu_read_byte_address - BASE_ADDRESS + lane];
                end
            end
        end
    end

    logic [7:0] ddr_gap;
    integer debug_response_count;
    logic [15:0] lfsr;
    logic stress_mode;
    logic ddr_ready;

    wire [31:0] ddr_address = {5'b0, ddr_channel.trx_addr};
    wire ddr_in_range = (ddr_address >= BASE_ADDRESS) && (ddr_address + 16 <= END_ADDRESS);
    wire ddr_aligned = ddr_channel.trx_addr[3:0] == 4'b0;

    assign ddr_ready = rst_n_i && (!stress_mode || ddr_gap == 0);
    assign ddr_channel.ready = ddr_ready;

    initial begin
        stress_mode = 1'b0;
        if ($test$plusargs("fb_stress")) begin
            stress_mode = 1'b1;
        end
    end

    function automatic [127:0] read_beat(input logic [26:0] address);
        logic [127:0] value;
        begin
            value = '0;
            if ((address >= BASE_ADDRESS) && (address <= (END_ADDRESS - 32'd16))) begin
                for (int lane = 0; lane < 16; ++lane) begin
                    value[8 * lane +: 8] = memory[address - BASE_ADDRESS + lane];
                end
            end
            return value;
        end
    endfunction

    always_ff @(posedge clk_i) begin
        ddr_channel.trx_valid <= 1'b0;
        ddr_channel.trx_error <= 1'b0;

        if (!rst_n_i) begin
            ddr_gap <= '0;
            lfsr <= 16'h1;
            ddr_channel.trx_valid <= 1'b0;
            ddr_channel.trx_error <= 1'b0;
            ddr_channel.rdata <= '0;
            debug_response_count <= 0;
        end else begin
            lfsr <= {lfsr[14:0], lfsr[15] ^ lfsr[13] ^ lfsr[12] ^ lfsr[10]};

            ddr_channel.trx_valid <= ddr_channel.trx_req && ddr_ready;
            ddr_channel.trx_error <= 1'b0;

            if (ddr_gap != 0) begin
                ddr_gap <= ddr_gap - 1'b1;
            end
            if (ddr_channel.trx_req && ddr_ready) begin
                ddr_channel.trx_error <= !((ddr_channel.trx_type === 1'b0) && (ddr_channel.trx_addr >= BASE_ADDRESS) &&
                                           (ddr_channel.trx_addr <= END_ADDRESS - 32'd16) && ddr_aligned);
                                           
                if ((ddr_channel.trx_type === 1'b0) && (ddr_channel.trx_addr >= BASE_ADDRESS) && (ddr_channel.trx_addr <= END_ADDRESS - 32'd16) && ddr_aligned) begin
                    ddr_channel.rdata <= read_beat(ddr_channel.trx_addr);
                end else begin
                    ddr_channel.rdata <= '0;
                end

                ddr_gap <= stress_mode ? lfsr[2:0] : '0;
            end
        end
    end

endmodule
