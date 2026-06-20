// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtb_top.h for the primary calling header

#include "Vtb_top__pch.h"
#include "Vtb_top___024root.h"

VL_ATTR_COLD void Vtb_top___024root___eval_static__TOP(Vtb_top___024root* vlSelf);
VL_ATTR_COLD void Vtb_top___024root____Vm_traceActivitySetAll(Vtb_top___024root* vlSelf);

VL_ATTR_COLD void Vtb_top___024root___eval_static(Vtb_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_top___024root___eval_static\n"); );
    Vtb_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    Vtb_top___024root___eval_static__TOP(vlSelf);
    Vtb_top___024root____Vm_traceActivitySetAll(vlSelf);
}

VL_ATTR_COLD void Vtb_top___024root___eval_static__TOP(Vtb_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_top___024root___eval_static__TOP\n"); );
    Vtb_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.tb_top__DOT__u_dut__DOT__wbm_dat_i = 0U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__wbm_ack_i = 0U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__clk_50MHz = 0U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__clk_100MHz = 0U;
}

VL_ATTR_COLD void Vtb_top___024root___eval_initial__TOP(Vtb_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_top___024root___eval_initial__TOP\n"); );
    Vtb_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.tb_top__DOT__u_dut__DOT__SD_memory__DOT__unnamedblk1__DOT__i = 0U;
    while (VL_GTS_III(32, 0x400U, vlSelfRef.tb_top__DOT__u_dut__DOT__SD_memory__DOT__unnamedblk1__DOT__i)) {
        vlSelfRef.tb_top__DOT__u_dut__DOT__SD_memory__DOT__mem[(0x3ffU 
                                                                & vlSelfRef.tb_top__DOT__u_dut__DOT__SD_memory__DOT__unnamedblk1__DOT__i)] = 0xaabbccddU;
        vlSelfRef.tb_top__DOT__u_dut__DOT__SD_memory__DOT__unnamedblk1__DOT__i 
            = ((IData)(1U) + vlSelfRef.tb_top__DOT__u_dut__DOT__SD_memory__DOT__unnamedblk1__DOT__i);
    }
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
            VL_FATAL_MT("common/tb/tb_top.sv", 3, "", "Settle region did not converge.");
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
        VL_DBG_MSGF("         'stl' region trigger index 1 is active: @([hybrid] tb_top.u_dut.sd_dat_t)\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD void Vtb_top___024root___stl_sequent__TOP__0(Vtb_top___024root* vlSelf);
VL_ATTR_COLD void Vtb_top___024root___stl_comb__TOP__0(Vtb_top___024root* vlSelf);

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
        Vtb_top___024root___stl_comb__TOP__0(vlSelf);
        Vtb_top___024root____Vm_traceActivitySetAll(vlSelf);
    }
}

VL_ATTR_COLD void Vtb_top___024root___stl_sequent__TOP__0(Vtb_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_top___024root___stl_sequent__TOP__0\n"); );
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
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__timeout_increment = 0U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__tristate_enable = 1U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__timeout_increment = 0U;
    vlSelfRef.write_done_o = vlSelfRef.write_i;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__tx_buffer__DOT__inc_write_ptr 
        = (0x7fU & ((IData)(1U) + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__tx_buffer__DOT__write_ptr)));
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__tx_buffer__DOT__inc_read_ptr 
        = (0x7fU & ((IData)(1U) + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__tx_buffer__DOT__read_ptr)));
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__rx_buffer__DOT__inc_write_ptr 
        = (0x7fU & ((IData)(1U) + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__rx_buffer__DOT__write_ptr)));
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__rx_buffer__DOT__inc_read_ptr 
        = (0x7fU & ((IData)(1U) + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__rx_buffer__DOT__read_ptr)));
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__inc_write_ptr 
        = (0x1fU & ((IData)(1U) + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__write_ptr)));
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__inc_read_ptr 
        = (0x1fU & ((IData)(1U) + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__read_ptr)));
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__bram_rd_ext_addr_next 
        = (0x7fU & ((IData)(1U) + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_rd_ext_addr)));
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd 
        = (0x3fU & (IData)((vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_latch 
                            >> 0x28U)));
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_arg 
        = (IData)((vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_latch 
                   >> 8U));
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__cmd_timeout = 0U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__force_stop = 0U;
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
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_buffer_rx_write 
        = (1U & ((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_rx_write_detector__DOT__previous_signal)) 
                 & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__write_selector) 
                    >> 3U)));
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__bram_rd_ext_addr_sel 
        = (0xffU & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__sel_rd_ext)
                     ? ((IData)(0x80U) + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_rd_ext_addr))
                     : (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_rd_ext_addr)));
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__bram_wr_sd_addr_sel 
        = (0xffU & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__sel_wr_sd)
                     ? ((IData)(0x80U) + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_wr_sd_addr))
                     : (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_wr_sd_addr)));
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__ext_read_go_r 
        = ((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__b__DOT__stage_3)) 
           & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__ext_read_go_s));
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
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__data_burst_NXT 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__data_burst_CRT;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__cmd_crc_error = 0U;
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
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__card_app_cmd_NXT 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__card_app_cmd_CRT;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__rcv136_NXT 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__rcv136_CRT;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__cmd_bit = 1U;
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
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_direction = 0U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_activate = 0U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_cmd_io = 
        (1U & ((~ ((~ ((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_cmd_oe)) 
                       | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__mode_spi_s))) 
                   | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__tristate_enable_flop))) 
               | (((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__tristate_enable_flop) 
                   & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__cmd_bit_flop)) 
                  | ((~ ((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_cmd_oe)) 
                         | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__mode_spi_s))) 
                     & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_cmd_out)))));
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__tx_data_byte 
        = (0xffU & (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT____Vcellout__register_interface__data_tx_data_o 
                    >> (0x1fU & VL_SHIFTL_III(5,5,32, (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__tx_data_selector), 3U))));
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__shift 
        = ((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__sd_clk)) 
           & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__clk_previous));
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__sample 
        = ((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__clk_previous)) 
           & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__sd_clk));
    vlSelfRef.tb_top__DOT__u_dut__DOT__sd_dat_o = ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__mode_spi_s)
                                                    ? 
                                                   (1U 
                                                    & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_cmd_oe)
                                                        ? (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_cmd_out)
                                                        : 
                                                       ((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_oe)) 
                                                        | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_out))))
                                                    : (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_out));
    vlSelfRef.read_done_o = ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__is_delayed)
                              ? (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__read_done_delay)
                              : (IData)(vlSelfRef.read_i));
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
            }
        } else if ((9U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__tristate_enable = 0U;
        } else if ((0xaU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__tristate_enable = 0U;
        } else if ((0xbU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__tristate_enable = 0U;
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
            }
        }
    }
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_activate_edge 
        = ((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_fsm_activation_detector__DOT__previous_signal)) 
           & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_activate));
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__tristate_enable = 1U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7_initialize = 0U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__cmd_argument_NXT 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__cmd_argument_CRT;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__cmd_number_NXT 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__cmd_number_CRT;
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
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_tx_read = 0U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__cmd_response_type = 0U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__timeout_reset = 0U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__cmd_response_valid = 0U;
    if ((1U & (~ ((((((((0U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT)) 
                        | (1U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) 
                       | (2U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) 
                      | (3U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) 
                     | (4U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) 
                    | (5U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) 
                   | (6U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) 
                  | (7U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT)))))) {
        if ((8U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
            if ((9U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
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
        if ((8U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
            if ((0x2faf080U == vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__timeout_counter)) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__cmd_response_valid = 0U;
            }
        } else if ((9U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
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
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__resp_NXT 
        = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__resp_CRT;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__bit_increment = 0U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__bit_reset = 0U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_increment = 0U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7_compute = 0U;
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__rx_valid = 0U;
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
                if ((4U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
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
                    if ((5U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                        if (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__sample) {
                            if ((0x400U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register))) {
                                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__unnamedblk1__DOT__i = 4U;
                            }
                        }
                    }
                }
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
            }
        }
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
            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__tristate_enable = 1U;
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
            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_increment 
                = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__shift;
        } else if ((8U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__tristate_enable = 1U;
            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_increment 
                = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__shift;
        } else {
            if ((9U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__tristate_enable = 1U;
            } else if ((0xbU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__tristate_enable = 0U;
            } else if ((0xdU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__tristate_enable = 0U;
            } else if ((0xaU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__tristate_enable = 0U;
            }
            if ((9U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))) {
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
    }
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
            if ((0x200U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register))) {
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__rcv136_NXT 
                    = (((2U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__cmd_number)) 
                        | (9U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__cmd_number))) 
                       | (0xaU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__cmd_number)));
                vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_NXT = 1U;
            }
            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7_initialize = 1U;
            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__cmd_argument_NXT 
                = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__cmd_argument;
            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__cmd_number_NXT 
                = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__cmd_number;
            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__timeout_reset = 1U;
            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__resp_NXT = 0U;
        } else {
            if ((1U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
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
            if ((1U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
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
        }
        if ((0U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
            if ((1U != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
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
                            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__bit_reset = 1U;
                            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7_NXT 
                                = vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7_CRT;
                        }
                    }
                } else {
                    if ((0xbU != (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
                        if ((0xcU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
                            vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__bit_reset = 1U;
                        }
                    }
                    if ((0xbU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))) {
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
    vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT____Vcellinp__tx_buffer__read_i 
        = (((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__tx_buffer_empty)) 
            & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_tx_read)) 
           & (3U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__tx_data_selector)));
}

VL_ATTR_COLD void Vtb_top___024root___stl_comb__TOP__0(Vtb_top___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_top___024root___stl_comb__TOP__0\n"); );
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
        VL_DBG_MSGF("         'act' region trigger index 0 is active: @([hybrid] tb_top.u_dut.sd_dat_t)\n");
    }
    if ((2ULL & vlSelfRef.__VactTriggered.word(0U))) {
        VL_DBG_MSGF("         'act' region trigger index 1 is active: @(posedge clk)\n");
    }
    if ((4ULL & vlSelfRef.__VactTriggered.word(0U))) {
        VL_DBG_MSGF("         'act' region trigger index 2 is active: @(posedge tb_top.u_dut.clk_50MHz)\n");
    }
    if ((8ULL & vlSelfRef.__VactTriggered.word(0U))) {
        VL_DBG_MSGF("         'act' region trigger index 3 is active: @(posedge tb_top.u_dut.dut.sd_clk)\n");
    }
    if ((0x10ULL & vlSelfRef.__VactTriggered.word(0U))) {
        VL_DBG_MSGF("         'act' region trigger index 4 is active: @(negedge rst_n)\n");
    }
    if ((0x20ULL & vlSelfRef.__VactTriggered.word(0U))) {
        VL_DBG_MSGF("         'act' region trigger index 5 is active: @(posedge tb_top.u_dut.sd_model.isdph.spi_cs)\n");
    }
    if ((0x40ULL & vlSelfRef.__VactTriggered.word(0U))) {
        VL_DBG_MSGF("         'act' region trigger index 6 is active: @(negedge tb_top.u_dut.dut.sd_clk)\n");
    }
    if ((0x80ULL & vlSelfRef.__VactTriggered.word(0U))) {
        VL_DBG_MSGF("         'act' region trigger index 7 is active: @([true] __VdlySched.awaitingCurrentTime())\n");
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
        VL_DBG_MSGF("         'nba' region trigger index 0 is active: @([hybrid] tb_top.u_dut.sd_dat_t)\n");
    }
    if ((2ULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        VL_DBG_MSGF("         'nba' region trigger index 1 is active: @(posedge clk)\n");
    }
    if ((4ULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        VL_DBG_MSGF("         'nba' region trigger index 2 is active: @(posedge tb_top.u_dut.clk_50MHz)\n");
    }
    if ((8ULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        VL_DBG_MSGF("         'nba' region trigger index 3 is active: @(posedge tb_top.u_dut.dut.sd_clk)\n");
    }
    if ((0x10ULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        VL_DBG_MSGF("         'nba' region trigger index 4 is active: @(negedge rst_n)\n");
    }
    if ((0x20ULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        VL_DBG_MSGF("         'nba' region trigger index 5 is active: @(posedge tb_top.u_dut.sd_model.isdph.spi_cs)\n");
    }
    if ((0x40ULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        VL_DBG_MSGF("         'nba' region trigger index 6 is active: @(negedge tb_top.u_dut.dut.sd_clk)\n");
    }
    if ((0x80ULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        VL_DBG_MSGF("         'nba' region trigger index 7 is active: @([true] __VdlySched.awaitingCurrentTime())\n");
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
    vlSelfRef.__Vm_traceActivity[4U] = 1U;
    vlSelfRef.__Vm_traceActivity[5U] = 1U;
    vlSelfRef.__Vm_traceActivity[6U] = 1U;
    vlSelfRef.__Vm_traceActivity[7U] = 1U;
    vlSelfRef.__Vm_traceActivity[8U] = 1U;
    vlSelfRef.__Vm_traceActivity[9U] = 1U;
    vlSelfRef.__Vm_traceActivity[0xaU] = 1U;
    vlSelfRef.__Vm_traceActivity[0xbU] = 1U;
    vlSelfRef.__Vm_traceActivity[0xcU] = 1U;
    vlSelfRef.__Vm_traceActivity[0xdU] = 1U;
    vlSelfRef.__Vm_traceActivity[0xeU] = 1U;
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
    vlSelf->tb_top__DOT__u_dut__DOT__sd_cmd_io = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_data_io = VL_RAND_RESET_I(4);
    vlSelf->tb_top__DOT__u_dut__DOT__wbm_adr_o = VL_RAND_RESET_I(32);
    vlSelf->tb_top__DOT__u_dut__DOT__wbm_dat_i = VL_RAND_RESET_I(32);
    vlSelf->tb_top__DOT__u_dut__DOT__wbm_sel_o = VL_RAND_RESET_I(4);
    vlSelf->tb_top__DOT__u_dut__DOT__wbm_cyc_o = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__wbm_stb_o = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__wbm_we_o = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__wbm_ack_i = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__wbm_cti_o = VL_RAND_RESET_I(3);
    vlSelf->tb_top__DOT__u_dut__DOT__wbm_bte_o = VL_RAND_RESET_I(2);
    vlSelf->tb_top__DOT__u_dut__DOT__clk_50MHz = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__clk_100MHz = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_dat_o = VL_RAND_RESET_I(4);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_dat_t = VL_RAND_RESET_I(4);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_cmd_wire = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_dat_wire = VL_RAND_RESET_I(4);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_data_io__strong__out5 = VL_RAND_RESET_I(4);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_data_io__strong__out6 = VL_RAND_RESET_I(4);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_data_io__strong__out7 = VL_RAND_RESET_I(4);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_data_io__strong__out8 = VL_RAND_RESET_I(4);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__sd_card_detect = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__reset_done = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__cmd_crc_error = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__cmd_timeout = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__cmd_crc_error_ff = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__cmd_timeout_ff = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__cmd_response_valid = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__cmd_response_type = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__data_rx = VL_RAND_RESET_I(32);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__data_buffer_rx_write = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__data_timeout = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__data_crc_error = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__data_error = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__data_timeout_ff = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__data_crc_error_ff = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__data_error_ff = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__force_stop = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__data_direction = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__interrupt = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT____Vcellout__register_interface__data_tx_data_o = VL_RAND_RESET_I(32);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__reset_counter = VL_RAND_RESET_I(17);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__active = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__counter = VL_RAND_RESET_I(8);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__clk_previous = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__sd_clk = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__shift = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__sample = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__data_activate = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__data_activate_edge = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__tx_data_selector = VL_RAND_RESET_I(2);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__tx_data_byte = VL_RAND_RESET_I(8);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__data_rx_write = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__write_selector = VL_RAND_RESET_I(5);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__data_tx_read = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__card_detect_synchronizer__DOT__sync_signal = VL_RAND_RESET_I(2);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__mask = VL_RAND_RESET_I(32);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register = VL_RAND_RESET_I(15);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__status_register = VL_RAND_RESET_I(15);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__tx_buffer_empty = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__tx_buffer_full = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__rx_buffer_empty = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__rx_buffer_full = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__resp_buffer_empty = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__resp_buffer_full = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__cmd_number = VL_RAND_RESET_I(6);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__cmd_argument = VL_RAND_RESET_I(32);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register = VL_RAND_RESET_I(9);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__write_tx_buffer = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT____Vcellinp__tx_buffer__read_i = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT____Vcellinp__tx_buffer__rst_n_i = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__read_data_rx = VL_RAND_RESET_I(32);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__read_rx_buffer = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__read_response_data = VL_RAND_RESET_I(8);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__read_resp_buffer = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__is_delayed = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__read_done_delay = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__cmd_done_detector__DOT__previous_signal = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__data_done_detector__DOT__previous_signal = VL_RAND_RESET_I(1);
    for (int __Vi0 = 0; __Vi0 < 128; ++__Vi0) {
        vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__tx_buffer__DOT__buffer_memory[__Vi0] = VL_RAND_RESET_I(32);
    }
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__tx_buffer__DOT__write_ptr = VL_RAND_RESET_I(7);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__tx_buffer__DOT__read_ptr = VL_RAND_RESET_I(7);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__tx_buffer__DOT__inc_write_ptr = VL_RAND_RESET_I(7);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__tx_buffer__DOT__inc_read_ptr = VL_RAND_RESET_I(7);
    for (int __Vi0 = 0; __Vi0 < 128; ++__Vi0) {
        vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__rx_buffer__DOT__buffer_memory[__Vi0] = VL_RAND_RESET_I(32);
    }
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__rx_buffer__DOT__write_ptr = VL_RAND_RESET_I(7);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__rx_buffer__DOT__read_ptr = VL_RAND_RESET_I(7);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__rx_buffer__DOT__inc_write_ptr = VL_RAND_RESET_I(7);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__rx_buffer__DOT__inc_read_ptr = VL_RAND_RESET_I(7);
    for (int __Vi0 = 0; __Vi0 < 32; ++__Vi0) {
        vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__buffer_memory[__Vi0] = VL_RAND_RESET_I(8);
    }
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__write_ptr = VL_RAND_RESET_I(5);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__read_ptr = VL_RAND_RESET_I(5);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__inc_write_ptr = VL_RAND_RESET_I(5);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__inc_read_ptr = VL_RAND_RESET_I(5);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__genblk1__BRA__0__KET____DOT__event_detector__DOT__previous_signal = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__genblk1__BRA__1__KET____DOT__event_detector__DOT__previous_signal = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__genblk1__BRA__2__KET____DOT__event_detector__DOT__previous_signal = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__genblk1__BRA__3__KET____DOT__event_detector__DOT__previous_signal = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__genblk1__BRA__4__KET____DOT__event_detector__DOT__previous_signal = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__genblk1__BRA__5__KET____DOT__event_detector__DOT__previous_signal = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__genblk1__BRA__6__KET____DOT__event_detector__DOT__previous_signal = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__genblk1__BRA__7__KET____DOT__event_detector__DOT__previous_signal = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__genblk1__BRA__8__KET____DOT__event_detector__DOT__previous_signal = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__data_fsm_activation_detector__DOT__previous_signal = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__data_rx_write_detector__DOT__previous_signal = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__cmd_number_CRT = VL_RAND_RESET_I(6);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__cmd_number_NXT = VL_RAND_RESET_I(6);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__card_app_cmd_CRT = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__card_app_cmd_NXT = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__cmd_argument_CRT = VL_RAND_RESET_I(32);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__cmd_argument_NXT = VL_RAND_RESET_I(32);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__resp_CRT = VL_RAND_RESET_I(8);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__resp_NXT = VL_RAND_RESET_I(8);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7_CRT = VL_RAND_RESET_I(7);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7_NXT = VL_RAND_RESET_I(7);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__rcv136_CRT = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__rcv136_NXT = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__data_burst_CRT = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__data_burst_NXT = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__bit_counter = VL_RAND_RESET_I(8);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__bit_increment = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__bit_reset = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__timeout_counter = VL_RAND_RESET_I(26);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__timeout_increment = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__timeout_reset = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7_initialize = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7_compute = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7_data = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT = VL_RAND_RESET_I(4);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_NXT = VL_RAND_RESET_I(4);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__tristate_enable = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__cmd_bit = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__tristate_enable_flop = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__cmd_bit_flop = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7__DOT__crc = VL_RAND_RESET_I(7);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_CRT = VL_RAND_RESET_I(8);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_NXT = VL_RAND_RESET_I(8);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_CRT = VL_RAND_RESET_I(16);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_NXT = VL_RAND_RESET_I(16);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_wide_CRT = VL_RAND_RESET_Q(64);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_wide_NXT = VL_RAND_RESET_Q(64);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_counter = VL_RAND_RESET_I(12);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_increment = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_reset = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__timeout_counter = VL_RAND_RESET_I(20);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__timeout_increment = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__timeout_reset = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_initialize = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_compute = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_data = VL_RAND_RESET_I(4);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT = VL_RAND_RESET_I(4);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_NXT = VL_RAND_RESET_I(4);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__tristate_enable = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_line = VL_RAND_RESET_I(4);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__rx_valid = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__tristate_enable_flop = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_line_flop = VL_RAND_RESET_I(4);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__unnamedblk1__DOT__i = 0;
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__unnamedblk2__DOT__i = 0;
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__unnamedblk3__DOT__i = 0;
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_CRT = VL_RAND_RESET_I(16);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_NXT = VL_RAND_RESET_I(16);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT = VL_RAND_RESET_Q(64);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_NXT = VL_RAND_RESET_Q(64);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc16_in0 = VL_RAND_RESET_I(16);
    vlSelf->tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT____VdfgRegularize_h6e830a37_0_5 = VL_RAND_RESET_I(1);
    for (int __Vi0 = 0; __Vi0 < 1024; ++__Vi0) {
        vlSelf->tb_top__DOT__u_dut__DOT__SD_memory__DOT__mem[__Vi0] = VL_RAND_RESET_I(32);
    }
    vlSelf->tb_top__DOT__u_dut__DOT__SD_memory__DOT__unnamedblk1__DOT__i = VL_RAND_RESET_I(32);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_rd_ext_addr = VL_RAND_RESET_I(7);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_rd_ext_wren = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_rd_ext_data = VL_RAND_RESET_I(32);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_rd_ext_q = VL_RAND_RESET_I(32);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_wr_ext_addr = VL_RAND_RESET_I(7);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_wr_ext_q = VL_RAND_RESET_I(32);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__ext_read_act = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__ext_read_go = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__ext_read_addr = VL_RAND_RESET_I(32);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__ext_read_stop = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__ext_write_act = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__ext_write_done = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__ext_write_addr = VL_RAND_RESET_I(32);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_rd_sd_addr = VL_RAND_RESET_I(7);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_rd_sd_q = VL_RAND_RESET_I(32);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_wr_sd_addr = VL_RAND_RESET_I(7);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_wr_sd_wren = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_wr_sd_data = VL_RAND_RESET_I(32);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_wr_sd_q = VL_RAND_RESET_I(32);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__link_read_act = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__link_read_go = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__link_read_addr = VL_RAND_RESET_I(32);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__link_read_num = VL_RAND_RESET_I(32);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__link_read_stop = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__link_write_act = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__link_write_done = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__link_write_addr = VL_RAND_RESET_I(32);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__link_write_num = VL_RAND_RESET_I(32);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_cmd_in = VL_RAND_RESET_Q(48);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_cmd_in_crc_good = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_cmd_in_act = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_type = VL_RAND_RESET_I(4);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_act = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_done = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__odc = VL_RAND_RESET_I(11);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__ostate = VL_RAND_RESET_I(7);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__cmd_in_cmd = VL_RAND_RESET_I(6);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_in_act = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_in_busy = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_in_another = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_in_stop = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_in_done = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_in_crc_good = VL_RAND_RESET_I(1);
    VL_RAND_RESET_W(136, vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_out);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_busy = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_mode_4bit = VL_RAND_RESET_I(1);
    VL_RAND_RESET_W(512, vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_src = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_len = VL_RAND_RESET_I(10);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_busy = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_act = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_stop = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_done = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_mode_spi = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_mode_crc_disable = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__state = VL_RAND_RESET_I(5);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__bram_rd_ext_addr_next = VL_RAND_RESET_I(7);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__bytes_done = VL_RAND_RESET_I(16);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__ext_read_addr_latch = VL_RAND_RESET_I(32);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__ext_write_addr_latch = VL_RAND_RESET_I(32);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__ext_read_act_last = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__ext_write_act_last = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__wbm_ack_i_last = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__reset_s = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__a__DOT__stage_1 = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__a__DOT__stage_2 = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__a__DOT__stage_3 = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__link_read_addr_latch = VL_RAND_RESET_I(32);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__state = VL_RAND_RESET_I(5);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__reset_s = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__ext_read_go_s = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__ext_read_go_r = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__ext_write_done_s = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__sel_rd_sd = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__sel_rd_ext = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__buf_rd_a_full = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__buf_rd_b_full = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__buf_rd_a_block = VL_RAND_RESET_I(32);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__buf_rd_b_block = VL_RAND_RESET_I(32);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__bram_rd_ext_addr_sel = VL_RAND_RESET_I(8);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__sel_wr_sd = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__sel_wr_ext = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__bram_wr_sd_addr_sel = VL_RAND_RESET_I(8);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__a__DOT__stage_1 = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__a__DOT__stage_2 = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__a__DOT__stage_3 = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__b__DOT__stage_1 = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__b__DOT__stage_2 = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__b__DOT__stage_3 = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__c__DOT__stage_1 = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__c__DOT__stage_2 = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__c__DOT__stage_3 = VL_RAND_RESET_I(1);
    for (int __Vi0 = 0; __Vi0 < 256; ++__Vi0) {
        vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__isdb1__DOT__mem[__Vi0] = VL_RAND_RESET_I(32);
    }
    for (int __Vi0 = 0; __Vi0 < 256; ++__Vi0) {
        vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__isdb2__DOT__mem[__Vi0] = VL_RAND_RESET_I(32);
    }
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__block_read_byteaddr = VL_RAND_RESET_I(32);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__block_write_byteaddr = VL_RAND_RESET_I(32);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__block_preerase_num = VL_RAND_RESET_I(23);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__block_erase_start = VL_RAND_RESET_I(32);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__block_erase_end = VL_RAND_RESET_I(32);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_last = VL_RAND_RESET_I(6);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__info_card_desel = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__err_op_out_range = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__err_unhandled_cmd = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__err_cmd_crc = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__host_hc_support = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_latch = VL_RAND_RESET_Q(48);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_arg = VL_RAND_RESET_I(32);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_arg_blockaddr = VL_RAND_RESET_I(32);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_arg_byteaddr = VL_RAND_RESET_I(32);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state = VL_RAND_RESET_I(4);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state_next = VL_RAND_RESET_I(4);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_erase_state = VL_RAND_RESET_I(3);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_appcmd = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status = VL_RAND_RESET_I(32);
    VL_RAND_RESET_W(128, vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_sd_status);
    VL_RAND_RESET_W(128, vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_csd);
    VL_RAND_RESET_W(128, vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_cid);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_ocr = VL_RAND_RESET_I(32);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_rca = VL_RAND_RESET_I(16);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_scr = VL_RAND_RESET_Q(64);
    VL_RAND_RESET_W(112, vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function_caps);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function = VL_RAND_RESET_I(24);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function_check = VL_RAND_RESET_I(24);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_blocks_written = VL_RAND_RESET_I(32);
    VL_RAND_RESET_W(128, vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_arg);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_type = VL_RAND_RESET_I(4);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_in_gap_cnt = VL_RAND_RESET_I(10);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__state = VL_RAND_RESET_I(7);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_state = VL_RAND_RESET_I(7);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__spi_status_word = VL_RAND_RESET_I(16);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_scr = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_cid = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_csd = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_sdstatus = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_function = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_written = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_block = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_block_queue = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_recv_block = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__reset_s = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_s = VL_RAND_RESET_Q(48);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_crc_good_s = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_act_s = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__spi_sel_s = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_in_busy_s = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_in_done_s = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_in_crc_good_s = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_done_s = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_out_busy_s = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_out_done_s = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__a__DOT__stage_1 = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__a__DOT__stage_2 = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__a__DOT__stage_3 = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__b__DOT__stage_1 = VL_RAND_RESET_Q(48);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__b__DOT__stage_2 = VL_RAND_RESET_Q(48);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__b__DOT__stage_3 = VL_RAND_RESET_Q(48);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__c__DOT__stage_1 = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__c__DOT__stage_2 = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__c__DOT__stage_3 = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__d__DOT__stage_1 = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__d__DOT__stage_2 = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__d__DOT__stage_3 = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__e__DOT__stage_1 = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__e__DOT__stage_2 = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__e__DOT__stage_3 = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__f__DOT__stage_1 = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__f__DOT__stage_2 = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__f__DOT__stage_3 = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__g__DOT__stage_1 = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__g__DOT__stage_2 = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__g__DOT__stage_3 = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__h__DOT__stage_1 = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__h__DOT__stage_2 = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__h__DOT__stage_3 = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__i__DOT__stage_1 = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__i__DOT__stage_2 = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__i__DOT__stage_3 = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__j__DOT__stage_1 = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__j__DOT__stage_2 = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__j__DOT__stage_3 = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__k__DOT__stage_1 = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__k__DOT__stage_2 = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__k__DOT__stage_3 = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__spi_cnt = VL_RAND_RESET_I(8);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_cmd_out = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_cmd_oe = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__spi_cs = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_out = VL_RAND_RESET_I(4);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_oe = VL_RAND_RESET_I(4);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_spi_dat = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_cmd_last = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_last = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__cmd_in_latch = VL_RAND_RESET_Q(47);
    VL_RAND_RESET_W(136, vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__resp_out_latch);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc7_in = VL_RAND_RESET_I(7);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc7_out = VL_RAND_RESET_I(7);
    VL_RAND_RESET_W(513, vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_out3 = VL_RAND_RESET_I(16);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_out2 = VL_RAND_RESET_I(16);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_out1 = VL_RAND_RESET_I(16);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_out0 = VL_RAND_RESET_I(16);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_in3 = VL_RAND_RESET_I(16);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_in2 = VL_RAND_RESET_I(16);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_in1 = VL_RAND_RESET_I(16);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_in0 = VL_RAND_RESET_I(16);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_check3 = VL_RAND_RESET_I(16);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_check2 = VL_RAND_RESET_I(16);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_check1 = VL_RAND_RESET_I(16);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_check0 = VL_RAND_RESET_I(16);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__idc = VL_RAND_RESET_I(11);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__istate = VL_RAND_RESET_I(7);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__didc = VL_RAND_RESET_I(16);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dodc = VL_RAND_RESET_I(16);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__distate = VL_RAND_RESET_I(7);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dostate = VL_RAND_RESET_I(7);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__spi_di_token = VL_RAND_RESET_I(8);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dout_buf = VL_RAND_RESET_I(32);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dout_4bit = VL_RAND_RESET_I(4);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dout_1bit = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__do_crc_token = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_in_act_s = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_in_stop_s = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_in_another_s = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__resp_act_s = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__resp_type_s = VL_RAND_RESET_I(4);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_len_s = VL_RAND_RESET_I(10);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_act_s = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_stop_s = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__card_state_s = VL_RAND_RESET_I(4);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__mode_4bit_s = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__mode_spi_s = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__mode_crc_disable_s = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__a__DOT__stage_1 = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__a__DOT__stage_2 = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__a__DOT__stage_3 = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__i__DOT__stage_1 = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__i__DOT__stage_2 = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__i__DOT__stage_3 = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__j__DOT__stage_1 = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__j__DOT__stage_2 = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__j__DOT__stage_3 = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__b__DOT__stage_1 = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__b__DOT__stage_2 = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__b__DOT__stage_3 = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__c__DOT__stage_1 = VL_RAND_RESET_I(4);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__c__DOT__stage_2 = VL_RAND_RESET_I(4);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__c__DOT__stage_3 = VL_RAND_RESET_I(4);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__d__DOT__stage_1 = VL_RAND_RESET_I(10);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__d__DOT__stage_2 = VL_RAND_RESET_I(10);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__d__DOT__stage_3 = VL_RAND_RESET_I(10);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__e__DOT__stage_1 = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__e__DOT__stage_2 = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__e__DOT__stage_3 = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__f__DOT__stage_1 = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__f__DOT__stage_2 = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__f__DOT__stage_3 = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__g__DOT__stage_1 = VL_RAND_RESET_I(4);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__g__DOT__stage_2 = VL_RAND_RESET_I(4);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__g__DOT__stage_3 = VL_RAND_RESET_I(4);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__h__DOT__stage_1 = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__h__DOT__stage_2 = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__h__DOT__stage_3 = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__k__DOT__stage_1 = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__k__DOT__stage_2 = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__k__DOT__stage_3 = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__l__DOT__stage_1 = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__l__DOT__stage_2 = VL_RAND_RESET_I(1);
    vlSelf->tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__l__DOT__stage_3 = VL_RAND_RESET_I(1);
    vlSelf->__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__link_write_done = VL_RAND_RESET_I(1);
    vlSelf->__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__sel_rd_sd = VL_RAND_RESET_I(1);
    vlSelf->__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__link_read_go = VL_RAND_RESET_I(1);
    vlSelf->__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_in_gap_cnt = VL_RAND_RESET_I(10);
    vlSelf->__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_erase_state = VL_RAND_RESET_I(3);
    vlSelf->__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_blocks_written = VL_RAND_RESET_I(32);
    vlSelf->__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_appcmd = VL_RAND_RESET_I(1);
    vlSelf->__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_rca = VL_RAND_RESET_I(16);
    vlSelf->__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status = VL_RAND_RESET_I(32);
    vlSelf->__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state = VL_RAND_RESET_I(4);
    vlSelf->__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_ocr = VL_RAND_RESET_I(32);
    VL_RAND_RESET_W(128, vlSelf->__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_cid);
    VL_RAND_RESET_W(128, vlSelf->__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_csd);
    vlSelf->__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_scr = VL_RAND_RESET_Q(64);
    VL_RAND_RESET_W(128, vlSelf->__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_sd_status);
    VL_RAND_RESET_W(112, vlSelf->__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function_caps);
    vlSelf->__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function = VL_RAND_RESET_I(24);
    vlSelf->__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function_check = VL_RAND_RESET_I(24);
    vlSelf->__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_scr = VL_RAND_RESET_I(1);
    vlSelf->__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_cid = VL_RAND_RESET_I(1);
    vlSelf->__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_csd = VL_RAND_RESET_I(1);
    vlSelf->__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_sdstatus = VL_RAND_RESET_I(1);
    vlSelf->__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_function = VL_RAND_RESET_I(1);
    vlSelf->__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_written = VL_RAND_RESET_I(1);
    vlSelf->__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_block = VL_RAND_RESET_I(1);
    vlSelf->__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_block_queue = VL_RAND_RESET_I(1);
    vlSelf->__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_recv_block = VL_RAND_RESET_I(1);
    vlSelf->__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_in_act = VL_RAND_RESET_I(1);
    vlSelf->__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_in_stop = VL_RAND_RESET_I(1);
    vlSelf->__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_stop = VL_RAND_RESET_I(1);
    vlSelf->__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_mode_4bit = VL_RAND_RESET_I(1);
    vlSelf->__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_mode_crc_disable = VL_RAND_RESET_I(1);
    vlSelf->__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__link_read_num = VL_RAND_RESET_I(32);
    vlSelf->__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__link_write_num = VL_RAND_RESET_I(32);
    vlSelf->__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__state = VL_RAND_RESET_I(7);
    vlSelf->__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state_next = VL_RAND_RESET_I(4);
    vlSelf->__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_type = VL_RAND_RESET_I(4);
    vlSelf->__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__link_write_addr = VL_RAND_RESET_I(32);
    vlSelf->__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__block_write_byteaddr = VL_RAND_RESET_I(32);
    vlSelf->__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__link_read_addr = VL_RAND_RESET_I(32);
    vlSelf->__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__block_read_byteaddr = VL_RAND_RESET_I(32);
    VL_RAND_RESET_W(128, vlSelf->__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_arg);
    vlSelf->__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_state = VL_RAND_RESET_I(7);
    vlSelf->__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_mode_spi = VL_RAND_RESET_I(1);
    vlSelf->__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_type = VL_RAND_RESET_I(4);
    vlSelf->__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_in_another = VL_RAND_RESET_I(1);
    vlSelf->__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__spi_cnt = VL_RAND_RESET_I(8);
    vlSelf->__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__istate = VL_RAND_RESET_I(7);
    vlSelf->__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__cmd_in_latch = VL_RAND_RESET_Q(47);
    vlSelf->__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc7_in = VL_RAND_RESET_I(7);
    vlSelf->__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__distate = VL_RAND_RESET_I(7);
    vlSelf->__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_check3 = VL_RAND_RESET_I(16);
    vlSelf->__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_check2 = VL_RAND_RESET_I(16);
    vlSelf->__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_check1 = VL_RAND_RESET_I(16);
    vlSelf->__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_check0 = VL_RAND_RESET_I(16);
    vlSelf->__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_wr_sd_data = VL_RAND_RESET_I(32);
    vlSelf->__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_in0 = VL_RAND_RESET_I(16);
    vlSelf->__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_wr_sd_addr = VL_RAND_RESET_I(7);
    vlSelf->__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__spi_di_token = VL_RAND_RESET_I(8);
    vlSelf->__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_oe = VL_RAND_RESET_I(4);
    vlSelf->__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_busy = VL_RAND_RESET_I(1);
    vlSelf->__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_done = VL_RAND_RESET_I(1);
    vlSelf->__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_rd_sd_addr = VL_RAND_RESET_I(7);
    vlSelf->__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_cmd_oe = VL_RAND_RESET_I(1);
    vlSelf->__Vdly__tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_done = VL_RAND_RESET_I(1);
    vlSelf->__VdlyVal__tb_top__DOT__u_dut__DOT__clk_100MHz__v0 = VL_RAND_RESET_I(1);
    vlSelf->__VdlySet__tb_top__DOT__u_dut__DOT__clk_100MHz__v0 = 0;
    vlSelf->__VdlyVal__tb_top__DOT__u_dut__DOT__clk_50MHz__v0 = VL_RAND_RESET_I(1);
    vlSelf->__VdlySet__tb_top__DOT__u_dut__DOT__clk_50MHz__v0 = 0;
    vlSelf->__VdlyVal__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__isdb1__DOT__mem__v0 = VL_RAND_RESET_I(32);
    vlSelf->__VdlyDim0__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__isdb1__DOT__mem__v0 = VL_RAND_RESET_I(8);
    vlSelf->__VdlySet__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__isdb1__DOT__mem__v0 = 0;
    vlSelf->__VdlyVal__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__isdb2__DOT__mem__v0 = VL_RAND_RESET_I(32);
    vlSelf->__VdlyDim0__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__isdb2__DOT__mem__v0 = VL_RAND_RESET_I(8);
    vlSelf->__VdlySet__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__isdb2__DOT__mem__v0 = 0;
    vlSelf->__Vtrigprevexpr___TOP__tb_top__DOT__u_dut__DOT__sd_dat_t__0 = VL_RAND_RESET_I(4);
    vlSelf->__VstlDidInit = 0;
    vlSelf->__Vtrigprevexpr___TOP__tb_top__DOT__u_dut__DOT__sd_dat_t__1 = VL_RAND_RESET_I(4);
    vlSelf->__Vtrigprevexpr___TOP__clk__0 = VL_RAND_RESET_I(1);
    vlSelf->__Vtrigprevexpr___TOP__tb_top__DOT__u_dut__DOT__clk_50MHz__0 = VL_RAND_RESET_I(1);
    vlSelf->__Vtrigprevexpr___TOP__tb_top__DOT__u_dut__DOT__dut__DOT__sd_clk__0 = VL_RAND_RESET_I(1);
    vlSelf->__Vtrigprevexpr___TOP__rst_n__0 = VL_RAND_RESET_I(1);
    vlSelf->__Vtrigprevexpr___TOP__tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__spi_cs__0 = VL_RAND_RESET_I(1);
    vlSelf->__VactDidInit = 0;
    for (int __Vi0 = 0; __Vi0 < 15; ++__Vi0) {
        vlSelf->__Vm_traceActivity[__Vi0] = 0;
    }
}
