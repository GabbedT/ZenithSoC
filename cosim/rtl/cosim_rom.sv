`ifndef COSIM_ROM_SV
`define COSIM_ROM_SV

module cosim_rom #(
    parameter int WORDS = 4096    // 16 KB
)(
    input  logic clk_i,
    input  logic rst_n_i,
    fetch_interface.slave fetch_channel
);

    logic [31:0] rom [WORDS];

    localparam int AW = $clog2(WORDS);

    /* Registered read (1 clock cycle), same as memory_bank port B */
    logic [31:0] instr_q;
    logic valid_q;

    always_ff @(posedge clk_i or negedge rst_n_i) begin
        if (!rst_n_i) begin
            valid_q <= 1'b0;
            instr_q <= '0;
        end else begin
            valid_q <= fetch_channel.fetch & ~fetch_channel.invalidate;
            instr_q <= rom[fetch_channel.address[AW+1:2]];
        end
    end

    assign fetch_channel.valid       = valid_q;
    assign fetch_channel.instruction = instr_q;

    /* ROM never stalls */
    assign fetch_channel.stall = 1'b0;

    /* Preload via DP */
    export "DPI-C" function rom_preload_word;

    function void rom_preload_word(
        input int unsigned byte_addr,
        input int unsigned data
    );
        automatic int unsigned widx = byte_addr >> 2;

        if (widx < WORDS) begin
            rom[widx] = data;
        end
    endfunction

endmodule : cosim_rom

`endif