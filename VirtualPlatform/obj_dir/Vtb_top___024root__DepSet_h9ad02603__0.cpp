// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtb_top.h for the primary calling header

#include "Vtb_top__pch.h"
#include "Vtb_top___024root.h"

void Vtb_top___024root___ico_sequent__TOP__0(Vtb_top___024root* vlSelf);

void Vtb_top___024root___eval_ico(Vtb_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_top___024root___eval_ico\n"); );
    Vtb_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1ULL & vlSelfRef.__VicoTriggered.word(0U))) {
        Vtb_top___024root___ico_sequent__TOP__0(vlSelf);
    }
}

VL_INLINE_OPT void Vtb_top___024root___ico_sequent__TOP__0(Vtb_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_top___024root___ico_sequent__TOP__0\n"); );
    Vtb_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.write_done_o = vlSelfRef.write_i;
    vlSelfRef.write_error_o = ((2U == (3U & vlSelfRef.write_address_i)) 
                               & (IData)(vlSelfRef.write_i));
    vlSelfRef.read_error_o = ((1U == (3U & vlSelfRef.read_address_i)) 
                              & (IData)(vlSelfRef.read_i));
    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT____Vcellinp__TX_buffer__write_i 
        = ((~ (vlSelfRef.write_address_i >> 1U)) & 
           ((((IData)(1U) << (1U & vlSelfRef.write_address_i)) 
             >> 1U) & ((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__tx_full)) 
                       & (IData)(vlSelfRef.write_i))));
    vlSelfRef.read_done_o = ((2U == (3U & vlSelfRef.read_address_i))
                              ? (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__read_done_delay)
                              : (IData)(vlSelfRef.read_i));
    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT____Vcellinp__RX_buffer__read_i 
        = ((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__rx_empty)) 
           & ((2U == (3U & vlSelfRef.read_address_i)) 
              & (IData)(vlSelfRef.read_i)));
    vlSelfRef.read_data_o = 0U;
    if ((0U == (3U & vlSelfRef.read_address_i))) {
        vlSelfRef.read_data_o = vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register;
    } else if ((2U == (3U & vlSelfRef.read_address_i))) {
        vlSelfRef.read_data_o = vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__rx_buffer_read;
    } else if ((3U == (3U & vlSelfRef.read_address_i))) {
        vlSelfRef.read_data_o = vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__event_register;
    }
}

void Vtb_top___024root___eval_triggers__ico(Vtb_top___024root* vlSelf);

bool Vtb_top___024root___eval_phase__ico(Vtb_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_top___024root___eval_phase__ico\n"); );
    Vtb_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    CData/*0:0*/ __VicoExecute;
    // Body
    Vtb_top___024root___eval_triggers__ico(vlSelf);
    __VicoExecute = vlSelfRef.__VicoTriggered.any();
    if (__VicoExecute) {
        Vtb_top___024root___eval_ico(vlSelf);
    }
    return (__VicoExecute);
}

void Vtb_top___024root___act_sequent__TOP__0(Vtb_top___024root* vlSelf);

void Vtb_top___024root___eval_act(Vtb_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_top___024root___eval_act\n"); );
    Vtb_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1ULL & vlSelfRef.__VactTriggered.word(0U))) {
        Vtb_top___024root___act_sequent__TOP__0(vlSelf);
        vlSelfRef.__Vm_traceActivity[1U] = 1U;
    }
}

VL_INLINE_OPT void Vtb_top___024root___act_sequent__TOP__0(Vtb_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_top___024root___act_sequent__TOP__0\n"); );
    Vtb_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmit 
        = ((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__tx_empty)) 
           & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__tx_idle));
    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT____Vcellinp__TX_buffer__read_i 
        = ((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__tx_empty)) 
           & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmit));
    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__tx_idle = 0U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__reset_bit_count = 0U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__counter_reset = 0U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__state_NXT 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__state_CRT;
    if ((0U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__state_CRT))) {
        vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__tx_idle = 1U;
        if (((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmit) 
             & (vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register 
                >> 0xaU))) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__tx_idle = 1U;
            vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__reset_bit_count = 1U;
            vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__counter_reset = 1U;
            vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__state_NXT = 1U;
        }
    } else if ((1U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__state_CRT))) {
        if (((0xfU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__sampling_counter)) 
             & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__sample))) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__counter_reset = 1U;
            vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__state_NXT = 2U;
        }
    } else if ((2U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__state_CRT))) {
        if (((0xfU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__sampling_counter)) 
             & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__sample))) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__counter_reset = 1U;
            if ((0x8000U & vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register)) {
                if ((0x4000U & vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register)) {
                    if ((7U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__bit_count))) {
                        vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__state_NXT 
                            = ((0x800U & vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register)
                                ? 3U : 4U);
                    }
                } else if ((6U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__bit_count))) {
                    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__state_NXT 
                        = ((0x800U & vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register)
                            ? 3U : 4U);
                }
            } else if ((0x4000U & vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register)) {
                if ((5U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__bit_count))) {
                    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__state_NXT 
                        = ((0x800U & vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register)
                            ? 3U : 4U);
                }
            } else if ((4U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__bit_count))) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__state_NXT 
                    = ((0x800U & vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register)
                        ? 3U : 4U);
            }
        }
    } else if ((3U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__state_CRT))) {
        if (((0xfU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__sampling_counter)) 
             & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__sample))) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__counter_reset = 1U;
            vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__state_NXT = 4U;
        }
    } else if ((4U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__state_CRT))) {
        if ((0x2000U & vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register)) {
            if ((0x2000U & vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register)) {
                if (((0xfU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__sampling_counter)) 
                     & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__sample))) {
                    if ((0U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__stop_bit_CRT))) {
                        vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__counter_reset = 1U;
                        vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__state_NXT = 0U;
                    }
                }
            }
        } else if (((0xfU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__sampling_counter)) 
                    & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__sample))) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__counter_reset = 1U;
            vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__state_NXT = 0U;
        }
    }
}

void Vtb_top___024root___nba_sequent__TOP__0(Vtb_top___024root* vlSelf);
void Vtb_top___024root___nba_comb__TOP__0(Vtb_top___024root* vlSelf);

void Vtb_top___024root___eval_nba(Vtb_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_top___024root___eval_nba\n"); );
    Vtb_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((2ULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        Vtb_top___024root___nba_sequent__TOP__0(vlSelf);
        vlSelfRef.__Vm_traceActivity[2U] = 1U;
    }
    if ((3ULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        Vtb_top___024root___nba_comb__TOP__0(vlSelf);
        vlSelfRef.__Vm_traceActivity[3U] = 1U;
    }
}

extern const VlUnpacked<CData/*7:0*/, 1024> Vtb_top__ConstPool__TABLE_h659593fe_0;

VL_INLINE_OPT void Vtb_top___024root___nba_sequent__TOP__0(Vtb_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_top___024root___nba_sequent__TOP__0\n"); );
    Vtb_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    SData/*9:0*/ __Vtableidx1;
    __Vtableidx1 = 0;
    CData/*3:0*/ __Vdly__tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__sampling_counter;
    __Vdly__tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__sampling_counter = 0;
    CData/*7:0*/ __Vdly__tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__tx_data;
    __Vdly__tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__tx_data = 0;
    CData/*0:0*/ __Vdly__tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__parity_bit;
    __Vdly__tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__parity_bit = 0;
    CData/*2:0*/ __Vdly__tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__bit_count;
    __Vdly__tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__bit_count = 0;
    CData/*1:0*/ __Vdly__tb_top__DOT__u_dut__DOT__u_uart__DOT__rx_line_sync__DOT__sync_signal;
    __Vdly__tb_top__DOT__u_dut__DOT__u_uart__DOT__rx_line_sync__DOT__sync_signal = 0;
    CData/*3:0*/ __Vdly__tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__sampling_counter;
    __Vdly__tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__sampling_counter = 0;
    CData/*7:0*/ __Vdly__tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__rx_data;
    __Vdly__tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__rx_data = 0;
    CData/*0:0*/ __Vdly__tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__parity_bit;
    __Vdly__tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__parity_bit = 0;
    CData/*2:0*/ __Vdly__tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__bit_count;
    __Vdly__tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__bit_count = 0;
    CData/*7:0*/ __VdlyVal__tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__TX_buffer__DOT__buffer_memory__v0;
    __VdlyVal__tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__TX_buffer__DOT__buffer_memory__v0 = 0;
    SData/*8:0*/ __VdlyDim0__tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__TX_buffer__DOT__buffer_memory__v0;
    __VdlyDim0__tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__TX_buffer__DOT__buffer_memory__v0 = 0;
    CData/*0:0*/ __VdlySet__tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__TX_buffer__DOT__buffer_memory__v0;
    __VdlySet__tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__TX_buffer__DOT__buffer_memory__v0 = 0;
    CData/*7:0*/ __VdlyVal__tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__RX_buffer__DOT__buffer_memory__v0;
    __VdlyVal__tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__RX_buffer__DOT__buffer_memory__v0 = 0;
    SData/*8:0*/ __VdlyDim0__tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__RX_buffer__DOT__buffer_memory__v0;
    __VdlyDim0__tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__RX_buffer__DOT__buffer_memory__v0 = 0;
    CData/*0:0*/ __VdlySet__tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__RX_buffer__DOT__buffer_memory__v0;
    __VdlySet__tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__RX_buffer__DOT__buffer_memory__v0 = 0;
    // Body
    __Vdly__tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__bit_count 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__bit_count;
    __Vdly__tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__rx_data 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__rx_data;
    __Vdly__tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__bit_count 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__bit_count;
    __VdlySet__tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__TX_buffer__DOT__buffer_memory__v0 = 0U;
    __VdlySet__tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__RX_buffer__DOT__buffer_memory__v0 = 0U;
    __Vdly__tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__tx_data 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__tx_data;
    __Vdly__tb_top__DOT__u_dut__DOT__u_uart__DOT__rx_line_sync__DOT__sync_signal 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__rx_line_sync__DOT__sync_signal;
    __Vdly__tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__parity_bit 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__parity_bit;
    __Vdly__tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__parity_bit 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__parity_bit;
    __Vdly__tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__sampling_counter 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__sampling_counter;
    __Vdly__tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__sampling_counter 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__sampling_counter;
    if (vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__start_receiving) {
        __Vdly__tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__bit_count = 0U;
        __Vdly__tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__rx_data = 0U;
        vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__stop_bit_CRT = 0U;
        __Vdly__tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__parity_bit 
            = (1U & (vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register 
                     >> 0xcU));
        vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__parity_error = 0U;
    } else {
        if (vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__reset_bit_count) {
            __Vdly__tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__bit_count = 0U;
        } else if (vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__shift_data) {
            __Vdly__tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__bit_count 
                = (7U & ((IData)(1U) + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__bit_count)));
        }
        if (vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__shift_data) {
            __Vdly__tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__rx_data 
                = ((0x80U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__rx_line_sync__DOT__sync_signal) 
                             << 6U)) | (0x7fU & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__rx_data) 
                                                 >> 1U)));
        }
        vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__stop_bit_CRT 
            = vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__stop_bit_NXT;
        if (vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__update_parity) {
            __Vdly__tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__parity_bit 
                = ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__parity_bit) 
                   ^ ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__rx_line_sync__DOT__sync_signal) 
                      >> 1U));
        }
        if (vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__check_parity) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__parity_error 
                = ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__parity_bit) 
                   != (1U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__rx_line_sync__DOT__sync_signal) 
                             >> 1U)));
        }
    }
    if (vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmit) {
        __Vdly__tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__bit_count = 0U;
        __Vdly__tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__parity_bit 
            = (1U & (vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register 
                     >> 0xcU));
        vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__stop_bit_CRT = 0U;
    } else {
        if (vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__reset_bit_count) {
            __Vdly__tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__bit_count = 0U;
        } else if (vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__shift_data) {
            __Vdly__tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__bit_count 
                = (7U & ((IData)(1U) + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__bit_count)));
        }
        if (vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__update_parity) {
            __Vdly__tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__parity_bit 
                = (1U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__parity_bit) 
                         ^ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__tx_data)));
        }
        vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__stop_bit_CRT 
            = vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__stop_bit_NXT;
    }
    if (vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT____Vcellinp__TX_buffer__write_i) {
        __VdlyVal__tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__TX_buffer__DOT__buffer_memory__v0 
            = (0xffU & vlSelfRef.write_data_i);
        __VdlyDim0__tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__TX_buffer__DOT__buffer_memory__v0 
            = vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__TX_buffer__DOT__write_ptr;
        __VdlySet__tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__TX_buffer__DOT__buffer_memory__v0 = 1U;
    }
    if (vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT____Vcellinp__RX_buffer__write_i) {
        __VdlyVal__tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__RX_buffer__DOT__buffer_memory__v0 
            = vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__rx_data;
        __VdlyDim0__tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__RX_buffer__DOT__buffer_memory__v0 
            = vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__RX_buffer__DOT__write_ptr;
        __VdlySet__tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__RX_buffer__DOT__buffer_memory__v0 = 1U;
    }
    if (vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__load_data) {
        __Vdly__tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__tx_data 
            = vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__tx_data;
    } else if (vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__shift_data) {
        __Vdly__tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__tx_data 
            = (0xffU & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__tx_data) 
                        >> 1U));
    }
    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__read_done_delay 
        = ((IData)(vlSelfRef.rst_n) && ((2U == (3U 
                                                & vlSelfRef.read_address_i)) 
                                        & (IData)(vlSelfRef.read_i)));
    if (vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT____Vcellinp__RX_buffer__read_i) {
        vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__rx_buffer_read 
            = vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__RX_buffer__DOT__buffer_memory
            [vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__RX_buffer__DOT__read_ptr];
    }
    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__genblk1__BRA__0__KET____DOT__event_detector__DOT__previous_signal 
        = ((IData)(vlSelfRef.rst_n) && (1U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__event_register)));
    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__genblk1__BRA__1__KET____DOT__event_detector__DOT__previous_signal 
        = ((IData)(vlSelfRef.rst_n) && (1U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__event_register) 
                                              >> 1U)));
    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__genblk1__BRA__2__KET____DOT__event_detector__DOT__previous_signal 
        = ((IData)(vlSelfRef.rst_n) && (1U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__event_register) 
                                              >> 2U)));
    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__genblk1__BRA__3__KET____DOT__event_detector__DOT__previous_signal 
        = ((IData)(vlSelfRef.rst_n) && (1U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__event_register) 
                                              >> 3U)));
    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__genblk1__BRA__4__KET____DOT__event_detector__DOT__previous_signal 
        = ((IData)(vlSelfRef.rst_n) && (1U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__event_register) 
                                              >> 4U)));
    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__bit_count 
        = __Vdly__tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__bit_count;
    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__rx_data 
        = __Vdly__tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__rx_data;
    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__bit_count 
        = __Vdly__tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__bit_count;
    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__parity_bit 
        = __Vdly__tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__parity_bit;
    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__tx_data 
        = __Vdly__tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__tx_data;
    if (vlSelfRef.rst_n) {
        __Vdly__tb_top__DOT__u_dut__DOT__u_uart__DOT__rx_line_sync__DOT__sync_signal 
            = ((2U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__rx_line_sync__DOT__sync_signal) 
                      << 1U)) | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__uart_tx));
        if (vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__counter_reset) {
            __Vdly__tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__sampling_counter = 0U;
        } else if (((vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register 
                     >> 0xaU) & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__sample))) {
            if (vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__counter_enable) {
                __Vdly__tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__sampling_counter 
                    = (0xfU & ((IData)(1U) + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__sampling_counter)));
            }
        }
        vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__sampling_counter 
            = __Vdly__tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__sampling_counter;
        if (vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__counter_reset) {
            __Vdly__tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__sampling_counter = 0U;
        } else if (((vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register 
                     >> 9U) & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__sample))) {
            if (vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__counter_enable) {
                __Vdly__tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__sampling_counter 
                    = (0xfU & ((IData)(1U) + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__sampling_counter)));
            }
        }
    } else {
        __Vdly__tb_top__DOT__u_dut__DOT__u_uart__DOT__rx_line_sync__DOT__sync_signal = 3U;
        __Vdly__tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__sampling_counter = 0U;
        vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__sampling_counter 
            = __Vdly__tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__sampling_counter;
        __Vdly__tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__sampling_counter = 0U;
    }
    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__sampling_counter 
        = __Vdly__tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__sampling_counter;
    if (__VdlySet__tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__RX_buffer__DOT__buffer_memory__v0) {
        vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__RX_buffer__DOT__buffer_memory[__VdlyDim0__tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__RX_buffer__DOT__buffer_memory__v0] 
            = __VdlyVal__tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__RX_buffer__DOT__buffer_memory__v0;
    }
    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__parity_bit 
        = __Vdly__tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__parity_bit;
    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__rx_line_sync__DOT__sync_signal 
        = __Vdly__tb_top__DOT__u_dut__DOT__u_uart__DOT__rx_line_sync__DOT__sync_signal;
    if (vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT____Vcellinp__TX_buffer__read_i) {
        vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__tx_data 
            = vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__TX_buffer__DOT__buffer_memory
            [vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__TX_buffer__DOT__read_ptr];
    }
    vlSelfRef.tb_top__DOT__u_dut__DOT__uart_tx = ((1U 
                                                   & (~ (IData)(vlSelfRef.rst_n))) 
                                                  || (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__uart_tx));
    vlSelfRef.read_done_o = ((2U == (3U & vlSelfRef.read_address_i))
                              ? (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__read_done_delay)
                              : (IData)(vlSelfRef.read_i));
    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__counter_enable = 1U;
    if (vlSelfRef.rst_n) {
        vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__baud_rate_generator__DOT__counter 
            = (((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__baud_rate_generator__DOT__counter) 
                >= (vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register 
                    >> 0x11U)) ? 0U : (0x7fffU & ((IData)(1U) 
                                                  + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__baud_rate_generator__DOT__counter))));
        if ((1U == (((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT____Vcellinp__TX_buffer__write_i) 
                     << 1U) | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT____Vcellinp__TX_buffer__read_i)))) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__tx_full = 0U;
        } else if ((2U == (((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT____Vcellinp__TX_buffer__write_i) 
                            << 1U) | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT____Vcellinp__TX_buffer__read_i)))) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__tx_full 
                = ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__TX_buffer__DOT__read_ptr) 
                   == (0x1ffU & ((IData)(1U) + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__TX_buffer__DOT__write_ptr))));
        }
        if ((0x400U & vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register)) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__state_CRT 
                = vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__state_NXT;
        }
    } else {
        vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__baud_rate_generator__DOT__counter = 0U;
        vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__tx_full = 0U;
        vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__state_CRT = 0U;
    }
    if ((0U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__state_CRT))) {
        vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__counter_enable = 0U;
    }
    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__rx_idle = 0U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__counter_enable = 1U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__reset_bit_count = 0U;
    if (vlSelfRef.rst_n) {
        if ((0x200U & vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register)) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__state_CRT 
                = vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__state_NXT;
        }
    } else {
        vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__state_CRT = 0U;
    }
    if (__VdlySet__tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__TX_buffer__DOT__buffer_memory__v0) {
        vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__TX_buffer__DOT__buffer_memory[__VdlyDim0__tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__TX_buffer__DOT__buffer_memory__v0] 
            = __VdlyVal__tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__TX_buffer__DOT__buffer_memory__v0;
    }
    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__uart_tx = 1U;
    if (vlSelfRef.rst_n) {
        if (((3U == (3U & vlSelfRef.write_address_i)) 
             & (IData)(vlSelfRef.write_i))) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__event_register 
                = (0x1fU & vlSelfRef.write_data_i);
        } else {
            if (((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__rx_done) 
                 & (vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register 
                    >> 4U))) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__event_register 
                    = (1U | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__event_register));
            }
            if (((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__tx_done) 
                 & (vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register 
                    >> 5U))) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__event_register 
                    = (2U | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__event_register));
            }
            if (((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__rx_full) 
                 & (vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register 
                    >> 6U))) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__event_register 
                    = (4U | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__event_register));
            }
            if (((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__tx_empty) 
                 & (vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register 
                    >> 7U))) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__event_register 
                    = (8U | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__event_register));
            }
            if (((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__parity_error) 
                 & (vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register 
                    >> 8U))) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__event_register 
                    = (0x10U | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__event_register));
            }
        }
    } else {
        vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__event_register = 0U;
    }
    vlSelfRef.interrupt_o = (0U != (((((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__genblk1__BRA__4__KET____DOT__event_detector__DOT__previous_signal)) 
                                       & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__event_register) 
                                          >> 4U)) << 4U) 
                                     | ((8U & (((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__genblk1__BRA__3__KET____DOT__event_detector__DOT__previous_signal)) 
                                                << 3U) 
                                               & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__event_register))) 
                                        | (4U & (((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__genblk1__BRA__2__KET____DOT__event_detector__DOT__previous_signal)) 
                                                  << 2U) 
                                                 & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__event_register))))) 
                                    | ((2U & (((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__genblk1__BRA__1__KET____DOT__event_detector__DOT__previous_signal)) 
                                               << 1U) 
                                              & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__event_register))) 
                                       | (1U & ((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__genblk1__BRA__0__KET____DOT__event_detector__DOT__previous_signal)) 
                                                & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__event_register))))));
    if (vlSelfRef.rst_n) {
        if ((1U == (((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT____Vcellinp__TX_buffer__write_i) 
                     << 1U) | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT____Vcellinp__TX_buffer__read_i)))) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__tx_empty 
                = ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__TX_buffer__DOT__write_ptr) 
                   == (0x1ffU & ((IData)(1U) + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__TX_buffer__DOT__read_ptr))));
        } else if ((2U == (((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT____Vcellinp__TX_buffer__write_i) 
                            << 1U) | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT____Vcellinp__TX_buffer__read_i)))) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__tx_empty = 0U;
        }
        if ((((2U & vlSelfRef.write_address_i) ? 0U
               : (3U & ((IData)(1U) << (1U & vlSelfRef.write_address_i)))) 
             & (IData)(vlSelfRef.write_i))) {
            if ((1U & (IData)(vlSelfRef.write_strobe_i))) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register 
                    = ((0xffffff0fU & vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register) 
                       | (0xf0U & vlSelfRef.write_data_i));
            }
            if ((2U & (IData)(vlSelfRef.write_strobe_i))) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register 
                    = ((0xffff00ffU & vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register) 
                       | (0xff00U & vlSelfRef.write_data_i));
            }
            if ((4U & (IData)(vlSelfRef.write_strobe_i))) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register 
                    = ((0xff00ffffU & vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register) 
                       | (0xff0000U & vlSelfRef.write_data_i));
            }
            if ((8U & (IData)(vlSelfRef.write_strobe_i))) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register 
                    = ((0xffffffU & vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register) 
                       | (0xff000000U & vlSelfRef.write_data_i));
            }
        }
    } else {
        vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__tx_empty = 1U;
        vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register 
            = (0x6ac800U | (0xfU & vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register));
    }
    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register 
        = ((0xfffffff0U & vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register) 
           | ((((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__rx_full) 
                << 3U) | ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__rx_empty) 
                          << 2U)) | (((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__rx_full) 
                                      << 1U) | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__rx_empty))));
    if (vlSelfRef.rst_n) {
        if (vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT____Vcellinp__TX_buffer__read_i) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__TX_buffer__DOT__read_ptr 
                = vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__TX_buffer__DOT__inc_read_ptr;
        }
        if (vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT____Vcellinp__TX_buffer__write_i) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__TX_buffer__DOT__write_ptr 
                = vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__TX_buffer__DOT__inc_write_ptr;
        }
        if ((1U == (((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT____Vcellinp__RX_buffer__write_i) 
                     << 1U) | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT____Vcellinp__RX_buffer__read_i)))) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__rx_full = 0U;
            vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__rx_empty 
                = ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__RX_buffer__DOT__write_ptr) 
                   == (0x1ffU & ((IData)(1U) + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__RX_buffer__DOT__read_ptr))));
        } else if ((2U == (((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT____Vcellinp__RX_buffer__write_i) 
                            << 1U) | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT____Vcellinp__RX_buffer__read_i)))) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__rx_full 
                = ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__RX_buffer__DOT__read_ptr) 
                   == (0x1ffU & ((IData)(1U) + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__RX_buffer__DOT__write_ptr))));
            vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__rx_empty = 0U;
        }
        if (vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT____Vcellinp__RX_buffer__write_i) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__RX_buffer__DOT__write_ptr 
                = vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__RX_buffer__DOT__inc_write_ptr;
        }
        if (vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT____Vcellinp__RX_buffer__read_i) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__RX_buffer__DOT__read_ptr 
                = vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__RX_buffer__DOT__inc_read_ptr;
        }
    } else {
        vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__TX_buffer__DOT__read_ptr = 0U;
        vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__TX_buffer__DOT__write_ptr = 0U;
        vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__rx_full = 0U;
        vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__rx_empty = 1U;
        vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__RX_buffer__DOT__write_ptr = 0U;
        vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__RX_buffer__DOT__read_ptr = 0U;
    }
    vlSelfRef.read_data_o = 0U;
    if ((0U == (3U & vlSelfRef.read_address_i))) {
        vlSelfRef.read_data_o = vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register;
    } else if ((2U == (3U & vlSelfRef.read_address_i))) {
        vlSelfRef.read_data_o = vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__rx_buffer_read;
    } else if ((3U == (3U & vlSelfRef.read_address_i))) {
        vlSelfRef.read_data_o = vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__event_register;
    }
    __Vtableidx1 = (((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__rx_data) 
                     << 2U) | (3U & (vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register 
                                     >> 0xeU)));
    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__rx_data 
        = Vtb_top__ConstPool__TABLE_h659593fe_0[__Vtableidx1];
    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__sample 
        = ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__baud_rate_generator__DOT__counter) 
           == (vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register 
               >> 0x11U));
    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT____Vcellinp__TX_buffer__write_i 
        = ((~ (vlSelfRef.write_address_i >> 1U)) & 
           ((((IData)(1U) << (1U & vlSelfRef.write_address_i)) 
             >> 1U) & ((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__tx_full)) 
                       & (IData)(vlSelfRef.write_i))));
    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__update_parity = 0U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__load_data = 0U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__shift_data = 0U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__shift_data = 0U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__check_parity = 0U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__update_parity = 0U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__start_receiving = 0U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__tx_done = 0U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__stop_bit_NXT 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__stop_bit_CRT;
    if ((0U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__state_CRT))) {
        if ((1U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__state_CRT))) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__uart_tx = 0U;
            if (((0xfU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__sampling_counter)) 
                 & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__sample))) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__load_data = 1U;
            }
        } else if ((2U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__state_CRT))) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__uart_tx 
                = (1U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__tx_data));
        } else if ((3U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__state_CRT))) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__uart_tx 
                = vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__parity_bit;
        } else if ((4U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__state_CRT))) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__uart_tx = 1U;
        }
        if ((1U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__state_CRT))) {
            if ((2U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__state_CRT))) {
                if (((0xfU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__sampling_counter)) 
                     & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__sample))) {
                    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__update_parity = 1U;
                    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__shift_data = 1U;
                }
            }
            if ((2U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__state_CRT))) {
                if ((3U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__state_CRT))) {
                    if ((4U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__state_CRT))) {
                        if ((0x2000U & vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register)) {
                            if ((0x2000U & vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register)) {
                                if (((0xfU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__sampling_counter)) 
                                     & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__sample))) {
                                    if ((0U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__stop_bit_CRT))) {
                                        vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__tx_done = 1U;
                                    }
                                    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__stop_bit_NXT 
                                        = (1U & (~ (IData)(
                                                           (0U 
                                                            != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__stop_bit_CRT)))));
                                }
                            }
                        } else if (((0xfU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__sampling_counter)) 
                                    & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__sample))) {
                            vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__tx_done = 1U;
                        }
                    }
                }
            }
        }
    }
    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__stop_bit_NXT 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__stop_bit_CRT;
    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__counter_reset = 0U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__parity_error = 0U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__state_NXT 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__state_CRT;
    if ((0U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__state_CRT))) {
        vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__rx_idle = 1U;
        vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__counter_enable = 0U;
        if ((1U & (~ ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__rx_line_sync__DOT__sync_signal) 
                      >> 1U)))) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__reset_bit_count = 1U;
            vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__counter_reset = 1U;
            vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__state_NXT = 1U;
        }
    } else if ((1U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__state_CRT))) {
        if ((1U & (~ ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__rx_line_sync__DOT__sync_signal) 
                      >> 1U)))) {
            if (((7U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__sampling_counter)) 
                 & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__sample))) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__counter_reset = 1U;
            }
        }
        if ((2U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__rx_line_sync__DOT__sync_signal))) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__state_NXT = 0U;
        } else if (((7U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__sampling_counter)) 
                    & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__sample))) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__state_NXT = 2U;
        }
    } else if ((2U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__state_CRT))) {
        if (((0xfU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__sampling_counter)) 
             & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__sample))) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__counter_reset = 1U;
            if ((0x8000U & vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register)) {
                if ((0x4000U & vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register)) {
                    if ((7U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__bit_count))) {
                        vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__state_NXT 
                            = ((0x800U & vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register)
                                ? 3U : 4U);
                    }
                } else if ((6U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__bit_count))) {
                    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__state_NXT 
                        = ((0x800U & vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register)
                            ? 3U : 4U);
                }
            } else if ((0x4000U & vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register)) {
                if ((5U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__bit_count))) {
                    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__state_NXT 
                        = ((0x800U & vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register)
                            ? 3U : 4U);
                }
            } else if ((4U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__bit_count))) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__state_NXT 
                    = ((0x800U & vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register)
                        ? 3U : 4U);
            }
        }
    } else if ((3U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__state_CRT))) {
        if (((0xfU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__sampling_counter)) 
             & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__sample))) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__counter_reset = 1U;
            vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__state_NXT = 4U;
        }
    } else if ((4U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__state_CRT))) {
        if ((0x2000U & vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register)) {
            if ((0x2000U & vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register)) {
                if (((0xfU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__sampling_counter)) 
                     & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__sample))) {
                    if ((0U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__stop_bit_CRT))) {
                        vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__counter_reset = 1U;
                        vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__state_NXT = 0U;
                    }
                }
            }
        } else if (((0xfU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__sampling_counter)) 
                    & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__sample))) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__counter_reset = 1U;
            vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__state_NXT = 0U;
        }
    }
    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__rx_done = 0U;
    if ((0U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__state_CRT))) {
        if ((1U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__state_CRT))) {
            if ((2U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__state_CRT))) {
                if (((0xfU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__sampling_counter)) 
                     & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__sample))) {
                    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__shift_data = 1U;
                    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__update_parity = 1U;
                }
            }
            if ((2U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__state_CRT))) {
                if ((3U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__state_CRT))) {
                    if (((0xfU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__sampling_counter)) 
                         & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__sample))) {
                        vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__check_parity = 1U;
                    }
                }
                if ((3U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__state_CRT))) {
                    if ((4U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__state_CRT))) {
                        if ((0x2000U & vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register)) {
                            if ((0x2000U & vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register)) {
                                if (((0xfU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__sampling_counter)) 
                                     & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__sample))) {
                                    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__stop_bit_NXT 
                                        = (1U & (~ (IData)(
                                                           (0U 
                                                            != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__stop_bit_CRT)))));
                                    if ((0U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__stop_bit_CRT))) {
                                        vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__parity_error 
                                            = ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__parity_error) 
                                               & (vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register 
                                                  >> 0xbU));
                                        vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__rx_done = 1U;
                                    }
                                }
                            }
                        } else if (((0xfU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__sampling_counter)) 
                                    & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__sample))) {
                            vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__parity_error 
                                = vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__parity_error;
                            vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__rx_done = 1U;
                        }
                    }
                }
            }
        }
        if ((1U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__state_CRT))) {
            if ((1U & (~ ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__rx_line_sync__DOT__sync_signal) 
                          >> 1U)))) {
                if (((7U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__sampling_counter)) 
                     & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__sample))) {
                    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__start_receiving = 1U;
                }
            }
        }
    }
    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__TX_buffer__DOT__inc_read_ptr 
        = (0x1ffU & ((IData)(1U) + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__TX_buffer__DOT__read_ptr)));
    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__TX_buffer__DOT__inc_write_ptr 
        = (0x1ffU & ((IData)(1U) + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__TX_buffer__DOT__write_ptr)));
    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT____Vcellinp__RX_buffer__write_i 
        = ((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__rx_full)) 
           & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__rx_done));
    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT____Vcellinp__RX_buffer__read_i 
        = ((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__rx_empty)) 
           & ((2U == (3U & vlSelfRef.read_address_i)) 
              & (IData)(vlSelfRef.read_i)));
    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__RX_buffer__DOT__inc_write_ptr 
        = (0x1ffU & ((IData)(1U) + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__RX_buffer__DOT__write_ptr)));
    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__RX_buffer__DOT__inc_read_ptr 
        = (0x1ffU & ((IData)(1U) + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__RX_buffer__DOT__read_ptr)));
}

VL_INLINE_OPT void Vtb_top___024root___nba_comb__TOP__0(Vtb_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_top___024root___nba_comb__TOP__0\n"); );
    Vtb_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmit 
        = ((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__tx_empty)) 
           & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__tx_idle));
    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT____Vcellinp__TX_buffer__read_i 
        = ((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__tx_empty)) 
           & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmit));
    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__reset_bit_count = 0U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__tx_idle = 0U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__counter_reset = 0U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__state_NXT 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__state_CRT;
    if ((0U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__state_CRT))) {
        vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__tx_idle = 1U;
        if (((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmit) 
             & (vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register 
                >> 0xaU))) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__reset_bit_count = 1U;
            vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__tx_idle = 1U;
            vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__counter_reset = 1U;
            vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__state_NXT = 1U;
        }
    } else if ((1U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__state_CRT))) {
        if (((0xfU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__sampling_counter)) 
             & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__sample))) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__counter_reset = 1U;
            vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__state_NXT = 2U;
        }
    } else if ((2U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__state_CRT))) {
        if (((0xfU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__sampling_counter)) 
             & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__sample))) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__counter_reset = 1U;
            if ((0x8000U & vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register)) {
                if ((0x4000U & vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register)) {
                    if ((7U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__bit_count))) {
                        vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__state_NXT 
                            = ((0x800U & vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register)
                                ? 3U : 4U);
                    }
                } else if ((6U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__bit_count))) {
                    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__state_NXT 
                        = ((0x800U & vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register)
                            ? 3U : 4U);
                }
            } else if ((0x4000U & vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register)) {
                if ((5U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__bit_count))) {
                    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__state_NXT 
                        = ((0x800U & vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register)
                            ? 3U : 4U);
                }
            } else if ((4U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__bit_count))) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__state_NXT 
                    = ((0x800U & vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register)
                        ? 3U : 4U);
            }
        }
    } else if ((3U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__state_CRT))) {
        if (((0xfU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__sampling_counter)) 
             & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__sample))) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__counter_reset = 1U;
            vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__state_NXT = 4U;
        }
    } else if ((4U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__state_CRT))) {
        if ((0x2000U & vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register)) {
            if ((0x2000U & vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register)) {
                if (((0xfU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__sampling_counter)) 
                     & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__sample))) {
                    if ((0U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__stop_bit_CRT))) {
                        vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__counter_reset = 1U;
                        vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__state_NXT = 0U;
                    }
                }
            }
        } else if (((0xfU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__sampling_counter)) 
                    & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__sample))) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__counter_reset = 1U;
            vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__state_NXT = 0U;
        }
    }
}

void Vtb_top___024root___eval_triggers__act(Vtb_top___024root* vlSelf);

bool Vtb_top___024root___eval_phase__act(Vtb_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_top___024root___eval_phase__act\n"); );
    Vtb_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    VlTriggerVec<2> __VpreTriggered;
    CData/*0:0*/ __VactExecute;
    // Body
    Vtb_top___024root___eval_triggers__act(vlSelf);
    __VactExecute = vlSelfRef.__VactTriggered.any();
    if (__VactExecute) {
        __VpreTriggered.andNot(vlSelfRef.__VactTriggered, vlSelfRef.__VnbaTriggered);
        vlSelfRef.__VnbaTriggered.thisOr(vlSelfRef.__VactTriggered);
        Vtb_top___024root___eval_act(vlSelf);
    }
    return (__VactExecute);
}

bool Vtb_top___024root___eval_phase__nba(Vtb_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_top___024root___eval_phase__nba\n"); );
    Vtb_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    CData/*0:0*/ __VnbaExecute;
    // Body
    __VnbaExecute = vlSelfRef.__VnbaTriggered.any();
    if (__VnbaExecute) {
        Vtb_top___024root___eval_nba(vlSelf);
        vlSelfRef.__VnbaTriggered.clear();
    }
    return (__VnbaExecute);
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtb_top___024root___dump_triggers__ico(Vtb_top___024root* vlSelf);
#endif  // VL_DEBUG
#ifdef VL_DEBUG
VL_ATTR_COLD void Vtb_top___024root___dump_triggers__nba(Vtb_top___024root* vlSelf);
#endif  // VL_DEBUG
#ifdef VL_DEBUG
VL_ATTR_COLD void Vtb_top___024root___dump_triggers__act(Vtb_top___024root* vlSelf);
#endif  // VL_DEBUG

void Vtb_top___024root___eval(Vtb_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_top___024root___eval\n"); );
    Vtb_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    IData/*31:0*/ __VicoIterCount;
    CData/*0:0*/ __VicoContinue;
    IData/*31:0*/ __VnbaIterCount;
    CData/*0:0*/ __VnbaContinue;
    // Body
    __VicoIterCount = 0U;
    vlSelfRef.__VicoFirstIteration = 1U;
    __VicoContinue = 1U;
    while (__VicoContinue) {
        if (VL_UNLIKELY(((0x64U < __VicoIterCount)))) {
#ifdef VL_DEBUG
            Vtb_top___024root___dump_triggers__ico(vlSelf);
#endif
            VL_FATAL_MT("common/tb/tb_top.sv", 1, "", "Input combinational region did not converge.");
        }
        __VicoIterCount = ((IData)(1U) + __VicoIterCount);
        __VicoContinue = 0U;
        if (Vtb_top___024root___eval_phase__ico(vlSelf)) {
            __VicoContinue = 1U;
        }
        vlSelfRef.__VicoFirstIteration = 0U;
    }
    __VnbaIterCount = 0U;
    __VnbaContinue = 1U;
    while (__VnbaContinue) {
        if (VL_UNLIKELY(((0x64U < __VnbaIterCount)))) {
#ifdef VL_DEBUG
            Vtb_top___024root___dump_triggers__nba(vlSelf);
#endif
            VL_FATAL_MT("common/tb/tb_top.sv", 1, "", "NBA region did not converge.");
        }
        __VnbaIterCount = ((IData)(1U) + __VnbaIterCount);
        __VnbaContinue = 0U;
        vlSelfRef.__VactIterCount = 0U;
        vlSelfRef.__VactContinue = 1U;
        while (vlSelfRef.__VactContinue) {
            if (VL_UNLIKELY(((0x64U < vlSelfRef.__VactIterCount)))) {
#ifdef VL_DEBUG
                Vtb_top___024root___dump_triggers__act(vlSelf);
#endif
                VL_FATAL_MT("common/tb/tb_top.sv", 1, "", "Active region did not converge.");
            }
            vlSelfRef.__VactIterCount = ((IData)(1U) 
                                         + vlSelfRef.__VactIterCount);
            vlSelfRef.__VactContinue = 0U;
            if (Vtb_top___024root___eval_phase__act(vlSelf)) {
                vlSelfRef.__VactContinue = 1U;
            }
        }
        if (Vtb_top___024root___eval_phase__nba(vlSelf)) {
            __VnbaContinue = 1U;
        }
    }
}

#ifdef VL_DEBUG
void Vtb_top___024root___eval_debug_assertions(Vtb_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_top___024root___eval_debug_assertions\n"); );
    Vtb_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if (VL_UNLIKELY(((vlSelfRef.clk & 0xfeU)))) {
        Verilated::overWidthError("clk");}
    if (VL_UNLIKELY(((vlSelfRef.rst_n & 0xfeU)))) {
        Verilated::overWidthError("rst_n");}
    if (VL_UNLIKELY(((vlSelfRef.write_i & 0xfeU)))) {
        Verilated::overWidthError("write_i");}
    if (VL_UNLIKELY(((vlSelfRef.write_strobe_i & 0xf0U)))) {
        Verilated::overWidthError("write_strobe_i");}
    if (VL_UNLIKELY(((vlSelfRef.read_i & 0xfeU)))) {
        Verilated::overWidthError("read_i");}
}
#endif  // VL_DEBUG
