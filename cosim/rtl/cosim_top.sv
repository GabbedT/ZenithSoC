`ifndef COSIM_TOP_SV
    `define COSIM_TOP_SV

`ifdef COSIM_SOC_CONFIG
    `include "utils/pkg/soc_parameters.sv"
`endif

module cosim_top (
    input logic clk,
    input logic rst_n
);

    import store_unit_pkg::*;

    `define BE  cpu.system_cpu.apogeo_backend

    `define LDU cpu.cpu_load_channel
    `define STU cpu.cpu_store_channel
    `define LDUNIT `BE.execute_stage.LSU.ldu
    
    `define STRBUF `BE.execute_stage.LSU.stu.str_buffer

    `define RF cpu.system_cpu.apogeo_frontend.scheduler_unit.reg_file

    `define DCACHE cpu.dcache.dcache


//=============================================================================
//      CHANNELS
//=============================================================================

    trace_interface trace_ch();

    fetch_interface rom_ch();

    load_interface  ddr_ld();
    store_interface ddr_st();

    load_interface  io_ld();
    store_interface io_st();

    logic single_strx, instr_load, load_room, store_idle;


//=============================================================================
//      CPU COMPLEX (CPU + cache)
//=============================================================================

    cpu_complex
    `ifdef COSIM_SOC_CONFIG
    #(
        .PREDICTOR_SIZE(soc_parameters::PREDICTOR_SIZE),
        .BTB_SIZE(soc_parameters::BTB_SIZE),
        .STORE_BUFFER_SIZE(soc_parameters::STORE_BUFFER_SIZE),
        .INSTRUCTION_BUFFER_SIZE(soc_parameters::INSTRUCTION_BUFFER_SIZE),
        .ROB_DEPTH(soc_parameters::ROB_DEPTH),
        .DCACHE_SIZE(soc_parameters::DCACHE_SIZE),
        .DBLOCK_SIZE_BYTE(soc_parameters::DBLOCK_SIZE_BYTE),
        .ICACHE_SIZE(soc_parameters::ICACHE_SIZE),
        .IBLOCK_SIZE_BYTE(soc_parameters::IBLOCK_SIZE_BYTE)
    )
    `endif
    cpu (
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
        .str_ready_i        ( store_idle ),

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

    cosim_ddr #(
        `ifdef COSIM_SOC_CONFIG
        .DATA_MAX_BURST         ( soc_parameters::DBLOCK_SIZE_BYTE / 4 ),
        .INSTRUCTION_MAX_BURST  ( soc_parameters::IBLOCK_SIZE_BYTE / 4 )
        `else
        .DATA_MAX_BURST         ( 4 ),
        .INSTRUCTION_MAX_BURST  ( 4 )
        `endif
    ) ddr (
        .clk_i          ( clk ),
        .rst_n_i        ( rst_n ),
        .load_channel   ( ddr_ld ),
        .store_channel  ( ddr_st ),
        .single_trx_i   ( single_strx ),
        .instr_req_i    ( instr_load ),
        .load_empty_o   ( load_room ),
        .store_idle_o   ( store_idle )
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

    /* Mirror every accepted architectural load, including store-buffer
     * forwards and faulting loads that never appear on the cache channel. */
    logic [31:0] load_buffer [$];

    always_ff @(posedge clk) begin
        if (!rst_n) begin
            load_buffer.delete();
        end else begin

            if (`LDUNIT.accept_load) begin
                load_buffer.push_back(`LDUNIT.load_address_i);
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

            if (`LDU.invalidate) begin
                /* Commit is older than the flush.  Let a same-cycle retiring
                 * load consume the queue head first, then discard all younger
                 * accepted loads just as the RTL load FIFO does. */
                load_buffer.delete();
            end
        end
    end


//=============================================================================
//      DPI STATE READBACK
//=============================================================================

    export "DPI-C" function dut_gpr;

    function int unsigned dut_gpr(input int unsigned idx);
        if (idx == 0)
            return 32'd0;
        else
            return `RF.iregister[0][idx[4:0]];
    endfunction


    export "DPI-C" function dut_dcache_word;

    `ifdef COSIM_SOC_CONFIG
        localparam int DCACHE_INDEX_END = $clog2(soc_parameters::DCACHE_SIZE);
        localparam int DCACHE_OFFSET_END = $clog2(soc_parameters::DBLOCK_SIZE_BYTE);
    `else
        localparam int DCACHE_INDEX_END = 12;
        localparam int DCACHE_OFFSET_END = 4;
    `endif

    function int unsigned dut_dcache_word(
        input int unsigned addr,
        output int unsigned hit
    );
        automatic logic [DCACHE_INDEX_END-DCACHE_OFFSET_END-1:0] index =
            addr[DCACHE_INDEX_END-1:DCACHE_OFFSET_END];
        automatic logic [31-DCACHE_INDEX_END:0] tag = addr[31:DCACHE_INDEX_END];
        automatic logic [DCACHE_OFFSET_END-3:0] bank = addr[DCACHE_OFFSET_END-1:2];

        if (`DCACHE.valid_memory.valid_memory[index] &&
            (`DCACHE.tag_memory.memory[index] == tag)) begin

            hit = 32'd1;

            case (bank)
                0: return `DCACHE.data_memory.genblk1[0].cache_block_bank.bank_memory[index];
                1: return `DCACHE.data_memory.genblk1[1].cache_block_bank.bank_memory[index];
                2: return `DCACHE.data_memory.genblk1[2].cache_block_bank.bank_memory[index];
                3: return `DCACHE.data_memory.genblk1[3].cache_block_bank.bank_memory[index];
                `ifdef COSIM_SOC_CONFIG
                4: return `DCACHE.data_memory.genblk1[4].cache_block_bank.bank_memory[index];
                5: return `DCACHE.data_memory.genblk1[5].cache_block_bank.bank_memory[index];
                6: return `DCACHE.data_memory.genblk1[6].cache_block_bank.bank_memory[index];
                7: return `DCACHE.data_memory.genblk1[7].cache_block_bank.bank_memory[index];
                `endif
            endcase
        end

        hit = 32'd0;
        
        return 32'd0;
    endfunction

endmodule : cosim_top

`endif
