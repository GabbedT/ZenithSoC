// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Tracing declarations
#include "verilated_fst_c.h"


void Vtb_top___024root__traceDeclTypesSub0(VerilatedFst* tracep) {
    {
        const char* __VenumItemNames[]
        = {"SD_CTRL", "SD_STATUS", "SD_CMD_NUMBER", 
                                "SD_CMD_ARGUMENT", 
                                "SD_EVENT", "SD_CMD_RESPONSE", 
                                "SD_DATA_TX_BUFFER", 
                                "SD_DATA_RX_BUFFER"};
        const char* __VenumItemValues[]
        = {"0", "1", "10", "11", "100", "101", "110", 
                                "111"};
        tracep->declDTypeEnum(1, "sd_pkg::sd_registers_t", 8, 3, __VenumItemNames, __VenumItemValues);
    }
    {
        const char* __VenumItemNames[]
        = {"RESP48", "RESP136"};
        const char* __VenumItemValues[]
        = {"0", "1"};
        tracep->declDTypeEnum(2, "sd_pkg::sd_response_type_t", 2, 1, __VenumItemNames, __VenumItemValues);
    }
    {
        const char* __VenumItemNames[]
        = {"IDLE", "WAIT_START", "START0", "START1", 
                                "CMD_NUMBER", "CMD_ARGUMENT", 
                                "CRC7", "END_COMMAND", 
                                "WAIT_RESP", "RESP_DIR", 
                                "RCV48", "RCV136", 
                                "CHECK_CRC", "RECOVERY"};
        const char* __VenumItemValues[]
        = {"0", "1", "10", "11", "100", "101", "110", 
                                "111", "1000", "1001", 
                                "1010", "1011", "1100", 
                                "1101"};
        tracep->declDTypeEnum(3, "sd_command_controller.cmd_states_t", 14, 4, __VenumItemNames, __VenumItemValues);
    }
    {
        const char* __VenumItemNames[]
        = {"IDLE", "WAIT_RX", "WAIT_TX", "START_BIT", 
                                "RCV_DATA", "RCV_CRC", 
                                "CHECK_CRC", "SEND_DATA", 
                                "SEND_CRC", "STOP_BIT", 
                                "WAIT_END", "WAIT_TOKEN", 
                                "CARD_RESPONSE", "READ_TOKEN", 
                                "RECOVERY_CYCLE"};
        const char* __VenumItemValues[]
        = {"0", "1", "10", "11", "100", "101", "110", 
                                "111", "1000", "1001", 
                                "1010", "1011", "1100", 
                                "1101", "1110"};
        tracep->declDTypeEnum(4, "sd_data_controller.data_states_t", 15, 4, __VenumItemNames, __VenumItemValues);
    }
}

void Vtb_top___024root__trace_decl_types(VerilatedFst* tracep) {
    Vtb_top___024root__traceDeclTypesSub0(tracep);
}
