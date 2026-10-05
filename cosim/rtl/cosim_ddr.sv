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
    typedef struct {
        bit write;
        bit error;
        logic [127:0] data;
        longint unsigned due;
    } response_t;

    response_t pending[$];
    int unsigned random_state;
    int unsigned memory_seed = 1;
    int unsigned latency_min = LAT_MIN;
    int unsigned latency_max = LAT_MAX;
    int unsigned stall_percent = 25;
    bit randomize_timing = 0;
    longint unsigned cycle;
    longint unsigned next_read_cycle;

    // A private PRNG keeps memory scheduling independent from Verilator's RNG.
    function automatic int unsigned next_random(input int unsigned value);
        value ^= value << 13;
        value ^= value >> 17;
        value ^= value << 5;
        return value;
    endfunction

    initial begin
        void'($value$plusargs("mem_random=%d", randomize_timing));
        void'($value$plusargs("mem_seed=%d", memory_seed));
        void'($value$plusargs("mem_latency_min=%d", latency_min));
        void'($value$plusargs("mem_latency_max=%d", latency_max));
        void'($value$plusargs("mem_stall_percent=%d", stall_percent));
        if (!memory_seed) memory_seed = 32'h6d2b79f5;
        if (latency_min < 1 || latency_max < latency_min || latency_max > 10000 || stall_percent > 95)
            $fatal(1, "Invalid co-simulation DDR timing parameters");
    end

    assign ddr_channel.ready = rst_n_i && pending.size() < 32 &&
        (!randomize_timing || (random_state % 100) >= stall_percent);

    // Requests and responses remain ordered. Reads snapshot accepted memory;
    // writes update byte lanes on acceptance and acknowledge after their delay.
    // Space randomized read responses by four cycles because each 128-bit
    // response is serialized into four words and the interface has no rready.
    always @(posedge clk_i or negedge rst_n_i) begin
        if (!rst_n_i) begin
            pending.delete();
            cycle = 0;
            next_read_cycle = 0;
            random_state <= memory_seed;
            ddr_channel.read_valid <= 0;
            ddr_channel.write_valid <= 0;
            ddr_channel.read_error <= 0;
            ddr_channel.write_error <= 0;
            ddr_channel.rdata <= '0;
        end else begin
            automatic bit accepted = ddr_channel.trx_req && ddr_channel.ready;
            cycle = cycle + 1;
            random_state <= next_random(random_state);
            ddr_channel.read_valid <= 0;
            ddr_channel.write_valid <= 0;
            ddr_channel.read_error <= 0;
            ddr_channel.write_error <= 0;

            if (pending.size() != 0 && pending[0].due <= cycle &&
                (pending[0].write || !randomize_timing || cycle >= next_read_cycle)) begin
                automatic response_t response = pending.pop_front();
                if (response.write) begin
                    ddr_channel.write_valid <= 1;
                    ddr_channel.write_error <= response.error;
                end else begin
                    ddr_channel.read_valid <= 1;
                    ddr_channel.read_error <= response.error;
                    ddr_channel.rdata <= response.data;
                    next_read_cycle = cycle + 4;
                end
            end

            if (accepted) begin
                automatic response_t response;
                automatic int unsigned word_address = ddr_channel.trx_addr >> 3;
                automatic int unsigned delay_cycles = randomize_timing ?
                    latency_min + (next_random(random_state) % (latency_max - latency_min + 1)) :
                    (ddr_channel.trx_type ? 1 : 8);
                response.write = ddr_channel.trx_type;
                response.error = ddr_channel.trx_addr[3:0] != 0 || word_address + 1 >= DDR_WORDS;
                response.data = '0;
                response.due = cycle + delay_cycles;
                if (!response.error) begin
                    if (response.write) begin
                        for (int i = 0; i < 16; i++) begin
                            if (ddr_channel.wstrobe[i])
                                ddr_memory[word_address + (i >> 3)][8 * (i & 7) +: 8] =
                                    ddr_channel.wdata[8 * i +: 8];
                        end
                    end else begin
                        response.data = {ddr_memory[word_address + 1], ddr_memory[word_address]};
                    end
                end
                pending.push_back(response);
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


endmodule : cosim_ddr

`endif
