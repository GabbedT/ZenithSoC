`ifndef DDR_MASTER_SV
    `define DDR_MASTER_SV

module ddr_master (
    input logic clk_i,
    input logic rst_n_i,

    /* Transaction request */
    input logic request_i,
    input logic operation_i,
    input logic [26:0] address_i,
    input logic [15:0] write_strobe_i,
    input logic [127:0] write_data_i,

    /* Transaction response */
    output logic valid_o,
    output logic error_o,
    output logic accepted_o,
    output logic [127:0] read_data_o,

    /* Interface */
    dev2ddr_interface.master dev2ddr_channel
);

    /* Flow control */
    assign accepted_o = dev2ddr_channel.ready;

//====================================================================================
//      TRANSACTION PATH
//====================================================================================

    always_ff @(posedge clk_i) begin
        if (request_i & dev2ddr_channel.ready) begin
            /* Check ready to not lose any pending request */
            dev2ddr_channel.trx_addr <= address_i;
            dev2ddr_channel.trx_type <= operation_i;
        end
    end

    always_ff @(posedge clk_i) begin
        if (!rst_n_i) begin
            dev2ddr_channel.trx_req <= 1'b0;
        end else if (dev2ddr_channel.ready) begin
            dev2ddr_channel.trx_req <= request_i;
        end
    end

    always_ff @(posedge clk_i) begin
        if (!rst_n_i) begin
            error_o <= 1'b0;
        end else begin
            error_o <= dev2ddr_channel.read_error;
        end
    end


//====================================================================================
//      WRITE PATH
//====================================================================================

    always_ff @(posedge clk_i) begin
        if (request_i & dev2ddr_channel.ready) begin
            /* Check ready to not lose any pending request */
            dev2ddr_channel.wstrobe <= write_strobe_i;
            dev2ddr_channel.wdata <= write_data_i;
        end
    end


//====================================================================================
//      READ PATH
//====================================================================================

    always_ff @(posedge clk_i) begin
        if (dev2ddr_channel.read_valid) begin
            read_data_o <= dev2ddr_channel.rdata;
        end
    end

    always_ff @(posedge clk_i) begin
        if (!rst_n_i) begin
            valid_o <= 1'b0;
        end else begin
            valid_o <= dev2ddr_channel.read_valid;
        end
    end

endmodule : ddr_master

`endif
