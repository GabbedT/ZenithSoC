// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtb_top.h for the primary calling header

#include "Vtb_top__pch.h"
#include "Vtb_top___024root.h"

VL_ATTR_COLD void Vtb_top___024root___eval_static(Vtb_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_top___024root___eval_static\n"); );
    Vtb_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}

VL_ATTR_COLD void Vtb_top___024root___eval_initial(Vtb_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_top___024root___eval_initial\n"); );
    Vtb_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__Vtrigprevexpr___TOP__tb_top__DOT__u_dut__DOT__u_uart__DOT__tx_idle__0 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__tx_idle;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_top__DOT__u_dut__DOT__u_uart__DOT__tx_idle__1 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__tx_idle;
    vlSelfRef.__Vtrigprevexpr___TOP__clk__0 = vlSelfRef.clk;
}

VL_ATTR_COLD void Vtb_top___024root___eval_final(Vtb_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_top___024root___eval_final\n"); );
    Vtb_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtb_top___024root___dump_triggers__stl(Vtb_top___024root* vlSelf);
#endif  // VL_DEBUG
VL_ATTR_COLD bool Vtb_top___024root___eval_phase__stl(Vtb_top___024root* vlSelf);

VL_ATTR_COLD void Vtb_top___024root___eval_settle(Vtb_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_top___024root___eval_settle\n"); );
    Vtb_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    IData/*31:0*/ __VstlIterCount;
    CData/*0:0*/ __VstlContinue;
    // Body
    __VstlIterCount = 0U;
    vlSelfRef.__VstlFirstIteration = 1U;
    __VstlContinue = 1U;
    while (__VstlContinue) {
        if (VL_UNLIKELY(((0x64U < __VstlIterCount)))) {
#ifdef VL_DEBUG
            Vtb_top___024root___dump_triggers__stl(vlSelf);
#endif
            VL_FATAL_MT("common/tb/tb_top.sv", 1, "", "Settle region did not converge.");
        }
        __VstlIterCount = ((IData)(1U) + __VstlIterCount);
        __VstlContinue = 0U;
        if (Vtb_top___024root___eval_phase__stl(vlSelf)) {
            __VstlContinue = 1U;
        }
        vlSelfRef.__VstlFirstIteration = 0U;
    }
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtb_top___024root___dump_triggers__stl(Vtb_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_top___024root___dump_triggers__stl\n"); );
    Vtb_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1U & (~ vlSelfRef.__VstlTriggered.any()))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
    if ((1ULL & vlSelfRef.__VstlTriggered.word(0U))) {
        VL_DBG_MSGF("         'stl' region trigger index 0 is active: Internal 'stl' trigger - first iteration\n");
    }
    if ((2ULL & vlSelfRef.__VstlTriggered.word(0U))) {
        VL_DBG_MSGF("         'stl' region trigger index 1 is active: @([hybrid] tb_top.u_dut.u_uart.tx_idle)\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD void Vtb_top___024root___stl_sequent__TOP__0(Vtb_top___024root* vlSelf);
VL_ATTR_COLD void Vtb_top___024root____Vm_traceActivitySetAll(Vtb_top___024root* vlSelf);
void Vtb_top___024root___act_sequent__TOP__0(Vtb_top___024root* vlSelf);

VL_ATTR_COLD void Vtb_top___024root___eval_stl(Vtb_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_top___024root___eval_stl\n"); );
    Vtb_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1ULL & vlSelfRef.__VstlTriggered.word(0U))) {
        Vtb_top___024root___stl_sequent__TOP__0(vlSelf);
        Vtb_top___024root____Vm_traceActivitySetAll(vlSelf);
    }
    if ((3ULL & vlSelfRef.__VstlTriggered.word(0U))) {
        Vtb_top___024root___act_sequent__TOP__0(vlSelf);
        Vtb_top___024root____Vm_traceActivitySetAll(vlSelf);
    }
}

extern const VlUnpacked<CData/*7:0*/, 1024> Vtb_top__ConstPool__TABLE_h659593fe_0;

VL_ATTR_COLD void Vtb_top___024root___stl_sequent__TOP__0(Vtb_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_top___024root___stl_sequent__TOP__0\n"); );
    Vtb_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    SData/*9:0*/ __Vtableidx1;
    __Vtableidx1 = 0;
    // Body
    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__counter_enable = 1U;
    if ((0U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__state_CRT))) {
        vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__counter_enable = 0U;
    }
    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__rx_idle = 0U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__counter_enable = 1U;
    vlSelfRef.write_done_o = vlSelfRef.write_i;
    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__TX_buffer__DOT__inc_write_ptr 
        = (0x1ffU & ((IData)(1U) + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__TX_buffer__DOT__write_ptr)));
    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__TX_buffer__DOT__inc_read_ptr 
        = (0x1ffU & ((IData)(1U) + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__TX_buffer__DOT__read_ptr)));
    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__RX_buffer__DOT__inc_write_ptr 
        = (0x1ffU & ((IData)(1U) + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__RX_buffer__DOT__write_ptr)));
    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__RX_buffer__DOT__inc_read_ptr 
        = (0x1ffU & ((IData)(1U) + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__RX_buffer__DOT__read_ptr)));
    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__reset_bit_count = 0U;
    vlSelfRef.write_error_o = ((2U == (3U & vlSelfRef.write_address_i)) 
                               & (IData)(vlSelfRef.write_i));
    vlSelfRef.read_error_o = ((1U == (3U & vlSelfRef.read_address_i)) 
                              & (IData)(vlSelfRef.read_i));
    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__uart_tx = 1U;
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
    __Vtableidx1 = (((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__rx_data) 
                     << 2U) | (3U & (vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register 
                                     >> 0xeU)));
    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__rx_data 
        = Vtb_top__ConstPool__TABLE_h659593fe_0[__Vtableidx1];
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
    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__sample 
        = ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__baud_rate_generator__DOT__counter) 
           == (vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register 
               >> 0x11U));
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
    vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT____Vcellinp__RX_buffer__write_i 
        = ((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__rx_full)) 
           & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__rx_done));
}

VL_ATTR_COLD void Vtb_top___024root___eval_triggers__stl(Vtb_top___024root* vlSelf);

VL_ATTR_COLD bool Vtb_top___024root___eval_phase__stl(Vtb_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_top___024root___eval_phase__stl\n"); );
    Vtb_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    CData/*0:0*/ __VstlExecute;
    // Body
    Vtb_top___024root___eval_triggers__stl(vlSelf);
    __VstlExecute = vlSelfRef.__VstlTriggered.any();
    if (__VstlExecute) {
        Vtb_top___024root___eval_stl(vlSelf);
    }
    return (__VstlExecute);
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtb_top___024root___dump_triggers__ico(Vtb_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_top___024root___dump_triggers__ico\n"); );
    Vtb_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1U & (~ vlSelfRef.__VicoTriggered.any()))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
    if ((1ULL & vlSelfRef.__VicoTriggered.word(0U))) {
        VL_DBG_MSGF("         'ico' region trigger index 0 is active: Internal 'ico' trigger - first iteration\n");
    }
}
#endif  // VL_DEBUG

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtb_top___024root___dump_triggers__act(Vtb_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_top___024root___dump_triggers__act\n"); );
    Vtb_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1U & (~ vlSelfRef.__VactTriggered.any()))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
    if ((1ULL & vlSelfRef.__VactTriggered.word(0U))) {
        VL_DBG_MSGF("         'act' region trigger index 0 is active: @([hybrid] tb_top.u_dut.u_uart.tx_idle)\n");
    }
    if ((2ULL & vlSelfRef.__VactTriggered.word(0U))) {
        VL_DBG_MSGF("         'act' region trigger index 1 is active: @(posedge clk)\n");
    }
}
#endif  // VL_DEBUG

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtb_top___024root___dump_triggers__nba(Vtb_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_top___024root___dump_triggers__nba\n"); );
    Vtb_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1U & (~ vlSelfRef.__VnbaTriggered.any()))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
    if ((1ULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        VL_DBG_MSGF("         'nba' region trigger index 0 is active: @([hybrid] tb_top.u_dut.u_uart.tx_idle)\n");
    }
    if ((2ULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        VL_DBG_MSGF("         'nba' region trigger index 1 is active: @(posedge clk)\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD void Vtb_top___024root____Vm_traceActivitySetAll(Vtb_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_top___024root____Vm_traceActivitySetAll\n"); );
    Vtb_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__Vm_traceActivity[0U] = 1U;
    vlSelfRef.__Vm_traceActivity[1U] = 1U;
    vlSelfRef.__Vm_traceActivity[2U] = 1U;
    vlSelfRef.__Vm_traceActivity[3U] = 1U;
}

VL_ATTR_COLD void Vtb_top___024root___ctor_var_reset(Vtb_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_top___024root___ctor_var_reset\n"); );
    Vtb_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelf->clk = VL_RAND_RESET_I(1);
    vlSelf->rst_n = VL_RAND_RESET_I(1);
    vlSelf->write_i = VL_RAND_RESET_I(1);
    vlSelf->write_address_i = VL_RAND_RESET_I(32);
    vlSelf->write_data_i = VL_RAND_RESET_I(32);
    vlSelf->write_strobe_i = VL_RAND_RESET_I(4);
    vlSelf->write_done_o = VL_RAND_RESET_I(1);
    vlSelf->write_error_o = VL_RAND_RESET_I(1);
    vlSelf->read_i = VL_RAND_RESET_I(1);
    vlSelf->read_address_i = VL_RAND_RESET_I(32);
    vlSelf->read_data_o = VL_RAND_RESET_I(32);
    vlSelf->read_done_o = VL_RAND_RESET_I(1);
    vlSelf->read_error_o = VL_RAND_RESET_I(1);
    vlSelf->interrupt_o = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__uart_tx = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__u_uart__DOT__rx_data = VL_RAND_RESET_I(8);
    vlSelf->tb_top__DOT__u_dut__DOT__u_uart__DOT__tx_data = VL_RAND_RESET_I(8);
    vlSelf->tb_top__DOT__u_dut__DOT__u_uart__DOT__transmit = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__u_uart__DOT__tx_done = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__u_uart__DOT__tx_idle = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__u_uart__DOT__rx_done = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__u_uart__DOT__rx_idle = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__u_uart__DOT__parity_error = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__u_uart__DOT__read_done_delay = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__u_uart__DOT__sample = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__u_uart__DOT__uart_tx = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register = VL_RAND_RESET_I(32);
    vlSelf->tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__tx_empty = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__tx_full = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT____Vcellinp__TX_buffer__read_i = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT____Vcellinp__TX_buffer__write_i = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__rx_buffer_read = VL_RAND_RESET_I(8);
    vlSelf->tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__rx_empty = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__rx_full = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT____Vcellinp__RX_buffer__read_i = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT____Vcellinp__RX_buffer__write_i = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__event_register = VL_RAND_RESET_I(5);
    vlSelf->tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__uart_event = VL_RAND_RESET_I(3);
    for (int __Vi0 = 0; __Vi0 < 512; ++__Vi0) {
        vlSelf->tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__TX_buffer__DOT__buffer_memory[__Vi0] = VL_RAND_RESET_I(8);
    }
    vlSelf->tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__TX_buffer__DOT__write_ptr = VL_RAND_RESET_I(9);
    vlSelf->tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__TX_buffer__DOT__read_ptr = VL_RAND_RESET_I(9);
    vlSelf->tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__TX_buffer__DOT__inc_write_ptr = VL_RAND_RESET_I(9);
    vlSelf->tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__TX_buffer__DOT__inc_read_ptr = VL_RAND_RESET_I(9);
    for (int __Vi0 = 0; __Vi0 < 512; ++__Vi0) {
        vlSelf->tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__RX_buffer__DOT__buffer_memory[__Vi0] = VL_RAND_RESET_I(8);
    }
    vlSelf->tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__RX_buffer__DOT__write_ptr = VL_RAND_RESET_I(9);
    vlSelf->tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__RX_buffer__DOT__read_ptr = VL_RAND_RESET_I(9);
    vlSelf->tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__RX_buffer__DOT__inc_write_ptr = VL_RAND_RESET_I(9);
    vlSelf->tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__RX_buffer__DOT__inc_read_ptr = VL_RAND_RESET_I(9);
    vlSelf->tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__genblk1__BRA__0__KET____DOT__event_detector__DOT__previous_signal = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__genblk1__BRA__1__KET____DOT__event_detector__DOT__previous_signal = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__genblk1__BRA__2__KET____DOT__event_detector__DOT__previous_signal = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__genblk1__BRA__3__KET____DOT__event_detector__DOT__previous_signal = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__genblk1__BRA__4__KET____DOT__event_detector__DOT__previous_signal = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__u_uart__DOT__baud_rate_generator__DOT__counter = VL_RAND_RESET_I(15);
    vlSelf->tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__sampling_counter = VL_RAND_RESET_I(4);
    vlSelf->tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__counter_enable = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__counter_reset = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__tx_data = VL_RAND_RESET_I(8);
    vlSelf->tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__shift_data = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__load_data = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__parity_bit = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__update_parity = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__bit_count = VL_RAND_RESET_I(3);
    vlSelf->tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__stop_bit_CRT = VL_RAND_RESET_I(3);
    vlSelf->tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__stop_bit_NXT = VL_RAND_RESET_I(3);
    vlSelf->tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__reset_bit_count = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__state_CRT = VL_RAND_RESET_I(3);
    vlSelf->tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__state_NXT = VL_RAND_RESET_I(3);
    vlSelf->tb_top__DOT__u_dut__DOT__u_uart__DOT__rx_line_sync__DOT__sync_signal = VL_RAND_RESET_I(2);
    vlSelf->tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__sampling_counter = VL_RAND_RESET_I(4);
    vlSelf->tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__counter_enable = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__counter_reset = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__rx_data = VL_RAND_RESET_I(8);
    vlSelf->tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__shift_data = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__start_receiving = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__parity_bit = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__parity_error = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__update_parity = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__check_parity = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__bit_count = VL_RAND_RESET_I(3);
    vlSelf->tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__stop_bit_CRT = VL_RAND_RESET_I(3);
    vlSelf->tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__stop_bit_NXT = VL_RAND_RESET_I(3);
    vlSelf->tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__reset_bit_count = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__state_CRT = VL_RAND_RESET_I(3);
    vlSelf->tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__state_NXT = VL_RAND_RESET_I(3);
    vlSelf->__Vtrigprevexpr___TOP__tb_top__DOT__u_dut__DOT__u_uart__DOT__tx_idle__0 = VL_RAND_RESET_I(1);
    vlSelf->__VstlDidInit = 0;
    vlSelf->__Vtrigprevexpr___TOP__tb_top__DOT__u_dut__DOT__u_uart__DOT__tx_idle__1 = VL_RAND_RESET_I(1);
    vlSelf->__Vtrigprevexpr___TOP__clk__0 = VL_RAND_RESET_I(1);
    vlSelf->__VactDidInit = 0;
    for (int __Vi0 = 0; __Vi0 < 4; ++__Vi0) {
        vlSelf->__Vm_traceActivity[__Vi0] = 0;
    }
}
