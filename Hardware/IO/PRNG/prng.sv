`ifndef PRNG_SV
    `define PRNG_SV

module prng (
    input logic clk_i,
    input logic rst_n_i,

    /* Write interface */
    input logic write_i,
    input logic [31:0] write_data_i,
    input logic write_address_i,
    output logic write_done_o,
    
    /* Read interface */
    input logic read_i,
    input logic read_address_i,
    output logic read_done_o,
    output logic [31:0] read_data_o
);

    typedef union packed {
        logic [63:0] single;
        logic [1:0][31:0] multi;
    } lfsr_type_t; 

    lfsr_type_t lfsr;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                lfsr.single <= 64'b0;
            end else begin 
                if (write_i) begin
                    lfsr.single <= write_address_i ? {write_data_i, lfsr.multi[0]} : {lfsr.multi[1], write_data_i};
                end else begin
                    lfsr <= {lfsr.single[62:0], lfsr.single[63] ^ lfsr.single[62] ^ lfsr.single[60] ^ lfsr.single[59]};
                end
            end 
        end 

        always_ff @(posedge clk_i) begin
            read_data_o <= lfsr.multi[read_address_i];
            read_done_o <= read_i;
            write_done_o <= write_i;
        end 

endmodule : prng 

`endif 