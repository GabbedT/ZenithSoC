//-----------------------------------------------------------------------------
// Copyright (C) 2009 OutputLogic.com
// This source file may be used and distributed without restriction
// provided that this copyright statement is not removed from the file
// and that any derivative work contains the original copyright notice
// and the associated disclaimer.
//
// THIS SOURCE FILE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS
// OR IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
// WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//-----------------------------------------------------------------------------

`ifndef ETHERNET_CRC32_SV
    `define ETHERNET_CRC32_SV 

module ethernet_crc32 (
    /* Global signals */
    input logic clk_i,
    input logic rst_n_i,

    input logic initialize_i,
    input logic compute_i,
    input logic [7:0] data_i,

    output logic [31:0] crc32_o
);

    logic [31:0] crc,next_crc;

    always_ff @(posedge clk_i) begin
        if(!rst_n_i) begin
            crc <= '1;
        end else if (initialize_i) begin 
            crc <= '1;
        end else if (compute_i) begin
            crc <= next_crc;
        end
    end

    assign crc32_o = crc;

    /* Polynomial: 1 + x^1 + x^2 + x^4 + x^5 + x^7 + x^8 + x^10 + x^11 + x^12 + x^16 + x^22 + x^23 + x^26 + x^32 */
    assign next_crc[0] = crc[2] ^ crc[8] ^ data_i[2];
    assign next_crc[1] = crc[0] ^ crc[3] ^ crc[9] ^ data_i[0] ^ data_i[3];
    assign next_crc[2] = crc[0] ^ crc[1] ^ crc[4] ^ crc[10] ^ data_i[0] ^ data_i[1] ^ data_i[4];
    assign next_crc[3] = crc[1] ^ crc[2] ^ crc[5] ^ crc[11] ^ data_i[1] ^ data_i[2] ^ data_i[5];
    assign next_crc[4] = crc[0] ^ crc[2] ^ crc[3] ^ crc[6] ^ crc[12] ^ data_i[0] ^ data_i[2] ^ data_i[3] ^ data_i[6];
    assign next_crc[5] = crc[1] ^ crc[3] ^ crc[4] ^ crc[7] ^ crc[13] ^ data_i[1] ^ data_i[3] ^ data_i[4] ^ data_i[7];
    assign next_crc[6] = crc[4] ^ crc[5] ^ crc[14] ^ data_i[4] ^ data_i[5];
    assign next_crc[7] = crc[0] ^ crc[5] ^ crc[6] ^ crc[15] ^ data_i[0] ^ data_i[5] ^ data_i[6];
    assign next_crc[8] = crc[1] ^ crc[6] ^ crc[7] ^ crc[16] ^ data_i[1] ^ data_i[6] ^ data_i[7];
    assign next_crc[9] = crc[7] ^ crc[17] ^ data_i[7];
    assign next_crc[10] = crc[2] ^ crc[18] ^ data_i[2];
    assign next_crc[11] = crc[3] ^ crc[19] ^ data_i[3];
    assign next_crc[12] = crc[0] ^ crc[4] ^ crc[20] ^ data_i[0] ^ data_i[4];
    assign next_crc[13] = crc[0] ^ crc[1] ^ crc[5] ^ crc[21] ^ data_i[0] ^ data_i[1] ^ data_i[5];
    assign next_crc[14] = crc[1] ^ crc[2] ^ crc[6] ^ crc[22] ^ data_i[1] ^ data_i[2] ^ data_i[6];
    assign next_crc[15] = crc[2] ^ crc[3] ^ crc[7] ^ crc[23] ^ data_i[2] ^ data_i[3] ^ data_i[7];
    assign next_crc[16] = crc[0] ^ crc[2] ^ crc[3] ^ crc[4] ^ crc[24] ^ data_i[0] ^ data_i[2] ^ data_i[3] ^ data_i[4];
    assign next_crc[17] = crc[0] ^ crc[1] ^ crc[3] ^ crc[4] ^ crc[5] ^ crc[25] ^ data_i[0] ^ data_i[1] ^ data_i[3] ^ data_i[4] ^ data_i[5];
    assign next_crc[18] = crc[0] ^ crc[1] ^ crc[2] ^ crc[4] ^ crc[5] ^ crc[6] ^ crc[26] ^ data_i[0] ^ data_i[1] ^ data_i[2] ^ data_i[4] ^ data_i[5] ^ data_i[6];
    assign next_crc[19] = crc[1] ^ crc[2] ^ crc[3] ^ crc[5] ^ crc[6] ^ crc[7] ^ crc[27] ^ data_i[1] ^ data_i[2] ^ data_i[3] ^ data_i[5] ^ data_i[6] ^ data_i[7];
    assign next_crc[20] = crc[3] ^ crc[4] ^ crc[6] ^ crc[7] ^ crc[28] ^ data_i[3] ^ data_i[4] ^ data_i[6] ^ data_i[7];
    assign next_crc[21] = crc[2] ^ crc[4] ^ crc[5] ^ crc[7] ^ crc[29] ^ data_i[2] ^ data_i[4] ^ data_i[5] ^ data_i[7];
    assign next_crc[22] = crc[2] ^ crc[3] ^ crc[5] ^ crc[6] ^ crc[30] ^ data_i[2] ^ data_i[3] ^ data_i[5] ^ data_i[6];
    assign next_crc[23] = crc[3] ^ crc[4] ^ crc[6] ^ crc[7] ^ crc[31] ^ data_i[3] ^ data_i[4] ^ data_i[6] ^ data_i[7];
    assign next_crc[24] = crc[0] ^ crc[2] ^ crc[4] ^ crc[5] ^ crc[7] ^ data_i[0] ^ data_i[2] ^ data_i[4] ^ data_i[5] ^ data_i[7];
    assign next_crc[25] = crc[0] ^ crc[1] ^ crc[2] ^ crc[3] ^ crc[5] ^ crc[6] ^ data_i[0] ^ data_i[1] ^ data_i[2] ^ data_i[3] ^ data_i[5] ^ data_i[6];
    assign next_crc[26] = crc[0] ^ crc[1] ^ crc[2] ^ crc[3] ^ crc[4] ^ crc[6] ^ crc[7] ^ data_i[0] ^ data_i[1] ^ data_i[2] ^ data_i[3] ^ data_i[4] ^ data_i[6] ^ data_i[7];
    assign next_crc[27] = crc[1] ^ crc[3] ^ crc[4] ^ crc[5] ^ crc[7] ^ data_i[1] ^ data_i[3] ^ data_i[4] ^ data_i[5] ^ data_i[7];
    assign next_crc[28] = crc[0] ^ crc[4] ^ crc[5] ^ crc[6] ^ data_i[0] ^ data_i[4] ^ data_i[5] ^ data_i[6];
    assign next_crc[29] = crc[0] ^ crc[1] ^ crc[5] ^ crc[6] ^ crc[7] ^ data_i[0] ^ data_i[1] ^ data_i[5] ^ data_i[6] ^ data_i[7];
    assign next_crc[30] = crc[0] ^ crc[1] ^ crc[6] ^ crc[7] ^ data_i[0] ^ data_i[1] ^ data_i[6] ^ data_i[7];
    assign next_crc[31] = crc[1] ^ crc[7] ^ data_i[1] ^ data_i[7];

endmodule : ethernet_crc32

`endif 