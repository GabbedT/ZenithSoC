// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Tracing declarations
#include "verilated_fst_c.h"


void Vtb_top___024root__traceDeclTypesSub0(VerilatedFst* tracep) {
    {
        const char* __VenumItemNames[]
        = {"DBIT5", "DBIT6", "DBIT7", "DBIT8"};
        const char* __VenumItemValues[]
        = {"0", "1", "10", "11"};
        tracep->declDTypeEnum(1, "uart_pkg::uart_data_lenght_t", 4, 2, __VenumItemNames, __VenumItemValues);
    }
    {
        const char* __VenumItemNames[]
        = {"STOP1", "STOP2"};
        const char* __VenumItemValues[]
        = {"0", "1"};
        tracep->declDTypeEnum(2, "uart_pkg::uart_stop_bits_t", 2, 1, __VenumItemNames, __VenumItemValues);
    }
    {
        const char* __VenumItemNames[]
        = {"EVEN", "ODD"};
        const char* __VenumItemValues[]
        = {"0", "1"};
        tracep->declDTypeEnum(3, "uart_pkg::uart_parity_mode_t", 2, 1, __VenumItemNames, __VenumItemValues);
    }
    {
        const char* __VenumItemNames[]
        = {"IDLE", "START", "DATA", "PARITY", "STOP"};
        const char* __VenumItemValues[]
        = {"0", "1", "10", "11", "100"};
        tracep->declDTypeEnum(4, "uart_receiver.uart_fsm_states_t", 5, 3, __VenumItemNames, __VenumItemValues);
    }
    {
        const char* __VenumItemNames[]
        = {"UART_STATUS", "UART_TX_BUFFER", "UART_RX_BUFFER", 
                                "UART_EVENT"};
        const char* __VenumItemValues[]
        = {"0", "1", "10", "11"};
        tracep->declDTypeEnum(5, "uart_pkg::uart_registers_t", 4, 2, __VenumItemNames, __VenumItemValues);
    }
    {
        const char* __VenumItemNames[]
        = {"IDLE", "START", "DATA", "PARITY", "STOP"};
        const char* __VenumItemValues[]
        = {"0", "1", "10", "11", "100"};
        tracep->declDTypeEnum(6, "uart_transmitter.uart_fsm_states_t", 5, 3, __VenumItemNames, __VenumItemValues);
    }
}

void Vtb_top___024root__trace_decl_types(VerilatedFst* tracep) {
    Vtb_top___024root__traceDeclTypesSub0(tracep);
}
