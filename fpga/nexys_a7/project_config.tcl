# Settings shared by the implementation and behavioural-simulation projects.
# Keep the define set identical: in particular, _VIVADO_ selects the generated
# clocking and MIG instances, while VERILATOR must never be set for XSim.

set part xc7a100tcsg324-1
set top ZenithSoC

set memory_files [list \
    [file join $root_dir sw benchmark CoreMark out bootloader.hex] \
    [file join $root_dir hw apu synthesis_unit sine sine_quarter.hex] \
]

set project_defines [list \
    _VIVADO_ TRACE BMU FPU \
    BRANCH_PREDICTOR_DEPTH=1024 \
    BRANCH_TARGET_BUFFER_DEPTH=1024 \
    INSTRUCTION_BUFFER_SIZE=8 \
    STORE_BUFFER_DEPTH=8 \
    MUL_PIPE_STAGES=2 \
    "BOOT_START=32'h00000000" \
    "BOOT_END=32'h00003fff" \
    "IO_START=32'h00004000" \
    "IO_END=32'h08003fff" \
    "PRIVATE_REGION_START=32'h00000000" \
    "PRIVATE_REGION_END=32'h7fffffff" \
    "USER_MEMORY_REGION_START=32'h80000000" \
    "USER_MEMORY_REGION_END=32'hffffffff" \
    NON_MASKABLE_INTERRUPT=0 \
    TIMER_INTERRUPT=7 \
    EXTERNAL_INTERRUPT=11 \
    "HARDWARE_RESET_INTERRUPT=32'h7fffffff" \
    INSTR_MISALIGNED=0 \
    INSTR_ACCESS_FAULT=1 \
    INSTR_ILLEGAL=2 \
    BREAKPOINT=3 \
    LOAD_MISALIGNED=4 \
    LOAD_ACCESS_FAULT=5 \
    STORE_MISALIGNED=6 \
    STORE_ACCESS_FAULT=7 \
    U_SYSTEM_CALL=8 \
    M_SYSTEM_CALL=11 \
    SLEEP=16 \
    HANDLER_RETURN=17 \
    STORE_OPERATION=18 \
    LOAD_OPERATION=19 \
    BRANCH_OPERATION=20 \
    JUMP_OPERATION=21 \
    CSR_OPERATION=22 \
    NO_EVENT=31 \
]
