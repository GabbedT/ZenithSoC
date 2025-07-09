`ifndef SD_CRC_16_SV
    `define SD_CRC_16_SV

module sd_crc16 (
    input logic clk_i,
    input logic rst_n_i,
    
    /* Control signals */
    input logic initialize_i,
    input logic compute_i,

    /* Data */
    input logic [3:0] data_i,

    output logic [15:0] crc_o,
    output logic [3:0][15:0] crc_wide_o
);

    logic [15:0] crc_CRT, crc_NXT; logic [3:0][15:0] crc_wide_CRT, crc_wide_NXT;

    genvar i;

    /* Wide bus CRC, generate logic for each line */
    generate
        for (i = 0; i < 4; ++i) begin
            assign crc_wide_NXT[i][0] = crc_wide_CRT[i][15] ^ data_i[i];
            assign crc_wide_NXT[i][1] = crc_wide_CRT[i][0];
            assign crc_wide_NXT[i][2] = crc_wide_CRT[i][1];
            assign crc_wide_NXT[i][3] = crc_wide_CRT[i][2];
            assign crc_wide_NXT[i][4] = crc_wide_CRT[i][3];
            assign crc_wide_NXT[i][5] = crc_wide_CRT[i][4] ^ crc_wide_CRT[i][15] ^ data_i[i];
            assign crc_wide_NXT[i][6] = crc_wide_CRT[i][5];
            assign crc_wide_NXT[i][7] = crc_wide_CRT[i][6];
            assign crc_wide_NXT[i][8] = crc_wide_CRT[i][7];
            assign crc_wide_NXT[i][9] = crc_wide_CRT[i][8];
            assign crc_wide_NXT[i][10] = crc_wide_CRT[i][9];
            assign crc_wide_NXT[i][11] = crc_wide_CRT[i][10];
            assign crc_wide_NXT[i][12] = crc_wide_CRT[i][11] ^ crc_wide_CRT[i][15] ^ data_i[i];
            assign crc_wide_NXT[i][13] = crc_wide_CRT[i][12];
            assign crc_wide_NXT[i][14] = crc_wide_CRT[i][13];
            assign crc_wide_NXT[i][15] = crc_wide_CRT[i][14];
        end
    endgenerate

    /* Single bit CRC */
    assign crc_NXT[0] = crc_CRT[15] ^ data_i[0];
    assign crc_NXT[1] = crc_CRT[0];
    assign crc_NXT[2] = crc_CRT[1];
    assign crc_NXT[3] = crc_CRT[2];
    assign crc_NXT[4] = crc_CRT[3];
    assign crc_NXT[5] = crc_CRT[4] ^ crc_CRT[15] ^ data_i[0];
    assign crc_NXT[6] = crc_CRT[5];
    assign crc_NXT[7] = crc_CRT[6];
    assign crc_NXT[8] = crc_CRT[7];
    assign crc_NXT[9] = crc_CRT[8];
    assign crc_NXT[10] = crc_CRT[9];
    assign crc_NXT[11] = crc_CRT[10];
    assign crc_NXT[12] = crc_CRT[11] ^ crc_CRT[15] ^ data_i[0];
    assign crc_NXT[13] = crc_CRT[12];
    assign crc_NXT[14] = crc_CRT[13];
    assign crc_NXT[15] = crc_CRT[14];

    logic [15:0] crc16_in0;

    always_ff @(posedge clk_i) begin
        if(!rst_n_i | initialize_i) begin
            crc_CRT <= '0;
            crc_wide_CRT <= '0;
        end else if (compute_i) begin
            crc_CRT <= crc_NXT;
            crc_wide_CRT <= crc_wide_NXT;
        end
    end

    assign crc_o = crc_CRT;
    assign crc_wide_o = crc_wide_CRT;

endmodule : sd_crc16

`endif 