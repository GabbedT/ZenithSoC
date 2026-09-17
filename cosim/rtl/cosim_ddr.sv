`ifndef COSIM_DDR_SV
`define COSIM_DDR_SV

module cosim_ddr #(
    parameter int DATA_MAX_BURST        = 4,
    parameter int INSTRUCTION_MAX_BURST = 4,
    parameter int SIZE_BYTES            = 64 * 1024 * 1024,
    parameter int LAT_MIN               = 2,
    parameter int LAT_MAX               = 16
)(
    input  logic clk_i,
    input  logic rst_n_i,

    load_interface.slave  load_channel,
    store_interface.slave store_channel,

    input  logic single_trx_i,
    input  logic instr_req_i,
    output logic load_empty_o,
    output logic store_idle_o
);

    localparam int DDR_WORDS = SIZE_BYTES / 8;
    localparam int MODEL_LATENCY = 8;

    dev2ddr_interface ddr_channel();

    cache_ddr_interface #(
        .DATA_MAX_BURST        ( DATA_MAX_BURST        ),
        .INSTRUCTION_MAX_BURST ( INSTRUCTION_MAX_BURST )
    ) ddr_controller_interface (
        .clk_i   ( clk_i   ),
        .rst_n_i ( rst_n_i ),

        .load_channel  ( load_channel  ),
        .store_channel ( store_channel ),

        .single_trx_i ( single_trx_i ),
        .instr_req_i  ( instr_req_i  ),
        .load_empty_o ( load_empty_o ),
        .store_idle_o ( store_idle_o ),

        .ddr_channel ( ddr_channel )
    );


//====================================================================================
//      STREAMED DDR MODEL
//====================================================================================

    logic [63:0] ddr_memory [0:DDR_WORDS-1];
    logic [$clog2(DDR_WORDS) - 1:0] ddr_word_address;
    logic [MODEL_LATENCY - 1:0] read_valid;
    logic [MODEL_LATENCY - 1:0] read_error;
    logic [MODEL_LATENCY - 1:0][127:0] read_data;
    logic write_valid, write_error;

    assign ddr_word_address = ddr_channel.trx_addr[$clog2(DDR_WORDS)+2:3];
    assign ddr_channel.ready = rst_n_i;
    assign ddr_channel.read_valid = read_valid[MODEL_LATENCY - 1];
    assign ddr_channel.read_error = read_error[MODEL_LATENCY - 1];
    assign ddr_channel.rdata = read_data[MODEL_LATENCY - 1];
    assign ddr_channel.write_valid = write_valid;
    assign ddr_channel.write_error = write_error;

    always_ff @(posedge clk_i or negedge rst_n_i) begin
        if (!rst_n_i) begin
            read_valid <= '0;
            read_error <= '0;
            read_data <= '0;
            write_valid <= 1'b0;
            write_error <= 1'b0;
        end else begin
            read_valid <= {read_valid[MODEL_LATENCY - 2:0], 1'b0};
            read_error <= {read_error[MODEL_LATENCY - 2:0], 1'b0};
            read_data <= {read_data[MODEL_LATENCY - 2:0], 128'b0};
            write_valid <= 1'b0;
            write_error <= 1'b0;

            if (ddr_channel.trx_req & ddr_channel.ready) begin
                if (!ddr_channel.trx_type) begin
                    read_valid[0] <= 1'b1;
                    read_error[0] <= ddr_channel.trx_addr[3:0] != 4'b0;
                    read_data[0] <= {
                        ddr_memory[ddr_word_address + 1'b1],
                        ddr_memory[ddr_word_address]
                    };
                end else begin
                    write_valid <= 1'b1;
                    write_error <= ddr_channel.trx_addr[3:0] != 4'b0;

                    for (int i = 0; i < 16; i++) begin
                        if (ddr_channel.wstrobe[i]) begin
                            ddr_memory[ddr_word_address + (i >> 3)]
                                      [8 * (i & 7) +: 8] <=
                                ddr_channel.wdata[8 * i +: 8];
                        end
                    end
                end
            end
        end
    end


//====================================================================================
//      DPI MEMORY ACCESS
//====================================================================================

    export "DPI-C" function ddr_preload_word;

    function void ddr_preload_word(input int unsigned byte_addr, input int unsigned data);
        automatic int unsigned widx = byte_addr >> 3;

        if (widx < DDR_WORDS) begin
            if (byte_addr[2]) begin
                ddr_memory[widx][63:32] = data;
            end else begin
                ddr_memory[widx][31:0] = data;
            end
        end
    endfunction

    export "DPI-C" function ddr_peek_word;

    function int unsigned ddr_peek_word(input int unsigned byte_addr);
        automatic int unsigned idx = byte_addr >> 3;

        if (idx >= DDR_WORDS) begin
            return 0;
        end

        if (byte_addr[2]) begin
            return ddr_memory[idx][63:32];
        end else begin
            return ddr_memory[idx][31:0];
        end
    endfunction : ddr_peek_word

    logic unused;
    assign unused = (LAT_MIN == LAT_MAX);

endmodule : cosim_ddr

`endif
