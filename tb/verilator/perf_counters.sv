// ============================================================================
// ZenithSoC Performance Counters — Testbench-side bottleneck analysis
// ============================================================================
//
// Taps DUT signals through hierarchical references (no RTL changes needed).
// One instance per testbench; shared between verilator/ and tb/top/ flows.
//
// Counts granular pipeline events in 64-bit registers, then prints a formatted
// summary with a bottleneck line at end of simulation via $display.
// ============================================================================

`ifndef PERF_COUNTERS_SV
    `define PERF_COUNTERS_SV

module perf_counters (
    input logic clk_i,
    input logic rst_n_i
);

// ============================================================================
// SECTION A — Hierarchical tap aliases
// ============================================================================

    // -- CPU complex ---------------------------------------------------------
    wire cc_cache_flush_request = dut.ApogeoRV.cache_flush_request;
    wire cc_stall_data          = dut.ApogeoRV.stall_data;
    wire cc_stall_fetch         = dut.ApogeoRV.stall_fetch;
    wire cc_dcache_ld_req       = dut.ApogeoRV.dcache_load_channel.request;

    // -- Front-end -----------------------------------------------------------
    wire fe_stall_i              = dut.ApogeoRV.system_cpu.apogeo_frontend.stall_i;
    wire fe_stall_o              = dut.ApogeoRV.system_cpu.apogeo_frontend.stall;
    // fetch_channel is an interface — use icache controller stall as proxy
    // (ROM never stalls: it's a combinational read)
    wire fe_fetch_stall          = dut.ApogeoRV.icache.controller.stall_fetch_o;
    wire fe_ibuffer_full         = dut.ApogeoRV.system_cpu.apogeo_frontend.ibuffer_full;
    wire fe_mispredicted         = dut.ApogeoRV.system_cpu.apogeo_frontend.mispredicted;

    // -- Decode stage valid (packed struct, OR-reduce to check any valid) ----
    wire instr_waiting;
    // dc_stage_exu_valid is exu_valid_t (packed struct); | reduces to 1 if
    // any unit has a valid instruction at decode.
    assign instr_waiting = |dut.ApogeoRV.system_cpu.apogeo_frontend.dc_stage_exu_valid;

    // -- Scheduler -----------------------------------------------------------
    wire sch_rob_full_i   = dut.ApogeoRV.system_cpu.apogeo_frontend.scheduler_unit.rob_full_i;
    wire sch_flush_busy_i = dut.ApogeoRV.system_cpu.apogeo_frontend.scheduler_unit.flush_busy_i;
    // A fence stall is only meaningful when the decoded instruction is
    // actually a FENCE and either the scheduler queue or the backend is not
    // drained.  pipeline_empty_i alone is not enough to identify it.
    wire sch_fence_pending = dut.ApogeoRV.system_cpu.apogeo_frontend.dc_stage_fence;
    wire sch_fence_drain   = sch_fence_pending &&
                              ((!dut.ApogeoRV.system_cpu.apogeo_frontend.scheduler_unit.pipeline_empty) ||
                               (!dut.ApogeoRV.system_cpu.apogeo_frontend.scheduler_unit.pipeline_empty_i));

    // -- Scoreboard ----------------------------------------------------------
    wire scb_raw_hazard         = dut.ApogeoRV.system_cpu.apogeo_frontend.scheduler_unit.scoreboard_unit.raw_hazard;
    wire scb_latency_hazard     = dut.ApogeoRV.system_cpu.apogeo_frontend.scheduler_unit.scoreboard_unit.latency_hazard;
    wire scb_structural_hazard  = dut.ApogeoRV.system_cpu.apogeo_frontend.scheduler_unit.scoreboard_unit.structural_hazard;
    wire scb_block_store        = dut.ApogeoRV.system_cpu.apogeo_frontend.scheduler_unit.scoreboard_unit.block_store_operation;
    wire scb_issue_instruction  = dut.ApogeoRV.system_cpu.apogeo_frontend.scheduler_unit.scoreboard_unit.issue_instruction_o;

    // Per-unit RAW hazards (vectors — OR-reduce to detect any active)
    wire alu_raw_active, ldu_raw_active, mul_raw_active, div_raw_active;
    assign alu_raw_active = |dut.ApogeoRV.system_cpu.apogeo_frontend.scheduler_unit.scoreboard_unit.alu_raw_hazard;
    assign ldu_raw_active = |dut.ApogeoRV.system_cpu.apogeo_frontend.scheduler_unit.scoreboard_unit.ldu_raw_hazard;
    assign mul_raw_active = |dut.ApogeoRV.system_cpu.apogeo_frontend.scheduler_unit.scoreboard_unit.mul_raw_hazard;

    wire div_raw_active_internal;
    assign div_raw_active_internal = dut.ApogeoRV.system_cpu.apogeo_frontend.scheduler_unit.scoreboard_unit.div_raw_hazard;

    assign div_raw_active = div_raw_active_internal;

    // Per-unit latency hazards
    wire alu_lat_active, mul_lat_active, div_lat_active;
    assign alu_lat_active = |dut.ApogeoRV.system_cpu.apogeo_frontend.scheduler_unit.scoreboard_unit.alu_latency_hazard;
    assign mul_lat_active = |dut.ApogeoRV.system_cpu.apogeo_frontend.scheduler_unit.scoreboard_unit.mul_latency_hazard;

    wire div_lat_active_internal;
    assign div_lat_active_internal = dut.ApogeoRV.system_cpu.apogeo_frontend.scheduler_unit.scoreboard_unit.div_latency_hazard;
    assign div_lat_active = div_lat_active_internal;

    // Structural condition signals (flat logic)
    wire scb_div_executing   = dut.ApogeoRV.system_cpu.apogeo_frontend.scheduler_unit.scoreboard_unit.div_executing;
    wire scb_ldu_full        = dut.ApogeoRV.system_cpu.apogeo_frontend.scheduler_unit.scoreboard_unit.ldu_full;
    wire scb_stu_not_idle;
    assign scb_stu_not_idle = !dut.ApogeoRV.system_cpu.apogeo_frontend.scheduler_unit.scoreboard_unit.stu_idle_i;

    // -- Back-end ------------------------------------------------------------
    wire be_writeback_o    = dut.ApogeoRV.system_cpu.apogeo_backend.writeback_o;
    wire be_exception_o    = dut.ApogeoRV.system_cpu.apogeo_backend.exception_o;
    wire be_branch_flush_o = dut.ApogeoRV.system_cpu.apogeo_backend.branch_flush_o;
    wire be_flush_o        = dut.ApogeoRV.system_cpu.apogeo_backend.flush_o;
    wire be_executed_o     = dut.ApogeoRV.system_cpu.apogeo_backend.executed_o;
    wire be_stall_o        = dut.ApogeoRV.system_cpu.apogeo_backend.stall_o;

    // -- ROB -----------------------------------------------------------------
    wire rob_full = dut.ApogeoRV.system_cpu.apogeo_backend.rob.full_o;

    // -- Load unit -----------------------------------------------------------
    wire ldu_wait_o    = dut.ApogeoRV.system_cpu.apogeo_backend.execute_stage.LSU.ldu.wait_o;
    wire ldu_lbuf_full = dut.ApogeoRV.system_cpu.apogeo_backend.execute_stage.LSU.ldu.lbuf_full;

    // -- Store unit / store buffer -------------------------------------------
    wire stu_state_crt       = dut.ApogeoRV.system_cpu.apogeo_backend.execute_stage.LSU.stu.state_CRT;
    wire strbuf_full         = dut.ApogeoRV.system_cpu.apogeo_backend.execute_stage.LSU.stu.buffer_channel.full;
    wire strbuf_wait         = dut.ApogeoRV.system_cpu.apogeo_backend.execute_stage.LSU.stu.str_buffer.wait_o;

    // -- I-cache complex -----------------------------------------------------
    wire icache_flush_busy       = dut.ApogeoRV.icache.flush_busy_o;
    wire icache_ctrl_stall_fetch = dut.ApogeoRV.icache.controller.stall_fetch_o;
    // Fetch performance is measured here, so the RTL controller does not
    // need simulation-only state.  valid_o is one delivered instruction;
    // refill_instruction_valid identifies the critical-word restart path.
    wire icache_fetch_valid  = dut.ApogeoRV.icache.controller.valid_o & !icache_flush_busy;
    wire icache_fetch_refill = dut.ApogeoRV.icache.controller.refill_instruction_valid;

    // -- D-cache complex -----------------------------------------------------
    wire dcache_flush_busy = dut.ApogeoRV.dcache.flush_busy_o;
    wire [31:0] dcache_load_access  = dut.ApogeoRV.dcache.load_cache_controller.load_access_CRT;
    wire [31:0] dcache_load_hit     = dut.ApogeoRV.dcache.load_cache_controller.load_hit_CRT;
    wire [31:0] dcache_store_access = dut.ApogeoRV.dcache.store_cache_controller.store_access_CRT;
    wire [31:0] dcache_store_hit    = dut.ApogeoRV.dcache.store_cache_controller.store_hit_CRT;

    // -- DDR interface -------------------------------------------------------
    wire ddr_hold       = dut.ddr_hold;
    wire ddr_store_idle = dut.ddr_controller_interface.store_idle_o;
    wire ddr_load_empty = dut.ddr_controller_interface.load_empty_o;

    // -- Branch predictor (existing sim counters) ----------------------------
    wire [31:0] pred_predictions    = dut.ApogeoRV.system_cpu.apogeo_frontend.predictor_unit.predictions;
    wire [31:0] pred_mispredictions = dut.ApogeoRV.system_cpu.apogeo_frontend.predictor_unit.mispredictions;


// ============================================================================
// SECTION B — Event counter registers (64-bit)
// ============================================================================

    // --- B.1 Global ----------------------------------------------------------
    reg [63:0] cnt_total_cycles;
    reg [63:0] cnt_retired;
    reg [63:0] cnt_issued;
    reg [63:0] cnt_exceptions;
    reg [63:0] cnt_branches_executed;
    reg [63:0] cnt_mispredictions;
    reg [63:0] cnt_branch_flushes;
    reg [63:0] cnt_full_flushes;
    reg [63:0] cnt_fence_events;

    // --- B.2 Issue-slot breakdown -------------------------------------------
    reg [63:0] cnt_issue_slots;        // instruction issued this cycle
    reg [63:0] cnt_empty_slots;        // no instruction waiting at decode
    reg [63:0] cnt_stall_slots;        // instruction waiting but not issued

    // --- B.3 Stall-cause buckets (priority-encoded) -------------------------
    reg [63:0] cnt_st_backend;
    reg [63:0] cnt_st_flush_busy;
    reg [63:0] cnt_st_rob_full;
    reg [63:0] cnt_st_fence;           // FENCE waiting for pipeline drain
    reg [63:0] cnt_st_csr_wait;
    reg [63:0] cnt_st_fence_wb;        // FENCE waiting for cache flush
    reg [63:0] cnt_st_struct_div;
    reg [63:0] cnt_st_struct_ldu;
    reg [63:0] cnt_st_struct_stu;
    reg [63:0] cnt_st_store_blocked;
    reg [63:0] cnt_st_raw_alu;
    reg [63:0] cnt_st_raw_mul;
    reg [63:0] cnt_st_raw_div;
    reg [63:0] cnt_st_raw_ldu;
    reg [63:0] cnt_st_raw_stu;
    reg [63:0] cnt_st_lat_alu;
    reg [63:0] cnt_st_lat_mul;
    reg [63:0] cnt_st_lat_div;
    reg [63:0] cnt_st_other;            // fallback sanity bucket

    // --- B.4 Front-end ------------------------------------------------------
    reg [63:0] cnt_fe_fetch_stall;
    reg [63:0] cnt_fe_ibuffer_full;
    reg [63:0] cnt_fe_icache_conflict;
    reg [63:0] cnt_fe_fetch_invalidations;
    reg [63:0] cnt_be_bubbles;

    // --- B.5 I-cache --------------------------------------------------------
    reg [63:0] cnt_icache_flush_busy;
    // Note: icache refill stalls counted in fe_fetch_stall
    reg [31:0] icache_fetch_access;
    reg [31:0] icache_fetch_hit;

    // --- B.6 D-cache --------------------------------------------------------
    reg [63:0] cnt_dcache_flush_cycles;
    reg [63:0] cnt_dcache_flush_events;

    // --- B.7 Memory units ---------------------------------------------------
    reg [63:0] cnt_ldu_wait;
    reg [63:0] cnt_ldu_lbuf_full;
    reg [63:0] cnt_strbuf_full;
    reg [63:0] cnt_strbuf_wait;
    reg [63:0] cnt_stu_wait_buffer;
    reg [63:0] cnt_stu_wait_accept;
    reg [63:0] cnt_stu_wait_loadunit;

    // --- B.8 DDR arbitration ------------------------------------------------
    reg [63:0] cnt_ddr_stall_data;
    reg [63:0] cnt_ddr_stall_fetch;
    reg [63:0] cnt_ddr_hold;
    reg [63:0] cnt_ddr_store_idle;
    reg [63:0] cnt_ddr_load_empty;


// ============================================================================
// SECTION C — Edge detection (previous-value flops for pulse signals)
// ============================================================================

    reg prev_exception_o;
    reg prev_branch_flush_o;
    reg prev_flush_o;
    reg prev_mispredicted;
    reg prev_cache_flush_request;

    // Combined invalidate signal = OR of the three flush sources
    wire fe_fetch_invalidate_comb = fe_mispredicted
                                  | be_branch_flush_o
                                  | be_flush_o;
    reg prev_fetch_invalidate_comb;

    wire edge_exception       = be_exception_o    && !prev_exception_o;
    wire edge_branch_flush    = be_branch_flush_o && !prev_branch_flush_o;
    wire edge_full_flush      = be_flush_o        && !prev_flush_o;
    wire edge_mispredict      = fe_mispredicted   && !prev_mispredicted;
    wire edge_fetch_inval     = fe_fetch_invalidate_comb && !prev_fetch_invalidate_comb;
    wire edge_cache_flush     = cc_cache_flush_request && !prev_cache_flush_request;

    always_ff @(posedge clk_i) begin
        if (!rst_n_i) begin
            prev_exception_o        <= 1'b0;
            prev_branch_flush_o     <= 1'b0;
            prev_flush_o            <= 1'b0;
            prev_mispredicted       <= 1'b0;
            prev_fetch_invalidate_comb <= 1'b0;
            prev_cache_flush_request <= 1'b0;
        end else begin
            prev_exception_o        <= be_exception_o;
            prev_branch_flush_o     <= be_branch_flush_o;
            prev_flush_o            <= be_flush_o;
            prev_mispredicted       <= fe_mispredicted;
            prev_fetch_invalidate_comb <= fe_fetch_invalidate_comb;
            prev_cache_flush_request <= cc_cache_flush_request;
        end
    end


// ============================================================================
// SECTION D — Event counting (single always_ff)
// ============================================================================

    // STU FSM state encoding (matches store_unit.sv:162)
    localparam logic [1:0] STU_IDLE          = 2'd0;
    localparam logic [1:0] STU_WAIT_BUFFER   = 2'd1;
    localparam logic [1:0] STU_WAIT_ACCEPT   = 2'd2;
    localparam logic [1:0] STU_WAIT_LOADUNIT = 2'd3;

    // Stall signal: instruction is waiting at decode but not issued
    // issue_instruction_o from scoreboard is low when a hazard is active
    wire stalling = instr_waiting && !scb_issue_instruction;

    always_ff @(posedge clk_i) begin
        if (!rst_n_i) begin
            cnt_total_cycles        <= 64'd0;
            cnt_retired             <= 64'd0;
            cnt_issued              <= 64'd0;
            cnt_exceptions          <= 64'd0;
            cnt_branches_executed   <= 64'd0;
            cnt_mispredictions      <= 64'd0;
            cnt_branch_flushes      <= 64'd0;
            cnt_full_flushes        <= 64'd0;
            cnt_fence_events        <= 64'd0;

            cnt_issue_slots         <= 64'd0;
            cnt_empty_slots         <= 64'd0;
            cnt_stall_slots         <= 64'd0;

            cnt_st_backend          <= 64'd0;
            cnt_st_flush_busy       <= 64'd0;
            cnt_st_rob_full         <= 64'd0;
            cnt_st_fence            <= 64'd0;
            cnt_st_csr_wait         <= 64'd0;
            cnt_st_fence_wb         <= 64'd0;
            cnt_st_struct_div       <= 64'd0;
            cnt_st_struct_ldu       <= 64'd0;
            cnt_st_struct_stu       <= 64'd0;
            cnt_st_store_blocked    <= 64'd0;
            cnt_st_raw_alu          <= 64'd0;
            cnt_st_raw_mul          <= 64'd0;
            cnt_st_raw_div          <= 64'd0;
            cnt_st_raw_ldu          <= 64'd0;
            cnt_st_lat_alu          <= 64'd0;
            cnt_st_lat_mul          <= 64'd0;
            cnt_st_lat_div          <= 64'd0;
            cnt_st_other            <= 64'd0;

            cnt_fe_fetch_stall      <= 64'd0;
            cnt_fe_ibuffer_full     <= 64'd0;
            cnt_fe_icache_conflict  <= 64'd0;
            cnt_fe_fetch_invalidations <= 64'd0;
            cnt_be_bubbles          <= 64'd0;

            cnt_icache_flush_busy   <= 64'd0;
            icache_fetch_access     <= 32'd0;
            icache_fetch_hit        <= 32'd0;

            cnt_dcache_flush_cycles <= 64'd0;
            cnt_dcache_flush_events <= 64'd0;

            cnt_ldu_wait            <= 64'd0;
            cnt_ldu_lbuf_full       <= 64'd0;
            cnt_strbuf_full         <= 64'd0;
            cnt_strbuf_wait         <= 64'd0;
            cnt_stu_wait_buffer     <= 64'd0;
            cnt_stu_wait_accept     <= 64'd0;
            cnt_stu_wait_loadunit   <= 64'd0;

            cnt_ddr_stall_data      <= 64'd0;
            cnt_ddr_stall_fetch     <= 64'd0;
            cnt_ddr_hold            <= 64'd0;
            cnt_ddr_store_idle      <= 64'd0;
            cnt_ddr_load_empty      <= 64'd0;

        end else begin

            // ----- Global counters -------------------------------------------
            cnt_total_cycles <= cnt_total_cycles + 64'd1;

            if (be_writeback_o)
                cnt_retired <= cnt_retired + 64'd1;

            // An instruction issues when the scoreboard grants it AND the
            // scheduler is not blocked by backend/ROB/serialization.
            // rob_alloc_o pulses high exactly when an instruction is accepted.
            if (dut.ApogeoRV.system_cpu.apogeo_frontend.scheduler_unit.rob_alloc_o)
                cnt_issued <= cnt_issued + 64'd1;

            if (edge_exception)
                cnt_exceptions <= cnt_exceptions + 64'd1;

            if (be_executed_o)
                cnt_branches_executed <= cnt_branches_executed + 64'd1;

            if (edge_mispredict)
                cnt_mispredictions <= cnt_mispredictions + 64'd1;

            if (edge_branch_flush)
                cnt_branch_flushes <= cnt_branch_flushes + 64'd1;

            if (edge_full_flush)
                cnt_full_flushes <= cnt_full_flushes + 64'd1;

            if (edge_cache_flush)
                cnt_fence_events <= cnt_fence_events + 64'd1;

            // ----- Issue-slot breakdown --------------------------------------
            // These 3 buckets are mutually exclusive and sum to total_cycles.
            if (instr_waiting && scb_issue_instruction) begin
                // Instruction waiting AND scoreboard says issue (no hazards)
                // -> an instruction issues this cycle
                cnt_issue_slots <= cnt_issue_slots + 64'd1;
            end else if (!instr_waiting) begin
                // No valid instruction at decode stage -> empty slot (bubble
                // from I-cache miss, fetch stall, ibuffer empty, flush, etc.)
                cnt_empty_slots <= cnt_empty_slots + 64'd1;
            end else begin
                // instr_waiting && !issue_instruction: stalling due to hazard
                cnt_stall_slots <= cnt_stall_slots + 64'd1;

                // --- Priority-encoded stall attribution ----------------------
                // Each stall cycle is attributed to the first matching cause.
                // Priority order: structural causes first (backend, ROB,
                // serialization), then individual hazards.

                if (fe_stall_i) begin
                    // Back-end is asserting stall (trap_manager, commit buffer
                    // full, or CSR buffer full)
                    cnt_st_backend <= cnt_st_backend + 64'd1;
                end
                else if (sch_flush_busy_i) begin
                    cnt_st_flush_busy <= cnt_st_flush_busy + 64'd1;
                end
                else if (sch_rob_full_i) begin
                    cnt_st_rob_full <= cnt_st_rob_full + 64'd1;
                end
                // NOTE: fence/CSR serialization are registered inside the
                // scheduler (issued_fence_instruction, issued_csr_instruction).
                // They manifest as stall_o=1 with no other visible cause.
                // We detect them by exclusion: if none of the above are true
                // and no raw/latency/structural hazard is active, it's a
                // serialization stall.
                else if (scb_structural_hazard) begin
                    // Decompose structural hazard by checking individual
                    // conditions in priority order.
                    if (scb_div_executing)
                        cnt_st_struct_div <= cnt_st_struct_div + 64'd1;
                    else if (scb_ldu_full)
                        cnt_st_struct_ldu <= cnt_st_struct_ldu + 64'd1;
                    else if (scb_stu_not_idle)
                        cnt_st_struct_stu <= cnt_st_struct_stu + 64'd1;
                    else
                        cnt_st_struct_stu <= cnt_st_struct_stu + 64'd1;
                end
                else if (scb_block_store) begin
                    cnt_st_store_blocked <= cnt_st_store_blocked + 64'd1;
                end
                else if (alu_raw_active) begin
                    cnt_st_raw_alu <= cnt_st_raw_alu + 64'd1;
                end
                else if (ldu_raw_active) begin
                    cnt_st_raw_ldu <= cnt_st_raw_ldu + 64'd1;
                end
                else if (mul_raw_active) begin
                    cnt_st_raw_mul <= cnt_st_raw_mul + 64'd1;
                end
                else if (div_raw_active) begin
                    cnt_st_raw_div <= cnt_st_raw_div + 64'd1;
                end
                else if (alu_lat_active) begin
                    cnt_st_lat_alu <= cnt_st_lat_alu + 64'd1;
                end
                else if (mul_lat_active) begin
                    cnt_st_lat_mul <= cnt_st_lat_mul + 64'd1;
                end
                else if (div_lat_active) begin
                    cnt_st_lat_div <= cnt_st_lat_div + 64'd1;
                end
                else begin
                    // Serialization (fence pending, CSR pending, or other)
                    // Try to distinguish fence vs CSR by checking fence
                    // flush pending state.
                    if (cc_cache_flush_request || icache_flush_busy || dcache_flush_busy)
                        cnt_st_fence_wb <= cnt_st_fence_wb + 64'd1;
                    else if (sch_fence_drain)
                        cnt_st_fence <= cnt_st_fence + 64'd1;
                    else
                        cnt_st_csr_wait <= cnt_st_csr_wait + 64'd1;
                end
            end

            // ----- Front-end counters ----------------------------------------
            if (fe_fetch_stall)
                cnt_fe_fetch_stall <= cnt_fe_fetch_stall + 64'd1;

            if (fe_ibuffer_full)
                cnt_fe_ibuffer_full <= cnt_fe_ibuffer_full + 64'd1;

            if (cc_dcache_ld_req)
                cnt_fe_icache_conflict <= cnt_fe_icache_conflict + 64'd1;

            if (edge_fetch_inval)
                cnt_fe_fetch_invalidations <= cnt_fe_fetch_invalidations + 64'd1;

            if (be_stall_o)
                cnt_be_bubbles <= cnt_be_bubbles + 64'd1;

            // ----- I-cache counters ------------------------------------------
            if (icache_flush_busy)
                cnt_icache_flush_busy <= cnt_icache_flush_busy + 64'd1;

            if (icache_fetch_valid) begin
                icache_fetch_access <= icache_fetch_access + 32'd1;
                if (!icache_fetch_refill)
                    icache_fetch_hit <= icache_fetch_hit + 32'd1;
            end

            // ----- D-cache counters ------------------------------------------
            if (dcache_flush_busy)
                cnt_dcache_flush_cycles <= cnt_dcache_flush_cycles + 64'd1;

            if (edge_cache_flush)
                cnt_dcache_flush_events <= cnt_dcache_flush_events + 64'd1;

            // ----- Memory unit counters --------------------------------------
            if (ldu_wait_o)
                cnt_ldu_wait <= cnt_ldu_wait + 64'd1;

            if (ldu_lbuf_full)
                cnt_ldu_lbuf_full <= cnt_ldu_lbuf_full + 64'd1;

            if (strbuf_full)
                cnt_strbuf_full <= cnt_strbuf_full + 64'd1;

            if (strbuf_wait)
                cnt_strbuf_wait <= cnt_strbuf_wait + 64'd1;

            // STU FSM state tracking
            case (stu_state_crt)
                STU_WAIT_BUFFER:   cnt_stu_wait_buffer   <= cnt_stu_wait_buffer   + 64'd1;
                STU_WAIT_ACCEPT:   cnt_stu_wait_accept   <= cnt_stu_wait_accept   + 64'd1;
                STU_WAIT_LOADUNIT: cnt_stu_wait_loadunit <= cnt_stu_wait_loadunit + 64'd1;
                default: ; // IDLE — not counted as stall
            endcase

            // ----- DDR arbitration counters ----------------------------------
            if (cc_stall_data)
                cnt_ddr_stall_data <= cnt_ddr_stall_data + 64'd1;

            if (cc_stall_fetch)
                cnt_ddr_stall_fetch <= cnt_ddr_stall_fetch + 64'd1;

            if (ddr_hold)
                cnt_ddr_hold <= cnt_ddr_hold + 64'd1;

            if (ddr_store_idle)
                cnt_ddr_store_idle <= cnt_ddr_store_idle + 64'd1;

            if (ddr_load_empty)
                cnt_ddr_load_empty <= cnt_ddr_load_empty + 64'd1;

        end // !rst_n_i
    end // always_ff


// ============================================================================
// SECTION E — Report function
// ============================================================================

    function void report();
        automatic real ipc, cpi, empty_pct, stall_pct, issue_pct;
        automatic real icache_hit_rate, dcache_ld_hit_rate, dcache_st_hit_rate;
        automatic real mispred_rate;
        automatic real bucket_pct;
        automatic string bottleneck_name;
        automatic real    bottleneck_pct;
        automatic reg [63:0] bottleneck_val;

        // Derived metrics
        ipc = (cnt_total_cycles > 0) ? real'(cnt_retired) / real'(cnt_total_cycles) : 0.0;
        cpi = (cnt_retired > 0) ? real'(cnt_total_cycles) / real'(cnt_retired) : 0.0;
        empty_pct  = (cnt_total_cycles > 0) ? 100.0 * real'(cnt_empty_slots)  / real'(cnt_total_cycles) : 0.0;
        stall_pct  = (cnt_total_cycles > 0) ? 100.0 * real'(cnt_stall_slots)  / real'(cnt_total_cycles) : 0.0;
        issue_pct  = (cnt_total_cycles > 0) ? 100.0 * real'(cnt_issue_slots)  / real'(cnt_total_cycles) : 0.0;

        mispred_rate = (pred_predictions > 0) ? 100.0 * real'(pred_mispredictions) / real'(pred_predictions) : 0.0;

        icache_hit_rate    = (icache_fetch_access > 0) ? 100.0 * real'(icache_fetch_hit)    / real'(icache_fetch_access) : 0.0;
        dcache_ld_hit_rate = (dcache_load_access  > 0) ? 100.0 * real'(dcache_load_hit)     / real'(dcache_load_access)  : 0.0;
        dcache_st_hit_rate = (dcache_store_access > 0) ? 100.0 * real'(dcache_store_hit)    / real'(dcache_store_access) : 0.0;

        // ---- Print summary --------------------------------------------------
        $display("");
        $display("======================================================================");
        $display("  ZENITHSOC PERFORMANCE SUMMARY");
        $display("======================================================================");
        $display("");
        $display("  -- Global ----------------------------------------------------------");
        $display("  total_cycles           : %12d", cnt_total_cycles);
        $display("  retired_instructions   : %12d   IPC=%.3f  CPI=%.3f", cnt_retired, ipc, cpi);
        $display("  issued_instructions    : %12d", cnt_issued);
        $display("  branches_executed      : %12d", cnt_branches_executed);
        $display("");
        $display("  -- Issue Slots (%% of total cycles) ---------------------------------");
        $display("  issue_slots            : %12d  (%5.1f%%)", cnt_issue_slots, issue_pct);
        $display("  empty_slots            : %12d  (%5.1f%%)", cnt_empty_slots, empty_pct);
        $display("  stall_slots            : %12d  (%5.1f%%)", cnt_stall_slots, stall_pct);
        $display("");

        // ---- Stall cause breakdown -----------------------------------------
        if (cnt_stall_slots > 0) begin
            $display("  -- Stall Cause Breakdown (priority-encoded, %% of stall slots) ------");
            $display("  st_backend             : %12d  (%5.1f%%)", cnt_st_backend,     100.0 * real'(cnt_st_backend)     / real'(cnt_stall_slots));
            $display("  st_flush_busy          : %12d  (%5.1f%%)", cnt_st_flush_busy,  100.0 * real'(cnt_st_flush_busy)  / real'(cnt_stall_slots));
            $display("  st_rob_full            : %12d  (%5.1f%%)", cnt_st_rob_full,    100.0 * real'(cnt_st_rob_full)    / real'(cnt_stall_slots));
            $display("  st_csr_wait            : %12d  (%5.1f%%)", cnt_st_csr_wait,    100.0 * real'(cnt_st_csr_wait)    / real'(cnt_stall_slots));
            $display("  st_fence_wb            : %12d  (%5.1f%%)", cnt_st_fence_wb,    100.0 * real'(cnt_st_fence_wb)    / real'(cnt_stall_slots));
            $display("  st_fence               : %12d  (%5.1f%%)", cnt_st_fence,       100.0 * real'(cnt_st_fence)       / real'(cnt_stall_slots));
            $display("  st_struct_div          : %12d  (%5.1f%%)", cnt_st_struct_div,  100.0 * real'(cnt_st_struct_div)  / real'(cnt_stall_slots));
            $display("  st_struct_ldu          : %12d  (%5.1f%%)", cnt_st_struct_ldu,  100.0 * real'(cnt_st_struct_ldu)  / real'(cnt_stall_slots));
            $display("  st_struct_stu          : %12d  (%5.1f%%)", cnt_st_struct_stu,  100.0 * real'(cnt_st_struct_stu)  / real'(cnt_stall_slots));
            $display("  st_store_blocked       : %12d  (%5.1f%%)", cnt_st_store_blocked, 100.0 * real'(cnt_st_store_blocked) / real'(cnt_stall_slots));
            $display("  st_raw_alu             : %12d  (%5.1f%%)", cnt_st_raw_alu,     100.0 * real'(cnt_st_raw_alu)     / real'(cnt_stall_slots));
            $display("  st_raw_mul             : %12d  (%5.1f%%)", cnt_st_raw_mul,     100.0 * real'(cnt_st_raw_mul)     / real'(cnt_stall_slots));
            $display("  st_raw_div             : %12d  (%5.1f%%)", cnt_st_raw_div,     100.0 * real'(cnt_st_raw_div)     / real'(cnt_stall_slots));
            $display("  st_raw_ldu             : %12d  (%5.1f%%)", cnt_st_raw_ldu,     100.0 * real'(cnt_st_raw_ldu)     / real'(cnt_stall_slots));
            $display("  st_lat_alu             : %12d  (%5.1f%%)", cnt_st_lat_alu,     100.0 * real'(cnt_st_lat_alu)     / real'(cnt_stall_slots));
            $display("  st_lat_mul             : %12d  (%5.1f%%)", cnt_st_lat_mul,     100.0 * real'(cnt_st_lat_mul)     / real'(cnt_stall_slots));
            $display("  st_lat_div             : %12d  (%5.1f%%)", cnt_st_lat_div,     100.0 * real'(cnt_st_lat_div)     / real'(cnt_stall_slots));
            $display("  st_other               : %12d  (%5.1f%%)", cnt_st_other,       100.0 * real'(cnt_st_other)       / real'(cnt_stall_slots));
            $display("");
        end

        // ---- Flushes --------------------------------------------------------
        $display("  -- Flushes ---------------------------------------------------------");
        $display("  mispredictions         : %12d  (rate %.2f%%)", cnt_mispredictions, mispred_rate);
        $display("  branch_flushes         : %12d", cnt_branch_flushes);
        $display("  full_flushes           : %12d  (intr/except/mret)", cnt_full_flushes);
        $display("  fence_events           : %12d", cnt_fence_events);
        $display("  exceptions             : %12d", cnt_exceptions);
        $display("");

        // ---- Caches ---------------------------------------------------------
        $display("  -- Caches -----------------------------------------------------------");
        $display("  I$ accesses:%6d  hits:%6d  rate:%6.1f%%",
                icache_fetch_access, icache_fetch_hit, icache_hit_rate);
        $display("  I$ flush_busy_cycles   : %12d", cnt_icache_flush_busy);
        $display("  I$ refill_stall_cycles : %12d", cnt_fe_fetch_stall);
        $display("  D$ ld_accesses:%4d  ld_hits:%4d  rate:%6.1f%%",
                dcache_load_access, dcache_load_hit, dcache_ld_hit_rate);
        $display("  D$ st_accesses:%4d  st_hits:%4d  rate:%6.1f%%",
                dcache_store_access, dcache_store_hit, dcache_st_hit_rate);
        $display("  D$ flush_cycles:%4d  flush_events:%4d",
                cnt_dcache_flush_cycles, cnt_dcache_flush_events);
        $display("");

        // ---- Front-end / Memory --------------------------------------------
        $display("  -- Front-end --------------------------------------------------------");
        $display("  fe_fetch_stall         : %12d", cnt_fe_fetch_stall);
        $display("  fe_ibuffer_full        : %12d", cnt_fe_ibuffer_full);
        $display("  fe_icache_conflict     : %12d", cnt_fe_icache_conflict);
        $display("  fe_fetch_invalidations : %12d", cnt_fe_fetch_invalidations);
        $display("  be_bubbles             : %12d", cnt_be_bubbles);
        $display("");

        $display("  -- Memory Units -----------------------------------------------------");
        $display("  ldu_wait               : %12d", cnt_ldu_wait);
        $display("  ldu_lbuf_full          : %12d", cnt_ldu_lbuf_full);
        $display("  strbuf_full            : %12d", cnt_strbuf_full);
        $display("  strbuf_wait            : %12d", cnt_strbuf_wait);
        $display("  stu_wait_buffer        : %12d", cnt_stu_wait_buffer);
        $display("  stu_wait_accept        : %12d", cnt_stu_wait_accept);
        $display("  stu_wait_loadunit      : %12d", cnt_stu_wait_loadunit);
        $display("");

        $display("  -- DDR Arbitration --------------------------------------------------");
        $display("  ddr_stall_data         : %12d", cnt_ddr_stall_data);
        $display("  ddr_stall_fetch        : %12d", cnt_ddr_stall_fetch);
        $display("  ddr_hold               : %12d", cnt_ddr_hold);
        $display("  ddr_store_idle         : %12d", cnt_ddr_store_idle);
        $display("  ddr_load_empty         : %12d", cnt_ddr_load_empty);
        $display("");

        // ---- Invariant self-checks -----------------------------------------
        $display("  -- Self-checks ------------------------------------------------------");
        if (cnt_issue_slots + cnt_empty_slots + cnt_stall_slots == cnt_total_cycles)
            $display("  [PERF] OK: issue+empty+stall == total_cycles");
        else
            $display("  [PERF] MISMATCH: issue=%0d + empty=%0d + stall=%0d != total=%0d",
                     cnt_issue_slots, cnt_empty_slots, cnt_stall_slots, cnt_total_cycles);

        if (cnt_retired <= cnt_issued)
            $display("  [PERF] OK: retired <= issued");
        else
            $display("  [PERF] MISMATCH: retired=%0d > issued=%0d",
                     cnt_retired, cnt_issued);

        if (pred_mispredictions >= cnt_mispredictions)
            $display("  [PERF] OK: mispred_edge <= predictor_mispredictions");
        else
            $display("  [PERF] MISMATCH: mispred_edge=%0d > predictor=%0d",
                     cnt_mispredictions, pred_mispredictions);
        $display("");

        // ---- Bottleneck identification -------------------------------------
        // Find the single largest contributor among: empty slots (front-end
        // starvation), backend stall, and each individual stall bucket.
        bottleneck_val  = cnt_empty_slots;
        bottleneck_name = "empty_slots (front-end starvation)";

        if (cnt_st_backend > bottleneck_val) begin
            bottleneck_val = cnt_st_backend; bottleneck_name = "st_backend (trap/buffer full)"; end
        if (cnt_st_raw_ldu > bottleneck_val) begin
            bottleneck_val = cnt_st_raw_ldu; bottleneck_name = "st_raw_ldu (load-use RAW)"; end
        if (cnt_st_raw_alu > bottleneck_val) begin
            bottleneck_val = cnt_st_raw_alu; bottleneck_name = "st_raw_alu (ALU RAW)"; end
        if (cnt_st_flush_busy > bottleneck_val) begin
            bottleneck_val = cnt_st_flush_busy; bottleneck_name = "st_flush_busy (cache flush)"; end
        if (cnt_st_rob_full > bottleneck_val) begin
            bottleneck_val = cnt_st_rob_full; bottleneck_name = "st_rob_full (ROB backpressure)"; end
        if (cnt_st_struct_ldu > bottleneck_val) begin
            bottleneck_val = cnt_st_struct_ldu; bottleneck_name = "st_struct_ldu (load unit full)"; end
        if (cnt_st_store_blocked > bottleneck_val) begin
            bottleneck_val = cnt_st_store_blocked; bottleneck_name = "st_store_blocked (store waits loads)"; end
        if (cnt_st_raw_mul > bottleneck_val) begin
            bottleneck_val = cnt_st_raw_mul; bottleneck_name = "st_raw_mul (MUL RAW)"; end
        if (cnt_st_raw_div > bottleneck_val) begin
            bottleneck_val = cnt_st_raw_div; bottleneck_name = "st_raw_div (DIV RAW)"; end
        if (cnt_st_lat_alu > bottleneck_val) begin
            bottleneck_val = cnt_st_lat_alu; bottleneck_name = "st_lat_alu (ALU result conflict)"; end
        if (cnt_st_lat_div > bottleneck_val) begin
            bottleneck_val = cnt_st_lat_div; bottleneck_name = "st_lat_div (DIV result conflict)"; end
        if (cnt_st_lat_mul > bottleneck_val) begin
            bottleneck_val = cnt_st_lat_mul; bottleneck_name = "st_lat_mul (MUL result conflict)"; end
        if (cnt_st_struct_div > bottleneck_val) begin
            bottleneck_val = cnt_st_struct_div; bottleneck_name = "st_struct_div (DIV unit busy)"; end
        if (cnt_st_struct_stu > bottleneck_val) begin
            bottleneck_val = cnt_st_struct_stu; bottleneck_name = "st_struct_stu (store unit busy)"; end
        if (cnt_st_fence_wb > bottleneck_val) begin
            bottleneck_val = cnt_st_fence_wb; bottleneck_name = "st_fence_wb (FENCE wait flush)"; end
        if (cnt_st_csr_wait > bottleneck_val) begin
            bottleneck_val = cnt_st_csr_wait; bottleneck_name = "st_csr_wait (CSR serialization)"; end
        if (cnt_st_fence > bottleneck_val) begin
            bottleneck_val = cnt_st_fence; bottleneck_name = "st_fence (FENCE wait drain)"; end

        bottleneck_pct = (cnt_total_cycles > 0) ? 100.0 * real'(bottleneck_val) / real'(cnt_total_cycles) : 0.0;

        $display("  ====================================================================");
        $display("  BOTTLENECK: %s", bottleneck_name);
        $display("  (%0d cycles, %.1f%% of total)", bottleneck_val, bottleneck_pct);
        $display("  ====================================================================");
        $display("");
    endfunction : report

endmodule : perf_counters

`endif
