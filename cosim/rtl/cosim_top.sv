`ifndef COSIM_TOP_SV
    `define COSIM_TOP_SV

module cosim_top (
    input logic clk,
    input logic rst_n
);

    import store_unit_pkg::*;

    `define BE  cpu.system_cpu.apogeo_backend

    `define LDU cpu.cpu_load_channel
    `define STU cpu.cpu_store_channel


//=============================================================================
//      CHANNELS
//=============================================================================

    trace_interface trace_ch();

    fetch_interface rom_ch();

    load_interface  ddr_ld();
    store_interface ddr_st();

    load_interface  io_ld();
    store_interface io_st();

    logic single_strx, instr_load, load_room;


//=============================================================================
//      CPU COMPLEX (CPU + cache)
//=============================================================================

    cpu_complex cpu (
        .clk_i              ( clk ),
        .rst_n_i            ( rst_n ),
        .halt_i             ( 1'b0 ),

        .trace_channel      ( trace_ch ),

        .rom_fetch_channel  ( rom_ch ),

        .ddr_load_channel   ( ddr_ld ),
        .ddr_store_channel  ( ddr_st ),

        .io_load_channel    ( io_ld ),
        .io_store_channel   ( io_st ),

        .single_strx_o      ( single_strx ),
        .instr_load_o       ( instr_load ),

        .ldr_ready_i        ( load_room ),

        /* No interrupts */
        .gen_interrupt_i    ( 1'b0 ),
        .nmsk_interrupt_i   ( 1'b0 ),
        .timer_interrupt_i  ( 1'b0 ),
        .interrupt_vector_i ( 8'b0 ),
        .interrupt_ackn_o   (      )
    );


//=============================================================================
//      MEMORIES
//=============================================================================

    cosim_rom #(
        .WORDS(4096)
    ) boot_rom (
        .clk_i          ( clk ),
        .rst_n_i        ( rst_n ),
        .fetch_channel  ( rom_ch )
    );

    /* DBLOCK_SIZE_BYTE = IBLOCK_SIZE_BYTE = 16 -> 4-word burst */
    cosim_ddr #(
        .DATA_MAX_BURST         ( 4 ),
        .INSTRUCTION_MAX_BURST  ( 4 )
    ) ddr (
        .clk_i          ( clk ),
        .rst_n_i        ( rst_n ),
        .load_channel   ( ddr_ld ),
        .store_channel  ( ddr_st ),
        .single_trx_i   ( single_strx ),
        .instr_req_i    ( instr_load ),
        .load_empty_o   ( load_room )
    );

    cosim_io io_stub (
        .clk_i            ( clk ),
        .rst_n_i          ( rst_n ),
        .io_load_channel  ( io_ld ),
        .io_store_channel ( io_st )
    );

//=============================================================================
//      RVFI TAP
//=============================================================================

    import "DPI-C" function void rvfi_commit(
        input int unsigned is_exception,
        input int unsigned pc,
        input int unsigned info,       // exception_vector (incl. STORE/LOAD/CSR op)
        input int unsigned rd,
        input int unsigned rd_value,
        input int unsigned is_store,
        input int unsigned is_load,
        input int unsigned mem_addr,
        input int unsigned mem_data,
        input int unsigned mem_width   // 0=BYTE 1=HALF_WORD 2=WORD
    );

    typedef struct packed {
        logic [31:0] address;
        logic [31:0] data;
        logic [1:0]  width;    // store_width_t: 0=BYTE 1=HALF_WORD 2=WORD
    } store_pkt_t;

    /* The load buffer only stores the address: it is a queue of words,
     * not a single-field struct */
    store_pkt_t store_buffer[$];
    logic [31:0] load_buffer[$];

    /* LDU forward match */
    logic forward_match, forward_match_prev;
    assign forward_match = `BE.execute_stage.LSU.ldu.forward_match_i;

    always_ff @(posedge clk) begin
        forward_match_prev <= forward_match;
    end

    always_ff @(posedge clk) begin
        if (!rst_n) begin
            store_buffer.delete();
            load_buffer.delete();
        end else begin
            /* Populate the memory access queues */
            if (`LDU.request) begin
                load_buffer.push_back(`LDU.address);
            end

            if (`STU.request) begin
                /* Field-by-field construction */
                automatic store_pkt_t st_pkt;
                st_pkt.address = `STU.address;
                st_pkt.data    = `STU.data;
                st_pkt.width   = `STU.width;
                store_buffer.push_back(st_pkt);
            end

            if (`LDU.invalidate) begin
                if (!forward_match_prev && (load_buffer.size() != 0)) begin
                    void'(load_buffer.pop_front());
                end
            end

            /* Normal writeback: associate memory address/data and notify RVFI. */
            if (`BE.writeback_o) begin
                automatic int unsigned ev     = `BE.exception_vector;
                automatic int unsigned pc     = `BE.trap_iaddress;
                automatic int unsigned rd     = `BE.reg_destination_o;
                automatic int unsigned rdv    = `BE.writeback_result_o;
                automatic int unsigned is_s   = (ev == STORE_OPERATION);
                automatic int unsigned is_l   = (ev == LOAD_OPERATION);
                automatic int unsigned maddr  = 0;
                automatic int unsigned mdata  = 0;
                automatic int unsigned mwidth = 2;   // default WORD

                if (is_s && (store_buffer.size() != 0)) begin
                    automatic store_pkt_t s = store_buffer.pop_front();
                    maddr  = s.address;
                    mdata  = s.data;
                    mwidth = s.width;
                end
                else if (is_l && (load_buffer.size() != 0)) begin
                    maddr = load_buffer.pop_front();
                end

                rvfi_commit(32'd0, pc, ev, rd, rdv, is_s, is_l,
                            maddr, mdata, mwidth);
            end

            /* A committed synchronous trap (e.g. M/U ecall) is still a retired
             * event, therefore Spike must advance by one step. */
            if (`BE.exception_o) begin
                rvfi_commit(32'd1, `BE.trap_iaddress, `BE.exception_vector,
                            32'd0, 32'd0, 32'd0, 32'd0, 32'd0, 32'd0, 32'd2);
            end
        end
    end

endmodule : cosim_top

`endif