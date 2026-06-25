`ifndef COSIM_DDR_SV
`define COSIM_DDR_SV

module cosim_ddr #(
    parameter int DATA_MAX_BURST        = 4,   // DCACHE BLOCK_SIZE_BYTE / 4
    parameter int INSTRUCTION_MAX_BURST = 4,   // ICACHE BLOCK_SIZE_BYTE / 4
    parameter int SIZE_BYTES            = 64 * 1024 * 1024,
    parameter int DDR_LATENCY           = 4
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

    /* -------------------------------------------------------------------------
    * DDR PHYSICAL MODEL
    * (64-bit array + latency), same behaviour as ZenithSoC.sv
    * ------------------------------------------------------------------------- */
//====================================================================================
//      DDR MODEL
//====================================================================================

    logic [63:0] ddr_memory [0:DDR_WORDS - 1];

    logic [26:0] read_address_q;
    logic [$clog2(DDR_LATENCY + 1) - 1:0] latency_count;
    logic read_pending;

    `ifdef COSIM_DDR_TRACE
        int cyc = 0;
    `endif

    logic [$clog2(DDR_WORDS)-1:0] word_address;
    assign word_address = ddr_address[$clog2(DDR_WORDS) + 2:3];

    always_ff @(posedge clk_i or negedge rst_n_i) begin
        if (!rst_n_i) begin
            ddr_data_valid <= 1'b0;
            ddr_data_read  <= '0;
            read_address_q <= '0;
            latency_count  <= '0;
            read_pending   <= 1'b0;
        end else begin
            if (ddr_write && push_trx) begin
                for (int i = 0; i < 8; i = i + 1) begin
                    if (ddr_mask[i]) begin
                        ddr_memory[word_address][8*i +: 8] <= ddr_data_write[8*i +: 8];
                    end
                end
            end

            if (ddr_read && !read_pending && !ddr_data_valid) begin
                read_address_q <= ddr_address;
                read_pending   <= 1'b1;
                latency_count  <= DDR_LATENCY;
            end

            if (read_pending) begin
                if (latency_count != 0) begin
                    latency_count <= latency_count - 1'b1;
                end else begin
                    ddr_data_read  <= ddr_memory[read_address_q[26:3]];
                    ddr_data_valid <= 1'b1;
                    read_pending   <= 1'b0;
                end
            end

            if (ddr_data_valid && pull_trx) begin
                ddr_data_valid <= 1'b0;
            end

        `ifdef COSIM_DDR_TRACE

            cyc <= cyc + 1;

            if (ddr_read | ddr_write | push_trx | pull_trx |
                ddr_data_valid | read_pending |
                (ddr_controller_interface.state_CRT != 0) |
                load_channel.request |
                load_channel.valid |
                (ddr_controller_interface.ldr_count != 0)) begin

                $display(
                    "[ddrtrace] c=%0d st=%0d req=%0b lval=%0b "
                    "instr=%0b | rd=%0b push=%0b pull=%0b "
                    "addr=%08h rpend=%0b lat=%0d vld=%0b | "
                    "vaddr=%0b vdata=%0b | ldcnt=%0d ldrdy=%0b "
                    "ldempty=%0b ldsel=%0b ldwr=%0b ldrd=%0b "
                    "ldackn=%0b",

                    cyc,
                    ddr_controller_interface.state_CRT,
                    load_channel.request,
                    load_channel.valid,
                    instr_req_i,

                    ddr_read,
                    push_trx,
                    pull_trx,
                    ddr_address,
                    read_pending,
                    latency_count,
                    ddr_data_valid,

                    ddr_controller_interface.valid_address,
                    ddr_controller_interface.valid_data,

                    ddr_controller_interface.ldr_count,
                    ddr_controller_interface.ldr_ready,
                    ddr_controller_interface.ldr_empty,
                    ddr_controller_interface.ldr_select,
                    ddr_controller_interface.ldr_write,
                    ddr_controller_interface.ldr_read,
                    ddr_controller_interface.ldr_ackn
                );
            end

        `endif

        end
    end


//====================================================================================
//      CPP -> SV INITIALIZATION
//====================================================================================

    export "DPI-C" function ddr_preload_word;

    function void ddr_preload_word(
        input int unsigned byte_addr,
        input int unsigned data
    );

        automatic int unsigned widx = byte_addr >> 3;

        if (widx < DDR_WORDS) begin

            if (byte_addr[2]) begin
                ddr_memory[widx][63:32] = data;
            end else begin
                ddr_memory[widx][31:0] = data;
            end

        end

    endfunction

endmodule : cosim_ddr

`endif