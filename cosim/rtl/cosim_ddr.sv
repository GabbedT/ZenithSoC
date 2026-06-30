`ifndef COSIM_DDR_SV
`define COSIM_DDR_SV

module cosim_ddr #(
    parameter int DATA_MAX_BURST        = 4,   // DCACHE BLOCK_SIZE_BYTE / 4
    parameter int INSTRUCTION_MAX_BURST = 4,   // ICACHE BLOCK_SIZE_BYTE / 4
    parameter int SIZE_BYTES            = 64 * 1024 * 1024,
    parameter int LAT_MIN               = 2,
    parameter int LAT_MAX               = 16
)(
    input  logic clk_i,
    input  logic rst_n_i,

    /* Same interfaces seen by cpu_complex (slave side) */
    load_interface.slave  load_channel,
    store_interface.slave store_channel,

    input  logic single_trx_i,
    input  logic instr_req_i,
    output logic load_empty_o
);

    localparam int DDR_WORDS = SIZE_BYTES / 8;

    /* Nets connected to the physical model (same as ZenithSoC.sv) */
    logic [26:0] ddr_address;
    logic ddr_write, ddr_read, push_trx, pull_trx, ddr_data_valid, ddr_done;
    logic [63:0] ddr_data_write, ddr_data_read;
    logic [7:0] ddr_mask;
    logic ddr_ready;

    cache_ddr_interface #(
        .DATA_MAX_BURST        ( DATA_MAX_BURST        ),
        .INSTRUCTION_MAX_BURST ( INSTRUCTION_MAX_BURST )
    ) ddr_controller_interface (

        .clk_i   ( clk_i   ),
        .rst_n_i ( rst_n_i ),

        .hold_i ( 1'b0 ),

        .load_channel  ( load_channel  ),
        .store_channel ( store_channel ),

        .single_trx_i ( single_trx_i ),
        .instr_req_i  ( instr_req_i  ),
        .load_empty_o ( load_empty_o ),

        .address_o ( ddr_address ),

        .write_o ( ddr_write ),
        .read_o  ( ddr_read  ),

        .push_o       ( push_trx       ),
        .pull_o       ( pull_trx       ),
        .write_data_o ( ddr_data_write ),
        .write_mask_o ( ddr_mask       ),
        .read_data_i  ( ddr_data_read  ),
        .read_valid_i ( ddr_data_valid ),

        .done_o    ( ddr_done  ),
        .ready_i   ( ddr_ready )
    );

    assign ddr_ready = 1'b1; // Calibration is always completed in simulation


//====================================================================================
//      DDR MODEL
//====================================================================================

    logic [63:0] ddr_memory [0:DDR_WORDS-1];

    localparam int BEATS_PER_BURST = 2;

    /* FSM states */
    typedef enum logic [1:0] { IDLE, LAT, WAIT, BURST } burst_state_t;
    burst_state_t state;

    /* Buffer that contains each beat */
    logic [63:0] burst_buf [0:BEATS_PER_BURST-1];
    logic beat_current;

    /* Latency counter */
    logic [$clog2(LAT_MAX + 1) - 1:0] lat_cnt;

    /* DDR to word address */
    logic [$clog2(DDR_WORDS) - 1:0] word_address;
    assign word_address = ddr_address[$clog2(DDR_WORDS)+1:2];

    /* To interface */
    assign ddr_data_valid = (state == WAIT) | (state == BURST);
    assign ddr_data_read  = burst_buf[beat_current];

    always_ff @(posedge clk_i or negedge rst_n_i) begin
        if (!rst_n_i) begin
            state         <= IDLE;
            beat_current  <= '0;
            lat_cnt       <= '0;
            burst_buf[0]  <= '0;
            burst_buf[1]  <= '0;
        end else begin
            if (push_trx) begin
                for (int i = 0; i < 8; i++) begin
                    if (ddr_mask[i]) begin
                        /* Write DDR */
                        ddr_memory[word_address][8*i +: 8] <= ddr_data_write[8*i +: 8];
                    end
                end
            end

            case (state)
                IDLE: if (ddr_read) begin
                    burst_buf[0] <= ddr_memory[word_address];
                    burst_buf[1] <= ddr_memory[word_address + 1'b1];
                    beat_current <= '0;

                    /* Load a random latency */
                    lat_cnt <= $urandom_range(LAT_MIN, LAT_MAX);
                    state <= LAT;
                end

                LAT: begin
                    /* Wait random latency */
                    if (lat_cnt != '0) begin
                        lat_cnt <= lat_cnt - 1'b1;
                    end else begin
                        state <= WAIT;
                    end
                end

                WAIT: begin
                    /* Wait for pull request from interface */
                    if (pull_trx) begin
                        beat_current <= '0;
                        state <= BURST;
                    end
                end

                BURST: begin
                    /* Keep pulling until nothing left */
                    if (pull_trx) begin
                        if (beat_current == BEATS_PER_BURST - 1) begin
                            beat_current <= '0;
                            state    <= IDLE;
                        end else begin
                            beat_current <= beat_current + 1'b1;
                        end
                    end
                end

                default: state <= IDLE;

            endcase
        end
    end

    
    export "DPI-C" function ddr_preload_word;

    /* To load ELF */
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

    /* Readback a 32-bit word for final memory diff */
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