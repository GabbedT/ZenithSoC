// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Tracing implementation internals
#include "verilated_fst_c.h"
#include "Vtb_top__Syms.h"


void Vtb_top___024root__trace_chg_0_sub_0(Vtb_top___024root* vlSelf, VerilatedFst::Buffer* bufp);

void Vtb_top___024root__trace_chg_0(void* voidSelf, VerilatedFst::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_top___024root__trace_chg_0\n"); );
    // Init
    Vtb_top___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vtb_top___024root*>(voidSelf);
    Vtb_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    if (VL_UNLIKELY(!vlSymsp->__Vm_activity)) return;
    // Body
    Vtb_top___024root__trace_chg_0_sub_0((&vlSymsp->TOP), bufp);
}

void Vtb_top___024root__trace_chg_0_sub_0(Vtb_top___024root* vlSelf, VerilatedFst::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_top___024root__trace_chg_0_sub_0\n"); );
    Vtb_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    uint32_t* const oldp VL_ATTR_UNUSED = bufp->oldp(vlSymsp->__Vm_baseCode + 1);
    // Body
    if (VL_UNLIKELY((vlSelfRef.__Vm_traceActivity[1U]))) {
        bufp->chgCData(oldp+0,((0xffU & vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__mask)),8);
        bufp->chgCData(oldp+1,((0xffU & (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__mask 
                                         >> 8U))),8);
        bufp->chgCData(oldp+2,((0xffU & (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__mask 
                                         >> 0x10U))),8);
        bufp->chgCData(oldp+3,((vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__mask 
                                >> 0x18U)),8);
        bufp->chgBit(oldp+4,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__write_tx_buffer));
        bufp->chgBit(oldp+5,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__read_rx_buffer));
        bufp->chgBit(oldp+6,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__read_resp_buffer));
        bufp->chgBit(oldp+7,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__is_delayed));
    }
    if (VL_UNLIKELY(((vlSelfRef.__Vm_traceActivity[2U] 
                      | vlSelfRef.__Vm_traceActivity
                      [0xeU])))) {
        bufp->chgCData(oldp+8,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_data_io),4);
        bufp->chgCData(oldp+9,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_dat_t),4);
        bufp->chgBit(oldp+10,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_timeout));
        bufp->chgBit(oldp+11,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_crc_error));
        bufp->chgBit(oldp+12,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_error));
        bufp->chgCData(oldp+13,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_NXT),8);
        bufp->chgSData(oldp+14,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_NXT),16);
        bufp->chgSData(oldp+15,((0xffffU & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_wide_NXT))),16);
        bufp->chgSData(oldp+16,((0xffffU & (IData)(
                                                   (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_wide_NXT 
                                                    >> 0x10U)))),16);
        bufp->chgSData(oldp+17,((0xffffU & (IData)(
                                                   (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_wide_NXT 
                                                    >> 0x20U)))),16);
        bufp->chgSData(oldp+18,((0xffffU & (IData)(
                                                   (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_wide_NXT 
                                                    >> 0x30U)))),16);
        bufp->chgBit(oldp+19,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_reset));
        bufp->chgBit(oldp+20,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__timeout_reset));
        bufp->chgBit(oldp+21,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_initialize));
        bufp->chgBit(oldp+22,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_compute));
        bufp->chgCData(oldp+23,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_data),4);
        bufp->chgCData(oldp+24,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_NXT),4);
        bufp->chgCData(oldp+25,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_line),4);
        bufp->chgSData(oldp+26,((0xffffU & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_NXT))),16);
        bufp->chgSData(oldp+27,((0xffffU & (IData)(
                                                   (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_NXT 
                                                    >> 0x10U)))),16);
        bufp->chgSData(oldp+28,((0xffffU & (IData)(
                                                   (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_NXT 
                                                    >> 0x20U)))),16);
        bufp->chgSData(oldp+29,((0xffffU & (IData)(
                                                   (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_NXT 
                                                    >> 0x30U)))),16);
        bufp->chgBit(oldp+30,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__spi_cs));
        bufp->chgCData(oldp+31,((1U & (~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__spi_cs)))),4);
        bufp->chgBit(oldp+32,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_spi_dat));
    }
    if (VL_UNLIKELY((vlSelfRef.__Vm_traceActivity[3U]))) {
        bufp->chgBit(oldp+33,((1U & (~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__active)))));
        bufp->chgBit(oldp+34,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__sd_clk));
        bufp->chgBit(oldp+35,((1U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__card_detect_synchronizer__DOT__sync_signal) 
                                     >> 1U))));
        bufp->chgBit(oldp+36,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__sd_card_detect));
        bufp->chgBit(oldp+37,(((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__reset_done)) 
                               & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register) 
                                  >> 0xeU))));
        bufp->chgBit(oldp+38,((1U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register) 
                                     >> 0xdU))));
        bufp->chgBit(oldp+39,((1U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register) 
                                     >> 0xbU))));
        bufp->chgBit(oldp+40,((1U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register) 
                                     >> 0xaU))));
        bufp->chgBit(oldp+41,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__reset_done));
        bufp->chgCData(oldp+42,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__cmd_number),6);
        bufp->chgIData(oldp+43,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__cmd_argument),32);
        bufp->chgBit(oldp+44,((1U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register) 
                                     >> 9U))));
        bufp->chgBit(oldp+45,((0U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT))));
        bufp->chgBit(oldp+46,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__cmd_crc_error));
        bufp->chgBit(oldp+47,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__cmd_timeout));
        bufp->chgBit(oldp+48,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__cmd_crc_error_ff));
        bufp->chgBit(oldp+49,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__cmd_timeout_ff));
        bufp->chgBit(oldp+50,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__cmd_response_valid));
        bufp->chgBit(oldp+51,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__cmd_response_type));
        bufp->chgCData(oldp+52,((0xffU & vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT____Vcellout__register_interface__data_tx_data_o)),8);
        bufp->chgCData(oldp+53,((0xffU & (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT____Vcellout__register_interface__data_tx_data_o 
                                          >> 8U))),8);
        bufp->chgCData(oldp+54,((0xffU & (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT____Vcellout__register_interface__data_tx_data_o 
                                          >> 0x10U))),8);
        bufp->chgCData(oldp+55,((vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT____Vcellout__register_interface__data_tx_data_o 
                                 >> 0x18U)),8);
        bufp->chgBit(oldp+56,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__tx_buffer_empty));
        bufp->chgBit(oldp+57,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__rx_buffer_full));
        bufp->chgBit(oldp+58,(((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_tx_read) 
                               & ((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__tx_buffer_empty)) 
                                  & (3U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__tx_data_selector))))));
        bufp->chgCData(oldp+59,((0xffU & vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_rx)),8);
        bufp->chgCData(oldp+60,((0xffU & (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_rx 
                                          >> 8U))),8);
        bufp->chgCData(oldp+61,((0xffU & (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_rx 
                                          >> 0x10U))),8);
        bufp->chgCData(oldp+62,((vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_rx 
                                 >> 0x18U)),8);
        bufp->chgBit(oldp+63,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_buffer_rx_write));
        bufp->chgBit(oldp+64,((0U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT))));
        bufp->chgBit(oldp+65,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_timeout_ff));
        bufp->chgBit(oldp+66,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_crc_error_ff));
        bufp->chgBit(oldp+67,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_error_ff));
        bufp->chgBit(oldp+68,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__force_stop));
        bufp->chgBit(oldp+69,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_direction));
        bufp->chgBit(oldp+70,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__data_burst_CRT));
        bufp->chgIData(oldp+71,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__reset_counter),17);
        bufp->chgBit(oldp+72,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__active));
        bufp->chgCData(oldp+73,(((0x800U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register))
                                  ? 1U : 0x7cU)),8);
        bufp->chgCData(oldp+74,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__counter),8);
        bufp->chgBit(oldp+75,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__clk_previous));
        bufp->chgBit(oldp+76,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__shift));
        bufp->chgBit(oldp+77,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__sample));
        bufp->chgBit(oldp+78,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_activate));
        bufp->chgBit(oldp+79,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_activate_edge));
        bufp->chgCData(oldp+80,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__tx_data_selector),2);
        bufp->chgBit(oldp+81,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_tx_read));
        bufp->chgCData(oldp+82,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__tx_data_byte),8);
        bufp->chgCData(oldp+83,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_CRT),8);
        bufp->chgBit(oldp+84,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_rx_write));
        bufp->chgCData(oldp+85,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__write_selector),5);
        bufp->chgBit(oldp+86,((1U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__write_selector) 
                                     >> 3U))));
        bufp->chgCData(oldp+87,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__card_detect_synchronizer__DOT__sync_signal),2);
        bufp->chgCData(oldp+88,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__cmd_number_CRT),6);
        bufp->chgCData(oldp+89,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__cmd_number_NXT),6);
        bufp->chgBit(oldp+90,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__card_app_cmd_CRT));
        bufp->chgBit(oldp+91,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__card_app_cmd_NXT));
        bufp->chgIData(oldp+92,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__cmd_argument_CRT),32);
        bufp->chgIData(oldp+93,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__cmd_argument_NXT),32);
        bufp->chgCData(oldp+94,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__resp_CRT),8);
        bufp->chgCData(oldp+95,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7_CRT),7);
        bufp->chgBit(oldp+96,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__rcv136_CRT));
        bufp->chgBit(oldp+97,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__rcv136_NXT));
        bufp->chgBit(oldp+98,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__data_burst_NXT));
        bufp->chgCData(oldp+99,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__bit_counter),8);
        bufp->chgBit(oldp+100,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__bit_reset));
        bufp->chgIData(oldp+101,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__timeout_counter),26);
        bufp->chgBit(oldp+102,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__timeout_increment));
        bufp->chgCData(oldp+103,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7__DOT__crc),7);
        bufp->chgBit(oldp+104,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7_initialize));
        bufp->chgCData(oldp+105,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT),4);
        bufp->chgBit(oldp+106,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__tristate_enable));
        bufp->chgBit(oldp+107,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__cmd_bit));
        bufp->chgBit(oldp+108,(((0x3aU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__cmd_number)) 
                                | (((0x29U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__cmd_number)) 
                                    & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__card_app_cmd_CRT)) 
                                   | (1U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__cmd_number))))));
        bufp->chgBit(oldp+109,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__tristate_enable_flop));
        bufp->chgBit(oldp+110,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__cmd_bit_flop));
        bufp->chgSData(oldp+111,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_CRT),16);
        bufp->chgSData(oldp+112,((0xffffU & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_wide_CRT))),16);
        bufp->chgSData(oldp+113,((0xffffU & (IData)(
                                                    (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_wide_CRT 
                                                     >> 0x10U)))),16);
        bufp->chgSData(oldp+114,((0xffffU & (IData)(
                                                    (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_wide_CRT 
                                                     >> 0x20U)))),16);
        bufp->chgSData(oldp+115,((0xffffU & (IData)(
                                                    (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16_wide_CRT 
                                                     >> 0x30U)))),16);
        bufp->chgSData(oldp+116,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_counter),12);
        bufp->chgBit(oldp+117,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__bit_increment));
        bufp->chgIData(oldp+118,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__timeout_counter),20);
        bufp->chgBit(oldp+119,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__timeout_increment));
        bufp->chgSData(oldp+120,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_CRT),16);
        bufp->chgSData(oldp+121,((0xffffU & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT))),16);
        bufp->chgSData(oldp+122,((0xffffU & (IData)(
                                                    (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT 
                                                     >> 0x10U)))),16);
        bufp->chgSData(oldp+123,((0xffffU & (IData)(
                                                    (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT 
                                                     >> 0x20U)))),16);
        bufp->chgSData(oldp+124,((0xffffU & (IData)(
                                                    (vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_wide_CRT 
                                                     >> 0x30U)))),16);
        bufp->chgCData(oldp+125,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT),4);
        bufp->chgBit(oldp+126,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__tristate_enable));
        bufp->chgBit(oldp+127,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__rx_valid));
        bufp->chgBit(oldp+128,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__tristate_enable_flop));
        bufp->chgCData(oldp+129,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__data_line_flop),4);
        bufp->chgIData(oldp+130,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__unnamedblk1__DOT__i),32);
        bufp->chgIData(oldp+131,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__unnamedblk2__DOT__i),32);
        bufp->chgIData(oldp+132,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__unnamedblk3__DOT__i),32);
        bufp->chgBit(oldp+133,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_fsm_activation_detector__DOT__previous_signal));
        bufp->chgBit(oldp+134,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_rx_write_detector__DOT__previous_signal));
        bufp->chgIData(oldp+135,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT____Vcellout__register_interface__data_tx_data_o),32);
        bufp->chgIData(oldp+136,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_rx),32);
        bufp->chgBit(oldp+137,((1U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register) 
                                      >> 0xeU))));
        bufp->chgBit(oldp+138,((1U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register) 
                                      >> 0xcU))));
        bufp->chgSData(oldp+139,((0x1ffU & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__control_register))),9);
        bufp->chgBit(oldp+140,((1U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__status_register) 
                                      >> 0xeU))));
        bufp->chgBit(oldp+141,((1U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__status_register) 
                                      >> 0xdU))));
        bufp->chgBit(oldp+142,((1U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__status_register) 
                                      >> 0xcU))));
        bufp->chgBit(oldp+143,((1U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__status_register) 
                                      >> 0xbU))));
        bufp->chgBit(oldp+144,((1U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__status_register) 
                                      >> 0xaU))));
        bufp->chgBit(oldp+145,((1U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__status_register) 
                                      >> 9U))));
        bufp->chgBit(oldp+146,((1U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__status_register) 
                                      >> 8U))));
        bufp->chgBit(oldp+147,((1U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__status_register) 
                                      >> 7U))));
        bufp->chgBit(oldp+148,((1U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__status_register) 
                                      >> 6U))));
        bufp->chgBit(oldp+149,((1U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__status_register) 
                                      >> 5U))));
        bufp->chgBit(oldp+150,((1U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__status_register) 
                                      >> 4U))));
        bufp->chgBit(oldp+151,((1U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__status_register) 
                                      >> 3U))));
        bufp->chgBit(oldp+152,((1U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__status_register) 
                                      >> 2U))));
        bufp->chgBit(oldp+153,((1U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__status_register) 
                                      >> 1U))));
        bufp->chgBit(oldp+154,((1U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__status_register))));
        bufp->chgBit(oldp+155,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__tx_buffer_full));
        bufp->chgBit(oldp+156,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__rx_buffer_empty));
        bufp->chgBit(oldp+157,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__resp_buffer_empty));
        bufp->chgBit(oldp+158,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__resp_buffer_full));
        bufp->chgBit(oldp+159,(((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__cmd_done_detector__DOT__previous_signal)) 
                                & (0U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_CRT)))));
        bufp->chgBit(oldp+160,(((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__data_done_detector__DOT__previous_signal)) 
                                & (0U == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__state_CRT)))));
        bufp->chgBit(oldp+161,((1U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register) 
                                      >> 8U))));
        bufp->chgBit(oldp+162,((1U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register) 
                                      >> 7U))));
        bufp->chgBit(oldp+163,((1U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register) 
                                      >> 6U))));
        bufp->chgBit(oldp+164,((1U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register) 
                                      >> 5U))));
        bufp->chgBit(oldp+165,((1U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register) 
                                      >> 4U))));
        bufp->chgBit(oldp+166,((1U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register) 
                                      >> 3U))));
        bufp->chgBit(oldp+167,((1U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register) 
                                      >> 2U))));
        bufp->chgBit(oldp+168,((1U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register) 
                                      >> 1U))));
        bufp->chgBit(oldp+169,((1U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register))));
        bufp->chgSData(oldp+170,(((((((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__genblk1__BRA__8__KET____DOT__event_detector__DOT__previous_signal)) 
                                      & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register) 
                                         >> 8U)) << 8U) 
                                    | ((0x80U & (((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__genblk1__BRA__7__KET____DOT__event_detector__DOT__previous_signal)) 
                                                  << 7U) 
                                                 & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register))) 
                                       | (0x40U & (
                                                   ((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__genblk1__BRA__6__KET____DOT__event_detector__DOT__previous_signal)) 
                                                    << 6U) 
                                                   & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register))))) 
                                   | ((0x20U & (((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__genblk1__BRA__5__KET____DOT__event_detector__DOT__previous_signal)) 
                                                 << 5U) 
                                                & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register))) 
                                      | (0x10U & ((
                                                   (~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__genblk1__BRA__4__KET____DOT__event_detector__DOT__previous_signal)) 
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
                                        | (1U & ((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__genblk1__BRA__0__KET____DOT__event_detector__DOT__previous_signal)) 
                                                 & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register))))))),9);
        bufp->chgIData(oldp+171,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__read_data_rx),32);
        bufp->chgCData(oldp+172,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__read_response_data),8);
        bufp->chgBit(oldp+173,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__read_done_delay));
        bufp->chgBit(oldp+174,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__cmd_done_detector__DOT__previous_signal));
        bufp->chgBit(oldp+175,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__data_done_detector__DOT__previous_signal));
        bufp->chgBit(oldp+176,((1U & ((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__genblk1__BRA__0__KET____DOT__event_detector__DOT__previous_signal)) 
                                      & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register)))));
        bufp->chgBit(oldp+177,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__genblk1__BRA__0__KET____DOT__event_detector__DOT__previous_signal));
        bufp->chgBit(oldp+178,((1U & ((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__genblk1__BRA__1__KET____DOT__event_detector__DOT__previous_signal)) 
                                      & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register) 
                                         >> 1U)))));
        bufp->chgBit(oldp+179,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__genblk1__BRA__1__KET____DOT__event_detector__DOT__previous_signal));
        bufp->chgBit(oldp+180,((1U & ((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__genblk1__BRA__2__KET____DOT__event_detector__DOT__previous_signal)) 
                                      & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register) 
                                         >> 2U)))));
        bufp->chgBit(oldp+181,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__genblk1__BRA__2__KET____DOT__event_detector__DOT__previous_signal));
        bufp->chgBit(oldp+182,((1U & ((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__genblk1__BRA__3__KET____DOT__event_detector__DOT__previous_signal)) 
                                      & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register) 
                                         >> 3U)))));
        bufp->chgBit(oldp+183,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__genblk1__BRA__3__KET____DOT__event_detector__DOT__previous_signal));
        bufp->chgBit(oldp+184,((1U & ((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__genblk1__BRA__4__KET____DOT__event_detector__DOT__previous_signal)) 
                                      & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register) 
                                         >> 4U)))));
        bufp->chgBit(oldp+185,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__genblk1__BRA__4__KET____DOT__event_detector__DOT__previous_signal));
        bufp->chgBit(oldp+186,((1U & ((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__genblk1__BRA__5__KET____DOT__event_detector__DOT__previous_signal)) 
                                      & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register) 
                                         >> 5U)))));
        bufp->chgBit(oldp+187,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__genblk1__BRA__5__KET____DOT__event_detector__DOT__previous_signal));
        bufp->chgBit(oldp+188,((1U & ((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__genblk1__BRA__6__KET____DOT__event_detector__DOT__previous_signal)) 
                                      & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register) 
                                         >> 6U)))));
        bufp->chgBit(oldp+189,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__genblk1__BRA__6__KET____DOT__event_detector__DOT__previous_signal));
        bufp->chgBit(oldp+190,((1U & ((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__genblk1__BRA__7__KET____DOT__event_detector__DOT__previous_signal)) 
                                      & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register) 
                                         >> 7U)))));
        bufp->chgBit(oldp+191,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__genblk1__BRA__7__KET____DOT__event_detector__DOT__previous_signal));
        bufp->chgBit(oldp+192,(((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__genblk1__BRA__8__KET____DOT__event_detector__DOT__previous_signal)) 
                                & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__event_register) 
                                   >> 8U))));
        bufp->chgBit(oldp+193,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__genblk1__BRA__8__KET____DOT__event_detector__DOT__previous_signal));
        bufp->chgCData(oldp+194,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__buffer_memory[0]),8);
        bufp->chgCData(oldp+195,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__buffer_memory[1]),8);
        bufp->chgCData(oldp+196,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__buffer_memory[2]),8);
        bufp->chgCData(oldp+197,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__buffer_memory[3]),8);
        bufp->chgCData(oldp+198,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__buffer_memory[4]),8);
        bufp->chgCData(oldp+199,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__buffer_memory[5]),8);
        bufp->chgCData(oldp+200,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__buffer_memory[6]),8);
        bufp->chgCData(oldp+201,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__buffer_memory[7]),8);
        bufp->chgCData(oldp+202,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__buffer_memory[8]),8);
        bufp->chgCData(oldp+203,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__buffer_memory[9]),8);
        bufp->chgCData(oldp+204,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__buffer_memory[10]),8);
        bufp->chgCData(oldp+205,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__buffer_memory[11]),8);
        bufp->chgCData(oldp+206,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__buffer_memory[12]),8);
        bufp->chgCData(oldp+207,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__buffer_memory[13]),8);
        bufp->chgCData(oldp+208,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__buffer_memory[14]),8);
        bufp->chgCData(oldp+209,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__buffer_memory[15]),8);
        bufp->chgCData(oldp+210,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__buffer_memory[16]),8);
        bufp->chgCData(oldp+211,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__buffer_memory[17]),8);
        bufp->chgCData(oldp+212,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__buffer_memory[18]),8);
        bufp->chgCData(oldp+213,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__buffer_memory[19]),8);
        bufp->chgCData(oldp+214,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__buffer_memory[20]),8);
        bufp->chgCData(oldp+215,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__buffer_memory[21]),8);
        bufp->chgCData(oldp+216,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__buffer_memory[22]),8);
        bufp->chgCData(oldp+217,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__buffer_memory[23]),8);
        bufp->chgCData(oldp+218,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__buffer_memory[24]),8);
        bufp->chgCData(oldp+219,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__buffer_memory[25]),8);
        bufp->chgCData(oldp+220,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__buffer_memory[26]),8);
        bufp->chgCData(oldp+221,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__buffer_memory[27]),8);
        bufp->chgCData(oldp+222,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__buffer_memory[28]),8);
        bufp->chgCData(oldp+223,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__buffer_memory[29]),8);
        bufp->chgCData(oldp+224,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__buffer_memory[30]),8);
        bufp->chgCData(oldp+225,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__buffer_memory[31]),8);
        bufp->chgCData(oldp+226,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__write_ptr),5);
        bufp->chgCData(oldp+227,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__read_ptr),5);
        bufp->chgCData(oldp+228,((0x1fU & ((IData)(1U) 
                                           + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__write_ptr)))),5);
        bufp->chgCData(oldp+229,((0x1fU & ((IData)(1U) 
                                           + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__response_buffer__DOT__read_ptr)))),5);
        bufp->chgCData(oldp+230,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__rx_buffer__DOT__write_ptr),7);
        bufp->chgCData(oldp+231,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__rx_buffer__DOT__read_ptr),7);
        bufp->chgCData(oldp+232,((0x7fU & ((IData)(1U) 
                                           + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__rx_buffer__DOT__write_ptr)))),7);
        bufp->chgCData(oldp+233,((0x7fU & ((IData)(1U) 
                                           + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__rx_buffer__DOT__read_ptr)))),7);
        bufp->chgBit(oldp+234,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT____Vcellinp__tx_buffer__read_i));
        bufp->chgCData(oldp+235,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__tx_buffer__DOT__write_ptr),7);
        bufp->chgCData(oldp+236,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__tx_buffer__DOT__read_ptr),7);
        bufp->chgCData(oldp+237,((0x7fU & ((IData)(1U) 
                                           + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__tx_buffer__DOT__write_ptr)))),7);
        bufp->chgCData(oldp+238,((0x7fU & ((IData)(1U) 
                                           + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT__tx_buffer__DOT__read_ptr)))),7);
    }
    if (VL_UNLIKELY((vlSelfRef.__Vm_traceActivity[4U]))) {
        bufp->chgIData(oldp+239,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_wr_sd_q),32);
        bufp->chgCData(oldp+240,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__card_state_s),4);
        bufp->chgCData(oldp+241,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__c__DOT__stage_3),4);
        bufp->chgSData(oldp+242,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__d__DOT__stage_3),10);
        bufp->chgBit(oldp+243,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__f__DOT__stage_3));
        bufp->chgCData(oldp+244,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__g__DOT__stage_1),4);
        bufp->chgCData(oldp+245,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__g__DOT__stage_2),4);
        bufp->chgCData(oldp+246,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__g__DOT__stage_3),4);
        bufp->chgBit(oldp+247,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__h__DOT__stage_3));
        bufp->chgBit(oldp+248,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__i__DOT__stage_3));
        bufp->chgBit(oldp+249,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__j__DOT__stage_3));
        bufp->chgBit(oldp+250,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__k__DOT__stage_3));
        bufp->chgBit(oldp+251,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__l__DOT__stage_3));
    }
    if (VL_UNLIKELY(((vlSelfRef.__Vm_traceActivity[4U] 
                      | vlSelfRef.__Vm_traceActivity
                      [0xbU])))) {
        bufp->chgBit(oldp+252,(((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__h__DOT__stage_3)) 
                                & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__mode_4bit_s))));
        bufp->chgBit(oldp+253,(((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__i__DOT__stage_3)) 
                                & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_in_stop_s))));
        bufp->chgBit(oldp+254,(((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__j__DOT__stage_3)) 
                                & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_in_another_s))));
        bufp->chgBit(oldp+255,(((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__k__DOT__stage_3)) 
                                & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__mode_spi_s))));
        bufp->chgBit(oldp+256,(((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__l__DOT__stage_3)) 
                                & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__mode_crc_disable_s))));
    }
    if (VL_UNLIKELY((vlSelfRef.__Vm_traceActivity[5U]))) {
        bufp->chgIData(oldp+257,(vlSelfRef.tb_top__DOT__u_dut__DOT__wbm_adr_o),32);
        bufp->chgIData(oldp+258,(vlSelfRef.tb_top__DOT__u_dut__DOT__wbm_dat_i),32);
        bufp->chgIData(oldp+259,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_wr_ext_q),32);
        bufp->chgCData(oldp+260,(vlSelfRef.tb_top__DOT__u_dut__DOT__wbm_sel_o),4);
        bufp->chgBit(oldp+261,(vlSelfRef.tb_top__DOT__u_dut__DOT__wbm_cyc_o));
        bufp->chgBit(oldp+262,(vlSelfRef.tb_top__DOT__u_dut__DOT__wbm_stb_o));
        bufp->chgBit(oldp+263,(vlSelfRef.tb_top__DOT__u_dut__DOT__wbm_we_o));
        bufp->chgBit(oldp+264,(vlSelfRef.tb_top__DOT__u_dut__DOT__wbm_ack_i));
        bufp->chgCData(oldp+265,(vlSelfRef.tb_top__DOT__u_dut__DOT__wbm_cti_o),3);
        bufp->chgCData(oldp+266,(vlSelfRef.tb_top__DOT__u_dut__DOT__wbm_bte_o),2);
        bufp->chgSData(oldp+267,((0x3ffU & (vlSelfRef.tb_top__DOT__u_dut__DOT__wbm_adr_o 
                                            >> 2U))),10);
        bufp->chgCData(oldp+268,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_rd_ext_addr),7);
        bufp->chgBit(oldp+269,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_rd_ext_wren));
        bufp->chgIData(oldp+270,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_rd_ext_data),32);
        bufp->chgIData(oldp+271,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_rd_ext_q),32);
        bufp->chgCData(oldp+272,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_wr_ext_addr),7);
        bufp->chgBit(oldp+273,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__ext_read_act));
        bufp->chgBit(oldp+274,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__ext_read_go));
        bufp->chgIData(oldp+275,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__ext_read_addr),32);
        bufp->chgBit(oldp+276,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__ext_read_stop));
        bufp->chgBit(oldp+277,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__ext_write_act));
        bufp->chgBit(oldp+278,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__ext_write_done));
        bufp->chgIData(oldp+279,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__ext_write_addr),32);
        bufp->chgQData(oldp+280,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_s),48);
        bufp->chgBit(oldp+282,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__a__DOT__stage_3));
        bufp->chgQData(oldp+283,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__b__DOT__stage_1),48);
        bufp->chgQData(oldp+285,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__b__DOT__stage_2),48);
        bufp->chgQData(oldp+287,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__b__DOT__stage_3),48);
        bufp->chgBit(oldp+289,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__c__DOT__stage_3));
        bufp->chgBit(oldp+290,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__e__DOT__stage_3));
        bufp->chgBit(oldp+291,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__g__DOT__stage_3));
        bufp->chgBit(oldp+292,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__i__DOT__stage_3));
        bufp->chgBit(oldp+293,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__k__DOT__stage_3));
        bufp->chgIData(oldp+294,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__link_read_addr_latch),32);
        bufp->chgCData(oldp+295,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__state),5);
        bufp->chgBit(oldp+296,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__reset_s));
        bufp->chgBit(oldp+297,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__ext_read_go_s));
        bufp->chgBit(oldp+298,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__ext_read_go_r));
        bufp->chgBit(oldp+299,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__ext_write_done_s));
        bufp->chgBit(oldp+300,(((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__c__DOT__stage_3)) 
                                & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__ext_write_done_s))));
        bufp->chgBit(oldp+301,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__sel_rd_ext));
        bufp->chgBit(oldp+302,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__buf_rd_a_full));
        bufp->chgBit(oldp+303,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__buf_rd_b_full));
        bufp->chgIData(oldp+304,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__buf_rd_a_block),32);
        bufp->chgIData(oldp+305,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__buf_rd_b_block),32);
        bufp->chgCData(oldp+306,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__bram_rd_ext_addr_sel),8);
        bufp->chgBit(oldp+307,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__sel_wr_sd));
        bufp->chgBit(oldp+308,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__sel_wr_ext));
        bufp->chgCData(oldp+309,((0xffU & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__sel_wr_ext)
                                            ? ((IData)(0x80U) 
                                               + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_wr_ext_addr))
                                            : (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_wr_ext_addr)))),8);
        bufp->chgBit(oldp+310,(((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__a__DOT__stage_3)) 
                                & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__reset_s))));
        bufp->chgBit(oldp+311,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__a__DOT__stage_1));
        bufp->chgBit(oldp+312,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__a__DOT__stage_2));
        bufp->chgBit(oldp+313,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__a__DOT__stage_3));
        bufp->chgBit(oldp+314,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__b__DOT__stage_1));
        bufp->chgBit(oldp+315,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__b__DOT__stage_2));
        bufp->chgBit(oldp+316,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__b__DOT__stage_3));
        bufp->chgBit(oldp+317,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__c__DOT__stage_1));
        bufp->chgBit(oldp+318,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__c__DOT__stage_2));
        bufp->chgBit(oldp+319,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__c__DOT__stage_3));
        bufp->chgCData(oldp+320,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__state),5);
        bufp->chgCData(oldp+321,((0x7fU & ((IData)(1U) 
                                           + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_rd_ext_addr)))),7);
        bufp->chgSData(oldp+322,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__bytes_done),16);
        bufp->chgIData(oldp+323,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__ext_read_addr_latch),32);
        bufp->chgIData(oldp+324,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__ext_write_addr_latch),32);
        bufp->chgBit(oldp+325,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__ext_read_act_last));
        bufp->chgBit(oldp+326,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__ext_write_act_last));
        bufp->chgBit(oldp+327,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__wbm_ack_i_last));
        bufp->chgBit(oldp+328,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__reset_s));
        bufp->chgBit(oldp+329,(((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__a__DOT__stage_3)) 
                                & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__reset_s))));
        bufp->chgBit(oldp+330,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__a__DOT__stage_1));
        bufp->chgBit(oldp+331,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__a__DOT__stage_2));
        bufp->chgBit(oldp+332,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdw__DOT__a__DOT__stage_3));
    }
    if (VL_UNLIKELY(((vlSelfRef.__Vm_traceActivity[5U] 
                      | vlSelfRef.__Vm_traceActivity
                      [8U])))) {
        bufp->chgBit(oldp+333,(((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__a__DOT__stage_3)) 
                                & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__reset_s))));
        bufp->chgBit(oldp+334,(((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__c__DOT__stage_3)) 
                                & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_crc_good_s))));
        bufp->chgBit(oldp+335,(((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__e__DOT__stage_3)) 
                                & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_in_busy_s))));
        bufp->chgBit(oldp+336,(((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__g__DOT__stage_3)) 
                                & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_in_crc_good_s))));
        bufp->chgBit(oldp+337,(((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__i__DOT__stage_3)) 
                                & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_out_busy_s))));
        bufp->chgBit(oldp+338,(((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__k__DOT__stage_3)) 
                                & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__spi_sel_s))));
    }
    if (VL_UNLIKELY((vlSelfRef.__Vm_traceActivity[6U]))) {
        bufp->chgBit(oldp+339,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_cmd_out));
        bufp->chgSData(oldp+340,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__odc),11);
        bufp->chgCData(oldp+341,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__ostate),7);
        bufp->chgCData(oldp+342,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_out),4);
        bufp->chgWData(oldp+343,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__resp_out_latch),136);
        bufp->chgCData(oldp+348,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc7_out),7);
        bufp->chgWData(oldp+349,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_reg_latch),513);
        bufp->chgSData(oldp+366,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_out3),16);
        bufp->chgSData(oldp+367,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_out2),16);
        bufp->chgSData(oldp+368,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_out1),16);
        bufp->chgSData(oldp+369,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_out0),16);
        bufp->chgSData(oldp+370,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dodc),16);
        bufp->chgCData(oldp+371,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dostate),7);
        bufp->chgIData(oldp+372,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dout_buf),32);
    }
    if (VL_UNLIKELY((vlSelfRef.__Vm_traceActivity[7U]))) {
        bufp->chgIData(oldp+373,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_rd_sd_q),32);
        bufp->chgBit(oldp+374,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__resp_act_s));
        bufp->chgBit(oldp+375,(((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__b__DOT__stage_3)) 
                                & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__resp_act_s))));
        bufp->chgCData(oldp+376,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__resp_type_s),4);
        bufp->chgSData(oldp+377,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_len_s),10);
        bufp->chgBit(oldp+378,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_act_s));
        bufp->chgBit(oldp+379,(((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__e__DOT__stage_3)) 
                                & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_act_s))));
        bufp->chgBit(oldp+380,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_stop_s));
        bufp->chgBit(oldp+381,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__b__DOT__stage_1));
        bufp->chgBit(oldp+382,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__b__DOT__stage_2));
        bufp->chgBit(oldp+383,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__b__DOT__stage_3));
        bufp->chgCData(oldp+384,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__c__DOT__stage_1),4);
        bufp->chgCData(oldp+385,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__c__DOT__stage_2),4);
        bufp->chgSData(oldp+386,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__d__DOT__stage_1),10);
        bufp->chgSData(oldp+387,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__d__DOT__stage_2),10);
        bufp->chgBit(oldp+388,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__e__DOT__stage_1));
        bufp->chgBit(oldp+389,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__e__DOT__stage_2));
        bufp->chgBit(oldp+390,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__e__DOT__stage_3));
        bufp->chgBit(oldp+391,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__f__DOT__stage_1));
        bufp->chgBit(oldp+392,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__f__DOT__stage_2));
    }
    if (VL_UNLIKELY((vlSelfRef.__Vm_traceActivity[8U]))) {
        bufp->chgBit(oldp+393,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__link_read_act));
        bufp->chgBit(oldp+394,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__link_read_go));
        bufp->chgIData(oldp+395,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__link_read_addr),32);
        bufp->chgIData(oldp+396,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__link_read_num),32);
        bufp->chgBit(oldp+397,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__link_read_stop));
        bufp->chgBit(oldp+398,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__link_write_act));
        bufp->chgBit(oldp+399,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__link_write_done));
        bufp->chgIData(oldp+400,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__link_write_addr),32);
        bufp->chgIData(oldp+401,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__link_write_num),32);
        bufp->chgCData(oldp+402,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_type),4);
        bufp->chgBit(oldp+403,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_act));
        bufp->chgCData(oldp+404,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state),4);
        bufp->chgCData(oldp+405,((0x3fU & (IData)((vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_latch 
                                                   >> 0x28U)))),6);
        bufp->chgWData(oldp+406,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_out),136);
        bufp->chgBit(oldp+411,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_busy));
        bufp->chgWData(oldp+412,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_reg),512);
        bufp->chgBit(oldp+428,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_src));
        bufp->chgSData(oldp+429,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_len),10);
        bufp->chgBit(oldp+430,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_act));
        bufp->chgBit(oldp+431,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_stop));
        bufp->chgIData(oldp+432,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__block_read_byteaddr),32);
        bufp->chgIData(oldp+433,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__block_write_byteaddr),32);
        bufp->chgIData(oldp+434,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__block_preerase_num),23);
        bufp->chgIData(oldp+435,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__block_erase_start),32);
        bufp->chgIData(oldp+436,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__block_erase_end),32);
        bufp->chgCData(oldp+437,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_last),6);
        bufp->chgBit(oldp+438,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__info_card_desel));
        bufp->chgBit(oldp+439,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__err_op_out_range));
        bufp->chgBit(oldp+440,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__err_unhandled_cmd));
        bufp->chgBit(oldp+441,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__err_cmd_crc));
        bufp->chgBit(oldp+442,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__host_hc_support));
        bufp->chgQData(oldp+443,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_latch),48);
        bufp->chgIData(oldp+445,((IData)((vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_latch 
                                          >> 8U))),32);
        bufp->chgCData(oldp+446,((0x7fU & (IData)((vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_latch 
                                                   >> 1U)))),7);
        bufp->chgIData(oldp+447,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_arg_blockaddr),32);
        bufp->chgIData(oldp+448,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_arg_byteaddr),32);
        bufp->chgCData(oldp+449,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_state_next),4);
        bufp->chgCData(oldp+450,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_erase_state),3);
        bufp->chgBit(oldp+451,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_appcmd));
        bufp->chgIData(oldp+452,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_status),32);
        bufp->chgWData(oldp+453,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_sd_status),128);
        bufp->chgWData(oldp+457,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_csd),128);
        bufp->chgWData(oldp+461,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_cid),128);
        bufp->chgIData(oldp+465,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_ocr),32);
        bufp->chgSData(oldp+466,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_rca),16);
        bufp->chgQData(oldp+467,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_scr),64);
        bufp->chgWData(oldp+469,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function_caps),112);
        bufp->chgIData(oldp+473,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function),24);
        bufp->chgIData(oldp+474,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_function_check),24);
        bufp->chgIData(oldp+475,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__card_blocks_written),32);
        bufp->chgWData(oldp+476,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_arg),128);
        bufp->chgCData(oldp+480,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_type),4);
        bufp->chgSData(oldp+481,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_in_gap_cnt),10);
        bufp->chgCData(oldp+482,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__state),7);
        bufp->chgCData(oldp+483,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_state),7);
        bufp->chgSData(oldp+484,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__spi_status_word),16);
        bufp->chgBit(oldp+485,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_scr));
        bufp->chgBit(oldp+486,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_cid));
        bufp->chgBit(oldp+487,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_csd));
        bufp->chgBit(oldp+488,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_sdstatus));
        bufp->chgBit(oldp+489,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_function));
        bufp->chgBit(oldp+490,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_written));
        bufp->chgBit(oldp+491,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_block));
        bufp->chgBit(oldp+492,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_send_block_queue));
        bufp->chgBit(oldp+493,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_op_recv_block));
        bufp->chgBit(oldp+494,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__reset_s));
        bufp->chgBit(oldp+495,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_crc_good_s));
        bufp->chgBit(oldp+496,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_act_s));
        bufp->chgBit(oldp+497,(((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__d__DOT__stage_3)) 
                                & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__cmd_in_act_s))));
        bufp->chgBit(oldp+498,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__spi_sel_s));
        bufp->chgBit(oldp+499,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_in_busy_s));
        bufp->chgBit(oldp+500,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_in_done_s));
        bufp->chgBit(oldp+501,(((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__f__DOT__stage_3)) 
                                & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_in_done_s))));
        bufp->chgBit(oldp+502,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_in_crc_good_s));
        bufp->chgBit(oldp+503,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_done_s));
        bufp->chgBit(oldp+504,(((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__h__DOT__stage_3)) 
                                & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__resp_done_s))));
        bufp->chgBit(oldp+505,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_out_busy_s));
        bufp->chgBit(oldp+506,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_out_done_s));
        bufp->chgBit(oldp+507,(((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__j__DOT__stage_3)) 
                                & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__data_out_done_s))));
        bufp->chgBit(oldp+508,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__a__DOT__stage_1));
        bufp->chgBit(oldp+509,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__a__DOT__stage_2));
        bufp->chgBit(oldp+510,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__c__DOT__stage_1));
        bufp->chgBit(oldp+511,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__c__DOT__stage_2));
        bufp->chgBit(oldp+512,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__d__DOT__stage_1));
        bufp->chgBit(oldp+513,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__d__DOT__stage_2));
        bufp->chgBit(oldp+514,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__d__DOT__stage_3));
        bufp->chgBit(oldp+515,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__e__DOT__stage_1));
        bufp->chgBit(oldp+516,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__e__DOT__stage_2));
        bufp->chgBit(oldp+517,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__f__DOT__stage_1));
        bufp->chgBit(oldp+518,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__f__DOT__stage_2));
        bufp->chgBit(oldp+519,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__f__DOT__stage_3));
        bufp->chgBit(oldp+520,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__g__DOT__stage_1));
        bufp->chgBit(oldp+521,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__g__DOT__stage_2));
        bufp->chgBit(oldp+522,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__h__DOT__stage_1));
        bufp->chgBit(oldp+523,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__h__DOT__stage_2));
        bufp->chgBit(oldp+524,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__h__DOT__stage_3));
        bufp->chgBit(oldp+525,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__i__DOT__stage_1));
        bufp->chgBit(oldp+526,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__i__DOT__stage_2));
        bufp->chgBit(oldp+527,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__j__DOT__stage_1));
        bufp->chgBit(oldp+528,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__j__DOT__stage_2));
        bufp->chgBit(oldp+529,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__j__DOT__stage_3));
        bufp->chgBit(oldp+530,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__k__DOT__stage_1));
        bufp->chgBit(oldp+531,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdl__DOT__k__DOT__stage_2));
        bufp->chgBit(oldp+532,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__sel_rd_sd));
    }
    if (VL_UNLIKELY((vlSelfRef.__Vm_traceActivity[9U]))) {
        bufp->chgCData(oldp+533,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_rd_sd_addr),7);
        bufp->chgBit(oldp+534,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_resp_done));
        bufp->chgBit(oldp+535,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_busy));
        bufp->chgBit(oldp+536,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_out_done));
    }
    if (VL_UNLIKELY((vlSelfRef.__Vm_traceActivity[0xaU]))) {
        bufp->chgCData(oldp+537,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_wr_sd_addr),7);
        bufp->chgBit(oldp+538,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_wr_sd_wren));
        bufp->chgIData(oldp+539,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_wr_sd_data),32);
        bufp->chgQData(oldp+540,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_cmd_in),48);
        bufp->chgBit(oldp+542,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_cmd_in_crc_good));
        bufp->chgBit(oldp+543,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_cmd_in_act));
        bufp->chgBit(oldp+544,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_in_busy));
        bufp->chgBit(oldp+545,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_in_done));
        bufp->chgBit(oldp+546,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_in_crc_good));
        bufp->chgBit(oldp+547,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_cmd_last));
        bufp->chgBit(oldp+548,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_last));
        bufp->chgQData(oldp+549,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__cmd_in_latch),47);
        bufp->chgCData(oldp+551,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc7_in),7);
        bufp->chgSData(oldp+552,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_in3),16);
        bufp->chgSData(oldp+553,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_in2),16);
        bufp->chgSData(oldp+554,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_in1),16);
        bufp->chgSData(oldp+555,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_in0),16);
        bufp->chgSData(oldp+556,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_check3),16);
        bufp->chgSData(oldp+557,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_check2),16);
        bufp->chgSData(oldp+558,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_check1),16);
        bufp->chgSData(oldp+559,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__crc16_check0),16);
        bufp->chgSData(oldp+560,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__idc),11);
        bufp->chgCData(oldp+561,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__istate),7);
        bufp->chgSData(oldp+562,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__didc),16);
        bufp->chgCData(oldp+563,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__distate),7);
        bufp->chgCData(oldp+564,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__spi_di_token),8);
        bufp->chgBit(oldp+565,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__do_crc_token));
    }
    if (VL_UNLIKELY((vlSelfRef.__Vm_traceActivity[0xbU]))) {
        bufp->chgBit(oldp+566,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_in_act_s));
        bufp->chgBit(oldp+567,(((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__a__DOT__stage_3)) 
                                & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_in_act_s))));
        bufp->chgBit(oldp+568,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_in_stop_s));
        bufp->chgBit(oldp+569,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_in_another_s));
        bufp->chgBit(oldp+570,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__mode_4bit_s));
        bufp->chgBit(oldp+571,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__mode_spi_s));
        bufp->chgBit(oldp+572,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__mode_crc_disable_s));
        bufp->chgBit(oldp+573,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__a__DOT__stage_1));
        bufp->chgBit(oldp+574,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__a__DOT__stage_2));
        bufp->chgBit(oldp+575,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__a__DOT__stage_3));
        bufp->chgBit(oldp+576,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__h__DOT__stage_1));
        bufp->chgBit(oldp+577,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__h__DOT__stage_2));
        bufp->chgBit(oldp+578,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__i__DOT__stage_1));
        bufp->chgBit(oldp+579,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__i__DOT__stage_2));
        bufp->chgBit(oldp+580,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__j__DOT__stage_1));
        bufp->chgBit(oldp+581,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__j__DOT__stage_2));
        bufp->chgBit(oldp+582,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__k__DOT__stage_1));
        bufp->chgBit(oldp+583,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__k__DOT__stage_2));
        bufp->chgBit(oldp+584,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__l__DOT__stage_1));
        bufp->chgBit(oldp+585,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__l__DOT__stage_2));
    }
    if (VL_UNLIKELY((vlSelfRef.__Vm_traceActivity[0xcU]))) {
        bufp->chgBit(oldp+586,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_cmd_io));
        bufp->chgCData(oldp+587,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__resp_NXT),8);
        bufp->chgCData(oldp+588,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7_NXT),7);
        bufp->chgBit(oldp+589,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__bit_increment));
        bufp->chgBit(oldp+590,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__timeout_reset));
        bufp->chgBit(oldp+591,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7_compute));
        bufp->chgBit(oldp+592,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__crc7_data));
        bufp->chgCData(oldp+593,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__command_controller__DOT__state_NXT),4);
    }
    if (VL_UNLIKELY((vlSelfRef.__Vm_traceActivity[0xdU]))) {
        bufp->chgBit(oldp+594,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_in_act));
        bufp->chgBit(oldp+595,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_in_another));
        bufp->chgBit(oldp+596,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_data_in_stop));
        bufp->chgBit(oldp+597,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_mode_4bit));
        bufp->chgBit(oldp+598,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_mode_spi));
        bufp->chgBit(oldp+599,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__phy_mode_crc_disable));
    }
    bufp->chgBit(oldp+600,(vlSelfRef.clk));
    bufp->chgBit(oldp+601,(vlSelfRef.rst_n));
    bufp->chgBit(oldp+602,(vlSelfRef.write_i));
    bufp->chgIData(oldp+603,(vlSelfRef.write_address_i),32);
    bufp->chgIData(oldp+604,(vlSelfRef.write_data_i),32);
    bufp->chgCData(oldp+605,(vlSelfRef.write_strobe_i),4);
    bufp->chgBit(oldp+606,(vlSelfRef.write_done_o));
    bufp->chgBit(oldp+607,(vlSelfRef.write_error_o));
    bufp->chgBit(oldp+608,(vlSelfRef.read_i));
    bufp->chgIData(oldp+609,(vlSelfRef.read_address_i),32);
    bufp->chgIData(oldp+610,(vlSelfRef.read_data_o),32);
    bufp->chgBit(oldp+611,(vlSelfRef.read_done_o));
    bufp->chgBit(oldp+612,(vlSelfRef.read_error_o));
    bufp->chgBit(oldp+613,(vlSelfRef.interrupt_o));
    bufp->chgBit(oldp+614,(vlSelfRef.tb_top__DOT__u_dut__DOT__clk_50MHz));
    bufp->chgBit(oldp+615,(vlSelfRef.tb_top__DOT__u_dut__DOT__clk_100MHz));
    bufp->chgBit(oldp+616,((1U & ((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_cmd_oe)) 
                                  | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__mode_spi_s)))));
    bufp->chgCData(oldp+617,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_dat_o),4);
    bufp->chgIData(oldp+618,(vlSelfRef.tb_top__DOT__u_dut__DOT__SD_memory__DOT__unnamedblk1__DOT__i),32);
    bufp->chgCData(oldp+619,((7U & vlSelfRef.write_address_i)),3);
    bufp->chgCData(oldp+620,((0xffU & vlSelfRef.write_data_i)),8);
    bufp->chgCData(oldp+621,((0xffU & (vlSelfRef.write_data_i 
                                       >> 8U))),8);
    bufp->chgCData(oldp+622,((0xffU & (vlSelfRef.write_data_i 
                                       >> 0x10U))),8);
    bufp->chgCData(oldp+623,((vlSelfRef.write_data_i 
                              >> 0x18U)),8);
    bufp->chgCData(oldp+624,((7U & vlSelfRef.read_address_i)),3);
    bufp->chgSData(oldp+625,((((0xe000U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_CRT) 
                                           << 1U)) 
                               | (0x1000U & ((0xfffff000U 
                                              & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_CRT) 
                                                 << 1U)) 
                                             ^ ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT____VdfgRegularize_h6e830a37_0_5) 
                                                << 0xcU)))) 
                              | ((0xfc0U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_CRT) 
                                            << 1U)) 
                                 | ((0x20U & ((0xffffffe0U 
                                               & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_CRT) 
                                                  << 1U)) 
                                              ^ ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT____VdfgRegularize_h6e830a37_0_5) 
                                                 << 5U))) 
                                    | ((0x1eU & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT__crc_CRT) 
                                                 << 1U)) 
                                       | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__data_controller__DOT__crc16__DOT____VdfgRegularize_h6e830a37_0_5)))))),16);
    bufp->chgBit(oldp+626,(vlSelfRef.tb_top__DOT__u_dut__DOT__dut__DOT__register_interface__DOT____Vcellinp__tx_buffer__rst_n_i));
    bufp->chgBit(oldp+627,((1U & ((~ ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_oe) 
                                      >> 3U)) & (~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__spi_cs))))));
    bufp->chgCData(oldp+628,((0xffU & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__sel_rd_sd)
                                        ? ((IData)(0x80U) 
                                           + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_rd_sd_addr))
                                        : (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__bram_rd_sd_addr)))),8);
    bufp->chgCData(oldp+629,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdm__DOT__bram_wr_sd_addr_sel),8);
    bufp->chgCData(oldp+630,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__spi_cnt),8);
    bufp->chgBit(oldp+631,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_cmd_oe));
    bufp->chgBit(oldp+632,((1U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_cmd_oe)
                                   ? (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_cmd_out)
                                   : ((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_oe)) 
                                      | (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_out))))));
    bufp->chgCData(oldp+633,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__sd_dat_oe),4);
    bufp->chgCData(oldp+634,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dout_4bit),4);
    bufp->chgBit(oldp+635,(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__dout_1bit));
    bufp->chgBit(oldp+636,(((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__f__DOT__stage_3)) 
                            & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__sd_model__DOT__isdph__DOT__data_out_stop_s))));
}

void Vtb_top___024root__trace_cleanup(void* voidSelf, VerilatedFst* /*unused*/) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_top___024root__trace_cleanup\n"); );
    // Init
    Vtb_top___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vtb_top___024root*>(voidSelf);
    Vtb_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    // Body
    vlSymsp->__Vm_activity = false;
    vlSymsp->TOP.__Vm_traceActivity[0U] = 0U;
    vlSymsp->TOP.__Vm_traceActivity[1U] = 0U;
    vlSymsp->TOP.__Vm_traceActivity[2U] = 0U;
    vlSymsp->TOP.__Vm_traceActivity[3U] = 0U;
    vlSymsp->TOP.__Vm_traceActivity[4U] = 0U;
    vlSymsp->TOP.__Vm_traceActivity[5U] = 0U;
    vlSymsp->TOP.__Vm_traceActivity[6U] = 0U;
    vlSymsp->TOP.__Vm_traceActivity[7U] = 0U;
    vlSymsp->TOP.__Vm_traceActivity[8U] = 0U;
    vlSymsp->TOP.__Vm_traceActivity[9U] = 0U;
    vlSymsp->TOP.__Vm_traceActivity[0xaU] = 0U;
    vlSymsp->TOP.__Vm_traceActivity[0xbU] = 0U;
    vlSymsp->TOP.__Vm_traceActivity[0xcU] = 0U;
    vlSymsp->TOP.__Vm_traceActivity[0xdU] = 0U;
    vlSymsp->TOP.__Vm_traceActivity[0xeU] = 0U;
}
