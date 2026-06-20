// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtb_top.h for the primary calling header

#include "Vtb_top__pch.h"
#include "Vtb_top__Syms.h"
#include "Vtb_top___024root.h"

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtb_top___024root___dump_triggers__ico(Vtb_top___024root* vlSelf);
#endif  // VL_DEBUG

void Vtb_top___024root___eval_triggers__ico(Vtb_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_top___024root___eval_triggers__ico\n"); );
    Vtb_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__VicoTriggered.set(0U, (IData)(vlSelfRef.__VicoFirstIteration));
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vtb_top___024root___dump_triggers__ico(vlSelf);
    }
#endif
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtb_top___024root___dump_triggers__act(Vtb_top___024root* vlSelf);
#endif  // VL_DEBUG

void Vtb_top___024root___eval_triggers__act(Vtb_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_top___024root___eval_triggers__act\n"); );
    Vtb_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__VactTriggered.set(0U, ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_dat_t) 
                                       != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_top__DOT__u_dut__DOT__sd_dat_t__1)));
    vlSelfRef.__VactTriggered.set(1U, ((IData)(vlSelfRef.clk) 
                                       & (~ (IData)(vlSelfRef.__Vtrigprevexpr___TOP__clk__0))));
    vlSelfRef.__VactTriggered.set(2U, ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__clk_50MHz) 
                                       & (~ (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_top__DOT__u_dut__DOT__clk_50MHz__0))));
    vlSelfRef.__VactTriggered.set(3U, ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__sd_clk) 
                                       & (~ (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_top__DOT__u_dut__DOT__dut__DOT__sd_clk__0))));
    vlSelfRef.__VactTriggered.set(4U, ((~ (IData)(vlSelfRef.rst_n)) 
                                       & (IData)(vlSelfRef.__Vtrigprevexpr___TOP__rst_n__0)));
    vlSelfRef.__VactTriggered.set(5U, ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__spi_cs) 
                                       & (~ (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__spi_cs__0))));
    vlSelfRef.__VactTriggered.set(6U, ((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__sd_clk)) 
                                       & (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_top__DOT__u_dut__DOT__dut__DOT__sd_clk__0)));
    vlSelfRef.__VactTriggered.set(7U, vlSelfRef.__VdlySched.awaitingCurrentTime());
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
    if (VL_UNLIKELY(((1U & (~ (IData)(vlSelfRef.__VactDidInit)))))) {
        vlSelfRef.__VactDidInit = 1U;
        vlSelfRef.__VactTriggered.set(0U, 1U);
    }
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vtb_top___024root___dump_triggers__act(vlSelf);
    }
#endif
}
