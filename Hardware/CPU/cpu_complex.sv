`ifndef CPU_COMPLEX_SV
    `define CPU_COMPLEX_SV

`include "../Memory/Cache/data_cache_complex.sv"
`include "../Memory/Cache/instruction_cache_complex.sv"

`include "ApogeoRV/Hardware/ApogeoRV.sv"

`include "../System/edge_detector.sv"

`include "ApogeoRV/Hardware/Include/Interfaces/bus_interface.sv"

module cpu_complex #(
    /* Predictor table size */ 
    parameter PREDICTOR_SIZE = 512, 

    /* Branch target buffer cache size */
    parameter BTB_SIZE = 512, 

    /* Store buffer entries number */
    parameter STORE_BUFFER_SIZE = 4, 

    /* Maximum number of instruction held by the buffer */
    parameter INSTRUCTION_BUFFER_SIZE = 8,

    /* Reorder Buffer entries */
    parameter ROB_DEPTH = 32,

    /* Data cache parameters */
    parameter DCACHE_SIZE = 2**12,
    parameter DBLOCK_SIZE_BYTE = 16,

    /* Instruction cache parameters */
    parameter ICACHE_SIZE = 2**12,
    parameter IBLOCK_SIZE_BYTE = 16
) (
    input logic clk_i,
    input logic rst_n_i,

    /* Boot ROM interface */
    fetch_interface.master rom_fetch_channel,

    /* Cache to DDR channels */
    load_interface.master ddr_load_channel, 
    store_interface.master ddr_store_channel,

    /* MMIO channels */
    load_interface.master io_load_channel, 
    store_interface.master io_store_channel,

    /* Store transaction is not burst */
    output logic single_strx_o,
    output logic instr_load_o,

    /* Room for load requests */
    input logic ldr_ready_i,

    /* Interrupts */
    input logic gen_interrupt_i,
    input logic nmsk_interrupt_i,
    input logic timer_interrupt_i, 
    input logic [7:0] interrupt_vector_i,
    output logic interrupt_ackn_o
);

//====================================================================================
//      DATA CACHE
//====================================================================================

    /* CPU channels */
    fetch_interface cpu_fetch_channel(); 
    load_interface cpu_load_channel();
    store_interface cpu_store_channel();

    /* Core instantiation */
    ApogeoRV #(PREDICTOR_SIZE, BTB_SIZE, STORE_BUFFER_SIZE, INSTRUCTION_BUFFER_SIZE, ROB_DEPTH) system_cpu (
        .clk_i   ( clk_i   ),
        .rst_n_i ( rst_n_i ),

        .fetch_channel ( cpu_fetch_channel ), 

        .interrupt_i        ( gen_interrupt_i    ), 
        .non_maskable_int_i ( nmsk_interrupt_i   ),
        .timer_interrupt_i  ( timer_interrupt_i  ), 
        .interrupt_vector_i ( interrupt_vector_i ),
        .interrupt_ackn_o   ( interrupt_ackn_o   ),

        .load_channel  ( cpu_load_channel  ), 
        .store_channel ( cpu_store_channel )
    );

    assign rom_fetch_channel.fetch = cpu_fetch_channel.fetch & (cpu_fetch_channel.address <= `BOOT_END);
    assign rom_fetch_channel.address = cpu_fetch_channel.address;
    assign rom_fetch_channel.invalidate = cpu_fetch_channel.invalidate;
    assign rom_fetch_channel.stall = 1'b0;


//====================================================================================
//      DATA CACHE
//====================================================================================

    load_interface dcache_load_channel();
    store_interface dcache_store_channel();

    logic stall_data;
    
    data_cache_complex #(DCACHE_SIZE, DBLOCK_SIZE_BYTE) dcache (
        .clk_i   ( clk_i      ),
        .rst_n_i ( rst_n_i    ),
        .stall_i ( stall_data ),

        .single_trx_o ( single_strx_o ),

        /* Load unit interface */
        .ldu_channel ( cpu_load_channel ),

        /* Store unit interface */
        .stu_channel ( cpu_store_channel ),

        /* DDR Memory load interface */
        .ddr_load_channel ( dcache_load_channel ),

        /* DDR  Memory store interface */
        .ddr_store_channel ( dcache_store_channel ),

        /* I/O Memory load interface */
        .io_load_channel ( io_load_channel ),

        /* I/O  Memory store interface */
        .io_store_channel ( io_store_channel )
    );


    assign ddr_store_channel.address = dcache_store_channel.address - `USER_MEMORY_REGION_START;
    assign ddr_store_channel.data = dcache_store_channel.data;
    assign ddr_store_channel.request = dcache_store_channel.request;
    assign ddr_store_channel.width = dcache_store_channel.width;

    assign dcache_store_channel.done = ddr_store_channel.done;

//====================================================================================
//      INSTRUCTION CACHE
//====================================================================================

    logic stall_fetch, region_switch;

    load_interface icache_load_channel();
    fetch_interface icache_fetch_channel();

    instruction_cache_complex #(ICACHE_SIZE, IBLOCK_SIZE_BYTE) icache (
        .clk_i           ( clk_i         ), 
        .rst_n_i         ( rst_n_i       ),
        .stall_i         ( stall_fetch   ),
        .region_switch_i ( region_switch ),

        /* Fetch unit interface */
        .fetch_channel ( icache_fetch_channel ),

        /* Memory load interface */ 
        .load_channel ( icache_load_channel )
    );

    assign icache_fetch_channel.fetch = cpu_fetch_channel.fetch & (cpu_fetch_channel.address >= `USER_MEMORY_REGION_START);
    assign icache_fetch_channel.address = cpu_fetch_channel.address;
    assign icache_fetch_channel.invalidate = cpu_fetch_channel.invalidate;


        always_comb begin
            cpu_fetch_channel.valid = rom_fetch_channel.valid | icache_fetch_channel.valid;
            cpu_fetch_channel.stall = rom_fetch_channel.stall | icache_fetch_channel.stall;

            case ({rom_fetch_channel.valid, icache_fetch_channel.valid})
                2'b01: cpu_fetch_channel.instruction = icache_fetch_channel.instruction;

                default: cpu_fetch_channel.instruction = rom_fetch_channel.instruction;
            endcase
        end


    logic rom_region;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                rom_region <= 1'b1;
            end else begin 
                if (rom_fetch_channel.fetch) begin
                    rom_region <= 1'b1;
                end else if (icache_fetch_channel.fetch) begin
                    rom_region <= 1'b0;
                end
            end 
        end 

    /* ROM region to DDR region */
    assign region_switch = rom_region & icache_fetch_channel.fetch;


//====================================================================================
//      LOAD CHANNEL ARBITER
//====================================================================================

    localparam ICACHE = 1'b0;
    localparam DCACHE = 1'b1;


        always_comb begin : request_arbiter
            /* Default value */
            ddr_load_channel.address = '0;
            instr_load_o = 1'b0;
            
            case ({icache_load_channel.request, dcache_load_channel.request})
                2'b11, 2'b01: begin
                    /* Data cache has more priority */
                    ddr_load_channel.address = dcache_load_channel.address - `USER_MEMORY_REGION_START;
                end

                2'b10: begin
                    ddr_load_channel.address = icache_load_channel.address - `USER_MEMORY_REGION_START;

                    instr_load_o = 1'b1;
                end
            endcase

            ddr_load_channel.request = icache_load_channel.request | dcache_load_channel.request; 
            ddr_load_channel.invalidate = icache_load_channel.invalidate | dcache_load_channel.invalidate; 
        end : request_arbiter


    logic valid_negedge;

    edge_detector #(0, 0) valid_negedge_detector (
        .clk_i   ( clk_i ),
        .rst_n_i ( rst_n_i ),

        .signal_i ( ddr_load_channel.valid  ),
        .edge_o   ( valid_negedge           )
    );

    /* Select the routing of the incoming data and valid signal */
    logic priority_bit, valid_stall;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                priority_bit <= ICACHE;
                valid_stall <= 1'b0;
            end else begin 
                if (ddr_load_channel.request & !valid_stall) begin
                    priority_bit <= dcache_load_channel.request & !icache_load_channel.request;
                end

                if (valid_negedge) begin
                    valid_stall <= 1'b0;
                end else if (ddr_load_channel.request) begin
                    valid_stall <= 1'b1;
                end
            end 
        end 

    assign stall_data = valid_stall & !priority_bit;

    assign stall_fetch = valid_stall & priority_bit;


        always_comb begin : response_arbiter
            dcache_load_channel.valid = 1'b0;
            icache_load_channel.valid = 1'b0;
            dcache_load_channel.data = '0;
            icache_load_channel.data = '0;

            if (priority_bit == ICACHE) begin
                icache_load_channel.valid = ddr_load_channel.valid;
                icache_load_channel.data = ddr_load_channel.data;
            end else if (priority_bit == DCACHE) begin
                dcache_load_channel.valid = ddr_load_channel.valid;
                dcache_load_channel.data = ddr_load_channel.data;
            end
        end : response_arbiter

endmodule : cpu_complex

`endif 