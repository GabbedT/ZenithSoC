// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtb_top.h for the primary calling header

#include "Vtb_top__pch.h"
#include "Vtb_top___024root.h"

VL_ATTR_COLD void Vtb_top___024root___eval_initial__TOP(Vtb_top___024root* vlSelf);
VlCoroutine Vtb_top___024root___eval_initial__TOP__Vtiming__0(Vtb_top___024root* vlSelf);
VlCoroutine Vtb_top___024root___eval_initial__TOP__Vtiming__1(Vtb_top___024root* vlSelf);

void Vtb_top___024root___eval_initial(Vtb_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_top___024root___eval_initial\n"); );
    Vtb_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    Vtb_top___024root___eval_initial__TOP(vlSelf);
    Vtb_top___024root___eval_initial__TOP__Vtiming__0(vlSelf);
    Vtb_top___024root___eval_initial__TOP__Vtiming__1(vlSelf);
    vlSelfRef.__Vtrigprevexpr___TOP__tb_top__DOT__u_dut__DOT__sd_dat_t__0 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_dat_t;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_top__DOT__u_dut__DOT__sd_dat_t__1 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_dat_t;
    vlSelfRef.__Vtrigprevexpr___TOP__clk__0 = vlSelfRef.clk;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_top__DOT__u_dut__DOT__clk_50MHz__0 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__clk_50MHz;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_top__DOT__u_dut__DOT__dut__DOT__sd_clk__0 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__sd_clk;
    vlSelfRef.__Vtrigprevexpr___TOP__rst_n__0 = vlSelfRef.rst_n;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__spi_cs__0 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__spi_cs;
}

VL_INLINE_OPT VlCoroutine Vtb_top___024root___eval_initial__TOP__Vtiming__0(Vtb_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_top___024root___eval_initial__TOP__Vtiming__0\n"); );
    Vtb_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    while (1U) {
        co_await vlSelfRef.__VdlySched.delay(5ULL, 
                                             nullptr, 
                                             "blocks/sd/rtl/dut_wrapper.sv", 
                                             101);
        vlSelfRef.__VdlyVal__tb_top__DOT__u_dut__DOT__clk_100MHz__v0 
            = (1U & (~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__clk_100MHz)));
        vlSelfRef.__VdlySet__tb_top__DOT__u_dut__DOT__clk_100MHz__v0 = 1U;
    }
}

VL_INLINE_OPT VlCoroutine Vtb_top___024root___eval_initial__TOP__Vtiming__1(Vtb_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_top___024root___eval_initial__TOP__Vtiming__1\n"); );
    Vtb_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    while (1U) {
        co_await vlSelfRef.__VdlySched.delay(0xaULL, 
                                             nullptr, 
                                             "blocks/sd/rtl/dut_wrapper.sv", 
                                             100);
        vlSelfRef.__VdlyVal__tb_top__DOT__u_dut__DOT__clk_50MHz__v0 
            = (1U & (~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__clk_50MHz)));
        vlSelfRef.__VdlySet__tb_top__DOT__u_dut__DOT__clk_50MHz__v0 = 1U;
    }
}

void Vtb_top___024root___ico_sequent__TOP__0(Vtb_top___024root* vlSelf);

void Vtb_top___024root___eval_ico(Vtb_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_top___024root___eval_ico\n"); );
    Vtb_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1ULL & vlSelfRef.__VicoTriggered.word(0U))) {
        Vtb_top___024root___ico_sequent__TOP__0(vlSelf);
        vlSelfRef.__Vm_traceActivity[1U] = 1U;
    }
}

VL_INLINE_OPT void Vtb_top___024root___ico_sequent__TOP__0(Vtb_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_top___024root___ico_sequent__TOP__0\n"); );
    Vtb_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__mask 
        = ((0xffff0000U & vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__mask) 
           | ((((2U & (IData)(vlSelfRef.write_strobe_i))
                 ? 0xffU : 0U) << 8U) | ((1U & (IData)(vlSelfRef.write_strobe_i))
                                          ? 0xffU : 0U)));
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__mask 
        = ((0xffffU & vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__mask) 
           | ((((8U & (IData)(vlSelfRef.write_strobe_i))
                 ? 0xffU : 0U) << 0x18U) | (((4U & (IData)(vlSelfRef.write_strobe_i))
                                              ? 0xffU
                                              : 0U) 
                                            << 0x10U)));
    vlSelfRef.write_done_o = vlSelfRef.write_i;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT____Vcellinp__tx_buffer__rst_n_i 
        = ((~ ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register) 
               >> 0xcU)) & (IData)(vlSelfRef.rst_n));
    vlSelfRef.write_error_o = (((7U == (7U & vlSelfRef.write_address_i)) 
                                | (5U == (7U & vlSelfRef.write_address_i))) 
                               & (IData)(vlSelfRef.write_i));
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__write_tx_buffer 
        = ((IData)(vlSelfRef.write_i) & (6U == (7U 
                                                & vlSelfRef.write_address_i)));
    vlSelfRef.read_error_o = ((6U == (7U & vlSelfRef.read_address_i)) 
                              & (IData)(vlSelfRef.read_i));
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__read_rx_buffer 
        = ((IData)(vlSelfRef.read_i) & (7U == (7U & vlSelfRef.read_address_i)));
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__read_resp_buffer 
        = ((IData)(vlSelfRef.read_i) & (5U == (7U & vlSelfRef.read_address_i)));
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__is_delayed 
        = ((7U == (7U & vlSelfRef.read_address_i)) 
           | (5U == (7U & vlSelfRef.read_address_i)));
    vlSelfRef.read_data_o = 0U;
    if ((0U == (7U & vlSelfRef.read_address_i))) {
        vlSelfRef.read_data_o = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register;
    } else if ((1U == (7U & vlSelfRef.read_address_i))) {
        vlSelfRef.read_data_o = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__status_register;
    } else if ((2U == (7U & vlSelfRef.read_address_i))) {
        vlSelfRef.read_data_o = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__cmd_number;
    } else if ((3U == (7U & vlSelfRef.read_address_i))) {
        vlSelfRef.read_data_o = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__cmd_argument;
    } else if ((4U == (7U & vlSelfRef.read_address_i))) {
        vlSelfRef.read_data_o = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register;
    } else if ((5U == (7U & vlSelfRef.read_address_i))) {
        vlSelfRef.read_data_o = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__read_response_data;
    } else if ((7U == (7U & vlSelfRef.read_address_i))) {
        vlSelfRef.read_data_o = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__read_data_rx;
    }
    vlSelfRef.read_done_o = ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__is_delayed)
                              ? (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__read_done_delay)
                              : (IData)(vlSelfRef.read_i));
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
        vlSelfRef.__Vm_traceActivity[2U] = 1U;
    }
}

VL_INLINE_OPT void Vtb_top___024root___act_sequent__TOP__0(Vtb_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_top___024root___act_sequent__TOP__0\n"); );
    Vtb_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    CData/*0:0*/ tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT____VdfgRegularize_h6e830a37_0_1;
    tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT____VdfgRegularize_h6e830a37_0_1 = 0;
    CData/*0:0*/ tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT____VdfgRegularize_h6e830a37_0_2;
    tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT____VdfgRegularize_h6e830a37_0_2 = 0;
    CData/*0:0*/ tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT____VdfgRegularize_h6e830a37_0_3;
    tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT____VdfgRegularize_h6e830a37_0_3 = 0;
    CData/*0:0*/ tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT____VdfgRegularize_h6e830a37_0_4;
    tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT____VdfgRegularize_h6e830a37_0_4 = 0;
    // Body
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io__strong__out5 
        = ((0xeU & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io__strong__out5)) 
           | (1U & ((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_dat_t)) 
                    & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_dat_o))));
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io__strong__out6 
        = ((0xdU & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io__strong__out6)) 
           | (2U & (((~ ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_dat_t) 
                         >> 1U)) << 1U) & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_dat_o))));
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io__strong__out7 
        = ((0xbU & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io__strong__out7)) 
           | (4U & (((~ ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_dat_t) 
                         >> 2U)) << 2U) & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_dat_o))));
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io__strong__out8 
        = ((7U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io__strong__out8)) 
           | ((IData)(((~ ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_dat_t) 
                           >> 3U)) & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_dat_o) 
                                      >> 3U))) << 3U));
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io = 
        (0xfU & ((~ (((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__tristate_enable_flop)
                       ? 0xfU : 0U) | (((1U & (~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_dat_t))) 
                                        | (2U & ((~ 
                                                  ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_dat_t) 
                                                   >> 1U)) 
                                                 << 1U))) 
                                       | ((4U & ((~ 
                                                  ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_dat_t) 
                                                   >> 2U)) 
                                                 << 2U)) 
                                          | (8U & (
                                                   (~ 
                                                    ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_dat_t) 
                                                     >> 3U)) 
                                                   << 3U)))))) 
                 | ((((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__tristate_enable_flop)
                       ? 0xfU : 0U) & (((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__tristate_enable_flop)
                                         ? 0xfU : 0U) 
                                       & (((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__tristate_enable_flop)
                                            ? (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_line_flop)
                                            : 0U) & 
                                          ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__tristate_enable_flop)
                                            ? 0xfU : 0U)))) 
                    | (((1U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io__strong__out5) 
                               & (~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_dat_t)))) 
                        | (2U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io__strong__out6) 
                                 & ((~ ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_dat_t) 
                                        >> 1U)) << 1U)))) 
                       | ((4U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io__strong__out7) 
                                 & ((~ ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_dat_t) 
                                        >> 2U)) << 2U))) 
                          | (8U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io__strong__out8) 
                                   & ((~ ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_dat_t) 
                                          >> 3U)) << 3U))))))));
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_timeout = 0U;
    if ((1U & (~ ((((((((0U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT)) 
                        | (1U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) 
                       | (4U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) 
                      | (5U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) 
                     | (6U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) 
                    | (0xeU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) 
                   | (2U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) 
                  | (3U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT)))))) {
        if ((7U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
            if ((8U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                if ((9U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                    if ((0xbU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                        if ((1U & (~ ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__sample) 
                                      & (~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io)))))) {
                            if ((0xf4240U <= vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__timeout_counter)) {
                                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_timeout = 1U;
                            }
                        }
                    } else if ((0xdU != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                        if ((0xaU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                            if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__sample) {
                                if ((1U & (~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io)))) {
                                    if ((0xf4240U <= vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__timeout_counter)) {
                                        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_timeout = 1U;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_compute = 0U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__timeout_reset = 0U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_initialize = 0U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__spi_cs 
        = (1U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io) 
                 >> 3U));
    if (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__mode_spi_s) {
        vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_spi_dat 
            = (1U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_cmd_io));
        vlSelfRef.tb_top__DOT__u_dut__DOT__sd_dat_t 
            = (0xfU & (~ (1U & (~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__spi_cs)))));
    } else {
        vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_spi_dat 
            = (1U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io));
        vlSelfRef.tb_top__DOT__u_dut__DOT__sd_dat_t 
            = (0xfU & ((8U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__card_state_s))
                        ? 0xfU : (~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_oe))));
    }
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_reset = 0U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_NXT 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_CRT;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_wide_NXT 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_wide_CRT;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_NXT 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_NXT 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_CRT;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_crc_error = 0U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_error = 0U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_data = 0U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_line = 0xfU;
    if (((((((((0U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT)) 
               | (1U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) 
              | (4U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) 
             | (5U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) 
            | (6U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) 
           | (0xeU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) 
          | (2U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) 
         | (3U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT)))) {
        if ((0U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
            if ((1U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__sample) {
                    if ((0x400U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register))) {
                        if ((0U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io))) {
                            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_compute = 1U;
                            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_data 
                                = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io;
                        }
                    } else if ((1U & (~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io)))) {
                        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_compute = 1U;
                        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_data 
                            = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io;
                    }
                }
            } else if ((4U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__sample) {
                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_compute = 1U;
                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_data 
                        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io;
                }
            } else if ((5U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                if ((6U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                    if ((0xeU != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                        if ((2U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_line 
                                = ((0x400U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register))
                                    ? 0U : (0xeU & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_line)));
                        }
                    }
                }
            }
            if ((1U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                if ((4U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                    if ((5U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                        if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__sample) {
                            if ((1U & (~ ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register) 
                                          >> 0xaU)))) {
                                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_NXT 
                                    = ((0xfffeU & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_CRT) 
                                                   << 1U)) 
                                       | (1U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io)));
                            }
                            if ((0x400U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register))) {
                                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_wide_NXT 
                                    = ((0xffffffff00000000ULL 
                                        & vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_wide_NXT) 
                                       | (IData)((IData)(
                                                         ((((IData)(
                                                                    (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_wide_CRT 
                                                                     >> 0x10U)) 
                                                            << 0x11U) 
                                                           | (0x10000U 
                                                              & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io) 
                                                                 << 0xfU))) 
                                                          | ((0xfffeU 
                                                              & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_wide_CRT) 
                                                                 << 1U)) 
                                                             | (1U 
                                                                & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io)))))));
                                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_wide_NXT 
                                    = ((0xffffffffULL 
                                        & vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_wide_NXT) 
                                       | ((QData)((IData)(
                                                          ((((IData)(
                                                                     (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_wide_CRT 
                                                                      >> 0x30U)) 
                                                             << 0x11U) 
                                                            | (0x10000U 
                                                               & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io) 
                                                                  << 0xdU))) 
                                                           | ((0xfffeU 
                                                               & ((IData)(
                                                                          (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_wide_CRT 
                                                                           >> 0x20U)) 
                                                                  << 1U)) 
                                                              | (1U 
                                                                 & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io) 
                                                                    >> 2U)))))) 
                                          << 0x20U));
                            }
                        }
                    }
                }
            }
        }
        if ((0U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
            if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_activate) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__timeout_reset = 1U;
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_initialize = 1U;
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_reset = 1U;
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_NXT 
                    = ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_direction)
                        ? 1U : 2U);
            }
        } else {
            if ((1U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__sample) {
                    if ((0x400U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register))) {
                        if ((0U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io))) {
                            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__timeout_reset = 1U;
                            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_NXT = 4U;
                        }
                    } else if ((1U & (~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io)))) {
                        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__timeout_reset = 1U;
                        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_NXT = 4U;
                    }
                }
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_NXT = 0U;
                if ((0x2710U == vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__timeout_counter)) {
                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_NXT = 0U;
                }
            } else {
                if ((4U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                    if ((5U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                        if ((6U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__timeout_reset = 1U;
                        }
                    }
                }
                if ((4U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                    if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__sample) {
                        if ((0x400U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register))) {
                            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_NXT 
                                = ((0xf0U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_CRT) 
                                             << 4U)) 
                                   | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io));
                            if ((0x3ffU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_counter))) {
                                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_NXT = 5U;
                            }
                        } else {
                            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_NXT 
                                = ((0xfeU & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_CRT) 
                                             << 1U)) 
                                   | (1U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io)));
                            if ((0xfffU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_counter))) {
                                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_NXT = 5U;
                            }
                        }
                    }
                } else if ((5U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                    if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__sample) {
                        if ((0xfU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_counter))) {
                            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_NXT = 6U;
                        }
                    }
                } else if ((6U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_crc_error 
                        = ((0x400U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register))
                            ? (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT 
                               != vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_wide_CRT)
                            : ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_CRT) 
                               != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_CRT)));
                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_NXT 
                        = ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__data_burst_CRT)
                            ? 1U : 0U);
                } else if ((0xeU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                    if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__shift) {
                        if ((0xfU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_counter))) {
                            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_NXT = 2U;
                        }
                    }
                } else if ((2U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                    if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__shift) {
                        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_NXT = 3U;
                    }
                } else if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__shift) {
                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_NXT = 7U;
                }
            }
            if ((1U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                if ((4U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                    if ((5U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                        if ((6U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_initialize = 1U;
                        }
                    }
                }
                if ((4U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                    if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__sample) {
                        if ((0x400U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register))) {
                            if ((0x3ffU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_counter))) {
                                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_reset = 1U;
                            }
                        } else if ((0xfffU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_counter))) {
                            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_reset = 1U;
                        }
                    }
                } else if ((5U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                    if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__sample) {
                        if ((0xfU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_counter))) {
                            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_reset = 1U;
                        }
                    }
                } else if ((6U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_reset = 1U;
                } else if ((0xeU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                    if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__shift) {
                        if ((0xfU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_counter))) {
                            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_reset = 1U;
                        }
                    }
                } else if ((2U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_reset = 1U;
                }
            }
        }
    } else {
        if ((7U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_compute 
                = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__shift;
            if ((0x400U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register))) {
                if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__shift) {
                    if ((0x3ffU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_counter))) {
                        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_reset = 1U;
                        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_NXT 
                            = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_CRT;
                        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_NXT = 8U;
                    }
                }
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_line 
                    = (0xfU & ((1U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_counter))
                                ? (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__tx_data_byte)
                                : ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__tx_data_byte) 
                                   >> 4U)));
            } else {
                if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__shift) {
                    if ((0xfffU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_counter))) {
                        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_reset = 1U;
                        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_NXT 
                            = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_CRT;
                        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_NXT = 8U;
                    }
                }
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_line 
                    = ((0xeU & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_line)) 
                       | (1U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__tx_data_byte) 
                                >> (7U & ((IData)(7U) 
                                          - (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_counter))))));
            }
            if ((1U & (~ ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register) 
                          >> 0xaU)))) {
                if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__shift) {
                    if ((0xfffU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_counter))) {
                        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_wide_NXT 
                            = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT;
                    }
                }
            }
            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_data 
                = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_line;
        } else if ((8U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
            if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__shift) {
                if ((0xfU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_counter))) {
                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_reset = 1U;
                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_NXT = 9U;
                }
            }
            if ((0U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_counter))) {
                if ((1U & (~ ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register) 
                              >> 0xaU)))) {
                    if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__shift) {
                        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_NXT 
                            = (0xffffU & VL_SHIFTL_III(16,16,32, (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_CRT), 1U));
                    }
                }
                if ((0x400U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register))) {
                    if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__shift) {
                        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_wide_NXT 
                            = ((0xffffffff00000000ULL 
                                & vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_wide_NXT) 
                               | (IData)((IData)(((
                                                   VL_SHIFTL_III(16,16,32, (IData)(
                                                                                (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT 
                                                                                >> 0x10U)), 1U) 
                                                   << 0x10U) 
                                                  | (0xffffU 
                                                     & VL_SHIFTL_III(16,16,32, (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT), 1U))))));
                        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_wide_NXT 
                            = ((0xffffffffULL & vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_wide_NXT) 
                               | ((QData)((IData)((
                                                   (VL_SHIFTL_III(16,16,32, (IData)(
                                                                                (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT 
                                                                                >> 0x30U)), 1U) 
                                                    << 0x10U) 
                                                   | (0xffffU 
                                                      & VL_SHIFTL_III(16,16,32, (IData)(
                                                                                (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT 
                                                                                >> 0x20U)), 1U))))) 
                                  << 0x20U));
                    }
                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_line 
                        = (((8U & ((IData)((vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT 
                                            >> 0x3fU)) 
                                   << 3U)) | (4U & 
                                              ((IData)(
                                                       (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT 
                                                        >> 0x2fU)) 
                                               << 2U))) 
                           | ((2U & ((IData)((vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT 
                                              >> 0x1fU)) 
                                     << 1U)) | (1U 
                                                & (IData)(
                                                          (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT 
                                                           >> 0xfU)))));
                } else {
                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_line 
                        = ((0xeU & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_line)) 
                           | (1U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_CRT) 
                                    >> 0xfU)));
                }
            } else {
                if ((1U & (~ ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register) 
                              >> 0xaU)))) {
                    if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__shift) {
                        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_NXT 
                            = (0xffffU & VL_SHIFTL_III(16,16,32, (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_CRT), 1U));
                    }
                }
                if ((0x400U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register))) {
                    if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__shift) {
                        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_wide_NXT 
                            = ((0xffffffff00000000ULL 
                                & vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_wide_NXT) 
                               | (IData)((IData)(((
                                                   VL_SHIFTL_III(16,16,32, (IData)(
                                                                                (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_wide_CRT 
                                                                                >> 0x10U)), 1U) 
                                                   << 0x10U) 
                                                  | (0xffffU 
                                                     & VL_SHIFTL_III(16,16,32, (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_wide_CRT), 1U))))));
                        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_wide_NXT 
                            = ((0xffffffffULL & vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_wide_NXT) 
                               | ((QData)((IData)((
                                                   (VL_SHIFTL_III(16,16,32, (IData)(
                                                                                (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_wide_CRT 
                                                                                >> 0x30U)), 1U) 
                                                    << 0x10U) 
                                                   | (0xffffU 
                                                      & VL_SHIFTL_III(16,16,32, (IData)(
                                                                                (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_wide_CRT 
                                                                                >> 0x20U)), 1U))))) 
                                  << 0x20U));
                    }
                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_line 
                        = (((8U & ((IData)((vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_wide_CRT 
                                            >> 0x3fU)) 
                                   << 3U)) | (4U & 
                                              ((IData)(
                                                       (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_wide_CRT 
                                                        >> 0x2fU)) 
                                               << 2U))) 
                           | ((2U & ((IData)((vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_wide_CRT 
                                              >> 0x1fU)) 
                                     << 1U)) | (1U 
                                                & (IData)(
                                                          (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_wide_CRT 
                                                           >> 0xfU)))));
                } else {
                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_line 
                        = ((0xeU & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_line)) 
                           | (1U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_CRT) 
                                    >> 0xfU)));
                }
            }
        } else if ((9U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
            if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__shift) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_reset = 1U;
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_NXT = 0xbU;
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_NXT = 0U;
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_line 
                    = ((0x400U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register))
                        ? 0xfU : (1U | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_line)));
            }
        } else if ((0xbU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
            if (((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__sample) 
                 & (~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io)))) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_reset = 1U;
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_NXT = 0U;
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_NXT = 0xdU;
            } else if ((0xf4240U <= vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__timeout_counter)) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_NXT = 0U;
            }
        } else if ((0xdU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
            if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__sample) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_NXT 
                    = ((0xfeU & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_CRT) 
                                 << 1U)) | (1U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io)));
                if ((3U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_counter))) {
                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_reset = 1U;
                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_crc_error 
                        = (5U == (7U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_NXT) 
                                        >> 1U)));
                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_error 
                        = (6U == (7U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_NXT) 
                                        >> 1U)));
                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_NXT 
                        = ((2U != (7U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_NXT) 
                                         >> 1U))) ? 0U
                            : 0xaU);
                }
            }
        } else if ((0xaU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
            if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__sample) {
                if ((1U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io))) {
                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_reset = 1U;
                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_NXT 
                        = ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__data_burst_CRT)
                            ? 0xeU : 0U);
                } else if ((0xf4240U <= vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__timeout_counter)) {
                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_NXT = 0U;
                }
            }
        }
        if ((7U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
            if ((8U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                if ((9U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                    if ((0xbU != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                        if ((0xdU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                            if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__sample) {
                                if ((3U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_counter))) {
                                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__timeout_reset = 1U;
                                }
                            }
                        } else if ((0xaU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                            if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__sample) {
                                if ((1U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io))) {
                                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__timeout_reset = 1U;
                                }
                            }
                        }
                    }
                }
            }
            if ((8U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__shift) {
                    if ((0xfU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_counter))) {
                        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_initialize = 1U;
                    }
                }
            } else if ((9U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                if ((0xbU != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                    if ((0xdU != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                        if ((0xaU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                            if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__sample) {
                                if ((1U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io))) {
                                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_initialize = 1U;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT____VdfgRegularize_h6e830a37_0_5 
        = (1U & (((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_CRT) 
                  >> 0xfU) ^ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_data)));
    tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT____VdfgRegularize_h6e830a37_0_4 
        = (IData)(((vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT 
                    >> 0x3fU) ^ ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_data) 
                                 >> 3U)));
    tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT____VdfgRegularize_h6e830a37_0_3 
        = (1U & ((IData)((vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT 
                          >> 0x2fU)) ^ ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_data) 
                                        >> 2U)));
    tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT____VdfgRegularize_h6e830a37_0_2 
        = (1U & ((IData)((vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT 
                          >> 0x1fU)) ^ ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_data) 
                                        >> 1U)));
    tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT____VdfgRegularize_h6e830a37_0_1 
        = (1U & ((IData)((vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT 
                          >> 0xfU)) ^ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_data)));
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_NXT 
        = (((0xe000U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_CRT) 
                        << 1U)) | (0x1000U & ((0xfffff000U 
                                               & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_CRT) 
                                                  << 1U)) 
                                              ^ ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT____VdfgRegularize_h6e830a37_0_5) 
                                                 << 0xcU)))) 
           | ((0xfc0U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_CRT) 
                         << 1U)) | ((0x20U & ((0xffffffe0U 
                                               & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_CRT) 
                                                  << 1U)) 
                                              ^ ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT____VdfgRegularize_h6e830a37_0_5) 
                                                 << 5U))) 
                                    | ((0x1eU & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_CRT) 
                                                 << 1U)) 
                                       | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT____VdfgRegularize_h6e830a37_0_5)))));
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_NXT 
        = (((QData)((IData)((((((IData)((vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT 
                                         >> 0x3cU)) 
                                << 0x1dU) | (0x10000000U 
                                             & (((IData)(
                                                         (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT 
                                                          >> 0x3bU)) 
                                                 ^ (IData)(tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT____VdfgRegularize_h6e830a37_0_4)) 
                                                << 0x1cU))) 
                              | ((0xfc00000U & ((IData)(
                                                        (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT 
                                                         >> 0x35U)) 
                                                << 0x16U)) 
                                 | ((0x200000U & (((IData)(
                                                           (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT 
                                                            >> 0x34U)) 
                                                   ^ (IData)(tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT____VdfgRegularize_h6e830a37_0_4)) 
                                                  << 0x15U)) 
                                    | ((0x1e0000U & 
                                        ((IData)((vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT 
                                                  >> 0x30U)) 
                                         << 0x11U)) 
                                       | ((IData)(tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT____VdfgRegularize_h6e830a37_0_4) 
                                          << 0x10U))))) 
                             | (((0xe000U & ((IData)(
                                                     (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT 
                                                      >> 0x2cU)) 
                                             << 0xdU)) 
                                 | (0x1000U & (((IData)(
                                                        (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT 
                                                         >> 0x2bU)) 
                                                ^ (IData)(tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT____VdfgRegularize_h6e830a37_0_3)) 
                                               << 0xcU))) 
                                | ((0xfc0U & ((IData)(
                                                      (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT 
                                                       >> 0x25U)) 
                                              << 6U)) 
                                   | ((0x20U & (((IData)(
                                                         (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT 
                                                          >> 0x24U)) 
                                                 ^ (IData)(tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT____VdfgRegularize_h6e830a37_0_3)) 
                                                << 5U)) 
                                      | ((0x1eU & ((IData)(
                                                           (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT 
                                                            >> 0x20U)) 
                                                   << 1U)) 
                                         | (IData)(tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT____VdfgRegularize_h6e830a37_0_3)))))))) 
            << 0x20U) | (QData)((IData)((((((IData)(
                                                    (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT 
                                                     >> 0x1cU)) 
                                            << 0x1dU) 
                                           | (0x10000000U 
                                              & (((IData)(
                                                          (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT 
                                                           >> 0x1bU)) 
                                                  ^ (IData)(tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT____VdfgRegularize_h6e830a37_0_2)) 
                                                 << 0x1cU))) 
                                          | ((0xfc00000U 
                                              & ((IData)(
                                                         (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT 
                                                          >> 0x15U)) 
                                                 << 0x16U)) 
                                             | ((0x200000U 
                                                 & (((IData)(
                                                             (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT 
                                                              >> 0x14U)) 
                                                     ^ (IData)(tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT____VdfgRegularize_h6e830a37_0_2)) 
                                                    << 0x15U)) 
                                                | ((0x1e0000U 
                                                    & ((IData)(
                                                               (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT 
                                                                >> 0x10U)) 
                                                       << 0x11U)) 
                                                   | ((IData)(tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT____VdfgRegularize_h6e830a37_0_2) 
                                                      << 0x10U))))) 
                                         | (((0xe000U 
                                              & ((IData)(
                                                         (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT 
                                                          >> 0xcU)) 
                                                 << 0xdU)) 
                                             | (0x1000U 
                                                & (((IData)(
                                                            (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT 
                                                             >> 0xbU)) 
                                                    ^ (IData)(tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT____VdfgRegularize_h6e830a37_0_1)) 
                                                   << 0xcU))) 
                                            | ((0xfc0U 
                                                & ((IData)(
                                                           (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT 
                                                            >> 5U)) 
                                                   << 6U)) 
                                               | ((0x20U 
                                                   & (((IData)(
                                                               (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT 
                                                                >> 4U)) 
                                                       ^ (IData)(tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT____VdfgRegularize_h6e830a37_0_1)) 
                                                      << 5U)) 
                                                  | ((0x1eU 
                                                      & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT) 
                                                         << 1U)) 
                                                     | (IData)(tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT____VdfgRegularize_h6e830a37_0_1)))))))));
}

void Vtb_top___024root___nba_sequent__TOP__0(Vtb_top___024root* vlSelf);
void Vtb_top___024root___nba_sequent__TOP__1(Vtb_top___024root* vlSelf);
void Vtb_top___024root___nba_sequent__TOP__2(Vtb_top___024root* vlSelf);
void Vtb_top___024root___nba_sequent__TOP__3(Vtb_top___024root* vlSelf);
void Vtb_top___024root___nba_sequent__TOP__4(Vtb_top___024root* vlSelf);
void Vtb_top___024root___nba_sequent__TOP__5(Vtb_top___024root* vlSelf);
void Vtb_top___024root___nba_sequent__TOP__6(Vtb_top___024root* vlSelf);
void Vtb_top___024root___nba_sequent__TOP__8(Vtb_top___024root* vlSelf);
void Vtb_top___024root___nba_sequent__TOP__9(Vtb_top___024root* vlSelf);
void Vtb_top___024root___nba_sequent__TOP__10(Vtb_top___024root* vlSelf);
void Vtb_top___024root___nba_sequent__TOP__11(Vtb_top___024root* vlSelf);
void Vtb_top___024root___nba_comb__TOP__0(Vtb_top___024root* vlSelf);
void Vtb_top___024root___nba_sequent__TOP__12(Vtb_top___024root* vlSelf);
void Vtb_top___024root___nba_sequent__TOP__13(Vtb_top___024root* vlSelf);
void Vtb_top___024root___nba_sequent__TOP__14(Vtb_top___024root* vlSelf);
void Vtb_top___024root___nba_comb__TOP__1(Vtb_top___024root* vlSelf);
void Vtb_top___024root___nba_comb__TOP__2(Vtb_top___024root* vlSelf);
void Vtb_top___024root___nba_comb__TOP__3(Vtb_top___024root* vlSelf);
void Vtb_top___024root___nba_sequent__TOP__15(Vtb_top___024root* vlSelf);
void Vtb_top___024root___nba_comb__TOP__4(Vtb_top___024root* vlSelf);
void Vtb_top___024root___nba_comb__TOP__5(Vtb_top___024root* vlSelf);

void Vtb_top___024root___eval_nba(Vtb_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_top___024root___eval_nba\n"); );
    Vtb_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((2ULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        Vtb_top___024root___nba_sequent__TOP__0(vlSelf);
        vlSelfRef.__Vm_traceActivity[3U] = 1U;
    }
    if ((0x38ULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        Vtb_top___024root___nba_sequent__TOP__1(vlSelf);
    }
    if ((0x18ULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        Vtb_top___024root___nba_sequent__TOP__2(vlSelf);
    }
    if ((8ULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        Vtb_top___024root___nba_sequent__TOP__3(vlSelf);
        vlSelfRef.__Vm_traceActivity[4U] = 1U;
    }
    if ((4ULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        Vtb_top___024root___nba_sequent__TOP__4(vlSelf);
        vlSelfRef.__Vm_traceActivity[5U] = 1U;
    }
    if ((0x50ULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        Vtb_top___024root___nba_sequent__TOP__5(vlSelf);
        vlSelfRef.__Vm_traceActivity[6U] = 1U;
    }
    if ((0x80ULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        Vtb_top___024root___nba_sequent__TOP__6(vlSelf);
    }
    if ((8ULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        Vtb_top___024root___nba_sequent__TOP__8(vlSelf);
        vlSelfRef.__Vm_traceActivity[7U] = 1U;
    }
    if ((4ULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        Vtb_top___024root___nba_sequent__TOP__9(vlSelf);
        vlSelfRef.__Vm_traceActivity[8U] = 1U;
    }
    if ((0x50ULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        Vtb_top___024root___nba_sequent__TOP__10(vlSelf);
        vlSelfRef.__Vm_traceActivity[9U] = 1U;
    }
    if ((0x18ULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        Vtb_top___024root___nba_sequent__TOP__11(vlSelf);
        vlSelfRef.__Vm_traceActivity[0xaU] = 1U;
    }
    if ((0x54ULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        Vtb_top___024root___nba_comb__TOP__0(vlSelf);
    }
    if ((0x50ULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        Vtb_top___024root___nba_sequent__TOP__12(vlSelf);
    }
    if ((0x38ULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        Vtb_top___024root___nba_sequent__TOP__13(vlSelf);
    }
    if ((8ULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        Vtb_top___024root___nba_sequent__TOP__14(vlSelf);
        vlSelfRef.__Vm_traceActivity[0xbU] = 1U;
    }
    if ((0x1cULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        Vtb_top___024root___nba_comb__TOP__1(vlSelf);
    }
    if ((0x5aULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        Vtb_top___024root___nba_comb__TOP__2(vlSelf);
        vlSelfRef.__Vm_traceActivity[0xcU] = 1U;
    }
    if ((0x58ULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        Vtb_top___024root___nba_comb__TOP__3(vlSelf);
    }
    if ((4ULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        Vtb_top___024root___nba_sequent__TOP__15(vlSelf);
        vlSelfRef.__Vm_traceActivity[0xdU] = 1U;
    }
    if ((0x59ULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        Vtb_top___024root___nba_comb__TOP__4(vlSelf);
    }
    if ((0x5bULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        Vtb_top___024root___nba_comb__TOP__5(vlSelf);
        vlSelfRef.__Vm_traceActivity[0xeU] = 1U;
    }
}

extern const VlUnpacked<CData/*0:0*/, 256> Vtb_top__ConstPool__TABLE_h38363b78_0;
extern const VlUnpacked<CData/*4:0*/, 256> Vtb_top__ConstPool__TABLE_hdcf19efa_0;

VL_INLINE_OPT void Vtb_top___024root___nba_sequent__TOP__0(Vtb_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_top___024root___nba_sequent__TOP__0\n"); );
    Vtb_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    CData/*7:0*/ __Vtableidx1;
    __Vtableidx1 = 0;
    IData/*16:0*/ __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__reset_counter;
    __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__reset_counter = 0;
    CData/*7:0*/ __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__counter;
    __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__counter = 0;
    CData/*1:0*/ __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__tx_data_selector;
    __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__tx_data_selector = 0;
    IData/*31:0*/ __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__data_rx;
    __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__data_rx = 0;
    CData/*4:0*/ __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__write_selector;
    __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__write_selector = 0;
    CData/*1:0*/ __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__card_detect_synchronizer__DOT__sync_signal;
    __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__card_detect_synchronizer__DOT__sync_signal = 0;
    SData/*14:0*/ __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register;
    __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register = 0;
    SData/*8:0*/ __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register;
    __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register = 0;
    CData/*7:0*/ __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__bit_counter;
    __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__bit_counter = 0;
    IData/*25:0*/ __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__timeout_counter;
    __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__timeout_counter = 0;
    CData/*6:0*/ __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7__DOT__crc;
    __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7__DOT__crc = 0;
    SData/*11:0*/ __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_counter;
    __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_counter = 0;
    IData/*19:0*/ __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__timeout_counter;
    __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__timeout_counter = 0;
    IData/*31:0*/ __VdlyVal__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__tx_buffer__DOT__buffer_memory__v0;
    __VdlyVal__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__tx_buffer__DOT__buffer_memory__v0 = 0;
    CData/*6:0*/ __VdlyDim0__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__tx_buffer__DOT__buffer_memory__v0;
    __VdlyDim0__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__tx_buffer__DOT__buffer_memory__v0 = 0;
    CData/*0:0*/ __VdlySet__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__tx_buffer__DOT__buffer_memory__v0;
    __VdlySet__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__tx_buffer__DOT__buffer_memory__v0 = 0;
    IData/*31:0*/ __VdlyVal__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__rx_buffer__DOT__buffer_memory__v0;
    __VdlyVal__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__rx_buffer__DOT__buffer_memory__v0 = 0;
    CData/*6:0*/ __VdlyDim0__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__rx_buffer__DOT__buffer_memory__v0;
    __VdlyDim0__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__rx_buffer__DOT__buffer_memory__v0 = 0;
    CData/*0:0*/ __VdlySet__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__rx_buffer__DOT__buffer_memory__v0;
    __VdlySet__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__rx_buffer__DOT__buffer_memory__v0 = 0;
    CData/*7:0*/ __VdlyVal__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__buffer_memory__v0;
    __VdlyVal__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__buffer_memory__v0 = 0;
    CData/*4:0*/ __VdlyDim0__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__buffer_memory__v0;
    __VdlyDim0__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__buffer_memory__v0 = 0;
    CData/*0:0*/ __VdlySet__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__buffer_memory__v0;
    __VdlySet__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__buffer_memory__v0 = 0;
    // Body
    __VdlySet__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__tx_buffer__DOT__buffer_memory__v0 = 0U;
    __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__card_detect_synchronizer__DOT__sync_signal 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__card_detect_synchronizer__DOT__sync_signal;
    __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__data_rx 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_rx;
    __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__write_selector 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__write_selector;
    __VdlySet__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__rx_buffer__DOT__buffer_memory__v0 = 0U;
    __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__timeout_counter 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__timeout_counter;
    __VdlySet__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__buffer_memory__v0 = 0U;
    __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__timeout_counter 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__timeout_counter;
    __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7__DOT__crc 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7__DOT__crc;
    __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__tx_data_selector 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__tx_data_selector;
    __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__bit_counter 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__bit_counter;
    __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__reset_counter 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__reset_counter;
    __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_counter 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_counter;
    __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register;
    __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__counter 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__counter;
    __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register;
    if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__write_tx_buffer) {
        __VdlyVal__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__tx_buffer__DOT__buffer_memory__v0 
            = vlSelfRef.write_data_i;
        __VdlyDim0__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__tx_buffer__DOT__buffer_memory__v0 
            = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__tx_buffer__DOT__write_ptr;
        __VdlySet__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__tx_buffer__DOT__buffer_memory__v0 = 1U;
    }
    if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_rx_write) {
        __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__data_rx 
            = ((vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_rx 
                << 8U) | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_CRT));
    }
    __Vtableidx1 = (((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__write_selector) 
                     << 3U) | (((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_rx_write) 
                                << 2U) | (((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_activate_edge) 
                                           << 1U) | (IData)(vlSelfRef.rst_n))));
    if (Vtb_top__ConstPool__TABLE_h38363b78_0[__Vtableidx1]) {
        __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__write_selector 
            = Vtb_top__ConstPool__TABLE_hdcf19efa_0
            [__Vtableidx1];
    }
    if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_buffer_rx_write) {
        __VdlyVal__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__rx_buffer__DOT__buffer_memory__v0 
            = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_rx;
        __VdlyDim0__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__rx_buffer__DOT__buffer_memory__v0 
            = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__rx_buffer__DOT__write_ptr;
        __VdlySet__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__rx_buffer__DOT__buffer_memory__v0 = 1U;
    }
    if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__cmd_response_valid) {
        __VdlyVal__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__buffer_memory__v0 
            = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__resp_NXT;
        __VdlyDim0__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__buffer_memory__v0 
            = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__write_ptr;
        __VdlySet__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__buffer_memory__v0 = 1U;
    }
    if ((1U & ((~ (IData)(vlSelfRef.rst_n)) | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7_initialize)))) {
        __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7__DOT__crc = 0U;
    } else if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7_compute) {
        __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7__DOT__crc 
            = ((0xfU & (IData)(__Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7__DOT__crc)) 
               | (0x70U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7__DOT__crc) 
                           << 1U)));
        __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7__DOT__crc 
            = ((0x77U & (IData)(__Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7__DOT__crc)) 
               | (8U & ((VL_REDXOR_8((0x44U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7__DOT__crc))) 
                         ^ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7_data)) 
                        << 3U)));
        __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7__DOT__crc 
            = ((0x78U & (IData)(__Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7__DOT__crc)) 
               | ((6U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7__DOT__crc) 
                         << 1U)) | (IData)((((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7__DOT__crc) 
                                             >> 6U) 
                                            ^ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7_data)))));
    }
    if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_activate_edge) {
        __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__tx_data_selector = 3U;
    } else if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_tx_read) {
        __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__tx_data_selector 
            = (3U & ((IData)(1U) + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__tx_data_selector)));
    }
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__resp_CRT 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__resp_NXT;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__rcv136_CRT 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__rcv136_NXT;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__cmd_number_CRT 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__cmd_number_NXT;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__cmd_argument_CRT 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__cmd_argument_NXT;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7_CRT 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7_NXT;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_rx_write_detector__DOT__previous_signal 
        = ((IData)(vlSelfRef.rst_n) && (1U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__write_selector) 
                                              >> 3U)));
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__read_done_delay 
        = ((IData)(vlSelfRef.rst_n) && ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__is_delayed) 
                                        & (IData)(vlSelfRef.read_i)));
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__tristate_enable_flop 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__tristate_enable;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__cmd_bit_flop 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__cmd_bit;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__data_burst_CRT 
        = ((IData)(vlSelfRef.rst_n) && (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__data_burst_NXT));
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__genblk1__BRA__0__KET____DOT__event_detector__DOT__previous_signal 
        = ((IData)(vlSelfRef.rst_n) && (1U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register)));
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__genblk1__BRA__1__KET____DOT__event_detector__DOT__previous_signal 
        = ((IData)(vlSelfRef.rst_n) && (1U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register) 
                                              >> 1U)));
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__genblk1__BRA__2__KET____DOT__event_detector__DOT__previous_signal 
        = ((IData)(vlSelfRef.rst_n) && (1U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register) 
                                              >> 2U)));
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__genblk1__BRA__3__KET____DOT__event_detector__DOT__previous_signal 
        = ((IData)(vlSelfRef.rst_n) && (1U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register) 
                                              >> 3U)));
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__genblk1__BRA__4__KET____DOT__event_detector__DOT__previous_signal 
        = ((IData)(vlSelfRef.rst_n) && (1U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register) 
                                              >> 4U)));
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__genblk1__BRA__5__KET____DOT__event_detector__DOT__previous_signal 
        = ((IData)(vlSelfRef.rst_n) && (1U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register) 
                                              >> 5U)));
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__genblk1__BRA__6__KET____DOT__event_detector__DOT__previous_signal 
        = ((IData)(vlSelfRef.rst_n) && (1U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register) 
                                              >> 6U)));
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__genblk1__BRA__7__KET____DOT__event_detector__DOT__previous_signal 
        = ((IData)(vlSelfRef.rst_n) && (1U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register) 
                                              >> 7U)));
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__genblk1__BRA__8__KET____DOT__event_detector__DOT__previous_signal 
        = ((IData)(vlSelfRef.rst_n) && (1U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register) 
                                              >> 8U)));
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_fsm_activation_detector__DOT__previous_signal 
        = ((IData)(vlSelfRef.rst_n) && (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_activate));
    if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__read_rx_buffer) {
        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__read_data_rx 
            = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__rx_buffer__DOT__buffer_memory
            [vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__rx_buffer__DOT__read_ptr];
    }
    if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__read_resp_buffer) {
        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__read_response_data 
            = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__buffer_memory
            [vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__read_ptr];
    }
    if (vlSelfRef.rst_n) {
        __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__card_detect_synchronizer__DOT__sync_signal 
            = (2U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__card_detect_synchronizer__DOT__sync_signal) 
                     << 1U));
        if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__timeout_reset) {
            __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__timeout_counter = 0U;
        } else if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__timeout_increment) {
            __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__timeout_counter 
                = (0x3ffffffU & ((IData)(1U) + vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__timeout_counter));
        }
        if (((3U == (7U & vlSelfRef.write_address_i)) 
             & (IData)(vlSelfRef.write_i))) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__cmd_argument 
                = ((vlSelfRef.write_data_i & vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__mask) 
                   | (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__cmd_argument 
                      & (~ vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__mask)));
        }
        if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__timeout_reset) {
            __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__timeout_counter = 0U;
        } else if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__timeout_increment) {
            __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__timeout_counter 
                = (0xfffffU & ((IData)(1U) + vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__timeout_counter));
        }
        if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__bit_reset) {
            __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__bit_counter = 0U;
        } else if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__bit_increment) {
            __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__bit_counter 
                = (0xffU & ((IData)(1U) + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__bit_counter)));
        }
        if (((2U == (7U & vlSelfRef.write_address_i)) 
             & (IData)(vlSelfRef.write_i))) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__cmd_number 
                = (0x3fU & ((vlSelfRef.write_data_i 
                             & vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__mask) 
                            | ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__cmd_number) 
                               & (~ vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__mask))));
        }
        if ((0x1869fU == vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__reset_counter)) {
            __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__reset_counter = 0U;
            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__active = 0U;
        } else {
            if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__active) {
                __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__reset_counter 
                    = (0x1ffffU & ((IData)(1U) + vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__reset_counter));
            }
            if (((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__reset_done)) 
                 & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register) 
                    >> 0xeU))) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__active = 1U;
            }
        }
        if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_reset) {
            __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_counter = 0U;
        } else if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_increment) {
            __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_counter 
                = (0xfffU & ((IData)(1U) + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_counter)));
        }
        if (((0U == (7U & vlSelfRef.write_address_i)) 
             & (IData)(vlSelfRef.write_i))) {
            __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register 
                = (0x7fffU & ((vlSelfRef.write_data_i 
                               & vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__mask) 
                              | ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register) 
                                 & (~ vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__mask))));
        } else {
            __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register 
                = (0x7dffU & (IData)(__Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register));
            __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register 
                = (0x6fffU & (IData)(__Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register));
            if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__reset_done) {
                __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register 
                    = (0x3fffU & (IData)(__Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register));
            }
        }
        if (((1U == (7U & vlSelfRef.write_address_i)) 
             & (IData)(vlSelfRef.write_i))) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__status_register 
                = (0x7fffU & ((vlSelfRef.write_data_i 
                               & vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__mask) 
                              | ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__status_register) 
                                 & (~ vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__mask))));
        } else {
            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__status_register 
                = ((0x7ffU & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__status_register)) 
                   | ((((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__sd_card_detect) 
                        << 0xeU) | ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__tx_buffer_empty) 
                                    << 0xdU)) | (((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__tx_buffer_full) 
                                                  << 0xcU) 
                                                 | ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__rx_buffer_empty) 
                                                    << 0xbU))));
            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__status_register 
                = ((0x787fU & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__status_register)) 
                   | ((((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__rx_buffer_full) 
                        << 0xaU) | ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__resp_buffer_empty) 
                                    << 9U)) | (((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__resp_buffer_full) 
                                                << 8U) 
                                               | ((0U 
                                                   == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT)) 
                                                  << 7U))));
            if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__cmd_crc_error_ff) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__status_register 
                    = (0x40U | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__status_register));
            }
            if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__cmd_timeout_ff) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__status_register 
                    = (0x20U | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__status_register));
            }
            if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__cmd_response_valid) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__status_register 
                    = ((0x7fefU & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__status_register)) 
                       | ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__cmd_response_type) 
                          << 4U));
            }
            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__status_register 
                = ((0x7ff7U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__status_register)) 
                   | ((0U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT)) 
                      << 3U));
            if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_crc_error_ff) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__status_register 
                    = (4U | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__status_register));
            }
            if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_timeout_ff) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__status_register 
                    = (1U | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__status_register));
            }
            if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_error_ff) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__status_register 
                    = (2U | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__status_register));
            }
        }
        if (((4U == (7U & vlSelfRef.write_address_i)) 
             & (IData)(vlSelfRef.write_i))) {
            __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register 
                = (0x1ffU & ((vlSelfRef.write_data_i 
                              & vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__mask) 
                             | ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register) 
                                & (~ vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__mask))));
        } else {
            if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__tx_buffer_empty) {
                __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register 
                    = ((0x17fU & (IData)(__Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register)) 
                       | (0x80U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register) 
                                   << 7U)));
            }
            if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__rx_buffer_full) {
                __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register 
                    = ((0x1bfU & (IData)(__Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register)) 
                       | (0x40U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register) 
                                   << 5U)));
            }
            if (((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__cmd_done_detector__DOT__previous_signal)) 
                 & (0U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT)))) {
                __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register 
                    = ((0x1dfU & (IData)(__Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register)) 
                       | (0x20U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register) 
                                   << 3U)));
            }
            if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__cmd_crc_error_ff) {
                __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register 
                    = ((0x1efU & (IData)(__Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register)) 
                       | (0x10U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register) 
                                   << 1U)));
            }
            if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__cmd_timeout_ff) {
                __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register 
                    = ((0x1f7U & (IData)(__Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register)) 
                       | (8U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register) 
                                >> 1U)));
            }
            if (((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__data_done_detector__DOT__previous_signal)) 
                 & (0U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT)))) {
                __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register 
                    = ((0x1fbU & (IData)(__Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register)) 
                       | (4U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register) 
                                >> 3U)));
            }
            if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_crc_error_ff) {
                __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register 
                    = ((0x1fdU & (IData)(__Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register)) 
                       | (2U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register) 
                                >> 5U)));
            }
            if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_timeout_ff) {
                __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register 
                    = ((0x1feU & (IData)(__Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register)) 
                       | (1U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register) 
                                >> 7U)));
            }
            if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__sd_card_detect) {
                __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register 
                    = ((0xffU & (IData)(__Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register)) 
                       | (0x100U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register)));
            }
        }
        if ((0x2000U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register))) {
            __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__counter 
                = (0xffU & ((IData)(1U) + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__counter)));
            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__clk_previous 
                = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__sd_clk;
            if (((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__counter) 
                 == (((0x800U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register))
                       ? 1U : 0x7cU) - (IData)(1U)))) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__sd_clk 
                    = (1U & (~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__sd_clk)));
                __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__counter = 0U;
            }
        }
        if ((1U == (((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_buffer_rx_write) 
                     << 1U) | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__read_rx_buffer)))) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__rx_buffer_empty 
                = ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__rx_buffer__DOT__write_ptr) 
                   == (0x7fU & ((IData)(1U) + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__rx_buffer__DOT__read_ptr))));
            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__rx_buffer_full = 0U;
        } else if ((2U == (((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_buffer_rx_write) 
                            << 1U) | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__read_rx_buffer)))) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__rx_buffer_empty = 0U;
            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__rx_buffer_full 
                = ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__rx_buffer__DOT__read_ptr) 
                   == (0x7fU & ((IData)(1U) + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__rx_buffer__DOT__write_ptr))));
        }
        if ((1U == (((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__cmd_response_valid) 
                     << 1U) | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__read_resp_buffer)))) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__resp_buffer_empty 
                = ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__write_ptr) 
                   == (0x1fU & ((IData)(1U) + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__read_ptr))));
            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__resp_buffer_full = 0U;
        } else if ((2U == (((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__cmd_response_valid) 
                            << 1U) | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__read_resp_buffer)))) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__resp_buffer_empty = 0U;
            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__resp_buffer_full 
                = ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__read_ptr) 
                   == (0x1fU & ((IData)(1U) + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__write_ptr))));
        }
        if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__read_resp_buffer) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__read_ptr 
                = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__inc_read_ptr;
        }
        if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__cmd_response_valid) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__write_ptr 
                = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__inc_write_ptr;
        }
        if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__read_rx_buffer) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__rx_buffer__DOT__read_ptr 
                = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__rx_buffer__DOT__inc_read_ptr;
        }
        if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_buffer_rx_write) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__rx_buffer__DOT__write_ptr 
                = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__rx_buffer__DOT__inc_write_ptr;
        }
    } else {
        __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__card_detect_synchronizer__DOT__sync_signal = 0U;
        __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__timeout_counter = 0U;
        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__cmd_argument = 0U;
        __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__timeout_counter = 0U;
        __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__bit_counter = 0U;
        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__cmd_number = 0U;
        __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__reset_counter = 0U;
        __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_counter = 0U;
        __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register = 0U;
        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__status_register = 0U;
        __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register = 0U;
        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__sd_clk = 0U;
        __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__counter = 0U;
        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__clk_previous = 0U;
        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__active = 0U;
        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__rx_buffer_empty = 1U;
        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__resp_buffer_empty = 1U;
        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__resp_buffer_full = 0U;
        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__rx_buffer_full = 0U;
        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__read_ptr = 0U;
        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__write_ptr = 0U;
        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__rx_buffer__DOT__read_ptr = 0U;
        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__rx_buffer__DOT__write_ptr = 0U;
    }
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_CRT 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_NXT;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_wide_CRT 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_wide_NXT;
    if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT____Vcellinp__tx_buffer__read_i) {
        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT____Vcellout__register_interface__data_tx_data_o 
            = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__tx_buffer__DOT__buffer_memory
            [vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__tx_buffer__DOT__read_ptr];
    }
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__card_app_cmd_CRT 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__card_app_cmd_NXT;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__tristate_enable_flop 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__tristate_enable;
    if ((1U & ((~ (IData)(vlSelfRef.rst_n)) | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_initialize)))) {
        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_CRT = 0U;
        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT = 0ULL;
    } else if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_compute) {
        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_CRT 
            = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_NXT;
        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT 
            = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_NXT;
    }
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_line_flop 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_line;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_rx 
        = __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__data_rx;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__timeout_counter 
        = __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__timeout_counter;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__timeout_counter 
        = __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__timeout_counter;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7__DOT__crc 
        = __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7__DOT__crc;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__tx_data_selector 
        = __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__tx_data_selector;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__bit_counter 
        = __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__bit_counter;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_counter 
        = __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_counter;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__write_selector 
        = __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__write_selector;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register 
        = __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register;
    if (__VdlySet__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__rx_buffer__DOT__buffer_memory__v0) {
        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__rx_buffer__DOT__buffer_memory[__VdlyDim0__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__rx_buffer__DOT__buffer_memory__v0] 
            = __VdlyVal__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__rx_buffer__DOT__buffer_memory__v0;
    }
    if (__VdlySet__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__buffer_memory__v0) {
        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__buffer_memory[__VdlyDim0__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__buffer_memory__v0] 
            = __VdlyVal__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__buffer_memory__v0;
    }
    if (__VdlySet__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__tx_buffer__DOT__buffer_memory__v0) {
        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__tx_buffer__DOT__buffer_memory[__VdlyDim0__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__tx_buffer__DOT__buffer_memory__v0] 
            = __VdlyVal__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__tx_buffer__DOT__buffer_memory__v0;
    }
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__counter 
        = __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__counter;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_CRT 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_NXT;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_rx_write 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__rx_valid;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_error_ff 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_error;
    if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT____Vcellinp__tx_buffer__rst_n_i) {
        if ((1U == (((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__write_tx_buffer) 
                     << 1U) | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT____Vcellinp__tx_buffer__read_i)))) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__tx_buffer_full = 0U;
            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__tx_buffer_empty 
                = ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__tx_buffer__DOT__write_ptr) 
                   == (0x7fU & ((IData)(1U) + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__tx_buffer__DOT__read_ptr))));
        } else if ((2U == (((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__write_tx_buffer) 
                            << 1U) | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT____Vcellinp__tx_buffer__read_i)))) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__tx_buffer_full 
                = ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__tx_buffer__DOT__read_ptr) 
                   == (0x7fU & ((IData)(1U) + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__tx_buffer__DOT__write_ptr))));
            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__tx_buffer_empty = 0U;
        }
        if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__write_tx_buffer) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__tx_buffer__DOT__write_ptr 
                = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__tx_buffer__DOT__inc_write_ptr;
        }
        if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT____Vcellinp__tx_buffer__read_i) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__tx_buffer__DOT__read_ptr 
                = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__tx_buffer__DOT__inc_read_ptr;
        }
    } else {
        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__tx_buffer_full = 0U;
        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__tx_buffer_empty = 1U;
        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__tx_buffer__DOT__write_ptr = 0U;
        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__tx_buffer__DOT__read_ptr = 0U;
    }
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__cmd_timeout_ff 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__cmd_timeout;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__cmd_crc_error_ff 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__cmd_crc_error;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__sd_card_detect 
        = (1U & (~ ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__card_detect_synchronizer__DOT__sync_signal) 
                    >> 1U)));
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_timeout_ff 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_timeout;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_crc_error_ff 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_crc_error;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__data_done_detector__DOT__previous_signal 
        = ((IData)(vlSelfRef.rst_n) && (0U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT)));
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__cmd_done_detector__DOT__previous_signal 
        = ((IData)(vlSelfRef.rst_n) && (0U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT)));
    vlSelfRef.read_done_o = ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__is_delayed)
                              ? (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__read_done_delay)
                              : (IData)(vlSelfRef.read_i));
    vlSelfRef.interrupt_o = (0U != ((((((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__genblk1__BRA__8__KET____DOT__event_detector__DOT__previous_signal)) 
                                        & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register) 
                                           >> 8U)) 
                                       << 8U) | ((0x80U 
                                                  & (((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__genblk1__BRA__7__KET____DOT__event_detector__DOT__previous_signal)) 
                                                      << 7U) 
                                                     & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register))) 
                                                 | (0x40U 
                                                    & (((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__genblk1__BRA__6__KET____DOT__event_detector__DOT__previous_signal)) 
                                                        << 6U) 
                                                       & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register))))) 
                                     | ((0x20U & ((
                                                   (~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__genblk1__BRA__5__KET____DOT__event_detector__DOT__previous_signal)) 
                                                   << 5U) 
                                                  & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register))) 
                                        | (0x10U & 
                                           (((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__genblk1__BRA__4__KET____DOT__event_detector__DOT__previous_signal)) 
                                             << 4U) 
                                            & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register))))) 
                                    | (((8U & (((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__genblk1__BRA__3__KET____DOT__event_detector__DOT__previous_signal)) 
                                                << 3U) 
                                               & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register))) 
                                        | (4U & (((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__genblk1__BRA__2__KET____DOT__event_detector__DOT__previous_signal)) 
                                                  << 2U) 
                                                 & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register)))) 
                                       | ((2U & (((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__genblk1__BRA__1__KET____DOT__event_detector__DOT__previous_signal)) 
                                                  << 1U) 
                                                 & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register))) 
                                          | (1U & (
                                                   (~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__genblk1__BRA__0__KET____DOT__event_detector__DOT__previous_signal)) 
                                                   & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register)))))));
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__tx_data_byte 
        = (0xffU & (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT____Vcellout__register_interface__data_tx_data_o 
                    >> (0x1fU & VL_SHIFTL_III(5,5,32, (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__tx_data_selector), 3U))));
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__shift 
        = ((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__sd_clk)) 
           & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__clk_previous));
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__sample 
        = ((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__clk_previous)) 
           & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__sd_clk));
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__card_detect_synchronizer__DOT__sync_signal 
        = __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__card_detect_synchronizer__DOT__sync_signal;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__reset_done 
        = ((IData)(vlSelfRef.rst_n) && (0x1869fU == vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__reset_counter));
    if ((1U & (((~ (IData)(vlSelfRef.rst_n)) | (~ ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register) 
                                                   >> 0xdU))) 
               | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__force_stop)))) {
        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT = 0U;
    } else if ((0x2000U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register))) {
        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT 
            = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_NXT;
    }
    if ((1U & ((~ (IData)(vlSelfRef.rst_n)) | (~ ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register) 
                                                  >> 0xdU))))) {
        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT = 0U;
    } else if ((0x2000U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register))) {
        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT 
            = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_NXT;
    }
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__reset_counter 
        = __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__reset_counter;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_buffer_rx_write 
        = (1U & ((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_rx_write_detector__DOT__previous_signal)) 
                 & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__write_selector) 
                    >> 3U)));
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register 
        = __Vdly__tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__inc_read_ptr 
        = (0x1fU & ((IData)(1U) + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__read_ptr)));
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__inc_write_ptr 
        = (0x1fU & ((IData)(1U) + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__write_ptr)));
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__tx_buffer__DOT__inc_write_ptr 
        = (0x7fU & ((IData)(1U) + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__tx_buffer__DOT__write_ptr)));
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__tx_buffer__DOT__inc_read_ptr 
        = (0x7fU & ((IData)(1U) + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__tx_buffer__DOT__read_ptr)));
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__rx_buffer__DOT__inc_read_ptr 
        = (0x7fU & ((IData)(1U) + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__rx_buffer__DOT__read_ptr)));
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__rx_buffer__DOT__inc_write_ptr 
        = (0x7fU & ((IData)(1U) + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__rx_buffer__DOT__write_ptr)));
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__timeout_increment = 0U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_increment = 0U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT____Vcellinp__tx_buffer__rst_n_i 
        = ((~ ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register) 
               >> 0xcU)) & (IData)(vlSelfRef.rst_n));
    if ((1U & (~ ((((((((0U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT)) 
                        | (1U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) 
                       | (4U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) 
                      | (5U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) 
                     | (6U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) 
                    | (0xeU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) 
                   | (2U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) 
                  | (3U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT)))))) {
        if ((7U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
            if ((8U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                if ((0U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_counter))) {
                    if ((0x400U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register))) {
                        if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__shift) {
                            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__unnamedblk2__DOT__i = 4U;
                        }
                    }
                }
                if ((0U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_counter))) {
                    if ((0x400U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register))) {
                        if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__shift) {
                            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__unnamedblk3__DOT__i = 4U;
                        }
                    }
                }
            }
        }
    }
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__rx_valid = 0U;
    vlSelfRef.read_data_o = 0U;
    if ((0U == (7U & vlSelfRef.read_address_i))) {
        vlSelfRef.read_data_o = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register;
    } else if ((1U == (7U & vlSelfRef.read_address_i))) {
        vlSelfRef.read_data_o = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__status_register;
    } else if ((2U == (7U & vlSelfRef.read_address_i))) {
        vlSelfRef.read_data_o = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__cmd_number;
    } else if ((3U == (7U & vlSelfRef.read_address_i))) {
        vlSelfRef.read_data_o = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__cmd_argument;
    } else if ((4U == (7U & vlSelfRef.read_address_i))) {
        vlSelfRef.read_data_o = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register;
    } else if ((5U == (7U & vlSelfRef.read_address_i))) {
        vlSelfRef.read_data_o = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__read_response_data;
    } else if ((7U == (7U & vlSelfRef.read_address_i))) {
        vlSelfRef.read_data_o = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__read_data_rx;
    }
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_tx_read = 0U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__timeout_increment = 0U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__tristate_enable = 1U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__cmd_timeout = 0U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__force_stop = 0U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__cmd_response_type = 0U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7_initialize = 0U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__data_burst_NXT 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__data_burst_CRT;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__cmd_crc_error = 0U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__cmd_response_valid = 0U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__card_app_cmd_NXT 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__card_app_cmd_CRT;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__cmd_argument_NXT 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__cmd_argument_CRT;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__cmd_number_NXT 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__cmd_number_CRT;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__rcv136_NXT 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__rcv136_CRT;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__bit_reset = 0U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__cmd_bit = 1U;
    if (((((((((0U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT)) 
               | (1U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) 
              | (2U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) 
             | (3U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) 
            | (4U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) 
           | (5U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) 
          | (6U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) 
         | (7U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT)))) {
        if ((0U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7_initialize = 1U;
            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__cmd_argument_NXT 
                = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__cmd_argument;
            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__cmd_number_NXT 
                = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__cmd_number;
            if ((0x200U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register))) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__rcv136_NXT 
                    = (((2U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__cmd_number)) 
                        | (9U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__cmd_number))) 
                       | (0xaU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__cmd_number)));
            }
        } else if ((1U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
            if ((2U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
                if ((3U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
                    if ((4U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
                        if ((5U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
                            if ((6U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
                                if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__shift) {
                                    if ((6U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__bit_counter))) {
                                        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7_initialize = 1U;
                                    }
                                }
                            }
                        }
                        if ((5U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
                            if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__shift) {
                                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__cmd_argument_NXT 
                                    = VL_SHIFTL_III(32,32,32, vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__cmd_argument_CRT, 1U);
                            }
                        }
                    }
                    if ((4U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
                        if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__shift) {
                            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__cmd_number_NXT 
                                = (0x3fU & VL_SHIFTL_III(6,6,32, (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__cmd_number_CRT), 1U));
                        }
                    }
                }
            }
        }
        if ((0U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
            if ((1U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
                if ((2U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
                    if ((3U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
                        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__bit_reset = 1U;
                    } else if ((4U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
                        if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__shift) {
                            if ((5U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__bit_counter))) {
                                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__bit_reset = 1U;
                            }
                        }
                    } else if ((5U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
                        if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__shift) {
                            if ((0x1fU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__bit_counter))) {
                                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__bit_reset = 1U;
                            }
                        }
                    } else if ((6U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
                        if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__shift) {
                            if ((6U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__bit_counter))) {
                                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__bit_reset = 1U;
                            }
                        }
                    } else {
                        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__bit_reset = 1U;
                    }
                }
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__cmd_bit 
                    = ((2U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT)) 
                       && ((3U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT)) 
                           || (1U & ((4U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))
                                      ? ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__cmd_number_CRT) 
                                         >> 5U) : (
                                                   (5U 
                                                    == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))
                                                    ? 
                                                   (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__cmd_argument_CRT 
                                                    >> 0x1fU)
                                                    : 
                                                   ((6U 
                                                     != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT)) 
                                                    || (1U 
                                                        & ((0U 
                                                            == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__bit_counter))
                                                            ? 
                                                           ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7__DOT__crc) 
                                                            >> 6U)
                                                            : 
                                                           ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7_CRT) 
                                                            >> 6U)))))))));
            }
        }
    } else if ((8U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
        if ((9U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
            if ((0xaU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
                if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__sample) {
                    if ((0x2fU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__bit_counter))) {
                        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__bit_reset = 1U;
                    }
                }
            } else if ((0xbU != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
                if ((0xcU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__bit_reset = 1U;
                }
            }
        }
    }
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_direction = 0U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_activate = 0U;
    if ((1U & (~ ((((((((0U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT)) 
                        | (1U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) 
                       | (2U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) 
                      | (3U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) 
                     | (4U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) 
                    | (5U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) 
                   | (6U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) 
                  | (7U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT)))))) {
        if ((8U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__timeout_increment = 1U;
            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__tristate_enable = 0U;
            if ((0x2faf080U == vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__timeout_counter)) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__cmd_timeout = 1U;
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__cmd_response_valid = 0U;
            }
        } else {
            if ((9U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__tristate_enable = 0U;
            } else if ((0xaU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__tristate_enable = 0U;
            } else if ((0xbU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__tristate_enable = 0U;
            }
            if ((9U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
                if ((0xaU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
                    if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__sample) {
                        if ((7U == (7U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__bit_counter)))) {
                            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__cmd_response_valid = 1U;
                        }
                        if ((0x2fU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__bit_counter))) {
                            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__cmd_response_valid = 1U;
                        }
                    }
                } else if ((0xbU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
                    if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__sample) {
                        if ((7U == (7U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__bit_counter)))) {
                            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__cmd_response_valid = 1U;
                        }
                    }
                }
            }
        }
        if ((8U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
            if ((9U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
                if ((0xaU != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
                    if ((0xbU != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
                        if ((0xcU != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
                            if ((0xdU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
                                if ((0xcU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__cmd_number))) {
                                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__force_stop = 1U;
                                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__data_burst_NXT = 0U;
                                } else if ((0x11U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__cmd_number))) {
                                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__data_burst_NXT = 0U;
                                } else if ((0x12U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__cmd_number))) {
                                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__data_burst_NXT = 1U;
                                } else if ((0x18U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__cmd_number))) {
                                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__data_burst_NXT = 0U;
                                } else if ((0x19U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__cmd_number))) {
                                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__data_burst_NXT = 1U;
                                }
                                if ((0x10U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__bit_counter))) {
                                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__card_app_cmd_NXT 
                                        = (0x37U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__cmd_number));
                                }
                                if ((0xcU != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__cmd_number))) {
                                    if ((0x11U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__cmd_number))) {
                                        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_direction = 1U;
                                        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_activate = 1U;
                                    } else if ((0x12U 
                                                == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__cmd_number))) {
                                        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_direction = 1U;
                                        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_activate = 1U;
                                    } else if ((0x18U 
                                                == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__cmd_number))) {
                                        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_direction = 0U;
                                        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_activate = 1U;
                                    } else if ((0x19U 
                                                == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__cmd_number))) {
                                        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_direction = 0U;
                                        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_activate = 1U;
                                    } else if ((6U 
                                                == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__cmd_number))) {
                                        if ((1U & (~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__card_app_cmd_CRT)))) {
                                            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_direction = 1U;
                                            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_activate = 1U;
                                        }
                                    } else if ((((0xdU 
                                                  == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__cmd_number)) 
                                                 || (0x16U 
                                                     == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__cmd_number))) 
                                                || (0x33U 
                                                    == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__cmd_number)))) {
                                        if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__card_app_cmd_CRT) {
                                            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_direction = 1U;
                                            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_activate = 1U;
                                        }
                                    }
                                }
                            }
                        }
                        if ((0xcU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
                            if (((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7_CRT) 
                                 != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7__DOT__crc))) {
                                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__cmd_crc_error = 1U;
                            }
                        }
                    }
                }
                if ((0xaU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
                    if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__sample) {
                        if ((7U == (7U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__bit_counter)))) {
                            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__cmd_response_type = 0U;
                        }
                    }
                } else if ((0xbU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
                    if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__sample) {
                        if ((7U == (7U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__bit_counter)))) {
                            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__cmd_response_type = 1U;
                        }
                    }
                }
            }
        }
    }
    if (((((((((0U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT)) 
               | (1U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) 
              | (4U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) 
             | (5U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) 
            | (6U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) 
           | (0xeU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) 
          | (2U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) 
         | (3U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT)))) {
        if ((0U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
            if ((1U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__timeout_increment = 1U;
            }
            if ((1U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                if ((4U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                    if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__sample) {
                        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_increment = 1U;
                        if ((0x400U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register))) {
                            if (((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_counter) 
                                 & (0U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_counter)))) {
                                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__rx_valid 
                                    = (1U & (~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__rx_buffer_full)));
                            }
                        } else if ((7U == (7U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_counter)))) {
                            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__rx_valid 
                                = (1U & (~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__rx_buffer_full)));
                        }
                    }
                } else if ((5U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                    if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__sample) {
                        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_increment = 1U;
                    }
                } else if ((6U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                    if ((0xeU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                        if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__shift) {
                            if ((0xfU != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_counter))) {
                                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_increment = 1U;
                            }
                        }
                    }
                }
                if ((4U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                    if ((5U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                        if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__sample) {
                            if ((0x400U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register))) {
                                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__unnamedblk1__DOT__i = 4U;
                            }
                        }
                    }
                    if ((5U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                        if ((6U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                            if ((0xeU != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                                if ((2U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                                    if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__shift) {
                                        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_tx_read = 1U;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    } else {
        if ((7U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
            if ((8U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                if ((9U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                    if ((0xbU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__timeout_increment = 1U;
                    } else if ((0xdU != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                        if ((0xaU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__timeout_increment = 1U;
                        }
                    }
                }
            }
        }
        if ((7U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_increment 
                = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__shift;
            if ((0x400U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register))) {
                if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__shift) {
                    if ((1U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_counter))) {
                        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_tx_read 
                            = (0x3ffU != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_counter));
                    }
                }
            } else if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__shift) {
                if ((7U == (7U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_counter)))) {
                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_tx_read 
                        = (0xfffU != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_counter));
                }
            }
        } else if ((8U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_increment 
                = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__shift;
        } else if ((9U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
            if ((0xbU != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                if ((0xdU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                    if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__sample) {
                        if ((3U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_counter))) {
                            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_increment = 1U;
                        }
                    }
                }
            }
        }
    }
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT____Vcellinp__tx_buffer__read_i 
        = (((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__tx_buffer_empty)) 
            & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_tx_read)) 
           & (3U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__tx_data_selector)));
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_activate_edge 
        = ((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_fsm_activation_detector__DOT__previous_signal)) 
           & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_activate));
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__tristate_enable = 1U;
    if (((((((((0U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT)) 
               | (1U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) 
              | (4U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) 
             | (5U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) 
            | (6U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) 
           | (0xeU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) 
          | (2U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) 
         | (3U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT)))) {
        if ((0U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
            if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_activate) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__tristate_enable 
                    = (1U & (~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_direction)));
            }
        } else if ((1U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__tristate_enable = 0U;
        } else if ((4U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__tristate_enable = 0U;
        } else if ((5U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__tristate_enable = 0U;
        } else if ((6U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
            if ((0xeU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__tristate_enable = 0U;
            } else if ((2U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__tristate_enable = 1U;
            }
        }
    } else if ((7U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__tristate_enable = 1U;
    } else if ((8U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__tristate_enable = 1U;
    } else if ((9U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__tristate_enable = 1U;
    } else if ((0xbU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__tristate_enable = 0U;
    } else if ((0xdU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__tristate_enable = 0U;
    } else if ((0xaU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__tristate_enable = 0U;
    }
}

VL_INLINE_OPT void Vtb_top___024root___nba_sequent__TOP__1(Vtb_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_top___024root___nba_sequent__TOP__1\n"); );
    Vtb_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__spi_cnt 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__spi_cnt;
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__spi_cnt 
        = ((1U & ((~ (IData)(vlSelfRef.rst_n)) | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__spi_cs)))
            ? 0U : (0xffU & ((IData)(1U) + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__spi_cnt))));
}

VL_INLINE_OPT void Vtb_top___024root___nba_sequent__TOP__2(Vtb_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_top___024root___nba_sequent__TOP__2\n"); );
    Vtb_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    SData/*10:0*/ __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__idc;
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__idc = 0;
    SData/*15:0*/ __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__didc;
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__didc = 0;
    // Body
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__didc 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__didc;
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__distate 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__distate;
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_check3 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_check3;
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_check2 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_check2;
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_check1 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_check1;
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_check0 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_check0;
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_wr_sd_data 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_wr_sd_data;
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_in0 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_in0;
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__spi_di_token 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__spi_di_token;
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_wr_sd_addr 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_wr_sd_addr;
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__idc 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__idc;
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__istate 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__istate;
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__cmd_in_latch 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__cmd_in_latch;
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc7_in 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc7_in;
}

VL_INLINE_OPT void Vtb_top___024root___nba_sequent__TOP__3(Vtb_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_top___024root___nba_sequent__TOP__3\n"); );
    Vtb_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__VdlySet__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__isdb2__DOT__mem__v0 = 0U;
    if (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_wr_sd_wren) {
        vlSelfRef.__VdlyVal__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__isdb2__DOT__mem__v0 
            = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_wr_sd_data;
        vlSelfRef.__VdlyDim0__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__isdb2__DOT__mem__v0 
            = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__bram_wr_sd_addr_sel;
        vlSelfRef.__VdlySet__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__isdb2__DOT__mem__v0 = 1U;
        vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_wr_sd_q 
            = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_wr_sd_data;
    } else {
        vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_wr_sd_q 
            = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__isdb2__DOT__mem
            [vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__bram_wr_sd_addr_sel];
    }
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__j__DOT__stage_3 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_in_another_s;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__i__DOT__stage_3 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_in_stop_s;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__c__DOT__stage_3 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__resp_type_s;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__f__DOT__stage_3 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_stop_s;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__h__DOT__stage_3 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__mode_4bit_s;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__l__DOT__stage_3 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__mode_crc_disable_s;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__g__DOT__stage_3 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__card_state_s;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__k__DOT__stage_3 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__mode_spi_s;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__d__DOT__stage_3 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_len_s;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__card_state_s 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__g__DOT__stage_2;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__g__DOT__stage_2 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__g__DOT__stage_1;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__g__DOT__stage_1 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state;
}

VL_INLINE_OPT void Vtb_top___024root___nba_sequent__TOP__4(Vtb_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_top___024root___nba_sequent__TOP__4\n"); );
    Vtb_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    CData/*0:0*/ __Vdly__tb_top__DOT__u_dut__DOT__wbm_ack_i;
    __Vdly__tb_top__DOT__u_dut__DOT__wbm_ack_i = 0;
    IData/*31:0*/ __Vdly__tb_top__DOT__u_dut__DOT__wbm_dat_i;
    __Vdly__tb_top__DOT__u_dut__DOT__wbm_dat_i = 0;
    CData/*0:0*/ __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__ext_read_go;
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__ext_read_go = 0;
    CData/*4:0*/ __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__state;
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__state = 0;
    IData/*31:0*/ __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__ext_read_addr_latch;
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__ext_read_addr_latch = 0;
    SData/*15:0*/ __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__bytes_done;
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__bytes_done = 0;
    IData/*31:0*/ __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__ext_write_addr_latch;
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__ext_write_addr_latch = 0;
    CData/*6:0*/ __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_wr_ext_addr;
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_wr_ext_addr = 0;
    CData/*0:0*/ __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__ext_write_done;
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__ext_write_done = 0;
    CData/*0:0*/ __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__sel_rd_ext;
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__sel_rd_ext = 0;
    CData/*0:0*/ __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__buf_rd_a_full;
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__buf_rd_a_full = 0;
    CData/*0:0*/ __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__buf_rd_b_full;
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__buf_rd_b_full = 0;
    CData/*0:0*/ __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__sel_wr_sd;
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__sel_wr_sd = 0;
    CData/*0:0*/ __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__sel_wr_ext;
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__sel_wr_ext = 0;
    CData/*4:0*/ __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__state;
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__state = 0;
    IData/*31:0*/ __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__ext_read_addr;
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__ext_read_addr = 0;
    IData/*31:0*/ __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__link_read_addr_latch;
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__link_read_addr_latch = 0;
    CData/*7:0*/ __VdlyVal__tb_top__DOT__u_dut__DOT__SD_memory__DOT__mem__v0;
    __VdlyVal__tb_top__DOT__u_dut__DOT__SD_memory__DOT__mem__v0 = 0;
    SData/*9:0*/ __VdlyDim0__tb_top__DOT__u_dut__DOT__SD_memory__DOT__mem__v0;
    __VdlyDim0__tb_top__DOT__u_dut__DOT__SD_memory__DOT__mem__v0 = 0;
    CData/*0:0*/ __VdlySet__tb_top__DOT__u_dut__DOT__SD_memory__DOT__mem__v0;
    __VdlySet__tb_top__DOT__u_dut__DOT__SD_memory__DOT__mem__v0 = 0;
    CData/*7:0*/ __VdlyVal__tb_top__DOT__u_dut__DOT__SD_memory__DOT__mem__v1;
    __VdlyVal__tb_top__DOT__u_dut__DOT__SD_memory__DOT__mem__v1 = 0;
    SData/*9:0*/ __VdlyDim0__tb_top__DOT__u_dut__DOT__SD_memory__DOT__mem__v1;
    __VdlyDim0__tb_top__DOT__u_dut__DOT__SD_memory__DOT__mem__v1 = 0;
    CData/*0:0*/ __VdlySet__tb_top__DOT__u_dut__DOT__SD_memory__DOT__mem__v1;
    __VdlySet__tb_top__DOT__u_dut__DOT__SD_memory__DOT__mem__v1 = 0;
    CData/*7:0*/ __VdlyVal__tb_top__DOT__u_dut__DOT__SD_memory__DOT__mem__v2;
    __VdlyVal__tb_top__DOT__u_dut__DOT__SD_memory__DOT__mem__v2 = 0;
    SData/*9:0*/ __VdlyDim0__tb_top__DOT__u_dut__DOT__SD_memory__DOT__mem__v2;
    __VdlyDim0__tb_top__DOT__u_dut__DOT__SD_memory__DOT__mem__v2 = 0;
    CData/*0:0*/ __VdlySet__tb_top__DOT__u_dut__DOT__SD_memory__DOT__mem__v2;
    __VdlySet__tb_top__DOT__u_dut__DOT__SD_memory__DOT__mem__v2 = 0;
    CData/*7:0*/ __VdlyVal__tb_top__DOT__u_dut__DOT__SD_memory__DOT__mem__v3;
    __VdlyVal__tb_top__DOT__u_dut__DOT__SD_memory__DOT__mem__v3 = 0;
    SData/*9:0*/ __VdlyDim0__tb_top__DOT__u_dut__DOT__SD_memory__DOT__mem__v3;
    __VdlyDim0__tb_top__DOT__u_dut__DOT__SD_memory__DOT__mem__v3 = 0;
    CData/*0:0*/ __VdlySet__tb_top__DOT__u_dut__DOT__SD_memory__DOT__mem__v3;
    __VdlySet__tb_top__DOT__u_dut__DOT__SD_memory__DOT__mem__v3 = 0;
    // Body
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_in_gap_cnt 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_in_gap_cnt;
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_erase_state 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_erase_state;
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_blocks_written 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_blocks_written;
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_appcmd 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_appcmd;
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_rca 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_rca;
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_ocr 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_ocr;
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_cid[0U] 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_cid[0U];
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_cid[1U] 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_cid[1U];
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_cid[2U] 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_cid[2U];
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_cid[3U] 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_cid[3U];
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_csd[0U] 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_csd[0U];
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_csd[1U] 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_csd[1U];
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_csd[2U] 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_csd[2U];
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_csd[3U] 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_csd[3U];
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_scr 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_scr;
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_sd_status[0U] 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_sd_status[0U];
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_sd_status[1U] 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_sd_status[1U];
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_sd_status[2U] 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_sd_status[2U];
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_sd_status[3U] 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_sd_status[3U];
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function_caps[0U] 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function_caps[0U];
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function_caps[1U] 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function_caps[1U];
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function_caps[2U] 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function_caps[2U];
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function_caps[3U] 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function_caps[3U];
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function;
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function_check 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function_check;
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_scr 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_scr;
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_cid 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_cid;
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_csd 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_csd;
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_sdstatus 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_sdstatus;
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_function 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_function;
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_written 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_written;
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_block 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_block;
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_block_queue 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_block_queue;
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_recv_block 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_recv_block;
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_in_act 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_in_act;
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_in_stop 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_in_stop;
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_stop 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_stop;
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_mode_4bit 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_mode_4bit;
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_mode_crc_disable 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_mode_crc_disable;
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__link_read_num 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__link_read_num;
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__link_write_num 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__link_write_num;
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__state 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__state;
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state_next 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state_next;
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_type 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_type;
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__link_write_addr 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__link_write_addr;
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__block_write_byteaddr 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__block_write_byteaddr;
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__link_read_addr 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__link_read_addr;
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__block_read_byteaddr 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__block_read_byteaddr;
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_arg[0U] 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_arg[0U];
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_arg[1U] 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_arg[1U];
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_arg[2U] 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_arg[2U];
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_arg[3U] 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_arg[3U];
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_state 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_state;
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_mode_spi 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_mode_spi;
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_type 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_type;
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_in_another 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_in_another;
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status;
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state;
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__link_write_done 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__link_write_done;
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__sel_rd_sd 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__sel_rd_sd;
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__buf_rd_a_full 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__buf_rd_a_full;
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__buf_rd_b_full 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__buf_rd_b_full;
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__sel_wr_ext 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__sel_wr_ext;
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__link_read_go 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__link_read_go;
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__state 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__state;
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__ext_read_addr 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__ext_read_addr;
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__link_read_addr_latch 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__link_read_addr_latch;
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__sel_rd_ext 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__sel_rd_ext;
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__sel_wr_sd 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__sel_wr_sd;
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__ext_read_go 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__ext_read_go;
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__state 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__state;
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__ext_read_addr_latch 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__ext_read_addr_latch;
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__bytes_done 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__bytes_done;
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__ext_write_addr_latch 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__ext_write_addr_latch;
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_wr_ext_addr 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_wr_ext_addr;
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__ext_write_done 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__ext_write_done;
    vlSelfRef.__VdlySet__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__isdb1__DOT__mem__v0 = 0U;
    __Vdly__tb_top__DOT__u_dut__DOT__wbm_ack_i = vlSelfRef.tb_top__DOT__u_dut__DOT__wbm_ack_i;
    __Vdly__tb_top__DOT__u_dut__DOT__wbm_dat_i = vlSelfRef.tb_top__DOT__u_dut__DOT__wbm_dat_i;
    __VdlySet__tb_top__DOT__u_dut__DOT__SD_memory__DOT__mem__v0 = 0U;
    __VdlySet__tb_top__DOT__u_dut__DOT__SD_memory__DOT__mem__v1 = 0U;
    __VdlySet__tb_top__DOT__u_dut__DOT__SD_memory__DOT__mem__v2 = 0U;
    __VdlySet__tb_top__DOT__u_dut__DOT__SD_memory__DOT__mem__v3 = 0U;
    if (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_rd_ext_wren) {
        vlSelfRef.__VdlyVal__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__isdb1__DOT__mem__v0 
            = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_rd_ext_data;
        vlSelfRef.__VdlyDim0__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__isdb1__DOT__mem__v0 
            = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__bram_rd_ext_addr_sel;
        vlSelfRef.__VdlySet__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__isdb1__DOT__mem__v0 = 1U;
        vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_rd_ext_q 
            = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_rd_ext_data;
    } else {
        vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_rd_ext_q 
            = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__isdb1__DOT__mem
            [vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__bram_rd_ext_addr_sel];
    }
    if (vlSelfRef.rst_n) {
        __Vdly__tb_top__DOT__u_dut__DOT__wbm_ack_i = 0U;
        if ((((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__wbm_cyc_o) 
              & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__wbm_stb_o)) 
             & (~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__wbm_ack_i)))) {
            __Vdly__tb_top__DOT__u_dut__DOT__wbm_ack_i = 1U;
            if (vlSelfRef.tb_top__DOT__u_dut__DOT__wbm_we_o) {
                if ((1U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__wbm_sel_o))) {
                    __VdlyVal__tb_top__DOT__u_dut__DOT__SD_memory__DOT__mem__v0 
                        = (0xffU & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_wr_ext_q);
                    __VdlyDim0__tb_top__DOT__u_dut__DOT__SD_memory__DOT__mem__v0 
                        = (0x3ffU & (vlSelfRef.tb_top__DOT__u_dut__DOT__wbm_adr_o 
                                     >> 2U));
                    __VdlySet__tb_top__DOT__u_dut__DOT__SD_memory__DOT__mem__v0 = 1U;
                }
                if ((2U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__wbm_sel_o))) {
                    __VdlyVal__tb_top__DOT__u_dut__DOT__SD_memory__DOT__mem__v1 
                        = (0xffU & (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_wr_ext_q 
                                    >> 8U));
                    __VdlyDim0__tb_top__DOT__u_dut__DOT__SD_memory__DOT__mem__v1 
                        = (0x3ffU & (vlSelfRef.tb_top__DOT__u_dut__DOT__wbm_adr_o 
                                     >> 2U));
                    __VdlySet__tb_top__DOT__u_dut__DOT__SD_memory__DOT__mem__v1 = 1U;
                }
                if ((4U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__wbm_sel_o))) {
                    __VdlyVal__tb_top__DOT__u_dut__DOT__SD_memory__DOT__mem__v2 
                        = (0xffU & (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_wr_ext_q 
                                    >> 0x10U));
                    __VdlyDim0__tb_top__DOT__u_dut__DOT__SD_memory__DOT__mem__v2 
                        = (0x3ffU & (vlSelfRef.tb_top__DOT__u_dut__DOT__wbm_adr_o 
                                     >> 2U));
                    __VdlySet__tb_top__DOT__u_dut__DOT__SD_memory__DOT__mem__v2 = 1U;
                }
                if ((8U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__wbm_sel_o))) {
                    __VdlyVal__tb_top__DOT__u_dut__DOT__SD_memory__DOT__mem__v3 
                        = (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_wr_ext_q 
                           >> 0x18U);
                    __VdlyDim0__tb_top__DOT__u_dut__DOT__SD_memory__DOT__mem__v3 
                        = (0x3ffU & (vlSelfRef.tb_top__DOT__u_dut__DOT__wbm_adr_o 
                                     >> 2U));
                    __VdlySet__tb_top__DOT__u_dut__DOT__SD_memory__DOT__mem__v3 = 1U;
                }
            } else {
                __Vdly__tb_top__DOT__u_dut__DOT__wbm_dat_i 
                    = vlSelfRef.tb_top__DOT__u_dut__DOT__SD_memory__DOT__mem
                    [(0x3ffU & (vlSelfRef.tb_top__DOT__u_dut__DOT__wbm_adr_o 
                                >> 2U))];
            }
        }
    } else {
        __Vdly__tb_top__DOT__u_dut__DOT__wbm_ack_i = 0U;
    }
    vlSelfRef.tb_top__DOT__u_dut__DOT__wbm_cti_o = 0U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__wbm_bte_o = 0U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__c__DOT__stage_3 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__ext_write_done_s;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__i__DOT__stage_3 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_out_busy_s;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__b__DOT__stage_3 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__ext_read_go_s;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__a__DOT__stage_3 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__reset_s;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__a__DOT__stage_3 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__reset_s;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__a__DOT__stage_3 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__reset_s;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__k__DOT__stage_3 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__spi_sel_s;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__e__DOT__stage_3 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_in_busy_s;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__g__DOT__stage_3 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_in_crc_good_s;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__b__DOT__stage_3 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_s;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__c__DOT__stage_3 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_crc_good_s;
    if (__VdlySet__tb_top__DOT__u_dut__DOT__SD_memory__DOT__mem__v0) {
        vlSelfRef.tb_top__DOT__u_dut__DOT__SD_memory__DOT__mem[__VdlyDim0__tb_top__DOT__u_dut__DOT__SD_memory__DOT__mem__v0] 
            = ((0xffffff00U & vlSelfRef.tb_top__DOT__u_dut__DOT__SD_memory__DOT__mem
                [__VdlyDim0__tb_top__DOT__u_dut__DOT__SD_memory__DOT__mem__v0]) 
               | (IData)(__VdlyVal__tb_top__DOT__u_dut__DOT__SD_memory__DOT__mem__v0));
    }
    if (__VdlySet__tb_top__DOT__u_dut__DOT__SD_memory__DOT__mem__v1) {
        vlSelfRef.tb_top__DOT__u_dut__DOT__SD_memory__DOT__mem[__VdlyDim0__tb_top__DOT__u_dut__DOT__SD_memory__DOT__mem__v1] 
            = ((0xffff00ffU & vlSelfRef.tb_top__DOT__u_dut__DOT__SD_memory__DOT__mem
                [__VdlyDim0__tb_top__DOT__u_dut__DOT__SD_memory__DOT__mem__v1]) 
               | ((IData)(__VdlyVal__tb_top__DOT__u_dut__DOT__SD_memory__DOT__mem__v1) 
                  << 8U));
    }
    if (__VdlySet__tb_top__DOT__u_dut__DOT__SD_memory__DOT__mem__v2) {
        vlSelfRef.tb_top__DOT__u_dut__DOT__SD_memory__DOT__mem[__VdlyDim0__tb_top__DOT__u_dut__DOT__SD_memory__DOT__mem__v2] 
            = ((0xff00ffffU & vlSelfRef.tb_top__DOT__u_dut__DOT__SD_memory__DOT__mem
                [__VdlyDim0__tb_top__DOT__u_dut__DOT__SD_memory__DOT__mem__v2]) 
               | ((IData)(__VdlyVal__tb_top__DOT__u_dut__DOT__SD_memory__DOT__mem__v2) 
                  << 0x10U));
    }
    if (__VdlySet__tb_top__DOT__u_dut__DOT__SD_memory__DOT__mem__v3) {
        vlSelfRef.tb_top__DOT__u_dut__DOT__SD_memory__DOT__mem[__VdlyDim0__tb_top__DOT__u_dut__DOT__SD_memory__DOT__mem__v3] 
            = ((0xffffffU & vlSelfRef.tb_top__DOT__u_dut__DOT__SD_memory__DOT__mem
                [__VdlyDim0__tb_top__DOT__u_dut__DOT__SD_memory__DOT__mem__v3]) 
               | ((IData)(__VdlyVal__tb_top__DOT__u_dut__DOT__SD_memory__DOT__mem__v3) 
                  << 0x18U));
    }
    vlSelfRef.tb_top__DOT__u_dut__DOT__wbm_sel_o = 0xfU;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_wr_ext_q 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__isdb2__DOT__mem
        [(0xffU & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__sel_wr_ext)
                    ? ((IData)(0x80U) + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_wr_ext_addr))
                    : (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_wr_ext_addr)))];
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__ext_write_done_s 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__c__DOT__stage_2;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__ext_read_go_s 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__b__DOT__stage_2;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_s 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__b__DOT__stage_2;
    vlSelfRef.tb_top__DOT__u_dut__DOT__wbm_cyc_o = 0U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__wbm_stb_o = 0U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__wbm_we_o = 0U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_rd_ext_wren = 0U;
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__ext_read_go = 0U;
    if ((0U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__state))) {
        __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__state = 4U;
    } else if ((4U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__state))) {
        if (((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__ext_read_act_last)) 
             & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__ext_read_act))) {
            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__ext_read_addr_latch 
                = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__ext_read_addr;
            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__bytes_done = 0U;
            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_rd_ext_addr = 0x7fU;
            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__state = 8U;
        }
        if (((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__ext_write_act_last)) 
             & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__ext_write_act))) {
            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__ext_write_addr_latch 
                = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__ext_write_addr;
            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__bytes_done = 0U;
            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_wr_ext_addr = 0U;
            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__ext_write_done = 0U;
            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__state = 0xdU;
        }
    } else if ((8U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__state))) {
        vlSelfRef.tb_top__DOT__u_dut__DOT__wbm_cyc_o = 1U;
        if ((1U & (~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__wbm_ack_i)))) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__wbm_adr_o 
                = (VL_SHIFTL_III(32,32,32, vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__ext_read_addr_latch, 9U) 
                   + VL_SHIFTL_III(32,32,32, (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__bram_rd_ext_addr_next), 2U));
            vlSelfRef.tb_top__DOT__u_dut__DOT__wbm_stb_o = 1U;
        }
        if (((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__wbm_ack_i) 
             & (~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__wbm_ack_i_last)))) {
            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__bytes_done 
                = (0xffffU & ((IData)(4U) + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__bytes_done)));
            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_rd_ext_addr 
                = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__bram_rd_ext_addr_next;
            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_rd_ext_data 
                = vlSelfRef.tb_top__DOT__u_dut__DOT__wbm_dat_i;
            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_rd_ext_wren = 1U;
            vlSelfRef.tb_top__DOT__u_dut__DOT__wbm_stb_o = 0U;
            if ((0x200U == ((IData)(4U) + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__bytes_done)))) {
                __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__state = 9U;
            }
        }
    } else if ((9U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__state))) {
        __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__ext_read_go = 1U;
        if (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__ext_read_stop) {
            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__state = 4U;
        }
    } else if ((0xdU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__state))) {
        vlSelfRef.tb_top__DOT__u_dut__DOT__wbm_cyc_o = 1U;
        vlSelfRef.tb_top__DOT__u_dut__DOT__wbm_we_o = 1U;
        if ((1U & (~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__wbm_ack_i)))) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__wbm_adr_o 
                = (VL_SHIFTL_III(32,32,32, vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__ext_write_addr_latch, 9U) 
                   + VL_SHIFTL_III(32,32,32, (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_wr_ext_addr), 2U));
            vlSelfRef.tb_top__DOT__u_dut__DOT__wbm_stb_o = 1U;
        }
        if (((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__wbm_ack_i) 
             & (~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__wbm_ack_i_last)))) {
            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_wr_ext_addr 
                = (0x7fU & ((IData)(1U) + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_wr_ext_addr)));
            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__bytes_done 
                = (0xffffU & ((IData)(4U) + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__bytes_done)));
            vlSelfRef.tb_top__DOT__u_dut__DOT__wbm_stb_o = 0U;
            if ((0x200U == ((IData)(4U) + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__bytes_done)))) {
                __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__state = 0xeU;
            }
        }
    } else if ((0xeU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__state))) {
        __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__ext_write_done = 1U;
        __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__state = 4U;
    }
    if ((1U & (~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__reset_s)))) {
        __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__state = 0U;
    }
    vlSelfRef.tb_top__DOT__u_dut__DOT__wbm_dat_i = __Vdly__tb_top__DOT__u_dut__DOT__wbm_dat_i;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__state 
        = __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__state;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__ext_read_addr_latch 
        = __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__ext_read_addr_latch;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__bytes_done 
        = __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__bytes_done;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__ext_write_addr_latch 
        = __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__ext_write_addr_latch;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_wr_ext_addr 
        = __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_wr_ext_addr;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__c__DOT__stage_2 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__c__DOT__stage_1;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__b__DOT__stage_2 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__b__DOT__stage_1;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__b__DOT__stage_2 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__b__DOT__stage_1;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__bram_rd_ext_addr_next 
        = (0x7fU & ((IData)(1U) + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_rd_ext_addr)));
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__wbm_ack_i_last 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__wbm_ack_i;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__reset_s 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__a__DOT__stage_2;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__ext_read_act_last 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__ext_read_act;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__ext_write_act_last 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__ext_write_act;
    vlSelfRef.tb_top__DOT__u_dut__DOT__wbm_ack_i = __Vdly__tb_top__DOT__u_dut__DOT__wbm_ack_i;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__c__DOT__stage_1 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__ext_write_done;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__b__DOT__stage_1 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__ext_read_go;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__b__DOT__stage_1 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_cmd_in;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__a__DOT__stage_2 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__a__DOT__stage_1;
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__link_write_done = 0U;
    if (((((((((0U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__state)) 
               | (4U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__state))) 
              | (5U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__state))) 
             | (6U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__state))) 
            | (7U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__state))) 
           | (8U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__state))) 
          | (9U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__state))) 
         | (0xaU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__state)))) {
        if ((0U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__state))) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__ext_read_act = 0U;
            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__ext_read_stop = 0U;
            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__ext_write_act = 0U;
            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__sel_rd_sd = 0U;
            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__sel_rd_ext = 0U;
            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__buf_rd_a_full = 0U;
            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__buf_rd_b_full = 0U;
            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__sel_wr_sd = 0U;
            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__sel_wr_ext = 0U;
            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__link_read_go = 0U;
            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__state = 4U;
        } else if ((4U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__state))) {
            if (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__link_read_act) {
                __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__ext_read_addr 
                    = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__link_read_addr;
                __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__link_read_addr_latch 
                    = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__link_read_addr;
                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__ext_read_stop = 0U;
                __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__state = 5U;
            } else if (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__link_write_act) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__ext_write_addr 
                    = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__link_write_addr;
                __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__state = 0xdU;
            }
        } else if ((5U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__state))) {
            if ((((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__sel_rd_sd)
                   ? (((QData)((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__buf_rd_b_block)) 
                       << 1U) | (QData)((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__buf_rd_b_full)))
                   : (((QData)((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__buf_rd_a_block)) 
                       << 1U) | (QData)((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__buf_rd_a_full)))) 
                 == (1ULL | ((QData)((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__ext_read_addr)) 
                             << 1U)))) {
                __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__sel_rd_ext 
                    = (1U & (~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__sel_rd_ext)));
                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__link_read_go = 1U;
                __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__state = 8U;
            } else {
                __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__state = 6U;
            }
        } else if ((6U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__state))) {
            if (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__sel_rd_ext) {
                if (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__sel_rd_ext) {
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__buf_rd_b_block 
                        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__link_read_addr_latch;
                }
            } else {
                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__buf_rd_a_block 
                    = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__link_read_addr_latch;
            }
            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__ext_read_act = 1U;
            if (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__ext_read_go_r) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__ext_read_act = 0U;
                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__ext_read_stop = 1U;
                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__link_read_go = 1U;
                if (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__sel_rd_ext) {
                    if (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__sel_rd_ext) {
                        __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__buf_rd_b_full = 1U;
                    }
                } else {
                    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__buf_rd_a_full = 1U;
                }
                __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__state = 7U;
                __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__sel_rd_ext 
                    = (1U & (~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__sel_rd_ext)));
            }
        } else if ((7U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__state))) {
            if ((1U & (~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__ext_read_go)))) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__ext_read_stop = 0U;
                __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__state = 8U;
            }
        } else if ((8U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__state))) {
            if (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__sel_rd_ext) {
                if (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__sel_rd_ext) {
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__buf_rd_b_block 
                        = ((IData)(1U) + vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__link_read_addr_latch);
                }
            } else {
                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__buf_rd_a_block 
                    = ((IData)(1U) + vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__link_read_addr_latch);
            }
            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__ext_read_addr 
                = ((IData)(1U) + vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__link_read_addr_latch);
            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__ext_read_act = 1U;
            if (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__ext_read_go_r) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__ext_read_act = 0U;
                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__ext_read_stop = 1U;
                if (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__sel_rd_ext) {
                    if (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__sel_rd_ext) {
                        __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__buf_rd_b_full = 1U;
                    }
                } else {
                    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__buf_rd_a_full = 1U;
                }
                __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__state = 9U;
            }
        } else if ((9U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__state))) {
            if ((1U & (~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__ext_read_go)))) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__ext_read_stop = 0U;
                __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__state = 0xaU;
            }
        } else if (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__link_read_stop) {
            if (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__sel_rd_sd) {
                if (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__sel_rd_sd) {
                    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__buf_rd_b_full = 0U;
                }
            } else {
                __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__buf_rd_a_full = 0U;
            }
            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__link_read_go = 0U;
            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__state = 4U;
            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__sel_rd_sd 
                = (1U & (~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__sel_rd_sd)));
        }
    } else if ((0xdU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__state))) {
        vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__ext_write_act = 1U;
        if ((1U & (~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__ext_write_done)))) {
            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__sel_wr_sd 
                = (1U & (~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__sel_wr_sd)));
            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__state = 0xeU;
        }
    } else if ((0xeU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__state))) {
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__link_write_done = 1U;
        if (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__link_write_act) {
            if (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__ext_write_done) {
                __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__sel_wr_ext 
                    = (1U & (~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__sel_wr_ext)));
                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__ext_write_act = 0U;
                __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__state = 0x10U;
            }
        } else {
            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__state = 0xfU;
        }
    } else if ((0xfU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__state))) {
        if (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__ext_write_done) {
            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__sel_wr_ext 
                = (1U & (~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__sel_wr_ext)));
            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__ext_write_act = 0U;
            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__state = 0x10U;
        }
    } else if ((0x10U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__state))) {
        __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__buf_rd_a_full = 0U;
        __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__buf_rd_b_full = 0U;
        __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__state = 4U;
    }
    if ((1U & (~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__reset_s)))) {
        __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__state = 0U;
    }
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__buf_rd_a_full 
        = __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__buf_rd_a_full;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__buf_rd_b_full 
        = __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__buf_rd_b_full;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__state 
        = __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__state;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__link_read_addr_latch 
        = __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__link_read_addr_latch;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__ext_read_addr 
        = __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__ext_read_addr;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__ext_read_go_r 
        = ((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__b__DOT__stage_3)) 
           & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__ext_read_go_s));
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__ext_read_go 
        = __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__ext_read_go;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__ext_write_done 
        = __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__ext_write_done;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__sel_wr_ext 
        = __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__sel_wr_ext;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__sel_rd_ext 
        = __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__sel_rd_ext;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__sel_wr_sd 
        = __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__sel_wr_sd;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__a__DOT__stage_1 
        = vlSelfRef.rst_n;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__bram_rd_ext_addr_sel 
        = (0xffU & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__sel_rd_ext)
                     ? ((IData)(0x80U) + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_rd_ext_addr))
                     : (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_rd_ext_addr)));
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__reset_s 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__a__DOT__stage_2;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__a__DOT__stage_2 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__a__DOT__stage_1;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__a__DOT__stage_1 
        = vlSelfRef.rst_n;
}

extern const VlWide<16>/*511:0*/ Vtb_top__ConstPool__CONST_h0b2d9f06_0;
extern const VlWide<16>/*511:0*/ Vtb_top__ConstPool__CONST_h9fe1b771_0;
extern const VlWide<16>/*511:0*/ Vtb_top__ConstPool__CONST_h9b2d9f06_0;

VL_INLINE_OPT void Vtb_top___024root___nba_sequent__TOP__5(Vtb_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_top___024root___nba_sequent__TOP__5\n"); );
    Vtb_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    SData/*15:0*/ __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dodc;
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dodc = 0;
    CData/*6:0*/ __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dostate;
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dostate = 0;
    SData/*15:0*/ __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_out3;
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_out3 = 0;
    SData/*15:0*/ __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_out2;
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_out2 = 0;
    SData/*15:0*/ __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_out1;
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_out1 = 0;
    SData/*15:0*/ __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_out0;
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_out0 = 0;
    VlWide<17>/*512:0*/ __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch;
    VL_ZERO_W(513, __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch);
    IData/*31:0*/ __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dout_buf;
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dout_buf = 0;
    SData/*10:0*/ __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__odc;
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__odc = 0;
    CData/*6:0*/ __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__ostate;
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__ostate = 0;
    CData/*6:0*/ __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc7_out;
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc7_out = 0;
    VlWide<5>/*159:0*/ __Vtemp_1;
    // Body
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__odc 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__odc;
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__ostate 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__ostate;
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_done 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_done;
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc7_out 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc7_out;
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_cmd_oe 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_cmd_oe;
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dodc 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dodc;
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dostate 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dostate;
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_out3 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_out3;
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_out2 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_out2;
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_out1 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_out1;
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_out0 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_out0;
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_busy 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_busy;
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_done 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_done;
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_rd_sd_addr 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_rd_sd_addr;
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0U] 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0U];
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[1U] 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[1U];
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[2U] 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[2U];
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[3U] 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[3U];
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[4U] 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[4U];
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[5U] 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[5U];
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[6U] 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[6U];
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[7U] 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[7U];
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[8U] 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[8U];
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[9U] 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[9U];
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xaU] 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xaU];
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xbU] 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xbU];
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xcU] 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xcU];
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xdU] 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xdU];
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xeU] 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xeU];
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xfU] 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xfU];
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0x10U] 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0x10U];
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dout_buf 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dout_buf;
    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_oe 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_oe;
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__odc 
        = (0x7ffU & ((IData)(1U) + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__odc)));
    if ((0x40U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__ostate))) {
        __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__ostate = 0U;
    } else if ((0x20U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__ostate))) {
        __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__ostate = 0U;
    } else if ((0x10U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__ostate))) {
        if ((8U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__ostate))) {
            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__ostate = 0U;
        } else if ((4U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__ostate))) {
            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__ostate = 0U;
        } else if ((2U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__ostate))) {
            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__ostate = 0U;
        } else if ((1U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__ostate))) {
            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__ostate = 0U;
        } else {
            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_cmd_oe = 0U;
            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_done = 1U;
            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__ostate = 4U;
        }
    } else if ((8U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__ostate))) {
        if ((4U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__ostate))) {
            if ((2U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__ostate))) {
                if ((1U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__ostate))) {
                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_cmd_oe = 1U;
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_cmd_out 
                        = (1U & (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__resp_out_latch[4U] 
                                 >> 7U));
                    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc7_out 
                        = (((0x70U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc7_out) 
                                      << 1U)) | (8U 
                                                 & ((VL_REDXOR_8(
                                                                 (0x44U 
                                                                  & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc7_out))) 
                                                     ^ 
                                                     (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__resp_out_latch[4U] 
                                                      >> 7U)) 
                                                    << 3U))) 
                           | ((6U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc7_out) 
                                     << 1U)) | (IData)(
                                                       ((vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__resp_out_latch[4U] 
                                                         >> 7U) 
                                                        ^ 
                                                        ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc7_out) 
                                                         >> 6U)))));
                    __Vtemp_1[1U] = ((vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__resp_out_latch[0U] 
                                      >> 0x1fU) | (
                                                   vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__resp_out_latch[1U] 
                                                   << 1U));
                    __Vtemp_1[2U] = ((vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__resp_out_latch[1U] 
                                      >> 0x1fU) | (
                                                   vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__resp_out_latch[2U] 
                                                   << 1U));
                    __Vtemp_1[3U] = ((vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__resp_out_latch[2U] 
                                      >> 0x1fU) | (
                                                   vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__resp_out_latch[3U] 
                                                   << 1U));
                    __Vtemp_1[4U] = ((vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__resp_out_latch[3U] 
                                      >> 0x1fU) | (0xfeU 
                                                   & (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__resp_out_latch[4U] 
                                                      << 1U)));
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__resp_out_latch[0U] 
                        = (1U | (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__resp_out_latch[0U] 
                                 << 1U));
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__resp_out_latch[1U] 
                        = __Vtemp_1[1U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__resp_out_latch[2U] 
                        = __Vtemp_1[2U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__resp_out_latch[3U] 
                        = __Vtemp_1[3U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__resp_out_latch[4U] 
                        = __Vtemp_1[4U];
                    if (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__mode_spi_s) {
                        if (((1U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__resp_type_s)) 
                             || (0xeU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__resp_type_s)))) {
                            if ((7U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__odc))) {
                                __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__ostate = 0x10U;
                            }
                        } else if ((2U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__resp_type_s))) {
                            if ((0xfU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__odc))) {
                                __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__ostate = 0x10U;
                            }
                        } else if (((3U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__resp_type_s)) 
                                    || (7U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__resp_type_s)))) {
                            if ((0x27U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__odc))) {
                                __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__ostate = 0x10U;
                            }
                        }
                    } else if ((((((1U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__resp_type_s)) 
                                   || (8U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__resp_type_s))) 
                                  || (3U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__resp_type_s))) 
                                 || (6U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__resp_type_s))) 
                                || (7U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__resp_type_s)))) {
                        if ((3U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__resp_type_s))) {
                            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc7_out = 0x7fU;
                        }
                        if ((0x28U <= (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__odc))) {
                            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc7_out 
                                = (1U | (0x7eU & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc7_out) 
                                                  << 1U)));
                            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_cmd_out 
                                = (1U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc7_out) 
                                         >> 6U));
                        }
                        if ((0x2fU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__odc))) {
                            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__ostate = 0x10U;
                        }
                    } else if ((2U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__resp_type_s))) {
                        if ((8U > (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__odc))) {
                            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc7_out = 0U;
                        }
                        if ((0x80U <= (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__odc))) {
                            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc7_out 
                                = (1U | (0x7eU & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc7_out) 
                                                  << 1U)));
                            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_cmd_out 
                                = (1U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc7_out) 
                                         >> 6U));
                        }
                        if ((0x87U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__odc))) {
                            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__ostate = 0x10U;
                        }
                    }
                } else {
                    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__odc = 0U;
                    if ((1U & ((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__mode_spi_s)) 
                               | (7U == (7U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__spi_cnt)))))) {
                        __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__ostate = 0xfU;
                    }
                }
            } else {
                __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__ostate = 0U;
            }
        } else {
            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__ostate = 0U;
        }
    } else if ((4U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__ostate))) {
        if ((2U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__ostate))) {
            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__ostate = 0U;
        } else if ((1U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__ostate))) {
            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__ostate = 0U;
        } else if (((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__b__DOT__stage_3)) 
                    & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__resp_act_s))) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__resp_out_latch[0U] 
                = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_out[0U];
            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__resp_out_latch[1U] 
                = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_out[1U];
            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__resp_out_latch[2U] 
                = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_out[2U];
            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__resp_out_latch[3U] 
                = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_out[3U];
            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__resp_out_latch[4U] 
                = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_out[4U];
            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc7_out = 0U;
            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_done = 0U;
            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__ostate = 0xeU;
        }
    } else if ((2U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__ostate))) {
        __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__ostate = 0U;
    } else if ((1U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__ostate))) {
        __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__ostate = 0U;
    } else {
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_done = 0U;
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_cmd_oe = 0U;
        __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__ostate = 4U;
    }
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dodc 
        = (0xffffU & ((IData)(1U) + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dodc)));
    if ((0x40U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dostate))) {
        __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dostate = 0U;
    } else if ((0x20U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dostate))) {
        if ((0x10U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dostate))) {
            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dostate = 0U;
        } else if ((8U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dostate))) {
            if ((4U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dostate))) {
                __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dostate = 0U;
            } else if ((2U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dostate))) {
                __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dostate = 0U;
            } else if ((1U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dostate))) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_out 
                    = ((0xeU & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_out)) 
                       | (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xfU] 
                          >> 0x1fU));
                __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_out3 = 0U;
                __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_out2 = 0U;
                __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_out1 = 0U;
                __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_out0 = 0U;
                __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0U] 
                    = ((Vtb_top__ConstPool__CONST_h0b2d9f06_0[0U] 
                        & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0U]) 
                       << 1U);
                __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[1U] 
                    = (((Vtb_top__ConstPool__CONST_h0b2d9f06_0[0U] 
                         & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0U]) 
                        >> 0x1fU) | ((Vtb_top__ConstPool__CONST_h0b2d9f06_0[1U] 
                                      & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[1U]) 
                                     << 1U));
                __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[2U] 
                    = (((Vtb_top__ConstPool__CONST_h0b2d9f06_0[1U] 
                         & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[1U]) 
                        >> 0x1fU) | ((Vtb_top__ConstPool__CONST_h0b2d9f06_0[2U] 
                                      & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[2U]) 
                                     << 1U));
                __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[3U] 
                    = (((Vtb_top__ConstPool__CONST_h0b2d9f06_0[2U] 
                         & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[2U]) 
                        >> 0x1fU) | ((Vtb_top__ConstPool__CONST_h0b2d9f06_0[3U] 
                                      & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[3U]) 
                                     << 1U));
                __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[4U] 
                    = (((Vtb_top__ConstPool__CONST_h0b2d9f06_0[3U] 
                         & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[3U]) 
                        >> 0x1fU) | ((Vtb_top__ConstPool__CONST_h0b2d9f06_0[4U] 
                                      & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[4U]) 
                                     << 1U));
                __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[5U] 
                    = (((Vtb_top__ConstPool__CONST_h0b2d9f06_0[4U] 
                         & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[4U]) 
                        >> 0x1fU) | ((Vtb_top__ConstPool__CONST_h0b2d9f06_0[5U] 
                                      & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[5U]) 
                                     << 1U));
                __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[6U] 
                    = (((Vtb_top__ConstPool__CONST_h0b2d9f06_0[5U] 
                         & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[5U]) 
                        >> 0x1fU) | ((Vtb_top__ConstPool__CONST_h0b2d9f06_0[6U] 
                                      & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[6U]) 
                                     << 1U));
                __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[7U] 
                    = (((Vtb_top__ConstPool__CONST_h0b2d9f06_0[6U] 
                         & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[6U]) 
                        >> 0x1fU) | ((Vtb_top__ConstPool__CONST_h0b2d9f06_0[7U] 
                                      & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[7U]) 
                                     << 1U));
                __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[8U] 
                    = (((Vtb_top__ConstPool__CONST_h0b2d9f06_0[7U] 
                         & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[7U]) 
                        >> 0x1fU) | ((Vtb_top__ConstPool__CONST_h0b2d9f06_0[8U] 
                                      & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[8U]) 
                                     << 1U));
                __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[9U] 
                    = (((Vtb_top__ConstPool__CONST_h0b2d9f06_0[8U] 
                         & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[8U]) 
                        >> 0x1fU) | ((Vtb_top__ConstPool__CONST_h0b2d9f06_0[9U] 
                                      & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[9U]) 
                                     << 1U));
                __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xaU] 
                    = (((Vtb_top__ConstPool__CONST_h0b2d9f06_0[9U] 
                         & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[9U]) 
                        >> 0x1fU) | ((Vtb_top__ConstPool__CONST_h0b2d9f06_0[0xaU] 
                                      & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xaU]) 
                                     << 1U));
                __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xbU] 
                    = (((Vtb_top__ConstPool__CONST_h0b2d9f06_0[0xaU] 
                         & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xaU]) 
                        >> 0x1fU) | ((Vtb_top__ConstPool__CONST_h0b2d9f06_0[0xbU] 
                                      & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xbU]) 
                                     << 1U));
                __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xcU] 
                    = (((Vtb_top__ConstPool__CONST_h0b2d9f06_0[0xbU] 
                         & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xbU]) 
                        >> 0x1fU) | ((Vtb_top__ConstPool__CONST_h0b2d9f06_0[0xcU] 
                                      & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xcU]) 
                                     << 1U));
                __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xdU] 
                    = (((Vtb_top__ConstPool__CONST_h0b2d9f06_0[0xcU] 
                         & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xcU]) 
                        >> 0x1fU) | ((Vtb_top__ConstPool__CONST_h0b2d9f06_0[0xdU] 
                                      & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xdU]) 
                                     << 1U));
                __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xeU] 
                    = (((Vtb_top__ConstPool__CONST_h0b2d9f06_0[0xdU] 
                         & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xdU]) 
                        >> 0x1fU) | ((Vtb_top__ConstPool__CONST_h0b2d9f06_0[0xeU] 
                                      & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xeU]) 
                                     << 1U));
                __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xfU] 
                    = (((Vtb_top__ConstPool__CONST_h0b2d9f06_0[0xeU] 
                         & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xeU]) 
                        >> 0x1fU) | ((Vtb_top__ConstPool__CONST_h0b2d9f06_0[0xfU] 
                                      & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xfU]) 
                                     << 1U));
                if ((6U == (0x1fU & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dodc)))) {
                    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dodc = 0U;
                    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dostate = 0x1fU;
                    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dout_buf 
                        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_rd_sd_q;
                    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0U] 
                        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[0U];
                    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[1U] 
                        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[1U];
                    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[2U] 
                        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[2U];
                    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[3U] 
                        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[3U];
                    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[4U] 
                        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[4U];
                    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[5U] 
                        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[5U];
                    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[6U] 
                        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[6U];
                    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[7U] 
                        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[7U];
                    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[8U] 
                        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[8U];
                    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[9U] 
                        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[9U];
                    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xaU] 
                        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[0xaU];
                    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xbU] 
                        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[0xbU];
                    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xcU] 
                        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[0xcU];
                    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xdU] 
                        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[0xdU];
                    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xeU] 
                        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[0xeU];
                    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xfU] 
                        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[0xfU];
                    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0x10U] = 0U;
                }
            } else if ((0U == (7U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__spi_cnt)))) {
                __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0U] 
                    = Vtb_top__ConstPool__CONST_h9fe1b771_0[0U];
                __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[1U] 
                    = Vtb_top__ConstPool__CONST_h9fe1b771_0[1U];
                __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[2U] 
                    = Vtb_top__ConstPool__CONST_h9fe1b771_0[2U];
                __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[3U] 
                    = Vtb_top__ConstPool__CONST_h9fe1b771_0[3U];
                __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[4U] 
                    = Vtb_top__ConstPool__CONST_h9fe1b771_0[4U];
                __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[5U] 
                    = Vtb_top__ConstPool__CONST_h9fe1b771_0[5U];
                __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[6U] 
                    = Vtb_top__ConstPool__CONST_h9fe1b771_0[6U];
                __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[7U] 
                    = Vtb_top__ConstPool__CONST_h9fe1b771_0[7U];
                __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[8U] 
                    = Vtb_top__ConstPool__CONST_h9fe1b771_0[8U];
                __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[9U] 
                    = Vtb_top__ConstPool__CONST_h9fe1b771_0[9U];
                __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xaU] 
                    = Vtb_top__ConstPool__CONST_h9fe1b771_0[0xaU];
                __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xbU] 
                    = Vtb_top__ConstPool__CONST_h9fe1b771_0[0xbU];
                __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xcU] 
                    = Vtb_top__ConstPool__CONST_h9fe1b771_0[0xcU];
                __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xdU] 
                    = Vtb_top__ConstPool__CONST_h9fe1b771_0[0xdU];
                __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xeU] 
                    = Vtb_top__ConstPool__CONST_h9fe1b771_0[0xeU];
                __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xfU] 
                    = Vtb_top__ConstPool__CONST_h9fe1b771_0[0xfU];
                __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dodc = 0U;
                __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dostate = 0x29U;
                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_oe = 1U;
                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_out 
                    = (1U | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_out));
            }
        } else if ((4U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dostate))) {
            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dostate = 0U;
        } else if ((2U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dostate))) {
            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dostate = 0U;
        } else if ((1U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dostate))) {
            if ((1U & (~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_act_s)))) {
                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_busy = 0U;
                __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dostate = 4U;
            }
        } else {
            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_out 
                = ((1U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_out)) 
                   | ((8U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_out3) 
                             >> 0xcU)) | ((4U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_out2) 
                                                 >> 0xdU)) 
                                          | (2U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_out1) 
                                                   >> 0xeU)))));
            if (((0x10U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dodc)) 
                 | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_stop_s))) {
                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_done = 1U;
                __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dostate = 0x21U;
            }
            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_out 
                = ((0xeU & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_out)) 
                   | (1U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_out0) 
                            >> 0xfU)));
            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_out3 
                = (1U | (0xfffeU & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_out3) 
                                    << 1U)));
            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_out2 
                = (1U | (0xfffeU & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_out2) 
                                    << 1U)));
            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_out1 
                = (1U | (0xfffeU & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_out1) 
                                    << 1U)));
            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_out0 
                = (1U | (0xfffeU & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_out0) 
                                    << 1U)));
        }
    } else if ((0x10U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dostate))) {
        if ((8U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dostate))) {
            if ((4U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dostate))) {
                if ((2U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dostate))) {
                    if ((1U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dostate))) {
                        if (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__mode_4bit_s) {
                            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_out3 
                                = ((0xfffeU & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_out3) 
                                               << 1U)) 
                                   ^ ((IData)((((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dout_4bit) 
                                                >> 3U) 
                                               ^ ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_out3) 
                                                  >> 0xfU)))
                                       ? 0x1021U : 0U));
                            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_out2 
                                = ((0xfffeU & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_out2) 
                                               << 1U)) 
                                   ^ ((1U & (((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dout_4bit) 
                                              >> 2U) 
                                             ^ ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_out2) 
                                                >> 0xfU)))
                                       ? 0x1021U : 0U));
                            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_out1 
                                = ((0xfffeU & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_out1) 
                                               << 1U)) 
                                   ^ ((1U & (((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dout_4bit) 
                                              >> 1U) 
                                             ^ ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_out1) 
                                                >> 0xfU)))
                                       ? 0x1021U : 0U));
                            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_out0 
                                = ((0xfffeU & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_out0) 
                                               << 1U)) 
                                   ^ ((1U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dout_4bit) 
                                             ^ ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_out0) 
                                                >> 0xfU)))
                                       ? 0x1021U : 0U));
                            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dout_buf 
                                = (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dout_buf 
                                   << 4U);
                            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0U] 
                                = ((Vtb_top__ConstPool__CONST_h9b2d9f06_0[0U] 
                                    & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0U]) 
                                   << 4U);
                            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[1U] 
                                = (((Vtb_top__ConstPool__CONST_h9b2d9f06_0[0U] 
                                     & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0U]) 
                                    >> 0x1cU) | ((Vtb_top__ConstPool__CONST_h9b2d9f06_0[1U] 
                                                  & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[1U]) 
                                                 << 4U));
                            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[2U] 
                                = (((Vtb_top__ConstPool__CONST_h9b2d9f06_0[1U] 
                                     & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[1U]) 
                                    >> 0x1cU) | ((Vtb_top__ConstPool__CONST_h9b2d9f06_0[2U] 
                                                  & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[2U]) 
                                                 << 4U));
                            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[3U] 
                                = (((Vtb_top__ConstPool__CONST_h9b2d9f06_0[2U] 
                                     & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[2U]) 
                                    >> 0x1cU) | ((Vtb_top__ConstPool__CONST_h9b2d9f06_0[3U] 
                                                  & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[3U]) 
                                                 << 4U));
                            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[4U] 
                                = (((Vtb_top__ConstPool__CONST_h9b2d9f06_0[3U] 
                                     & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[3U]) 
                                    >> 0x1cU) | ((Vtb_top__ConstPool__CONST_h9b2d9f06_0[4U] 
                                                  & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[4U]) 
                                                 << 4U));
                            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[5U] 
                                = (((Vtb_top__ConstPool__CONST_h9b2d9f06_0[4U] 
                                     & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[4U]) 
                                    >> 0x1cU) | ((Vtb_top__ConstPool__CONST_h9b2d9f06_0[5U] 
                                                  & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[5U]) 
                                                 << 4U));
                            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[6U] 
                                = (((Vtb_top__ConstPool__CONST_h9b2d9f06_0[5U] 
                                     & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[5U]) 
                                    >> 0x1cU) | ((Vtb_top__ConstPool__CONST_h9b2d9f06_0[6U] 
                                                  & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[6U]) 
                                                 << 4U));
                            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[7U] 
                                = (((Vtb_top__ConstPool__CONST_h9b2d9f06_0[6U] 
                                     & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[6U]) 
                                    >> 0x1cU) | ((Vtb_top__ConstPool__CONST_h9b2d9f06_0[7U] 
                                                  & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[7U]) 
                                                 << 4U));
                            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[8U] 
                                = (((Vtb_top__ConstPool__CONST_h9b2d9f06_0[7U] 
                                     & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[7U]) 
                                    >> 0x1cU) | ((Vtb_top__ConstPool__CONST_h9b2d9f06_0[8U] 
                                                  & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[8U]) 
                                                 << 4U));
                            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[9U] 
                                = (((Vtb_top__ConstPool__CONST_h9b2d9f06_0[8U] 
                                     & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[8U]) 
                                    >> 0x1cU) | ((Vtb_top__ConstPool__CONST_h9b2d9f06_0[9U] 
                                                  & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[9U]) 
                                                 << 4U));
                            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xaU] 
                                = (((Vtb_top__ConstPool__CONST_h9b2d9f06_0[9U] 
                                     & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[9U]) 
                                    >> 0x1cU) | ((Vtb_top__ConstPool__CONST_h9b2d9f06_0[0xaU] 
                                                  & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xaU]) 
                                                 << 4U));
                            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xbU] 
                                = (((Vtb_top__ConstPool__CONST_h9b2d9f06_0[0xaU] 
                                     & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xaU]) 
                                    >> 0x1cU) | ((Vtb_top__ConstPool__CONST_h9b2d9f06_0[0xbU] 
                                                  & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xbU]) 
                                                 << 4U));
                            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xcU] 
                                = (((Vtb_top__ConstPool__CONST_h9b2d9f06_0[0xbU] 
                                     & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xbU]) 
                                    >> 0x1cU) | ((Vtb_top__ConstPool__CONST_h9b2d9f06_0[0xcU] 
                                                  & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xcU]) 
                                                 << 4U));
                            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xdU] 
                                = (((Vtb_top__ConstPool__CONST_h9b2d9f06_0[0xcU] 
                                     & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xcU]) 
                                    >> 0x1cU) | ((Vtb_top__ConstPool__CONST_h9b2d9f06_0[0xdU] 
                                                  & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xdU]) 
                                                 << 4U));
                            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xeU] 
                                = (((Vtb_top__ConstPool__CONST_h9b2d9f06_0[0xdU] 
                                     & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xdU]) 
                                    >> 0x1cU) | ((Vtb_top__ConstPool__CONST_h9b2d9f06_0[0xeU] 
                                                  & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xeU]) 
                                                 << 4U));
                            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xfU] 
                                = (((Vtb_top__ConstPool__CONST_h9b2d9f06_0[0xeU] 
                                     & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xeU]) 
                                    >> 0x1cU) | ((Vtb_top__ConstPool__CONST_h9b2d9f06_0[0xfU] 
                                                  & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xfU]) 
                                                 << 4U));
                            if ((0U == (7U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dodc)))) {
                                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_rd_sd_addr 
                                    = (0x7fU & ((IData)(1U) 
                                                + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_rd_sd_addr)));
                            }
                            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_out 
                                = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dout_4bit;
                            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_oe = 0xfU;
                            if ((7U == (7U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dodc)))) {
                                __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dout_buf 
                                    = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_rd_sd_q;
                            }
                        } else {
                            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_out0 
                                = ((0xfffeU & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_out0) 
                                               << 1U)) 
                                   ^ (((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dout_1bit) 
                                       ^ ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_out0) 
                                          >> 0xfU))
                                       ? 0x1021U : 0U));
                            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dout_buf 
                                = (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dout_buf 
                                   << 1U);
                            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0U] 
                                = ((Vtb_top__ConstPool__CONST_h0b2d9f06_0[0U] 
                                    & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0U]) 
                                   << 1U);
                            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[1U] 
                                = (((Vtb_top__ConstPool__CONST_h0b2d9f06_0[0U] 
                                     & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0U]) 
                                    >> 0x1fU) | ((Vtb_top__ConstPool__CONST_h0b2d9f06_0[1U] 
                                                  & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[1U]) 
                                                 << 1U));
                            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[2U] 
                                = (((Vtb_top__ConstPool__CONST_h0b2d9f06_0[1U] 
                                     & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[1U]) 
                                    >> 0x1fU) | ((Vtb_top__ConstPool__CONST_h0b2d9f06_0[2U] 
                                                  & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[2U]) 
                                                 << 1U));
                            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[3U] 
                                = (((Vtb_top__ConstPool__CONST_h0b2d9f06_0[2U] 
                                     & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[2U]) 
                                    >> 0x1fU) | ((Vtb_top__ConstPool__CONST_h0b2d9f06_0[3U] 
                                                  & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[3U]) 
                                                 << 1U));
                            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[4U] 
                                = (((Vtb_top__ConstPool__CONST_h0b2d9f06_0[3U] 
                                     & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[3U]) 
                                    >> 0x1fU) | ((Vtb_top__ConstPool__CONST_h0b2d9f06_0[4U] 
                                                  & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[4U]) 
                                                 << 1U));
                            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[5U] 
                                = (((Vtb_top__ConstPool__CONST_h0b2d9f06_0[4U] 
                                     & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[4U]) 
                                    >> 0x1fU) | ((Vtb_top__ConstPool__CONST_h0b2d9f06_0[5U] 
                                                  & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[5U]) 
                                                 << 1U));
                            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[6U] 
                                = (((Vtb_top__ConstPool__CONST_h0b2d9f06_0[5U] 
                                     & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[5U]) 
                                    >> 0x1fU) | ((Vtb_top__ConstPool__CONST_h0b2d9f06_0[6U] 
                                                  & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[6U]) 
                                                 << 1U));
                            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[7U] 
                                = (((Vtb_top__ConstPool__CONST_h0b2d9f06_0[6U] 
                                     & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[6U]) 
                                    >> 0x1fU) | ((Vtb_top__ConstPool__CONST_h0b2d9f06_0[7U] 
                                                  & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[7U]) 
                                                 << 1U));
                            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[8U] 
                                = (((Vtb_top__ConstPool__CONST_h0b2d9f06_0[7U] 
                                     & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[7U]) 
                                    >> 0x1fU) | ((Vtb_top__ConstPool__CONST_h0b2d9f06_0[8U] 
                                                  & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[8U]) 
                                                 << 1U));
                            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[9U] 
                                = (((Vtb_top__ConstPool__CONST_h0b2d9f06_0[8U] 
                                     & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[8U]) 
                                    >> 0x1fU) | ((Vtb_top__ConstPool__CONST_h0b2d9f06_0[9U] 
                                                  & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[9U]) 
                                                 << 1U));
                            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xaU] 
                                = (((Vtb_top__ConstPool__CONST_h0b2d9f06_0[9U] 
                                     & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[9U]) 
                                    >> 0x1fU) | ((Vtb_top__ConstPool__CONST_h0b2d9f06_0[0xaU] 
                                                  & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xaU]) 
                                                 << 1U));
                            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xbU] 
                                = (((Vtb_top__ConstPool__CONST_h0b2d9f06_0[0xaU] 
                                     & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xaU]) 
                                    >> 0x1fU) | ((Vtb_top__ConstPool__CONST_h0b2d9f06_0[0xbU] 
                                                  & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xbU]) 
                                                 << 1U));
                            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xcU] 
                                = (((Vtb_top__ConstPool__CONST_h0b2d9f06_0[0xbU] 
                                     & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xbU]) 
                                    >> 0x1fU) | ((Vtb_top__ConstPool__CONST_h0b2d9f06_0[0xcU] 
                                                  & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xcU]) 
                                                 << 1U));
                            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xdU] 
                                = (((Vtb_top__ConstPool__CONST_h0b2d9f06_0[0xcU] 
                                     & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xcU]) 
                                    >> 0x1fU) | ((Vtb_top__ConstPool__CONST_h0b2d9f06_0[0xdU] 
                                                  & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xdU]) 
                                                 << 1U));
                            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xeU] 
                                = (((Vtb_top__ConstPool__CONST_h0b2d9f06_0[0xdU] 
                                     & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xdU]) 
                                    >> 0x1fU) | ((Vtb_top__ConstPool__CONST_h0b2d9f06_0[0xeU] 
                                                  & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xeU]) 
                                                 << 1U));
                            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xfU] 
                                = (((Vtb_top__ConstPool__CONST_h0b2d9f06_0[0xeU] 
                                     & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xeU]) 
                                    >> 0x1fU) | ((Vtb_top__ConstPool__CONST_h0b2d9f06_0[0xfU] 
                                                  & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xfU]) 
                                                 << 1U));
                            if ((0U == (0x1fU & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dodc)))) {
                                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_rd_sd_addr 
                                    = (0x7fU & ((IData)(1U) 
                                                + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_rd_sd_addr)));
                            }
                            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_out 
                                = ((0xeU & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_out)) 
                                   | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dout_1bit));
                            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_oe = 1U;
                            if ((0x1fU == (0x1fU & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dodc)))) {
                                __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dout_buf 
                                    = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_rd_sd_q;
                            }
                        }
                        if ((((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__mode_4bit_s)
                               ? VL_SHIFTR_III(32,32,32, 
                                               ((IData)(1U) 
                                                + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dodc)), 1U)
                               : VL_SHIFTR_III(32,32,32, 
                                               ((IData)(1U) 
                                                + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dodc)), 3U)) 
                             == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_len_s))) {
                            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dodc = 0U;
                            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dostate = 0x20U;
                        }
                        if (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_stop_s) {
                            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_out = 0xfU;
                            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_done = 1U;
                            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dostate = 0x21U;
                        }
                    } else {
                        __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0U] 
                            = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[0U];
                        __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[1U] 
                            = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[1U];
                        __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[2U] 
                            = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[2U];
                        __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[3U] 
                            = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[3U];
                        __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[4U] 
                            = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[4U];
                        __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[5U] 
                            = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[5U];
                        __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[6U] 
                            = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[6U];
                        __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[7U] 
                            = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[7U];
                        __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[8U] 
                            = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[8U];
                        __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[9U] 
                            = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[9U];
                        __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xaU] 
                            = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[0xaU];
                        __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xbU] 
                            = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[0xbU];
                        __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xcU] 
                            = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[0xcU];
                        __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xdU] 
                            = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[0xdU];
                        __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xeU] 
                            = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[0xeU];
                        __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xfU] 
                            = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[0xfU];
                        __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0x10U] = 0U;
                        if ((0U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dodc))) {
                            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dodc = 0U;
                            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_oe 
                                = ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__mode_4bit_s)
                                    ? 0xfU : 1U);
                            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_out = 0U;
                            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dostate = 0x1fU;
                        }
                        __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_out3 = 0U;
                        __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_out2 = 0U;
                        __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_out1 = 0U;
                        __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_out0 = 0U;
                        __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dout_buf 
                            = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_rd_sd_q;
                    }
                } else {
                    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dostate = 0U;
                }
            } else if ((2U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dostate))) {
                if ((1U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dostate))) {
                    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dostate = 0U;
                } else {
                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_oe 
                        = (0xeU & (IData)(vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_oe));
                    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dostate = 4U;
                }
            } else if ((1U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dostate))) {
                if (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__mode_spi_s) {
                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_oe 
                        = (1U | (IData)(vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_oe));
                    if ((3U == (7U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__spi_cnt)))) {
                        vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_out 
                            = (0xeU & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_out));
                    } else if ((4U == (7U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__spi_cnt)))) {
                        vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_out 
                            = ((0xeU & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_out)) 
                               | (1U & (~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_in_crc_good))));
                    } else if ((5U == (7U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__spi_cnt)))) {
                        vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_out 
                            = ((0xeU & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_out)) 
                               | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_in_crc_good));
                    } else if ((6U == (7U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__spi_cnt)))) {
                        vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_out 
                            = ((0xeU & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_out)) 
                               | (1U & (~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_in_crc_good))));
                    } else if ((7U == (7U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__spi_cnt)))) {
                        vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_out 
                            = (1U | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_out));
                        __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dostate = 0x1aU;
                    }
                } else {
                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_oe 
                        = (1U | (IData)(vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_oe));
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_out 
                        = (0xeU & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_out));
                    if ((1U & (~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_in_act_s)))) {
                        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_oe 
                            = (1U | (IData)(vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_oe));
                        vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_out 
                            = (1U | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_out));
                        __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dostate = 0x1aU;
                    }
                }
            } else if (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__mode_spi_s) {
                if (((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__do_crc_token)) 
                     & (2U == (7U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__spi_cnt))))) {
                    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dostate = 0x19U;
                }
            } else if ((1U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dodc))) {
                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_oe 
                    = (1U | (IData)(vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_oe));
                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_out 
                    = (0xeU & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_out));
            } else if ((2U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dodc))) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_out 
                    = ((0xeU & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_out)) 
                       | (1U & (~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_in_crc_good))));
            } else if ((3U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dodc))) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_out 
                    = ((0xeU & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_out)) 
                       | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_in_crc_good));
            } else if ((4U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dodc))) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_out 
                    = ((0xeU & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_out)) 
                       | (1U & (~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_in_crc_good))));
            } else if ((5U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dodc))) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_out 
                    = (1U | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_out));
            } else if ((6U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dodc))) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_out 
                    = (0xeU & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_out));
                __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dostate = 0x19U;
            }
        } else {
            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dostate = 0U;
        }
    } else if ((8U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dostate))) {
        __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dostate = 0U;
    } else if ((4U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dostate))) {
        if ((2U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dostate))) {
            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dostate = 0U;
        } else if ((1U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dostate))) {
            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dostate = 0U;
        } else {
            __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dodc = 0U;
            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_oe = 0U;
            if (((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__e__DOT__stage_3)) 
                 & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_act_s))) {
                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_busy = 1U;
                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_done = 0U;
                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_rd_sd_addr = 0U;
                __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dostate 
                    = ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__mode_spi_s)
                        ? 0x28U : 0x1eU);
            } else if (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__do_crc_token) {
                __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dostate = 0x18U;
            }
        }
    } else if ((2U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dostate))) {
        __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dostate = 0U;
    } else if ((1U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dostate))) {
        __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dostate = 0U;
    } else {
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_oe = 0U;
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_busy = 0U;
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_done = 0U;
        __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dostate = 4U;
    }
    if ((1U & (~ (IData)(vlSelfRef.rst_n)))) {
        __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__ostate = 0U;
        __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dostate = 0U;
    }
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__odc 
        = __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__odc;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__ostate 
        = __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__ostate;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc7_out 
        = __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc7_out;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dodc 
        = __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dodc;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dostate 
        = __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dostate;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_out3 
        = __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_out3;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_out2 
        = __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_out2;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_out1 
        = __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_out1;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_out0 
        = __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_out0;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0U] 
        = __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0U];
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[1U] 
        = __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[1U];
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[2U] 
        = __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[2U];
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[3U] 
        = __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[3U];
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[4U] 
        = __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[4U];
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[5U] 
        = __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[5U];
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[6U] 
        = __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[6U];
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[7U] 
        = __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[7U];
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[8U] 
        = __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[8U];
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[9U] 
        = __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[9U];
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xaU] 
        = __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xaU];
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xbU] 
        = __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xbU];
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xcU] 
        = __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xcU];
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xdU] 
        = __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xdU];
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xeU] 
        = __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xeU];
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xfU] 
        = __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xfU];
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0x10U] 
        = __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0x10U];
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dout_buf 
        = __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dout_buf;
}

VL_INLINE_OPT void Vtb_top___024root___nba_sequent__TOP__6(Vtb_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_top___024root___nba_sequent__TOP__6\n"); );
    Vtb_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if (vlSelfRef.__VdlySet__tb_top__DOT__u_dut__DOT__clk_100MHz__v0) {
        vlSelfRef.__VdlySet__tb_top__DOT__u_dut__DOT__clk_100MHz__v0 = 0U;
        vlSelfRef.tb_top__DOT__u_dut__DOT__clk_100MHz 
            = vlSelfRef.__VdlyVal__tb_top__DOT__u_dut__DOT__clk_100MHz__v0;
    }
    if (vlSelfRef.__VdlySet__tb_top__DOT__u_dut__DOT__clk_50MHz__v0) {
        vlSelfRef.__VdlySet__tb_top__DOT__u_dut__DOT__clk_50MHz__v0 = 0U;
        vlSelfRef.tb_top__DOT__u_dut__DOT__clk_50MHz 
            = vlSelfRef.__VdlyVal__tb_top__DOT__u_dut__DOT__clk_50MHz__v0;
    }
}

VL_INLINE_OPT void Vtb_top___024root___nba_sequent__TOP__8(Vtb_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_top___024root___nba_sequent__TOP__8\n"); );
    Vtb_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if (vlSelfRef.__VdlySet__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__isdb2__DOT__mem__v0) {
        vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__isdb2__DOT__mem[vlSelfRef.__VdlyDim0__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__isdb2__DOT__mem__v0] 
            = vlSelfRef.__VdlyVal__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__isdb2__DOT__mem__v0;
    }
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__resp_type_s 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__c__DOT__stage_2;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__b__DOT__stage_3 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__resp_act_s;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_rd_sd_q 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__isdb1__DOT__mem
        [(0xffU & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__sel_rd_sd)
                    ? ((IData)(0x80U) + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_rd_sd_addr))
                    : (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_rd_sd_addr)))];
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_stop_s 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__f__DOT__stage_2;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_len_s 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__d__DOT__stage_2;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__e__DOT__stage_3 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_act_s;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__c__DOT__stage_2 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__c__DOT__stage_1;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__resp_act_s 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__b__DOT__stage_2;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__f__DOT__stage_2 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__f__DOT__stage_1;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__d__DOT__stage_2 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__d__DOT__stage_1;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_act_s 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__e__DOT__stage_2;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__c__DOT__stage_1 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_type;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__b__DOT__stage_2 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__b__DOT__stage_1;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__f__DOT__stage_1 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_stop;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__d__DOT__stage_1 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_len;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__e__DOT__stage_2 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__e__DOT__stage_1;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__b__DOT__stage_1 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_act;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__e__DOT__stage_1 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_act;
}

extern const VlWide<14>/*447:0*/ Vtb_top__ConstPool__CONST_hb0e0ad07_0;
extern const VlWide<12>/*383:0*/ Vtb_top__ConstPool__CONST_h997e551f_0;
extern const VlWide<12>/*383:0*/ Vtb_top__ConstPool__CONST_h6d62b317_0;
extern const VlWide<15>/*479:0*/ Vtb_top__ConstPool__CONST_hbd99daea_0;

VL_INLINE_OPT void Vtb_top___024root___nba_sequent__TOP__9(Vtb_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_top___024root___nba_sequent__TOP__9\n"); );
    Vtb_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if (vlSelfRef.__VdlySet__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__isdb1__DOT__mem__v0) {
        vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__isdb1__DOT__mem[vlSelfRef.__VdlyDim0__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__isdb1__DOT__mem__v0] 
            = vlSelfRef.__VdlyVal__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__isdb1__DOT__mem__v0;
    }
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__sel_rd_sd 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__sel_rd_sd;
    if ((0U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__state))) {
        vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__info_card_desel = 0U;
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_in_gap_cnt = 0U;
        vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__err_op_out_range = 0U;
        vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__err_unhandled_cmd = 0U;
        vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__err_cmd_crc = 0U;
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_erase_state = 0U;
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_blocks_written = 0U;
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_appcmd = 0U;
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_rca = 0U;
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status = 0U;
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state = 0U;
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_ocr = 0x40ff8000U;
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_cid[0U] = 0x1014fffU;
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_cid[1U] = 0x10000000U;
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_cid[2U] = 0x454c4c4fU;
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_cid[3U] = 0x7f504348U;
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_csd[0U] = 0xa4000ffU;
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_csd[1U] = 0x1f7f80U;
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_csd[2U] = 0x7b590000U;
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_csd[3U] = 0x400e0032U;
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_scr = 0x215000000000000ULL;
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_sd_status[0U] = 0U;
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_sd_status[1U] = 0x4001000U;
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_sd_status[2U] = 0U;
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_sd_status[3U] = 0U;
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function_caps[0U] = 0x80018001U;
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function_caps[1U] = 0x80018001U;
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function_caps[2U] = 0x80018001U;
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function_caps[3U] = 0x32U;
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function = 0U;
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function_check = 0U;
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_scr = 0U;
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_cid = 0U;
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_csd = 0U;
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_sdstatus = 0U;
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_function = 0U;
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_written = 0U;
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_block = 0U;
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_block_queue = 0U;
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_recv_block = 0U;
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_in_act = 0U;
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_in_stop = 0U;
        vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_act = 0U;
        vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_act = 0U;
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_stop = 0U;
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_mode_4bit = 0U;
        if (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_mode_spi) {
            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_mode_crc_disable = 1U;
            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__link_read_act = 0U;
            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__link_read_num = 0U;
            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__link_read_stop = 0U;
            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__link_write_act = 0U;
            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__link_write_num = 0U;
            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__block_preerase_num = 0U;
            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__host_hc_support = 0U;
            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__state = 9U;
        } else {
            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_mode_crc_disable = 0U;
            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__link_read_act = 0U;
            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__link_read_num = 0U;
            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__link_read_stop = 0U;
            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__link_write_act = 0U;
            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__link_write_num = 0U;
            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__block_preerase_num = 0U;
            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__host_hc_support = 0U;
            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__state = 4U;
        }
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status 
            = (0x100U | vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status);
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function_caps[0U] 
            = (0xfffffffdU & vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function_caps[0U]);
    } else if ((4U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__state))) {
        if (((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__d__DOT__stage_3)) 
             & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_act_s))) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_act = 0U;
            if (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_crc_good_s) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_latch 
                    = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_cmd_in;
                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status 
                    = (0xff3fffffU & vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status);
                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__state = 8U;
                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_last 
                    = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd;
            } else {
                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__err_cmd_crc = 1U;
                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status 
                    = (0x800000U | vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status);
            }
        }
    } else if ((8U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__state))) {
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__state = 9U;
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state_next 
            = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state;
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_type = 0xeU;
        if (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_appcmd) {
            if ((0x20U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                if ((0x10U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                    if ((8U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__state = 8U;
                        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_appcmd = 0U;
                    } else if ((4U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__state = 8U;
                        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_appcmd = 0U;
                    } else if ((2U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                        if ((1U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                            if (((4U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state)) 
                                 | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_mode_spi))) {
                                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_type = 1U;
                                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_scr = 1U;
                                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state_next = 5U;
                            }
                        } else {
                            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__state = 8U;
                            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_appcmd = 0U;
                        }
                    } else {
                        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__state = 8U;
                        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_appcmd = 0U;
                    }
                } else if ((8U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                    if ((4U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__state = 8U;
                        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_appcmd = 0U;
                    } else if ((2U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                        if ((1U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__state = 8U;
                            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_appcmd = 0U;
                        } else if (((4U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state)) 
                                    | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_mode_spi))) {
                            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_type = 1U;
                        }
                    } else if ((1U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                        if (((0U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state)) 
                             | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_mode_spi))) {
                            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_type = 3U;
                            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_ocr 
                                = (0x80000000U | vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_ocr);
                            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state_next = 1U;
                            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__host_hc_support 
                                = (1U & (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_arg 
                                         >> 0x1eU));
                        }
                    } else {
                        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__state = 8U;
                        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_appcmd = 0U;
                    }
                } else {
                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__state = 8U;
                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_appcmd = 0U;
                }
            } else if ((0x10U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                if ((8U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__state = 8U;
                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_appcmd = 0U;
                } else if ((4U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                    if ((2U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                        if ((1U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                            if (((4U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state)) 
                                 | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_mode_spi))) {
                                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_type = 1U;
                                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__block_preerase_num 
                                    = (0x7fffffU & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_arg);
                            }
                        } else if (((4U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state)) 
                                    | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_mode_spi))) {
                            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_type = 1U;
                            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_written = 1U;
                            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state_next = 5U;
                        }
                    } else {
                        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__state = 8U;
                        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_appcmd = 0U;
                    }
                } else {
                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__state = 8U;
                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_appcmd = 0U;
                }
            } else if ((8U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                if ((4U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                    if ((2U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__state = 8U;
                        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_appcmd = 0U;
                    } else if ((1U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                        if (((4U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state)) 
                             | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_mode_spi))) {
                            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_type = 1U;
                            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_sdstatus = 1U;
                            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state_next = 5U;
                        }
                    } else {
                        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__state = 8U;
                        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_appcmd = 0U;
                    }
                } else {
                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__state = 8U;
                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_appcmd = 0U;
                }
            } else if ((4U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                if ((2U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                    if ((1U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__state = 8U;
                        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_appcmd = 0U;
                    } else if (((4U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state)) 
                                | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_mode_spi))) {
                        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_type = 1U;
                        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_mode_4bit 
                            = (1U & (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_arg 
                                     >> 1U));
                    }
                } else {
                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__state = 8U;
                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_appcmd = 0U;
                }
            } else {
                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__state = 8U;
                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_appcmd = 0U;
            }
        } else {
            if ((0x20U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                if ((0x10U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                    if ((8U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                        if ((4U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__err_unhandled_cmd = 1U;
                            if ((1U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__err_unhandled_cmd = 0U;
                            }
                            if ((5U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__err_unhandled_cmd = 0U;
                            }
                        } else if ((2U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                            if ((1U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_mode_crc_disable 
                                    = (1U & (~ vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_arg));
                                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_type = 1U;
                            } else {
                                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_type = 3U;
                            }
                        } else {
                            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__err_unhandled_cmd = 1U;
                            if ((1U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__err_unhandled_cmd = 0U;
                            }
                            if ((5U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__err_unhandled_cmd = 0U;
                            }
                        }
                    } else if ((4U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                        if ((2U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                            if ((1U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                                if (((vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_arg 
                                      >> 0x10U) == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_rca))) {
                                    if ((((((((0U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state)) 
                                              || (3U 
                                                  == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state))) 
                                             || (4U 
                                                 == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state))) 
                                            || (5U 
                                                == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state))) 
                                           || (6U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state))) 
                                          || (7U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state))) 
                                         || (8U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state)))) {
                                        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_type = 1U;
                                        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_appcmd = 1U;
                                        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status 
                                            = (0x20U 
                                               | vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status);
                                    }
                                } else {
                                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_type = 0U;
                                }
                                if (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_mode_spi) {
                                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_type = 1U;
                                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_appcmd = 1U;
                                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status 
                                        = (0x20U | vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status);
                                }
                            } else {
                                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__err_unhandled_cmd = 1U;
                                if ((1U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__err_unhandled_cmd = 0U;
                                }
                                if ((5U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__err_unhandled_cmd = 0U;
                                }
                            }
                        } else {
                            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__err_unhandled_cmd = 1U;
                            if ((1U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__err_unhandled_cmd = 0U;
                            }
                            if ((5U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__err_unhandled_cmd = 0U;
                            }
                        }
                    } else {
                        vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__err_unhandled_cmd = 1U;
                        if ((1U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__err_unhandled_cmd = 0U;
                        }
                        if ((5U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__err_unhandled_cmd = 0U;
                        }
                    }
                } else if ((8U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__err_unhandled_cmd = 1U;
                    if ((1U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                        vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__err_unhandled_cmd = 0U;
                    }
                    if ((5U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                        vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__err_unhandled_cmd = 0U;
                    }
                } else if ((4U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                    if ((2U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                        if ((1U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__err_unhandled_cmd = 1U;
                            if ((1U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__err_unhandled_cmd = 0U;
                            }
                            if ((5U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__err_unhandled_cmd = 0U;
                            }
                        } else if (((4U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state)) 
                                    | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_mode_spi))) {
                            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_type = 8U;
                            if ((2U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_erase_state))) {
                                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status 
                                    = (0x10000000U 
                                       | vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status);
                            }
                            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_erase_state = 0U;
                        }
                    } else {
                        vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__err_unhandled_cmd = 1U;
                        if ((1U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__err_unhandled_cmd = 0U;
                        }
                        if ((5U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__err_unhandled_cmd = 0U;
                        }
                    }
                } else if ((2U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__err_unhandled_cmd = 1U;
                    if ((1U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                        vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__err_unhandled_cmd = 0U;
                    }
                    if ((5U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                        vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__err_unhandled_cmd = 0U;
                    }
                } else if ((1U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                    if (((4U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state)) 
                         | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_mode_spi))) {
                        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_erase_state = 0U;
                        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_type = 1U;
                        if ((1U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_erase_state))) {
                            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__block_erase_end 
                                = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_arg_blockaddr;
                            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_erase_state = 2U;
                        } else {
                            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status 
                                = (0x10000000U | vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status);
                        }
                    }
                } else if (((4U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state)) 
                            | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_mode_spi))) {
                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_erase_state = 0U;
                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_type = 1U;
                    if ((0U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_erase_state))) {
                        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_erase_state = 1U;
                        vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__block_erase_start 
                            = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_arg_blockaddr;
                    } else {
                        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status 
                            = (0x10000000U | vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status);
                    }
                }
            } else if ((0x10U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                if ((8U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                    if ((4U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                        vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__err_unhandled_cmd = 1U;
                        if ((1U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__err_unhandled_cmd = 0U;
                        }
                        if ((5U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__err_unhandled_cmd = 0U;
                        }
                    } else if ((2U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                        vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__err_unhandled_cmd = 1U;
                        if ((1U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__err_unhandled_cmd = 0U;
                        }
                        if ((5U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__err_unhandled_cmd = 0U;
                        }
                    } else if ((1U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                        if (((4U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state)) 
                             | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_mode_spi))) {
                            if ((0x8000U <= vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_arg_blockaddr)) {
                                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status 
                                    = (0x80000000U 
                                       | vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status);
                                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__err_op_out_range = 1U;
                            } else {
                                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_type = 1U;
                                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__link_write_addr 
                                    = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_arg_blockaddr;
                                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__block_write_byteaddr 
                                    = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_arg_byteaddr;
                                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__link_write_num = 0xffffffffU;
                                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_blocks_written = 0U;
                                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_recv_block = 1U;
                                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state_next = 6U;
                            }
                        }
                    } else if (((4U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state)) 
                                | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_mode_spi))) {
                        if ((0x8000U <= vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_arg_blockaddr)) {
                            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status 
                                = (0x80000000U | vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status);
                            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__err_op_out_range = 1U;
                        } else {
                            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_type = 1U;
                            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__link_write_addr 
                                = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_arg_blockaddr;
                            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__block_write_byteaddr 
                                = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_arg_byteaddr;
                            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__link_write_num = 1U;
                            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_blocks_written = 0U;
                            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_recv_block = 1U;
                            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state_next = 6U;
                        }
                    }
                } else if ((4U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__err_unhandled_cmd = 1U;
                    if ((1U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                        vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__err_unhandled_cmd = 0U;
                    }
                    if ((5U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                        vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__err_unhandled_cmd = 0U;
                    }
                } else if ((2U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                    if ((1U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                        vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__err_unhandled_cmd = 1U;
                        if ((1U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__err_unhandled_cmd = 0U;
                        }
                        if ((5U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__err_unhandled_cmd = 0U;
                        }
                    } else if (((4U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state)) 
                                | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_mode_spi))) {
                        if ((0x8000U <= vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_arg_blockaddr)) {
                            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status 
                                = (0x80000000U | vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status);
                            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__err_op_out_range = 1U;
                        } else {
                            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_type = 1U;
                            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__link_read_addr 
                                = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_arg_blockaddr;
                            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__block_read_byteaddr 
                                = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_arg_byteaddr;
                            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__link_read_num = 0xffffffffU;
                            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_block_queue = 1U;
                            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state_next = 5U;
                        }
                    }
                } else if ((1U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                    if (((4U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state)) 
                         | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_mode_spi))) {
                        if ((0x8000U <= vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_arg_blockaddr)) {
                            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status 
                                = (0x80000000U | vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status);
                            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__err_op_out_range = 1U;
                        } else {
                            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_type = 1U;
                            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__link_read_addr 
                                = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_arg_blockaddr;
                            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__block_read_byteaddr 
                                = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_arg_byteaddr;
                            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__link_read_num = 1U;
                            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_block_queue = 1U;
                            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state_next = 5U;
                        }
                    }
                } else if (((4U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state)) 
                            | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_mode_spi))) {
                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_type = 1U;
                    if ((0x200U < vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_arg)) {
                        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status 
                            = (0x20000000U | vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status);
                    }
                }
            } else if ((8U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                if ((4U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                    if ((2U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                        if ((1U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                            if (((vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_arg 
                                  >> 0x10U) == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_rca))) {
                                if (((((((3U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state)) 
                                         || (4U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state))) 
                                        || (5U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state))) 
                                       || (6U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state))) 
                                      || (7U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state))) 
                                     || (8U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state)))) {
                                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state_next = 9U;
                                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_type = 0U;
                                }
                            } else {
                                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_type = 0U;
                            }
                        } else {
                            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__err_unhandled_cmd = 1U;
                            if ((1U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__err_unhandled_cmd = 0U;
                            }
                            if ((5U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__err_unhandled_cmd = 0U;
                            }
                        }
                    } else if ((1U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                        if (((vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_arg 
                              >> 0x10U) == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_rca))) {
                            if (((((((3U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state)) 
                                     || (4U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state))) 
                                    || (5U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state))) 
                                   || (6U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state))) 
                                  || (7U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state))) 
                                 || (8U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state)))) {
                                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_type = 1U;
                            }
                        } else {
                            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_type = 0U;
                        }
                        if (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_mode_spi) {
                            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_type = 2U;
                        }
                    } else if ((((5U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state)) 
                                 || (6U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state))) 
                                || (7U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state)))) {
                        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_type = 8U;
                        if ((5U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state))) {
                            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state_next = 4U;
                        }
                        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_in_stop = 1U;
                        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_stop = 1U;
                        if ((6U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state))) {
                            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state_next = 4U;
                        }
                        if ((7U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state))) {
                            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state_next = 4U;
                        }
                    }
                } else if ((2U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                    if ((1U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                        vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__err_unhandled_cmd = 1U;
                        if ((1U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__err_unhandled_cmd = 0U;
                        }
                        if ((5U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__err_unhandled_cmd = 0U;
                        }
                    } else {
                        if (((vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_arg 
                              >> 0x10U) == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_rca))) {
                            if ((3U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state))) {
                                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_type = 2U;
                            }
                        } else {
                            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_type = 0U;
                        }
                        if (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_mode_spi) {
                            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_type = 1U;
                            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_cid = 1U;
                        }
                    }
                } else if ((1U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                    if (((vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_arg 
                          >> 0x10U) == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_rca))) {
                        if ((3U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state))) {
                            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_type = 2U;
                        }
                    } else {
                        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_type = 0U;
                    }
                    if (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_mode_spi) {
                        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_type = 1U;
                        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_csd = 1U;
                    }
                } else if (((IData)(((0U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state)) 
                                     & (0x100U == (0xf00U 
                                                   & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_arg)))) 
                            | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_mode_spi))) {
                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_type = 7U;
                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_arg[0U] 
                        = (0x100U | (0xffU & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_arg));
                } else {
                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_type = 0U;
                }
            } else if ((4U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                if ((2U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                    if ((1U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                        if (((vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_arg 
                              >> 0x10U) == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_rca))) {
                            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_type = 8U;
                            if ((3U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state))) {
                                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state_next = 4U;
                            } else if ((8U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state))) {
                                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state_next = 4U;
                            } else {
                                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_type = 0xeU;
                            }
                        } else {
                            if ((3U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state))) {
                                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state_next = 3U;
                            } else if ((4U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state))) {
                                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state_next = 3U;
                            } else if ((5U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state))) {
                                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state_next = 3U;
                            } else if ((7U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state))) {
                                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state_next = 8U;
                            }
                            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__info_card_desel = 1U;
                            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_type = 0U;
                        }
                    } else if ((4U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state))) {
                        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function_check 
                            = ((0xfffffU & vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function_check) 
                               | ((((0U == (0xfU & 
                                            (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_arg 
                                             >> 0x14U))) 
                                    || (0xfU == (0xfU 
                                                 & (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_arg 
                                                    >> 0x14U))))
                                    ? 0U : 0xfU) << 0x14U));
                        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_type = 1U;
                        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state_next = 5U;
                        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_function = 1U;
                        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function_check 
                            = ((0xf0ffffU & vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function_check) 
                               | ((((0U == (0xfU & 
                                            (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_arg 
                                             >> 0x10U))) 
                                    || (0xfU == (0xfU 
                                                 & (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_arg 
                                                    >> 0x10U))))
                                    ? 0U : 0xfU) << 0x10U));
                        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function_check 
                            = ((0xff0fffU & vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function_check) 
                               | ((((0U == (0xfU & 
                                            (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_arg 
                                             >> 0xcU))) 
                                    || (0xfU == (0xfU 
                                                 & (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_arg 
                                                    >> 0xcU))))
                                    ? 0U : 0xfU) << 0xcU));
                        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function_check 
                            = ((0xfff0ffU & vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function_check) 
                               | ((((0U == (0xfU & 
                                            (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_arg 
                                             >> 8U))) 
                                    || (0xfU == (0xfU 
                                                 & (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_arg 
                                                    >> 8U))))
                                    ? 0U : 0xfU) << 8U));
                        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function_check 
                            = ((0xffff0fU & vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function_check) 
                               | ((((0U == (0xfU & 
                                            (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_arg 
                                             >> 4U))) 
                                    || (0xfU == (0xfU 
                                                 & (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_arg 
                                                    >> 4U))))
                                    ? 0U : 0xfU) << 4U));
                        if ((0U == (0xfU & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_arg))) {
                            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function_check 
                                = (0xfffff0U & vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function_check);
                        } else if ((0xfU == (0xfU & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_arg))) {
                            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function_check 
                                = ((0xfffff0U & vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function_check) 
                                   | (0xfU & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function));
                        } else if ((1U == (0xfU & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_arg))) {
                            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function_check 
                                = (1U | (0xfffff0U 
                                         & vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function_check));
                            if ((vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_arg 
                                 >> 0x1fU)) {
                                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function 
                                    = (1U | (0xfffff0U 
                                             & vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function));
                            }
                        } else {
                            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function_check 
                                = (0xfU | vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function_check);
                        }
                    }
                } else {
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__err_unhandled_cmd = 1U;
                    if ((1U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                        vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__err_unhandled_cmd = 0U;
                    }
                    if ((5U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                        vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__err_unhandled_cmd = 0U;
                    }
                }
            } else if ((2U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                if ((1U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                    if (((2U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state)) 
                         || (3U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state)))) {
                        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_rca 
                            = (0xffffU & ((IData)(0x1337U) 
                                          + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_rca)));
                        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_type = 6U;
                        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state_next = 3U;
                    }
                } else if (((1U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state)) 
                            | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_mode_spi))) {
                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_type = 2U;
                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state_next = 2U;
                }
            } else if ((1U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                if (((0U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state)) 
                     | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_mode_spi))) {
                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_type = 1U;
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__host_hc_support 
                        = (1U & (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_arg 
                                 >> 0x1eU));
                }
            } else if ((9U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state))) {
                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__state = 0U;
                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_state = 0U;
                if (((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_mode_spi) 
                     | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__spi_sel_s))) {
                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_mode_spi = 1U;
                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_type = 1U;
                } else {
                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_type = 0U;
                }
            }
            if ((0U < (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_erase_state))) {
                if ((((0xdU != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd)) 
                      & (0x21U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) 
                     & (0x26U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd)))) {
                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_erase_state = 0U;
                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status 
                        = (0x2000U | vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status);
                }
            }
        }
    } else if ((9U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__state))) {
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status 
            = ((0xffffe1ffU & vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status) 
               | ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state) 
                  << 9U));
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__state = 0xaU;
    } else if ((0xaU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__state))) {
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__state = 0xbU;
        vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_busy = 0U;
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_type 
            = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_type;
        if ((0U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_type))) {
            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state 
                = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state_next;
            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__state = 4U;
        } else if ((0xeU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_type))) {
            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status 
                = (0x400000U | vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status);
            if (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_mode_spi) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_out[0U] = 0U;
                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_out[1U] = 0U;
                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_out[2U] = 0U;
                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_out[3U] = 0U;
                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_out[4U] 
                    = (0xffU & (4U | ((0xf8U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__spi_status_word) 
                                                >> 8U)) 
                                      | (3U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__spi_status_word) 
                                               >> 8U)))));
            } else {
                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__state = 4U;
            }
        } else if (((1U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_type)) 
                    || (8U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_type)))) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_out[0U] = 0U;
            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_out[1U] = 0U;
            if (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_mode_spi) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_out[2U] = 0U;
                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_out[3U] = 0U;
                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_out[4U] 
                    = (0xffU & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__spi_status_word) 
                                >> 8U));
            } else {
                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_out[2U] = 0x1000000U;
                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_out[3U] 
                    = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status;
                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_out[4U] 
                    = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd;
            }
        } else if ((2U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_type))) {
            if (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_mode_spi) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_out[0U] = 0U;
                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_out[1U] = 0U;
                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_out[2U] = 0U;
                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_out[3U] 
                    = ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__spi_status_word) 
                       << 0x18U);
                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_out[4U] 
                    = ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__spi_status_word) 
                       >> 8U);
            } else {
                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_out[0U] 
                    = (1U | (((9U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))
                               ? ((vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_csd[1U] 
                                   << 0x1fU) | (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_csd[0U] 
                                                >> 1U))
                               : ((vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_cid[1U] 
                                   << 0x1fU) | (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_cid[0U] 
                                                >> 1U))) 
                             << 1U));
                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_out[1U] 
                    = ((((9U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))
                          ? ((vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_csd[1U] 
                              << 0x1fU) | (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_csd[0U] 
                                           >> 1U)) : 
                         ((vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_cid[1U] 
                           << 0x1fU) | (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_cid[0U] 
                                        >> 1U))) >> 0x1fU) 
                       | (((9U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))
                            ? ((vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_csd[2U] 
                                << 0x1fU) | (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_csd[1U] 
                                             >> 1U))
                            : ((vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_cid[2U] 
                                << 0x1fU) | (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_cid[1U] 
                                             >> 1U))) 
                          << 1U));
                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_out[2U] 
                    = ((((9U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))
                          ? ((vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_csd[2U] 
                              << 0x1fU) | (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_csd[1U] 
                                           >> 1U)) : 
                         ((vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_cid[2U] 
                           << 0x1fU) | (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_cid[1U] 
                                        >> 1U))) >> 0x1fU) 
                       | (((9U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))
                            ? ((vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_csd[3U] 
                                << 0x1fU) | (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_csd[2U] 
                                             >> 1U))
                            : ((vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_cid[3U] 
                                << 0x1fU) | (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_cid[2U] 
                                             >> 1U))) 
                          << 1U));
                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_out[3U] 
                    = ((((9U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))
                          ? ((vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_csd[3U] 
                              << 0x1fU) | (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_csd[2U] 
                                           >> 1U)) : 
                         ((vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_cid[3U] 
                           << 0x1fU) | (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_cid[2U] 
                                        >> 1U))) >> 0x1fU) 
                       | (((9U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))
                            ? (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_csd[3U] 
                               >> 1U) : (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_cid[3U] 
                                         >> 1U)) << 1U));
                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_out[4U] = 0x3fU;
            }
        } else if ((3U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_type))) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_out[0U] = 0U;
            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_out[1U] = 0U;
            if (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_mode_spi) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_out[2U] = 0U;
                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_out[3U] 
                    = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_ocr;
                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_out[4U] 
                    = (0xffU & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__spi_status_word) 
                                >> 8U));
            } else {
                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_out[2U] = 0xff000000U;
                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_out[3U] 
                    = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_ocr;
                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_out[4U] = 0x3fU;
            }
        } else if ((6U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_type))) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_out[0U] = 0U;
            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_out[1U] = 0U;
            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_out[2U] = 0x1000000U;
            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_out[3U] 
                = (((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_rca) 
                    << 0x10U) | ((0xc000U & (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status 
                                             >> 8U)) 
                                 | ((0x2000U & (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status 
                                                >> 6U)) 
                                    | (0x1fffU & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status))));
            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_out[4U] = 3U;
        } else if ((7U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_type))) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_out[0U] = 0U;
            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_out[1U] = 0U;
            if (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_mode_spi) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_out[2U] = 0U;
                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_out[3U] 
                    = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_arg[0U];
                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_out[4U] 
                    = (0xffU & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__spi_status_word) 
                                >> 8U));
            } else {
                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_out[2U] = 0x1000000U;
                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_out[3U] 
                    = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_arg[0U];
                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_out[4U] = 8U;
            }
        }
    } else if ((0xbU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__state))) {
        vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_act = 1U;
        if (((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__h__DOT__stage_3)) 
             & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_done_s))) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_act = 0U;
            if (((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_appcmd) 
                 & (0x37U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd)))) {
                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_appcmd = 0U;
                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status 
                    = (0xffffffdfU & vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status);
            }
            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state 
                = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state_next;
            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__state = 4U;
            if ((0xdU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd))) {
                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status 
                    = (0x3ffffffU & vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status);
                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status 
                    = (0xfeffffffU & vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status);
                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status 
                    = (0xffcfffffU & vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status);
                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status 
                    = (0xfffe7fffU & vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status);
                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status 
                    = (0xffffdfffU & vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status);
                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status 
                    = (0xffffffdfU & vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status);
                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status 
                    = (0xfffffff7U & vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status);
            }
            if (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_mode_spi) {
                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status 
                    = (0x8fffffffU & vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status);
                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status 
                    = (0xff3fffffU & vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status);
                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status 
                    = (0xffffdfffU & vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status);
                if ((2U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_type))) {
                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status 
                        = (0x7fffffffU & vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status);
                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status 
                        = (0xfffeffffU & vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status);
                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status 
                        = (0xf3ffffffU & vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status);
                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status 
                        = (0xffc7ffffU & vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status);
                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status 
                        = (0xffff7fffU & vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status);
                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status 
                        = (0xfcffffffU & vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status);
                }
            }
        }
    }
    if (((((((((0U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_state)) 
               | (1U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_state))) 
              | (2U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_state))) 
             | (0xfU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_state))) 
            | (0xdU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_state))) 
           | (0xaU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_state))) 
          | (0xbU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_state))) 
         | (0xeU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_state)))) {
        if ((0U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_state))) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_act = 0U;
            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_state = 1U;
        } else if ((1U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_state))) {
            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status 
                = (0x100U | vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status);
            if (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_recv_block) {
                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_state = 0x14U;
            } else if ((((((((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_scr) 
                             | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_sdstatus)) 
                            | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_cid)) 
                           | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_csd)) 
                          | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_function)) 
                         | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_written)) 
                        | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_block_queue))) {
                if ((1U & (~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_done_s)))) {
                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_state = 2U;
                    if (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_block_queue) {
                        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_block_queue = 0U;
                        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_block = 1U;
                    }
                }
            }
        } else if ((2U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_state))) {
            if ((((((((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_scr) 
                      | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_sdstatus)) 
                     | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_cid)) 
                    | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_csd)) 
                   | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_function)) 
                  | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_written)) 
                 | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_block))) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_src = 1U;
                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_state = 0xaU;
                if (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_scr) {
                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_scr = 0U;
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_len = 8U;
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[0U] 
                        = Vtb_top__ConstPool__CONST_hb0e0ad07_0[0U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[1U] 
                        = Vtb_top__ConstPool__CONST_hb0e0ad07_0[1U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[2U] 
                        = Vtb_top__ConstPool__CONST_hb0e0ad07_0[2U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[3U] 
                        = Vtb_top__ConstPool__CONST_hb0e0ad07_0[3U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[4U] 
                        = Vtb_top__ConstPool__CONST_hb0e0ad07_0[4U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[5U] 
                        = Vtb_top__ConstPool__CONST_hb0e0ad07_0[5U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[6U] 
                        = Vtb_top__ConstPool__CONST_hb0e0ad07_0[6U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[7U] 
                        = Vtb_top__ConstPool__CONST_hb0e0ad07_0[7U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[8U] 
                        = Vtb_top__ConstPool__CONST_hb0e0ad07_0[8U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[9U] 
                        = Vtb_top__ConstPool__CONST_hb0e0ad07_0[9U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[0xaU] 
                        = Vtb_top__ConstPool__CONST_hb0e0ad07_0[0xaU];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[0xbU] 
                        = Vtb_top__ConstPool__CONST_hb0e0ad07_0[0xbU];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[0xcU] 
                        = Vtb_top__ConstPool__CONST_hb0e0ad07_0[0xcU];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[0xdU] 
                        = Vtb_top__ConstPool__CONST_hb0e0ad07_0[0xdU];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[0xeU] 
                        = (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_scr);
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[0xfU] 
                        = (IData)((vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_scr 
                                   >> 0x20U));
                } else if (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_sdstatus) {
                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_sdstatus = 0U;
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_len = 0x40U;
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[0U] 
                        = Vtb_top__ConstPool__CONST_h997e551f_0[0U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[1U] 
                        = Vtb_top__ConstPool__CONST_h997e551f_0[1U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[2U] 
                        = Vtb_top__ConstPool__CONST_h997e551f_0[2U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[3U] 
                        = Vtb_top__ConstPool__CONST_h997e551f_0[3U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[4U] 
                        = Vtb_top__ConstPool__CONST_h997e551f_0[4U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[5U] 
                        = Vtb_top__ConstPool__CONST_h997e551f_0[5U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[6U] 
                        = Vtb_top__ConstPool__CONST_h997e551f_0[6U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[7U] 
                        = Vtb_top__ConstPool__CONST_h997e551f_0[7U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[8U] 
                        = Vtb_top__ConstPool__CONST_h997e551f_0[8U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[9U] 
                        = Vtb_top__ConstPool__CONST_h997e551f_0[9U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[0xaU] 
                        = Vtb_top__ConstPool__CONST_h997e551f_0[0xaU];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[0xbU] 
                        = Vtb_top__ConstPool__CONST_h997e551f_0[0xbU];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[0xcU] 
                        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_sd_status[0U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[0xdU] 
                        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_sd_status[1U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[0xeU] 
                        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_sd_status[2U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[0xfU] 
                        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_sd_status[3U];
                } else if (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_function) {
                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_function = 0U;
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_len = 0x40U;
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[0U] 
                        = Vtb_top__ConstPool__CONST_h6d62b317_0[0U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[1U] 
                        = Vtb_top__ConstPool__CONST_h6d62b317_0[1U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[2U] 
                        = Vtb_top__ConstPool__CONST_h6d62b317_0[2U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[3U] 
                        = Vtb_top__ConstPool__CONST_h6d62b317_0[3U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[4U] 
                        = Vtb_top__ConstPool__CONST_h6d62b317_0[4U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[5U] 
                        = Vtb_top__ConstPool__CONST_h6d62b317_0[5U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[6U] 
                        = Vtb_top__ConstPool__CONST_h6d62b317_0[6U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[7U] 
                        = Vtb_top__ConstPool__CONST_h6d62b317_0[7U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[8U] 
                        = Vtb_top__ConstPool__CONST_h6d62b317_0[8U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[9U] 
                        = Vtb_top__ConstPool__CONST_h6d62b317_0[9U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[0xaU] 
                        = Vtb_top__ConstPool__CONST_h6d62b317_0[0xaU];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[0xbU] 
                        = ((vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function_check 
                            << 0x18U) | Vtb_top__ConstPool__CONST_h6d62b317_0[0xbU]);
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[0xcU] 
                        = ((vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function_caps[0U] 
                            << 0x10U) | (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function_check 
                                         >> 8U));
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[0xdU] 
                        = ((vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function_caps[0U] 
                            >> 0x10U) | (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function_caps[1U] 
                                         << 0x10U));
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[0xeU] 
                        = ((vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function_caps[1U] 
                            >> 0x10U) | (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function_caps[2U] 
                                         << 0x10U));
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[0xfU] 
                        = ((vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function_caps[2U] 
                            >> 0x10U) | (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function_caps[3U] 
                                         << 0x10U));
                } else if (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_written) {
                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_written = 0U;
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_len = 4U;
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[0U] 
                        = Vtb_top__ConstPool__CONST_hbd99daea_0[0U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[1U] 
                        = Vtb_top__ConstPool__CONST_hbd99daea_0[1U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[2U] 
                        = Vtb_top__ConstPool__CONST_hbd99daea_0[2U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[3U] 
                        = Vtb_top__ConstPool__CONST_hbd99daea_0[3U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[4U] 
                        = Vtb_top__ConstPool__CONST_hbd99daea_0[4U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[5U] 
                        = Vtb_top__ConstPool__CONST_hbd99daea_0[5U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[6U] 
                        = Vtb_top__ConstPool__CONST_hbd99daea_0[6U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[7U] 
                        = Vtb_top__ConstPool__CONST_hbd99daea_0[7U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[8U] 
                        = Vtb_top__ConstPool__CONST_hbd99daea_0[8U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[9U] 
                        = Vtb_top__ConstPool__CONST_hbd99daea_0[9U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[0xaU] 
                        = Vtb_top__ConstPool__CONST_hbd99daea_0[0xaU];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[0xbU] 
                        = Vtb_top__ConstPool__CONST_hbd99daea_0[0xbU];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[0xcU] 
                        = Vtb_top__ConstPool__CONST_hbd99daea_0[0xcU];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[0xdU] 
                        = Vtb_top__ConstPool__CONST_hbd99daea_0[0xdU];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[0xeU] 
                        = Vtb_top__ConstPool__CONST_hbd99daea_0[0xeU];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[0xfU] 
                        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_blocks_written;
                } else if (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_block) {
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_src = 0U;
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_len = 0x200U;
                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_state = 0xfU;
                }
                if (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_cid) {
                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_cid = 0U;
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_len = 0x10U;
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[0U] 
                        = Vtb_top__ConstPool__CONST_h997e551f_0[0U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[1U] 
                        = Vtb_top__ConstPool__CONST_h997e551f_0[1U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[2U] 
                        = Vtb_top__ConstPool__CONST_h997e551f_0[2U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[3U] 
                        = Vtb_top__ConstPool__CONST_h997e551f_0[3U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[4U] 
                        = Vtb_top__ConstPool__CONST_h997e551f_0[4U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[5U] 
                        = Vtb_top__ConstPool__CONST_h997e551f_0[5U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[6U] 
                        = Vtb_top__ConstPool__CONST_h997e551f_0[6U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[7U] 
                        = Vtb_top__ConstPool__CONST_h997e551f_0[7U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[8U] 
                        = Vtb_top__ConstPool__CONST_h997e551f_0[8U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[9U] 
                        = Vtb_top__ConstPool__CONST_h997e551f_0[9U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[0xaU] 
                        = Vtb_top__ConstPool__CONST_h997e551f_0[0xaU];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[0xbU] 
                        = Vtb_top__ConstPool__CONST_h997e551f_0[0xbU];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[0xcU] 
                        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_cid[0U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[0xdU] 
                        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_cid[1U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[0xeU] 
                        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_cid[2U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[0xfU] 
                        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_cid[3U];
                }
                if (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_csd) {
                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_csd = 0U;
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_len = 0x10U;
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[0U] 
                        = Vtb_top__ConstPool__CONST_h997e551f_0[0U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[1U] 
                        = Vtb_top__ConstPool__CONST_h997e551f_0[1U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[2U] 
                        = Vtb_top__ConstPool__CONST_h997e551f_0[2U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[3U] 
                        = Vtb_top__ConstPool__CONST_h997e551f_0[3U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[4U] 
                        = Vtb_top__ConstPool__CONST_h997e551f_0[4U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[5U] 
                        = Vtb_top__ConstPool__CONST_h997e551f_0[5U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[6U] 
                        = Vtb_top__ConstPool__CONST_h997e551f_0[6U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[7U] 
                        = Vtb_top__ConstPool__CONST_h997e551f_0[7U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[8U] 
                        = Vtb_top__ConstPool__CONST_h997e551f_0[8U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[9U] 
                        = Vtb_top__ConstPool__CONST_h997e551f_0[9U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[0xaU] 
                        = Vtb_top__ConstPool__CONST_h997e551f_0[0xaU];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[0xbU] 
                        = Vtb_top__ConstPool__CONST_h997e551f_0[0xbU];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[0xcU] 
                        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_csd[0U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[0xdU] 
                        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_csd[1U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[0xeU] 
                        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_csd[2U];
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg[0xfU] 
                        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_csd[3U];
                }
            }
        } else if ((0xfU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_state))) {
            if ((1U & (~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__link_read_go)))) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__link_read_stop = 0U;
                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_state = 0xdU;
            }
        } else if ((0xdU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_state))) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__link_read_act = 1U;
            if (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__link_read_go) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__link_read_act = 0U;
                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_state = 0xaU;
            }
        } else if ((0xaU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_state))) {
            if (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_done_s) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_act = 1U;
                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_state = 0xbU;
            }
        } else if ((0xbU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_state))) {
            if (((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__j__DOT__stage_3)) 
                 & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_out_done_s))) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_act = 0U;
                if (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_stop) {
                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_in_stop = 0U;
                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_stop = 0U;
                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_block = 0U;
                }
                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__link_read_stop = 1U;
                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_state = 0xeU;
            }
        } else {
            if ((1U & (~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__link_read_go)))) {
                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_state = 0xcU;
            }
            if (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_stop) {
                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_in_stop = 0U;
                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_stop = 0U;
                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_block = 0U;
                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__link_read_stop = 1U;
                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_state = 0xcU;
            }
        }
    } else if ((0xcU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_state))) {
        if ((1U & (~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_out_busy_s)))) {
            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state = 4U;
            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_state = 1U;
            if (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_block) {
                if ((1U == vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__link_read_num)) {
                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_block = 0U;
                } else {
                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state 
                        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state;
                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__link_read_addr 
                        = ((IData)(1U) + vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__link_read_addr);
                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__link_read_num 
                        = (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__link_read_num 
                           - (IData)(1U));
                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__block_read_byteaddr 
                        = ((IData)(0x200U) + vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__block_read_byteaddr);
                    if ((0x8000U <= vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__link_read_addr)) {
                        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status 
                            = (0x80000000U | vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status);
                        vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__err_op_out_range = 1U;
                    }
                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_state = 2U;
                }
            }
        }
    } else if ((0x14U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_state))) {
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_in_act = 1U;
        if (((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__f__DOT__stage_3)) 
             & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_in_done_s))) {
            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status 
                = (0xfffffeffU & vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status);
            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_state = 0x15U;
        }
        if (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_in_stop) {
            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state = 4U;
            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_in_act = 0U;
            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_state = 0x16U;
        }
    } else if ((0x15U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_state))) {
        if (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_in_crc_good_s) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__link_write_act = 1U;
            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state = 7U;
            if (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__link_write_done) {
                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state = 6U;
                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_in_act = 0U;
                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__link_write_act = 0U;
                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_state = 0x16U;
            }
        } else {
            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state = 4U;
            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_in_act = 0U;
            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_recv_block = 0U;
            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_state = 1U;
        }
    } else if ((0x16U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_state))) {
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_in_another = 0U;
        if ((1U & (~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_in_busy_s)))) {
            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_in_gap_cnt = 0U;
            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_state = 0x17U;
        }
    } else if ((0x17U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_state))) {
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_blocks_written 
            = ((IData)(1U) + vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_blocks_written);
        if (((1U == vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__link_write_num) 
             | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_in_stop))) {
            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state = 4U;
            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_in_stop = 0U;
            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_stop = 0U;
            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_in_another = 0U;
            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_recv_block = 0U;
            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_state = 1U;
        } else {
            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__link_write_addr 
                = ((IData)(1U) + vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__link_write_addr);
            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__link_write_num 
                = (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__link_write_num 
                   - (IData)(1U));
            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__block_write_byteaddr 
                = ((IData)(0x200U) + vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__block_write_byteaddr);
            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state = 6U;
            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status 
                = (0x100U | vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status);
            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_in_act = 0U;
            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_in_another = 0U;
            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_in_gap_cnt = 0U;
            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_state = 0x18U;
        }
    } else if ((0x18U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_state))) {
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_in_act = 0U;
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_in_another = 0U;
        if ((0x3ffU <= (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_in_gap_cnt))) {
            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_state = 0x14U;
            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_in_gap_cnt = 0U;
        } else {
            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_in_gap_cnt 
                = (0x3ffU & ((IData)(1U) + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_in_gap_cnt)));
        }
    }
    if ((1U & (~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__reset_s)))) {
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_mode_spi = 0U;
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__state = 0U;
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_state = 0U;
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_mode_crc_disable = 0U;
    }
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__link_read_go 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__link_read_go;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__link_write_done 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__link_write_done;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_in_gap_cnt 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_in_gap_cnt;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_erase_state 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_erase_state;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_blocks_written 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_blocks_written;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_appcmd 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_appcmd;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_rca 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_rca;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_ocr 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_ocr;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_cid[0U] 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_cid[0U];
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_cid[1U] 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_cid[1U];
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_cid[2U] 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_cid[2U];
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_cid[3U] 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_cid[3U];
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_csd[0U] 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_csd[0U];
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_csd[1U] 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_csd[1U];
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_csd[2U] 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_csd[2U];
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_csd[3U] 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_csd[3U];
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_scr 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_scr;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_sd_status[0U] 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_sd_status[0U];
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_sd_status[1U] 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_sd_status[1U];
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_sd_status[2U] 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_sd_status[2U];
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_sd_status[3U] 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_sd_status[3U];
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function_caps[0U] 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function_caps[0U];
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function_caps[1U] 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function_caps[1U];
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function_caps[2U] 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function_caps[2U];
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function_caps[3U] 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function_caps[3U];
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function_check 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function_check;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_scr 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_scr;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_cid 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_cid;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_csd 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_csd;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_sdstatus 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_sdstatus;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_function 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_function;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_written 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_written;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_block 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_block;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_block_queue 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_block_queue;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_recv_block 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_recv_block;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__link_read_num 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__link_read_num;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__link_write_num 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__link_write_num;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__state 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__state;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state_next 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state_next;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_type 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_type;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__block_write_byteaddr 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__block_write_byteaddr;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__block_read_byteaddr 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__block_read_byteaddr;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_arg[0U] 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_arg[0U];
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_arg[1U] 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_arg[1U];
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_arg[2U] 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_arg[2U];
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_arg[3U] 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_arg[3U];
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_state 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_state;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_type 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_type;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_stop 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_stop;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__link_read_addr 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__link_read_addr;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__link_write_addr 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__link_write_addr;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__spi_status_word 
        = (((((IData)((0U != (0x68000000U & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status))) 
              << 0xeU) | ((0x2000U & (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status 
                                      >> 0x11U)) | 
                          (0x1000U & (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status 
                                      >> 0x10U)))) 
            | (((0xc00U & (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status 
                           >> 0xcU)) | (0x200U & (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status 
                                                  >> 4U))) 
               | (((0U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state)) 
                   << 8U) | ((IData)((0U != (0x80010000U 
                                             & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status))) 
                             << 7U)))) | ((0x60U & 
                                           (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status 
                                            >> 0x15U)) 
                                          | ((0x1cU 
                                              & (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status 
                                                 >> 0x11U)) 
                                             | (((IData)(
                                                         (0U 
                                                          != 
                                                          (0x1008000U 
                                                           & vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status))) 
                                                 << 1U) 
                                                | (1U 
                                                   & (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status 
                                                      >> 0x19U))))));
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd 
        = (0x3fU & (IData)((vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_latch 
                            >> 0x28U)));
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_arg 
        = (IData)((vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_latch 
                   >> 8U));
    if (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__host_hc_support) {
        vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_arg_blockaddr 
            = (IData)((vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_latch 
                       >> 8U));
        vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_arg_byteaddr 
            = VL_SHIFTL_III(32,32,32, (IData)((vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_latch 
                                               >> 8U)), 9U);
    } else {
        vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_arg_blockaddr 
            = VL_SHIFTR_III(32,32,32, (IData)((vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_latch 
                                               >> 8U)), 9U);
        vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_arg_byteaddr 
            = (IData)((vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_latch 
                       >> 8U));
    }
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_out_busy_s 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__i__DOT__stage_2;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__h__DOT__stage_3 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_done_s;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__j__DOT__stage_3 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_out_done_s;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__reset_s 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__a__DOT__stage_2;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__spi_sel_s 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__k__DOT__stage_2;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_in_crc_good_s 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__g__DOT__stage_2;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_in_busy_s 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__e__DOT__stage_2;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__f__DOT__stage_3 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_in_done_s;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_crc_good_s 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__c__DOT__stage_2;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__d__DOT__stage_3 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_act_s;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__i__DOT__stage_2 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__i__DOT__stage_1;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_done_s 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__h__DOT__stage_2;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_out_done_s 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__j__DOT__stage_2;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__a__DOT__stage_2 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__a__DOT__stage_1;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__k__DOT__stage_2 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__k__DOT__stage_1;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__g__DOT__stage_2 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__g__DOT__stage_1;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__e__DOT__stage_2 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__e__DOT__stage_1;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_in_done_s 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__f__DOT__stage_2;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__c__DOT__stage_2 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__c__DOT__stage_1;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_act_s 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__d__DOT__stage_2;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__i__DOT__stage_1 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_busy;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__h__DOT__stage_2 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__h__DOT__stage_1;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__j__DOT__stage_2 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__j__DOT__stage_1;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__a__DOT__stage_1 
        = vlSelfRef.rst_n;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__k__DOT__stage_1 
        = (1U & ((~ ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_oe) 
                     >> 3U)) & (~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__spi_cs))));
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__g__DOT__stage_1 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_in_crc_good;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__e__DOT__stage_1 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_in_busy;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__f__DOT__stage_2 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__f__DOT__stage_1;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__c__DOT__stage_1 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_cmd_in_crc_good;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__d__DOT__stage_2 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__d__DOT__stage_1;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__h__DOT__stage_1 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_done;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__j__DOT__stage_1 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_done;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__f__DOT__stage_1 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_in_done;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__d__DOT__stage_1 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_cmd_in_act;
}

VL_INLINE_OPT void Vtb_top___024root___nba_sequent__TOP__10(Vtb_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_top___024root___nba_sequent__TOP__10\n"); );
    Vtb_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_rd_sd_addr 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_rd_sd_addr;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_busy 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_busy;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_done 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_done;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_done 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_done;
}

VL_INLINE_OPT void Vtb_top___024root___nba_sequent__TOP__11(Vtb_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_top___024root___nba_sequent__TOP__11\n"); );
    Vtb_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    SData/*10:0*/ __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__idc;
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__idc = 0;
    SData/*15:0*/ __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__didc;
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__didc = 0;
    // Body
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__idc 
        = (0x7ffU & ((IData)(1U) + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__idc)));
    if ((0x40U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__istate))) {
        if ((0x20U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__istate))) {
            if ((0x10U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__istate))) {
                if ((8U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__istate))) {
                    if ((4U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__istate))) {
                        if ((2U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__istate))) {
                            if ((1U & (~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__istate)))) {
                                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__istate = 0U;
                            }
                        } else {
                            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__istate = 0U;
                        }
                    } else {
                        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__istate = 0U;
                    }
                } else {
                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__istate = 0U;
                }
            } else {
                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__istate = 0U;
            }
        } else {
            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__istate = 0U;
        }
    } else if ((0x20U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__istate))) {
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__istate = 0U;
    } else if ((0x10U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__istate))) {
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__istate = 0U;
    } else if ((8U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__istate))) {
        if ((4U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__istate))) {
            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__istate = 0U;
        } else if ((2U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__istate))) {
            if ((1U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__istate))) {
                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__cmd_in_latch 
                    = ((0x7ffffffffffeULL & (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__cmd_in_latch 
                                             << 1U)) 
                       | (QData)((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_cmd_io)));
                if ((0x27U > (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__idc))) {
                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc7_in 
                        = (((0x70U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc7_in) 
                                      << 1U)) | (8U 
                                                 & ((VL_REDXOR_8(
                                                                 (0x44U 
                                                                  & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc7_in))) 
                                                     ^ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_cmd_io)) 
                                                    << 3U))) 
                           | ((6U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc7_in) 
                                     << 1U)) | ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_cmd_io) 
                                                ^ ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc7_in) 
                                                   >> 6U))));
                }
                if ((0x2dU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__idc))) {
                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__istate = 0xaU;
                }
            } else {
                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_cmd_in_crc_good 
                    = ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__mode_crc_disable_s) 
                       | ((0x7fU & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__cmd_in_latch)) 
                          == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc7_in)));
                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__istate = 4U;
                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_cmd_in 
                    = ((vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__cmd_in_latch 
                        << 1U) | (QData)((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_cmd_io)));
                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_cmd_in_act 
                    = (1U & (IData)((vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__cmd_in_latch 
                                     >> 0x2dU)));
            }
        } else {
            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__istate = 0U;
        }
    } else if ((4U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__istate))) {
        if ((2U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__istate))) {
            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__istate = 0U;
        } else if ((1U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__istate))) {
            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__istate = 0U;
        } else if ((1U & ((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__mode_spi_s)) 
                          | (~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_in_busy))))) {
            if ((((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_cmd_io)) 
                  & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_cmd_last)) 
                 & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__mode_spi_s) 
                    | (~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_cmd_oe))))) {
                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__cmd_in_latch = 0ULL;
                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc7_in = 0U;
                vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_cmd_in_act = 0U;
                __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__idc = 0U;
                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__istate = 0xbU;
            }
        }
    } else if ((2U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__istate))) {
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__istate = 0U;
    } else if ((1U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__istate))) {
        if ((0x3cU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__idc))) {
            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__istate = 4U;
        }
    } else {
        __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__idc = 0U;
        if (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_cmd_io) {
            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__istate = 1U;
        }
    }
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__idc 
        = __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__idc;
    if ((1U & (~ (IData)(vlSelfRef.rst_n)))) {
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__istate = 0U;
    }
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__istate 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__istate;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__cmd_in_latch 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__cmd_in_latch;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc7_in 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc7_in;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_cmd_last 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_cmd_io;
    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__didc 
        = (0xffffU & ((IData)(1U) + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__didc)));
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_wr_sd_wren = 0U;
    if ((0x40U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__distate))) {
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__distate = 0U;
    } else if ((0x20U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__distate))) {
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__distate = 0U;
    } else if ((0x10U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__distate))) {
        if ((8U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__distate))) {
            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__distate = 0U;
        } else if ((4U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__distate))) {
            if ((2U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__distate))) {
                if ((1U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__distate))) {
                    if ((1U & (~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_in_act_s)))) {
                        vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__do_crc_token = 0U;
                        vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_in_busy = 0U;
                        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__distate = 4U;
                    }
                } else {
                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_check3 
                        = ((0xfffeU & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_check3) 
                                       << 1U)) | (1U 
                                                  & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io) 
                                                     >> 3U)));
                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_check2 
                        = ((0xfffeU & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_check2) 
                                       << 1U)) | (1U 
                                                  & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io) 
                                                     >> 2U)));
                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_check1 
                        = ((0xfffeU & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_check1) 
                                       << 1U)) | (1U 
                                                  & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io) 
                                                     >> 1U)));
                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_check0 
                        = ((0xfffeU & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_check0) 
                                       << 1U)) | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_spi_dat));
                    if (((0x10U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__didc)) 
                         | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_in_stop_s))) {
                        vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_in_done = 1U;
                        vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_in_crc_good 
                            = ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__mode_crc_disable_s) 
                               || ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__mode_4bit_s)
                                    ? ((((QData)((IData)(
                                                         (((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_check3) 
                                                           << 0x10U) 
                                                          | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_check2)))) 
                                         << 0x20U) 
                                        | (QData)((IData)(
                                                          (((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_check1) 
                                                            << 0x10U) 
                                                           | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_check0))))) 
                                       == (((QData)((IData)(
                                                            (((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_in3) 
                                                              << 0x10U) 
                                                             | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_in2)))) 
                                            << 0x20U) 
                                           | (QData)((IData)(
                                                             (((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_in1) 
                                                               << 0x10U) 
                                                              | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_in0))))))
                                    : ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_check0) 
                                       == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_in0))));
                        __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__didc = 0U;
                        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__distate = 0x17U;
                        vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__do_crc_token 
                            = (1U & (~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_in_stop_s)));
                    }
                }
            } else if ((1U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__distate))) {
                if (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__mode_4bit_s) {
                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_wr_sd_data 
                        = ((vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_wr_sd_data 
                            << 4U) | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io));
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_in3 
                        = ((0xfffeU & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_in3) 
                                       << 1U)) ^ ((IData)(
                                                          (((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io) 
                                                            >> 3U) 
                                                           ^ 
                                                           ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_in3) 
                                                            >> 0xfU)))
                                                   ? 0x1021U
                                                   : 0U));
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_in2 
                        = ((0xfffeU & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_in2) 
                                       << 1U)) ^ ((1U 
                                                   & (((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io) 
                                                       >> 2U) 
                                                      ^ 
                                                      ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_in2) 
                                                       >> 0xfU)))
                                                   ? 0x1021U
                                                   : 0U));
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_in1 
                        = ((0xfffeU & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_in1) 
                                       << 1U)) ^ ((1U 
                                                   & (((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io) 
                                                       >> 1U) 
                                                      ^ 
                                                      ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_in1) 
                                                       >> 0xfU)))
                                                   ? 0x1021U
                                                   : 0U));
                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_in0 
                        = ((0xfffeU & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_in0) 
                                       << 1U)) ^ ((1U 
                                                   & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io) 
                                                      ^ 
                                                      ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_in0) 
                                                       >> 0xfU)))
                                                   ? 0x1021U
                                                   : 0U));
                    if ((0U == (7U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__didc)))) {
                        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_wr_sd_addr 
                            = (0x7fU & ((IData)(1U) 
                                        + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_wr_sd_addr)));
                    }
                    if ((7U == (7U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__didc)))) {
                        vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_wr_sd_wren = 1U;
                    }
                } else {
                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_wr_sd_data 
                        = ((vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_wr_sd_data 
                            << 1U) | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_spi_dat));
                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_in0 
                        = ((0xfffeU & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_in0) 
                                       << 1U)) ^ (((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_spi_dat) 
                                                   ^ 
                                                   ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_in0) 
                                                    >> 0xfU))
                                                   ? 0x1021U
                                                   : 0U));
                    if ((0U == (0x1fU & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__didc)))) {
                        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_wr_sd_addr 
                            = (0x7fU & ((IData)(1U) 
                                        + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_wr_sd_addr)));
                    }
                    if ((0x1fU == (0x1fU & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__didc)))) {
                        vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_wr_sd_wren = 1U;
                    }
                }
                if ((0x200U == ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__mode_4bit_s)
                                 ? VL_SHIFTR_III(32,32,32, 
                                                 ((IData)(1U) 
                                                  + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__didc)), 1U)
                                 : VL_SHIFTR_III(32,32,32, 
                                                 ((IData)(1U) 
                                                  + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__didc)), 3U)))) {
                    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__didc = 0U;
                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__distate = 0x16U;
                }
                if (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_in_stop_s) {
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_in_done = 1U;
                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__distate = 0x17U;
                }
            } else if (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__mode_spi_s) {
                vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__spi_di_token 
                    = ((0xfeU & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__spi_di_token) 
                                 << 1U)) | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_cmd_io));
                if ((7U == (7U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__spi_cnt)))) {
                    if (((0xfeU == ((0xfeU & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__spi_di_token) 
                                              << 1U)) 
                                    | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_cmd_io))) 
                         || (0xfcU == ((0xfeU & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__spi_di_token) 
                                                 << 1U)) 
                                       | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_cmd_io))))) {
                        vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_in3 = 0U;
                        vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_in2 = 0U;
                        vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_in1 = 0U;
                        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_in0 = 0U;
                        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_wr_sd_addr = 0x7fU;
                        __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__didc = 0U;
                        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__distate = 0x15U;
                    } else if ((0xfdU == ((0xfeU & 
                                           ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__spi_di_token) 
                                            << 1U)) 
                                          | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_cmd_io)))) {
                        vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_in_done = 1U;
                        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__distate = 0x17U;
                    }
                }
            } else {
                if ((((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io)) 
                      & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_last)) 
                     & (~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_oe)))) {
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_in3 = 0U;
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_in2 = 0U;
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_in1 = 0U;
                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_in0 = 0U;
                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_wr_sd_addr = 0x7fU;
                    __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__didc = 0U;
                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__distate = 0x15U;
                }
                if (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_in_stop_s) {
                    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_in_done = 1U;
                    vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__distate = 0x17U;
                }
            }
        } else {
            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__distate = 0U;
        }
    } else if ((8U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__distate))) {
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__distate = 0U;
    } else if ((4U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__distate))) {
        if ((2U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__distate))) {
            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__distate = 0U;
        } else if ((1U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__distate))) {
            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__distate = 0U;
        } else if ((((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__a__DOT__stage_3)) 
                     & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_in_act_s)) 
                    | ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_in_another_s) 
                       & (~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_in_stop_s))))) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_in_busy = 1U;
            vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_in_done = 0U;
            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__spi_di_token = 0U;
            vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__distate = 0x14U;
        }
    } else if ((2U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__distate))) {
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__distate = 0U;
    } else if ((1U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__distate))) {
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__distate = 0U;
    } else {
        vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__do_crc_token = 0U;
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__distate = 4U;
        vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_in_busy = 0U;
        vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_in_done = 0U;
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__spi_di_token = 0U;
    }
    if ((1U & (~ (IData)(vlSelfRef.rst_n)))) {
        vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__distate = 0U;
    }
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__didc 
        = __Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__didc;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__distate 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__distate;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_check3 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_check3;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_check2 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_check2;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_check1 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_check1;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_check0 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_check0;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_in0 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_in0;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__spi_di_token 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__spi_di_token;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_wr_sd_data 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_wr_sd_data;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_wr_sd_addr 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_wr_sd_addr;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_last 
        = (1U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io));
}

VL_INLINE_OPT void Vtb_top___024root___nba_comb__TOP__0(Vtb_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_top___024root___nba_comb__TOP__0\n"); );
    Vtb_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_src) {
        vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dout_4bit 
            = (0xfU & ((vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xfU] 
                        << 4U) | (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xfU] 
                                  >> 0x1cU)));
        vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dout_1bit 
            = (1U & (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch[0xfU] 
                     >> 0x1fU));
    } else {
        vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dout_4bit 
            = (0xfU & (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dout_buf 
                       >> 0x1cU));
        vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dout_1bit 
            = (1U & (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dout_buf 
                     >> 0x1fU));
    }
}

VL_INLINE_OPT void Vtb_top___024root___nba_sequent__TOP__12(Vtb_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_top___024root___nba_sequent__TOP__12\n"); );
    Vtb_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_cmd_oe 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_cmd_oe;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_oe 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_oe;
}

VL_INLINE_OPT void Vtb_top___024root___nba_sequent__TOP__13(Vtb_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_top___024root___nba_sequent__TOP__13\n"); );
    Vtb_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__spi_cnt 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__spi_cnt;
}

VL_INLINE_OPT void Vtb_top___024root___nba_sequent__TOP__14(Vtb_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_top___024root___nba_sequent__TOP__14\n"); );
    Vtb_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_in_another_s 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__j__DOT__stage_2;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__mode_crc_disable_s 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__l__DOT__stage_2;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__mode_4bit_s 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__h__DOT__stage_2;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_in_stop_s 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__i__DOT__stage_2;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__a__DOT__stage_3 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_in_act_s;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__mode_spi_s 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__k__DOT__stage_2;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__j__DOT__stage_2 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__j__DOT__stage_1;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__l__DOT__stage_2 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__l__DOT__stage_1;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__h__DOT__stage_2 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__h__DOT__stage_1;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__i__DOT__stage_2 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__i__DOT__stage_1;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_in_act_s 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__a__DOT__stage_2;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__k__DOT__stage_2 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__k__DOT__stage_1;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__j__DOT__stage_1 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_in_another;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__l__DOT__stage_1 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_mode_crc_disable;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__h__DOT__stage_1 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_mode_4bit;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__i__DOT__stage_1 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_in_stop;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__a__DOT__stage_2 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__a__DOT__stage_1;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__k__DOT__stage_1 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_mode_spi;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__a__DOT__stage_1 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_in_act;
}

VL_INLINE_OPT void Vtb_top___024root___nba_comb__TOP__1(Vtb_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_top___024root___nba_comb__TOP__1\n"); );
    Vtb_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__bram_wr_sd_addr_sel 
        = (0xffU & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__sel_wr_sd)
                     ? ((IData)(0x80U) + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_wr_sd_addr))
                     : (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_wr_sd_addr)));
}

VL_INLINE_OPT void Vtb_top___024root___nba_comb__TOP__2(Vtb_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_top___024root___nba_comb__TOP__2\n"); );
    Vtb_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_cmd_io = 
        (1U & ((~ ((~ ((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_cmd_oe)) 
                       | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__mode_spi_s))) 
                   | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__tristate_enable_flop))) 
               | (((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__tristate_enable_flop) 
                   & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__cmd_bit_flop)) 
                  | ((~ ((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_cmd_oe)) 
                         | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__mode_spi_s))) 
                     & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_cmd_out)))));
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__timeout_reset = 0U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__resp_NXT 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__resp_CRT;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__bit_increment = 0U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7_compute = 0U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7_data = 0U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7_NXT 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7_CRT;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_NXT 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT;
    if (((((((((0U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT)) 
               | (1U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) 
              | (2U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) 
             | (3U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) 
            | (4U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) 
           | (5U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) 
          | (6U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) 
         | (7U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT)))) {
        if ((0U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__timeout_reset = 1U;
            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__resp_NXT = 0U;
            if ((0x200U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register))) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_NXT = 1U;
            }
        } else if ((1U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
            if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__shift) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_NXT = 2U;
            }
        } else if ((2U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
            if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__shift) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_NXT = 3U;
            }
        } else if ((3U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
            if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__shift) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_NXT = 4U;
            }
        } else if ((4U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
            if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__shift) {
                if ((5U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__bit_counter))) {
                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_NXT = 5U;
                }
            }
        } else if ((5U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
            if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__shift) {
                if ((0x1fU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__bit_counter))) {
                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_NXT = 6U;
                }
            }
        } else if ((6U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
            if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__shift) {
                if ((6U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__bit_counter))) {
                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_NXT = 7U;
                }
            }
        } else if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__shift) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_NXT 
                = ((0U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__cmd_number))
                    ? 0U : 8U);
        }
        if ((0U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
            if ((1U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
                if ((2U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
                    if ((3U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
                        if ((4U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
                            if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__shift) {
                                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__bit_increment = 1U;
                            }
                        } else if ((5U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
                            if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__shift) {
                                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__bit_increment = 1U;
                            }
                        } else if ((6U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
                            if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__shift) {
                                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__bit_increment = 1U;
                            }
                        }
                        if ((4U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
                            if ((5U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
                                if ((6U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
                                    if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__shift) {
                                        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7_NXT 
                                            = (0x7fU 
                                               & ((0U 
                                                   == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__bit_counter))
                                                   ? 
                                                  VL_SHIFTL_III(7,7,32, (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7__DOT__crc), 1U)
                                                   : 
                                                  VL_SHIFTL_III(7,7,32, (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7_CRT), 1U)));
                                    }
                                }
                            }
                        }
                    }
                }
                if ((2U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
                    if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__shift) {
                        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7_compute = 1U;
                        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7_data = 0U;
                    }
                } else if ((3U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
                    if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__shift) {
                        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7_compute = 1U;
                        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7_data = 1U;
                    }
                } else if ((4U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
                    if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__shift) {
                        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7_compute = 1U;
                        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7_data 
                            = (1U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__cmd_number_CRT) 
                                     >> 5U));
                    }
                } else if ((5U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
                    if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__shift) {
                        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7_compute = 1U;
                        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7_data 
                            = (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__cmd_argument_CRT 
                               >> 0x1fU);
                    }
                }
            }
        }
    } else {
        if ((8U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
            if ((0x2faf080U == vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__timeout_counter)) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_NXT = 0xdU;
            }
            if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__sample) {
                if ((1U & (~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_cmd_io)))) {
                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__timeout_reset = 1U;
                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__resp_NXT 
                        = ((0xfeU & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__resp_CRT) 
                                     << 1U)) | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_cmd_io));
                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__bit_increment = 1U;
                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7_compute = 1U;
                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7_data 
                        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_cmd_io;
                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_NXT = 9U;
                }
            }
        } else if ((9U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
            if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__sample) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__resp_NXT 
                    = ((0xfeU & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__resp_CRT) 
                                 << 1U)) | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_cmd_io));
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__bit_increment = 1U;
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7_compute = 1U;
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7_data 
                    = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_cmd_io;
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_NXT 
                    = ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__rcv136_CRT)
                        ? 0xbU : 0xaU);
            }
        } else if ((0xaU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
            if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__sample) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__resp_NXT 
                    = ((0xfeU & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__resp_CRT) 
                                 << 1U)) | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_cmd_io));
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__bit_increment = 1U;
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7_compute 
                    = (0x28U > (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__bit_counter));
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7_data 
                    = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_cmd_io;
                if ((0x2fU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__bit_counter))) {
                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_NXT 
                        = (((0x3aU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__cmd_number)) 
                            | (((0x29U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__cmd_number)) 
                                & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__card_app_cmd_CRT)) 
                               | (1U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__cmd_number))))
                            ? 0xdU : 0xcU);
                }
            }
        } else if ((0xbU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
            if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__sample) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__resp_NXT 
                    = ((0xfeU & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__resp_CRT) 
                                 << 1U)) | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_cmd_io));
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__bit_increment = 1U;
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7_compute 
                    = ((0x80U > (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__bit_counter)) 
                       & (8U <= (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__bit_counter)));
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7_data 
                    = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_cmd_io;
                if ((0x87U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__bit_counter))) {
                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_NXT = 0xcU;
                }
            }
        } else {
            if ((0xcU != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
                if ((0xdU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__bit_increment 
                        = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__shift;
                }
            }
            if ((0xcU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
                if (((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7_CRT) 
                     != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7__DOT__crc))) {
                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_NXT = 0U;
                }
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_NXT = 0xdU;
            } else if ((0xdU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
                if ((0x10U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__bit_counter))) {
                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_NXT = 0U;
                }
            }
        }
        if ((8U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
            if ((9U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
                if ((0xaU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
                    if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__sample) {
                        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7_NXT 
                            = ((0x7eU & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7_CRT) 
                                         << 1U)) | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_cmd_io));
                        if ((0x2fU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__bit_counter))) {
                            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7_NXT 
                                = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7_CRT;
                        }
                    }
                } else if ((0xbU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
                    if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__sample) {
                        if (((0x80U <= (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__bit_counter)) 
                             & (0x87U > (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__bit_counter)))) {
                            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7_NXT 
                                = ((0x7eU & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7_CRT) 
                                             << 1U)) 
                                   | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_cmd_io));
                        }
                        if ((0x87U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__bit_counter))) {
                            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7_NXT 
                                = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7_CRT;
                        }
                    }
                }
            }
        }
    }
}

VL_INLINE_OPT void Vtb_top___024root___nba_comb__TOP__3(Vtb_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_top___024root___nba_comb__TOP__3\n"); );
    Vtb_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_dat_o = ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__mode_spi_s)
                                                    ? 
                                                   (1U 
                                                    & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_cmd_oe)
                                                        ? (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_cmd_out)
                                                        : 
                                                       ((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_oe)) 
                                                        | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_out))))
                                                    : (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_out));
}

VL_INLINE_OPT void Vtb_top___024root___nba_sequent__TOP__15(Vtb_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_top___024root___nba_sequent__TOP__15\n"); );
    Vtb_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_in_another 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_in_another;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_mode_crc_disable 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_mode_crc_disable;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_mode_4bit 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_mode_4bit;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_in_stop 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_in_stop;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_mode_spi 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_mode_spi;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_in_act 
        = vlSelfRef.__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_in_act;
}

VL_INLINE_OPT void Vtb_top___024root___nba_comb__TOP__4(Vtb_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_top___024root___nba_comb__TOP__4\n"); );
    Vtb_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io__strong__out5 
        = ((0xeU & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io__strong__out5)) 
           | (1U & ((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_dat_t)) 
                    & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_dat_o))));
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io__strong__out6 
        = ((0xdU & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io__strong__out6)) 
           | (2U & (((~ ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_dat_t) 
                         >> 1U)) << 1U) & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_dat_o))));
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io__strong__out7 
        = ((0xbU & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io__strong__out7)) 
           | (4U & (((~ ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_dat_t) 
                         >> 2U)) << 2U) & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_dat_o))));
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io__strong__out8 
        = ((7U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io__strong__out8)) 
           | ((IData)(((~ ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_dat_t) 
                           >> 3U)) & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_dat_o) 
                                      >> 3U))) << 3U));
}

VL_INLINE_OPT void Vtb_top___024root___nba_comb__TOP__5(Vtb_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_top___024root___nba_comb__TOP__5\n"); );
    Vtb_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    CData/*0:0*/ tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT____VdfgRegularize_h6e830a37_0_1;
    tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT____VdfgRegularize_h6e830a37_0_1 = 0;
    CData/*0:0*/ tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT____VdfgRegularize_h6e830a37_0_2;
    tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT____VdfgRegularize_h6e830a37_0_2 = 0;
    CData/*0:0*/ tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT____VdfgRegularize_h6e830a37_0_3;
    tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT____VdfgRegularize_h6e830a37_0_3 = 0;
    CData/*0:0*/ tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT____VdfgRegularize_h6e830a37_0_4;
    tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT____VdfgRegularize_h6e830a37_0_4 = 0;
    // Body
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io = 
        (0xfU & ((~ (((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__tristate_enable_flop)
                       ? 0xfU : 0U) | (((1U & (~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_dat_t))) 
                                        | (2U & ((~ 
                                                  ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_dat_t) 
                                                   >> 1U)) 
                                                 << 1U))) 
                                       | ((4U & ((~ 
                                                  ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_dat_t) 
                                                   >> 2U)) 
                                                 << 2U)) 
                                          | (8U & (
                                                   (~ 
                                                    ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_dat_t) 
                                                     >> 3U)) 
                                                   << 3U)))))) 
                 | ((((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__tristate_enable_flop)
                       ? 0xfU : 0U) & (((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__tristate_enable_flop)
                                         ? 0xfU : 0U) 
                                       & (((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__tristate_enable_flop)
                                            ? (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_line_flop)
                                            : 0U) & 
                                          ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__tristate_enable_flop)
                                            ? 0xfU : 0U)))) 
                    | (((1U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io__strong__out5) 
                               & (~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_dat_t)))) 
                        | (2U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io__strong__out6) 
                                 & ((~ ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_dat_t) 
                                        >> 1U)) << 1U)))) 
                       | ((4U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io__strong__out7) 
                                 & ((~ ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_dat_t) 
                                        >> 2U)) << 2U))) 
                          | (8U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io__strong__out8) 
                                   & ((~ ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_dat_t) 
                                          >> 3U)) << 3U))))))));
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_timeout = 0U;
    if ((1U & (~ ((((((((0U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT)) 
                        | (1U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) 
                       | (4U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) 
                      | (5U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) 
                     | (6U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) 
                    | (0xeU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) 
                   | (2U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) 
                  | (3U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT)))))) {
        if ((7U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
            if ((8U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                if ((9U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                    if ((0xbU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                        if ((1U & (~ ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__sample) 
                                      & (~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io)))))) {
                            if ((0xf4240U <= vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__timeout_counter)) {
                                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_timeout = 1U;
                            }
                        }
                    } else if ((0xdU != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                        if ((0xaU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                            if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__sample) {
                                if ((1U & (~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io)))) {
                                    if ((0xf4240U <= vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__timeout_counter)) {
                                        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_timeout = 1U;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_compute = 0U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__timeout_reset = 0U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_initialize = 0U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_reset = 0U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__spi_cs 
        = (1U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io) 
                 >> 3U));
    if (vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__mode_spi_s) {
        vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_spi_dat 
            = (1U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_cmd_io));
        vlSelfRef.tb_top__DOT__u_dut__DOT__sd_dat_t 
            = (0xfU & (~ (1U & (~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__spi_cs)))));
    } else {
        vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_spi_dat 
            = (1U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io));
        vlSelfRef.tb_top__DOT__u_dut__DOT__sd_dat_t 
            = (0xfU & ((8U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__card_state_s))
                        ? 0xfU : (~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_oe))));
    }
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_NXT 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_CRT;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_wide_NXT 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_wide_CRT;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_NXT 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_NXT 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_CRT;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_crc_error = 0U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_error = 0U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_data = 0U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_line = 0xfU;
    if (((((((((0U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT)) 
               | (1U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) 
              | (4U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) 
             | (5U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) 
            | (6U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) 
           | (0xeU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) 
          | (2U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) 
         | (3U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT)))) {
        if ((0U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
            if ((1U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__sample) {
                    if ((0x400U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register))) {
                        if ((0U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io))) {
                            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_compute = 1U;
                            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_data 
                                = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io;
                        }
                    } else if ((1U & (~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io)))) {
                        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_compute = 1U;
                        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_data 
                            = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io;
                    }
                }
            } else if ((4U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__sample) {
                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_compute = 1U;
                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_data 
                        = vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io;
                }
            } else if ((5U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                if ((6U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                    if ((0xeU != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                        if ((2U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_line 
                                = ((0x400U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register))
                                    ? 0U : (0xeU & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_line)));
                        }
                    }
                }
            }
            if ((1U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                if ((4U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                    if ((5U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                        if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__sample) {
                            if ((1U & (~ ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register) 
                                          >> 0xaU)))) {
                                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_NXT 
                                    = ((0xfffeU & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_CRT) 
                                                   << 1U)) 
                                       | (1U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io)));
                            }
                            if ((0x400U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register))) {
                                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_wide_NXT 
                                    = ((0xffffffff00000000ULL 
                                        & vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_wide_NXT) 
                                       | (IData)((IData)(
                                                         ((((IData)(
                                                                    (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_wide_CRT 
                                                                     >> 0x10U)) 
                                                            << 0x11U) 
                                                           | (0x10000U 
                                                              & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io) 
                                                                 << 0xfU))) 
                                                          | ((0xfffeU 
                                                              & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_wide_CRT) 
                                                                 << 1U)) 
                                                             | (1U 
                                                                & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io)))))));
                                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_wide_NXT 
                                    = ((0xffffffffULL 
                                        & vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_wide_NXT) 
                                       | ((QData)((IData)(
                                                          ((((IData)(
                                                                     (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_wide_CRT 
                                                                      >> 0x30U)) 
                                                             << 0x11U) 
                                                            | (0x10000U 
                                                               & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io) 
                                                                  << 0xdU))) 
                                                           | ((0xfffeU 
                                                               & ((IData)(
                                                                          (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_wide_CRT 
                                                                           >> 0x20U)) 
                                                                  << 1U)) 
                                                              | (1U 
                                                                 & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io) 
                                                                    >> 2U)))))) 
                                          << 0x20U));
                            }
                        }
                    }
                }
            }
        }
        if ((0U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
            if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_activate) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__timeout_reset = 1U;
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_initialize = 1U;
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_reset = 1U;
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_NXT 
                    = ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_direction)
                        ? 1U : 2U);
            }
        } else {
            if ((1U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__sample) {
                    if ((0x400U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register))) {
                        if ((0U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io))) {
                            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__timeout_reset = 1U;
                            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_NXT = 4U;
                        }
                    } else if ((1U & (~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io)))) {
                        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__timeout_reset = 1U;
                        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_NXT = 4U;
                    }
                }
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_NXT = 0U;
                if ((0x2710U == vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__timeout_counter)) {
                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_NXT = 0U;
                }
            } else {
                if ((4U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                    if ((5U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                        if ((6U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__timeout_reset = 1U;
                        }
                    }
                }
                if ((4U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                    if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__sample) {
                        if ((0x400U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register))) {
                            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_NXT 
                                = ((0xf0U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_CRT) 
                                             << 4U)) 
                                   | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io));
                            if ((0x3ffU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_counter))) {
                                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_NXT = 5U;
                            }
                        } else {
                            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_NXT 
                                = ((0xfeU & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_CRT) 
                                             << 1U)) 
                                   | (1U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io)));
                            if ((0xfffU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_counter))) {
                                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_NXT = 5U;
                            }
                        }
                    }
                } else if ((5U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                    if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__sample) {
                        if ((0xfU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_counter))) {
                            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_NXT = 6U;
                        }
                    }
                } else if ((6U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_crc_error 
                        = ((0x400U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register))
                            ? (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT 
                               != vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_wide_CRT)
                            : ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_CRT) 
                               != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_CRT)));
                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_NXT 
                        = ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__data_burst_CRT)
                            ? 1U : 0U);
                } else if ((0xeU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                    if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__shift) {
                        if ((0xfU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_counter))) {
                            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_NXT = 2U;
                        }
                    }
                } else if ((2U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                    if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__shift) {
                        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_NXT = 3U;
                    }
                } else if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__shift) {
                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_NXT = 7U;
                }
            }
            if ((1U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                if ((4U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                    if ((5U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                        if ((6U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_initialize = 1U;
                        }
                    }
                }
                if ((4U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                    if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__sample) {
                        if ((0x400U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register))) {
                            if ((0x3ffU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_counter))) {
                                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_reset = 1U;
                            }
                        } else if ((0xfffU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_counter))) {
                            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_reset = 1U;
                        }
                    }
                } else if ((5U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                    if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__sample) {
                        if ((0xfU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_counter))) {
                            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_reset = 1U;
                        }
                    }
                } else if ((6U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_reset = 1U;
                } else if ((0xeU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                    if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__shift) {
                        if ((0xfU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_counter))) {
                            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_reset = 1U;
                        }
                    }
                } else if ((2U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_reset = 1U;
                }
            }
        }
    } else {
        if ((7U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_compute 
                = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__shift;
            if ((0x400U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register))) {
                if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__shift) {
                    if ((0x3ffU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_counter))) {
                        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_reset = 1U;
                        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_NXT 
                            = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_CRT;
                        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_NXT = 8U;
                    }
                }
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_line 
                    = (0xfU & ((1U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_counter))
                                ? (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__tx_data_byte)
                                : ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__tx_data_byte) 
                                   >> 4U)));
            } else {
                if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__shift) {
                    if ((0xfffU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_counter))) {
                        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_reset = 1U;
                        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_NXT 
                            = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_CRT;
                        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_NXT = 8U;
                    }
                }
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_line 
                    = ((0xeU & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_line)) 
                       | (1U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__tx_data_byte) 
                                >> (7U & ((IData)(7U) 
                                          - (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_counter))))));
            }
            if ((1U & (~ ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register) 
                          >> 0xaU)))) {
                if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__shift) {
                    if ((0xfffU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_counter))) {
                        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_wide_NXT 
                            = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT;
                    }
                }
            }
            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_data 
                = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_line;
        } else if ((8U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
            if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__shift) {
                if ((0xfU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_counter))) {
                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_reset = 1U;
                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_NXT = 9U;
                }
            }
            if ((0U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_counter))) {
                if ((1U & (~ ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register) 
                              >> 0xaU)))) {
                    if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__shift) {
                        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_NXT 
                            = (0xffffU & VL_SHIFTL_III(16,16,32, (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_CRT), 1U));
                    }
                }
                if ((0x400U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register))) {
                    if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__shift) {
                        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_wide_NXT 
                            = ((0xffffffff00000000ULL 
                                & vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_wide_NXT) 
                               | (IData)((IData)(((
                                                   VL_SHIFTL_III(16,16,32, (IData)(
                                                                                (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT 
                                                                                >> 0x10U)), 1U) 
                                                   << 0x10U) 
                                                  | (0xffffU 
                                                     & VL_SHIFTL_III(16,16,32, (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT), 1U))))));
                        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_wide_NXT 
                            = ((0xffffffffULL & vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_wide_NXT) 
                               | ((QData)((IData)((
                                                   (VL_SHIFTL_III(16,16,32, (IData)(
                                                                                (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT 
                                                                                >> 0x30U)), 1U) 
                                                    << 0x10U) 
                                                   | (0xffffU 
                                                      & VL_SHIFTL_III(16,16,32, (IData)(
                                                                                (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT 
                                                                                >> 0x20U)), 1U))))) 
                                  << 0x20U));
                    }
                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_line 
                        = (((8U & ((IData)((vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT 
                                            >> 0x3fU)) 
                                   << 3U)) | (4U & 
                                              ((IData)(
                                                       (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT 
                                                        >> 0x2fU)) 
                                               << 2U))) 
                           | ((2U & ((IData)((vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT 
                                              >> 0x1fU)) 
                                     << 1U)) | (1U 
                                                & (IData)(
                                                          (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT 
                                                           >> 0xfU)))));
                } else {
                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_line 
                        = ((0xeU & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_line)) 
                           | (1U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_CRT) 
                                    >> 0xfU)));
                }
            } else {
                if ((1U & (~ ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register) 
                              >> 0xaU)))) {
                    if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__shift) {
                        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_NXT 
                            = (0xffffU & VL_SHIFTL_III(16,16,32, (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_CRT), 1U));
                    }
                }
                if ((0x400U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register))) {
                    if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__shift) {
                        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_wide_NXT 
                            = ((0xffffffff00000000ULL 
                                & vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_wide_NXT) 
                               | (IData)((IData)(((
                                                   VL_SHIFTL_III(16,16,32, (IData)(
                                                                                (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_wide_CRT 
                                                                                >> 0x10U)), 1U) 
                                                   << 0x10U) 
                                                  | (0xffffU 
                                                     & VL_SHIFTL_III(16,16,32, (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_wide_CRT), 1U))))));
                        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_wide_NXT 
                            = ((0xffffffffULL & vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_wide_NXT) 
                               | ((QData)((IData)((
                                                   (VL_SHIFTL_III(16,16,32, (IData)(
                                                                                (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_wide_CRT 
                                                                                >> 0x30U)), 1U) 
                                                    << 0x10U) 
                                                   | (0xffffU 
                                                      & VL_SHIFTL_III(16,16,32, (IData)(
                                                                                (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_wide_CRT 
                                                                                >> 0x20U)), 1U))))) 
                                  << 0x20U));
                    }
                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_line 
                        = (((8U & ((IData)((vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_wide_CRT 
                                            >> 0x3fU)) 
                                   << 3U)) | (4U & 
                                              ((IData)(
                                                       (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_wide_CRT 
                                                        >> 0x2fU)) 
                                               << 2U))) 
                           | ((2U & ((IData)((vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_wide_CRT 
                                              >> 0x1fU)) 
                                     << 1U)) | (1U 
                                                & (IData)(
                                                          (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_wide_CRT 
                                                           >> 0xfU)))));
                } else {
                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_line 
                        = ((0xeU & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_line)) 
                           | (1U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_CRT) 
                                    >> 0xfU)));
                }
            }
        } else if ((9U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
            if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__shift) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_reset = 1U;
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_NXT = 0xbU;
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_NXT = 0U;
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_line 
                    = ((0x400U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register))
                        ? 0xfU : (1U | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_line)));
            }
        } else if ((0xbU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
            if (((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__sample) 
                 & (~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io)))) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_reset = 1U;
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_NXT = 0U;
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_NXT = 0xdU;
            } else if ((0xf4240U <= vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__timeout_counter)) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_NXT = 0U;
            }
        } else if ((0xdU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
            if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__sample) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_NXT 
                    = ((0xfeU & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_CRT) 
                                 << 1U)) | (1U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io)));
                if ((3U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_counter))) {
                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_reset = 1U;
                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_crc_error 
                        = (5U == (7U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_NXT) 
                                        >> 1U)));
                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_error 
                        = (6U == (7U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_NXT) 
                                        >> 1U)));
                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_NXT 
                        = ((2U != (7U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_NXT) 
                                         >> 1U))) ? 0U
                            : 0xaU);
                }
            }
        } else if ((0xaU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
            if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__sample) {
                if ((1U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io))) {
                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_reset = 1U;
                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_NXT 
                        = ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__data_burst_CRT)
                            ? 0xeU : 0U);
                } else if ((0xf4240U <= vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__timeout_counter)) {
                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_NXT = 0U;
                }
            }
        }
        if ((7U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
            if ((8U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                if ((9U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                    if ((0xbU != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                        if ((0xdU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                            if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__sample) {
                                if ((3U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_counter))) {
                                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__timeout_reset = 1U;
                                }
                            }
                        } else if ((0xaU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                            if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__sample) {
                                if ((1U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io))) {
                                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__timeout_reset = 1U;
                                }
                            }
                        }
                    }
                }
            }
            if ((8U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__shift) {
                    if ((0xfU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_counter))) {
                        vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_initialize = 1U;
                    }
                }
            } else if ((9U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                if ((0xbU != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                    if ((0xdU != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                        if ((0xaU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                            if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__sample) {
                                if ((1U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io))) {
                                    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_initialize = 1U;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT____VdfgRegularize_h6e830a37_0_5 
        = (1U & (((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_CRT) 
                  >> 0xfU) ^ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_data)));
    tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT____VdfgRegularize_h6e830a37_0_4 
        = (IData)(((vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT 
                    >> 0x3fU) ^ ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_data) 
                                 >> 3U)));
    tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT____VdfgRegularize_h6e830a37_0_3 
        = (1U & ((IData)((vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT 
                          >> 0x2fU)) ^ ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_data) 
                                        >> 2U)));
    tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT____VdfgRegularize_h6e830a37_0_2 
        = (1U & ((IData)((vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT 
                          >> 0x1fU)) ^ ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_data) 
                                        >> 1U)));
    tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT____VdfgRegularize_h6e830a37_0_1 
        = (1U & ((IData)((vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT 
                          >> 0xfU)) ^ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_data)));
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_NXT 
        = (((0xe000U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_CRT) 
                        << 1U)) | (0x1000U & ((0xfffff000U 
                                               & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_CRT) 
                                                  << 1U)) 
                                              ^ ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT____VdfgRegularize_h6e830a37_0_5) 
                                                 << 0xcU)))) 
           | ((0xfc0U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_CRT) 
                         << 1U)) | ((0x20U & ((0xffffffe0U 
                                               & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_CRT) 
                                                  << 1U)) 
                                              ^ ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT____VdfgRegularize_h6e830a37_0_5) 
                                                 << 5U))) 
                                    | ((0x1eU & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_CRT) 
                                                 << 1U)) 
                                       | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT____VdfgRegularize_h6e830a37_0_5)))));
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_NXT 
        = (((QData)((IData)((((((IData)((vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT 
                                         >> 0x3cU)) 
                                << 0x1dU) | (0x10000000U 
                                             & (((IData)(
                                                         (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT 
                                                          >> 0x3bU)) 
                                                 ^ (IData)(tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT____VdfgRegularize_h6e830a37_0_4)) 
                                                << 0x1cU))) 
                              | ((0xfc00000U & ((IData)(
                                                        (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT 
                                                         >> 0x35U)) 
                                                << 0x16U)) 
                                 | ((0x200000U & (((IData)(
                                                           (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT 
                                                            >> 0x34U)) 
                                                   ^ (IData)(tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT____VdfgRegularize_h6e830a37_0_4)) 
                                                  << 0x15U)) 
                                    | ((0x1e0000U & 
                                        ((IData)((vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT 
                                                  >> 0x30U)) 
                                         << 0x11U)) 
                                       | ((IData)(tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT____VdfgRegularize_h6e830a37_0_4) 
                                          << 0x10U))))) 
                             | (((0xe000U & ((IData)(
                                                     (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT 
                                                      >> 0x2cU)) 
                                             << 0xdU)) 
                                 | (0x1000U & (((IData)(
                                                        (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT 
                                                         >> 0x2bU)) 
                                                ^ (IData)(tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT____VdfgRegularize_h6e830a37_0_3)) 
                                               << 0xcU))) 
                                | ((0xfc0U & ((IData)(
                                                      (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT 
                                                       >> 0x25U)) 
                                              << 6U)) 
                                   | ((0x20U & (((IData)(
                                                         (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT 
                                                          >> 0x24U)) 
                                                 ^ (IData)(tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT____VdfgRegularize_h6e830a37_0_3)) 
                                                << 5U)) 
                                      | ((0x1eU & ((IData)(
                                                           (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT 
                                                            >> 0x20U)) 
                                                   << 1U)) 
                                         | (IData)(tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT____VdfgRegularize_h6e830a37_0_3)))))))) 
            << 0x20U) | (QData)((IData)((((((IData)(
                                                    (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT 
                                                     >> 0x1cU)) 
                                            << 0x1dU) 
                                           | (0x10000000U 
                                              & (((IData)(
                                                          (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT 
                                                           >> 0x1bU)) 
                                                  ^ (IData)(tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT____VdfgRegularize_h6e830a37_0_2)) 
                                                 << 0x1cU))) 
                                          | ((0xfc00000U 
                                              & ((IData)(
                                                         (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT 
                                                          >> 0x15U)) 
                                                 << 0x16U)) 
                                             | ((0x200000U 
                                                 & (((IData)(
                                                             (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT 
                                                              >> 0x14U)) 
                                                     ^ (IData)(tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT____VdfgRegularize_h6e830a37_0_2)) 
                                                    << 0x15U)) 
                                                | ((0x1e0000U 
                                                    & ((IData)(
                                                               (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT 
                                                                >> 0x10U)) 
                                                       << 0x11U)) 
                                                   | ((IData)(tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT____VdfgRegularize_h6e830a37_0_2) 
                                                      << 0x10U))))) 
                                         | (((0xe000U 
                                              & ((IData)(
                                                         (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT 
                                                          >> 0xcU)) 
                                                 << 0xdU)) 
                                             | (0x1000U 
                                                & (((IData)(
                                                            (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT 
                                                             >> 0xbU)) 
                                                    ^ (IData)(tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT____VdfgRegularize_h6e830a37_0_1)) 
                                                   << 0xcU))) 
                                            | ((0xfc0U 
                                                & ((IData)(
                                                           (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT 
                                                            >> 5U)) 
                                                   << 6U)) 
                                               | ((0x20U 
                                                   & (((IData)(
                                                               (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT 
                                                                >> 4U)) 
                                                       ^ (IData)(tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT____VdfgRegularize_h6e830a37_0_1)) 
                                                      << 5U)) 
                                                  | ((0x1eU 
                                                      & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT) 
                                                         << 1U)) 
                                                     | (IData)(tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT____VdfgRegularize_h6e830a37_0_1)))))))));
}

void Vtb_top___024root___timing_resume(Vtb_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_top___024root___timing_resume\n"); );
    Vtb_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((0x80ULL & vlSelfRef.__VactTriggered.word(0U))) {
        vlSelfRef.__VdlySched.resume();
    }
}

void Vtb_top___024root___eval_triggers__act(Vtb_top___024root* vlSelf);

bool Vtb_top___024root___eval_phase__act(Vtb_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_top___024root___eval_phase__act\n"); );
    Vtb_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    VlTriggerVec<8> __VpreTriggered;
    CData/*0:0*/ __VactExecute;
    // Body
    Vtb_top___024root___eval_triggers__act(vlSelf);
    __VactExecute = vlSelfRef.__VactTriggered.any();
    if (__VactExecute) {
        __VpreTriggered.andNot(vlSelfRef.__VactTriggered, vlSelfRef.__VnbaTriggered);
        vlSelfRef.__VnbaTriggered.thisOr(vlSelfRef.__VactTriggered);
        Vtb_top___024root___timing_resume(vlSelf);
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
            VL_FATAL_MT("common/tb/tb_top.sv", 3, "", "Input combinational region did not converge.");
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
            VL_FATAL_MT("common/tb/tb_top.sv", 3, "", "NBA region did not converge.");
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
                VL_FATAL_MT("common/tb/tb_top.sv", 3, "", "Active region did not converge.");
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
