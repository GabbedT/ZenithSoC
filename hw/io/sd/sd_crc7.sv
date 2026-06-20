`ifndef SD_CRC7_SV
    `define SD_CRC7_SV

module sd_crc7 (
    input logic clk_i,
    input logic rst_n_i,
    
    /* Control signals */
    input logic initialize_i,
    input logic compute_i,

    /* Data */
    input logic data_i,

    output logic [6:0]  crc_o
);

    logic [6:0] crc;

        always_ff @(posedge clk_i) begin
            if (!rst_n_i | initialize_i) begin
                crc <= '0;
            end else if (compute_i) begin
                crc[6] <= crc[5];
                crc[5] <= crc[4];
                crc[4] <= crc[3];
                crc[3] <= crc[2] ^ crc[6] ^ data_i;
                crc[2] <= crc[1];
                crc[1] <= crc[0];
                crc[0] <= crc[6] ^ data_i;
            end
        end

    assign crc_o = crc;

endmodule : sd_crc7

`endif