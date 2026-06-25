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
    
    `define STRBUF `BE.execute_stage.LSU.stu.str_buffer


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


    /* Each retired instruction generates exactly one RVFI commit */
    import "DPI-C" function void rvfi_commit(
        input int unsigned is_exception,
        input int unsigned pc,
        input int unsigned info,      // Instruction type info
        input int unsigned rd,
        input int unsigned rd_value,
        input int unsigned is_store,
        input int unsigned is_load,
        input int unsigned mem_addr,
        input int unsigned mem_data,
        input int unsigned mem_width
    );

    /* The load buffer stores only the address */
    logic [31:0] load_buffer [$];

    /* LDU forward match, used to replicate the pop on invalidation */
    logic forward_match, forward_match_prev;
    assign forward_match = `BE.execute_stage.LSU.ldu.forward_match_i;

    always_ff @(posedge clk) begin
        forward_match_prev <= forward_match;
    end

    always_ff @(posedge clk) begin
        if (!rst_n) begin
            load_buffer.delete();
        end else begin

            /* Fill the load queue using the LDU address at request time */
            if (`LDU.request) begin
                load_buffer.push_back(`LDU.address);
            end

            if (`LDU.invalidate) begin
                if (!forward_match_prev && (load_buffer.size() != 0)) begin
                    void'(load_buffer.pop_front());
                end
            end

            if (`BE.writeback_o) begin
                automatic int unsigned ev     = `BE.exception_vector;
                automatic int unsigned pc     = `BE.trap_iaddress;
                automatic int unsigned rd     = `BE.reg_destination_o;
                automatic int unsigned rdv    = `BE.writeback_result_o;
                automatic int unsigned is_s   = (ev == STORE_OPERATION);
                automatic int unsigned is_l   = (ev == LOAD_OPERATION);
                automatic int unsigned maddr  = 0;
                automatic int unsigned mdata  = 0;
                automatic int unsigned mwidth = 2;   // Default WORD

                if (is_s) begin
                    /* Read directly from the internal store buffer during the retire cycle.
                     * valid_ptr points to the entry currently validated by BE. This is valid
                     * because store buffer waits for instruction retirement before commiting a store */
                    maddr  = `STRBUF.metadata_buffer[`STRBUF.valid_ptr].address;
                    mdata  = `STRBUF.data_buffer[0][`STRBUF.valid_ptr];
                    mwidth = `STRBUF.store_width_buffer[`STRBUF.valid_ptr];
                end else if (is_l && (load_buffer.size() != 0)) begin
                    maddr = load_buffer.pop_front();
                end

                rvfi_commit(32'd0, pc, ev, rd, rdv, is_s, is_l, maddr, mdata, mwidth);
            end

            /* A committed synchronous trap, such as ecall M/U, is still
             * a retired event, so Spike must execute one step. */
            if (`BE.exception_o) begin
                rvfi_commit(32'd1, `BE.trap_iaddress, `BE.exception_vector,
                            32'd0, 32'd0, 32'd0, 32'd0, 32'd0, 32'd0, 32'd2);
            end
        end
    end

endmodule : cosim_top

`endif