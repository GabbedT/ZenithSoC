module wishbone_ram #(
    parameter ADDR_WIDTH = 10,
    parameter DATA_WIDTH = 32
)(
    input wire                  clk,
    input wire                  rst,

    // Wishbone interface
    input  wire [ADDR_WIDTH-1:0] wbm_adr_i,
    input  wire [DATA_WIDTH-1:0] wbm_dat_i,
    output reg  [DATA_WIDTH-1:0] wbm_dat_o,
    input  wire                  wbm_we_i,
    input  wire                  wbm_stb_i,
    input  wire                  wbm_cyc_i,
    input  wire [3:0]            wbm_sel_i,
    output reg                   wbm_ack_o,
    input  wire [2:0]            wbm_cti_i,
    input  wire [1:0]            wbm_bte_i
);

    localparam DEPTH = 1 << ADDR_WIDTH;

    reg [DATA_WIDTH-1:0] mem [0:DEPTH-1];

    always @(posedge clk) begin
        if (!rst) begin
            wbm_ack_o <= 0;
        end else begin
            wbm_ack_o <= 0;
            if (wbm_cyc_i && wbm_stb_i && !wbm_ack_o) begin
                wbm_ack_o <= 1;
                if (wbm_we_i) begin
                    // Write with byte enables
                    if (wbm_sel_i[0]) mem[wbm_adr_i][7:0]   <= wbm_dat_i[7:0];
                    if (wbm_sel_i[1]) mem[wbm_adr_i][15:8]  <= wbm_dat_i[15:8];
                    if (wbm_sel_i[2]) mem[wbm_adr_i][23:16] <= wbm_dat_i[23:16];
                    if (wbm_sel_i[3]) mem[wbm_adr_i][31:24] <= wbm_dat_i[31:24];
                end else begin
                    wbm_dat_o <= mem[wbm_adr_i];
                end
            end
        end
    end

endmodule
