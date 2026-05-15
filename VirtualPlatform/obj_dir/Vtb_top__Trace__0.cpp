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
    if (VL_UNLIKELY(((vlSelfRef.__Vm_traceActivity[1U] 
                      | vlSelfRef.__Vm_traceActivity
                      [3U])))) {
        bufp->chgBit(oldp+0,(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmit));
        bufp->chgBit(oldp+1,(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__tx_idle));
        bufp->chgBit(oldp+2,(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT____Vcellinp__TX_buffer__read_i));
        bufp->chgBit(oldp+3,(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__counter_reset));
        bufp->chgBit(oldp+4,(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__reset_bit_count));
        bufp->chgCData(oldp+5,(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__state_NXT),3);
    }
    if (VL_UNLIKELY((vlSelfRef.__Vm_traceActivity[2U]))) {
        bufp->chgBit(oldp+6,(vlSelfRef.tb_top__DOT__u_dut__DOT__uart_tx));
        bufp->chgBit(oldp+7,((1U & ((vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register 
                                     >> 0x10U) & (~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__rx_full))))));
        bufp->chgCData(oldp+8,(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__rx_data),8);
        bufp->chgCData(oldp+9,(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__tx_data),8);
        bufp->chgBit(oldp+10,((1U & ((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__rx_full)) 
                                     & (vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register 
                                        >> 0x10U)))));
        bufp->chgBit(oldp+11,(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__tx_done));
        bufp->chgBit(oldp+12,(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__rx_done));
        bufp->chgBit(oldp+13,(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__rx_idle));
        bufp->chgBit(oldp+14,(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__parity_error));
        bufp->chgBit(oldp+15,(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__tx_empty));
        bufp->chgCData(oldp+16,((3U & (vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register 
                                       >> 0xeU))),2);
        bufp->chgBit(oldp+17,((1U & (vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register 
                                     >> 0xdU))));
        bufp->chgBit(oldp+18,((1U & (vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register 
                                     >> 0xcU))));
        bufp->chgBit(oldp+19,((1U & (vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register 
                                     >> 0xbU))));
        bufp->chgBit(oldp+20,((1U & (vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register 
                                     >> 0xaU))));
        bufp->chgBit(oldp+21,((1U & (vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register 
                                     >> 9U))));
        bufp->chgSData(oldp+22,((vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register 
                                 >> 0x11U)),15);
        bufp->chgBit(oldp+23,((1U & (vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register 
                                     >> 0x10U))));
        bufp->chgBit(oldp+24,(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__read_done_delay));
        bufp->chgBit(oldp+25,(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__sample));
        bufp->chgBit(oldp+26,(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__uart_tx));
        bufp->chgBit(oldp+27,((1U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__rx_line_sync__DOT__sync_signal) 
                                     >> 1U))));
        bufp->chgSData(oldp+28,(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__baud_rate_generator__DOT__counter),15);
        bufp->chgCData(oldp+29,(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__sampling_counter),4);
        bufp->chgBit(oldp+30,(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__counter_enable));
        bufp->chgBit(oldp+31,(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__counter_reset));
        bufp->chgCData(oldp+32,(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__rx_data),8);
        bufp->chgBit(oldp+33,(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__shift_data));
        bufp->chgBit(oldp+34,(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__start_receiving));
        bufp->chgBit(oldp+35,(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__parity_bit));
        bufp->chgBit(oldp+36,(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__parity_error));
        bufp->chgBit(oldp+37,(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__update_parity));
        bufp->chgBit(oldp+38,(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__check_parity));
        bufp->chgCData(oldp+39,(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__bit_count),3);
        bufp->chgCData(oldp+40,(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__stop_bit_CRT),3);
        bufp->chgCData(oldp+41,(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__stop_bit_NXT),3);
        bufp->chgBit(oldp+42,(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__reset_bit_count));
        bufp->chgCData(oldp+43,(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__state_CRT),3);
        bufp->chgCData(oldp+44,(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__state_NXT),3);
        bufp->chgBit(oldp+45,((0xfU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__sampling_counter))));
        bufp->chgCData(oldp+46,((3U & (vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register 
                                       >> 0xeU))),2);
        bufp->chgBit(oldp+47,((1U & (vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register 
                                     >> 0xdU))));
        bufp->chgBit(oldp+48,((1U & (vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register 
                                     >> 0xcU))));
        bufp->chgCData(oldp+49,((0x1fU & (vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register 
                                          >> 4U))),5);
        bufp->chgBit(oldp+50,((1U & (vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register 
                                     >> 3U))));
        bufp->chgBit(oldp+51,((1U & (vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register 
                                     >> 2U))));
        bufp->chgBit(oldp+52,((1U & (vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register 
                                     >> 1U))));
        bufp->chgBit(oldp+53,((1U & vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register)));
        bufp->chgBit(oldp+54,(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__tx_full));
        bufp->chgCData(oldp+55,(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__rx_buffer_read),8);
        bufp->chgBit(oldp+56,(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__rx_empty));
        bufp->chgBit(oldp+57,(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__rx_full));
        bufp->chgCData(oldp+58,(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__event_register),5);
        bufp->chgCData(oldp+59,((((((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__genblk1__BRA__4__KET____DOT__event_detector__DOT__previous_signal)) 
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
                                             & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__event_register)))))),5);
        bufp->chgBit(oldp+60,(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT____Vcellinp__RX_buffer__write_i));
        bufp->chgSData(oldp+61,(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__RX_buffer__DOT__write_ptr),9);
        bufp->chgSData(oldp+62,(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__RX_buffer__DOT__read_ptr),9);
        bufp->chgSData(oldp+63,((0x1ffU & ((IData)(1U) 
                                           + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__RX_buffer__DOT__write_ptr)))),9);
        bufp->chgSData(oldp+64,((0x1ffU & ((IData)(1U) 
                                           + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__RX_buffer__DOT__read_ptr)))),9);
        bufp->chgSData(oldp+65,(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__TX_buffer__DOT__write_ptr),9);
        bufp->chgSData(oldp+66,(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__TX_buffer__DOT__read_ptr),9);
        bufp->chgSData(oldp+67,((0x1ffU & ((IData)(1U) 
                                           + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__TX_buffer__DOT__write_ptr)))),9);
        bufp->chgSData(oldp+68,((0x1ffU & ((IData)(1U) 
                                           + (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__TX_buffer__DOT__read_ptr)))),9);
        bufp->chgBit(oldp+69,((1U & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__event_register))));
        bufp->chgBit(oldp+70,((1U & ((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__genblk1__BRA__0__KET____DOT__event_detector__DOT__previous_signal)) 
                                     & (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__event_register)))));
        bufp->chgBit(oldp+71,(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__genblk1__BRA__0__KET____DOT__event_detector__DOT__previous_signal));
        bufp->chgBit(oldp+72,((1U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__event_register) 
                                     >> 1U))));
        bufp->chgBit(oldp+73,((1U & ((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__genblk1__BRA__1__KET____DOT__event_detector__DOT__previous_signal)) 
                                     & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__event_register) 
                                        >> 1U)))));
        bufp->chgBit(oldp+74,(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__genblk1__BRA__1__KET____DOT__event_detector__DOT__previous_signal));
        bufp->chgBit(oldp+75,((1U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__event_register) 
                                     >> 2U))));
        bufp->chgBit(oldp+76,((1U & ((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__genblk1__BRA__2__KET____DOT__event_detector__DOT__previous_signal)) 
                                     & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__event_register) 
                                        >> 2U)))));
        bufp->chgBit(oldp+77,(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__genblk1__BRA__2__KET____DOT__event_detector__DOT__previous_signal));
        bufp->chgBit(oldp+78,((1U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__event_register) 
                                     >> 3U))));
        bufp->chgBit(oldp+79,((1U & ((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__genblk1__BRA__3__KET____DOT__event_detector__DOT__previous_signal)) 
                                     & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__event_register) 
                                        >> 3U)))));
        bufp->chgBit(oldp+80,(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__genblk1__BRA__3__KET____DOT__event_detector__DOT__previous_signal));
        bufp->chgBit(oldp+81,((1U & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__event_register) 
                                     >> 4U))));
        bufp->chgBit(oldp+82,(((~ (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__genblk1__BRA__4__KET____DOT__event_detector__DOT__previous_signal)) 
                               & ((IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__event_register) 
                                  >> 4U))));
        bufp->chgBit(oldp+83,(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__genblk1__BRA__4__KET____DOT__event_detector__DOT__previous_signal));
        bufp->chgCData(oldp+84,(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__rx_line_sync__DOT__sync_signal),2);
        bufp->chgCData(oldp+85,(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__sampling_counter),4);
        bufp->chgBit(oldp+86,(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__counter_enable));
        bufp->chgCData(oldp+87,(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__tx_data),8);
        bufp->chgBit(oldp+88,(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__shift_data));
        bufp->chgBit(oldp+89,(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__load_data));
        bufp->chgBit(oldp+90,(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__parity_bit));
        bufp->chgBit(oldp+91,(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__update_parity));
        bufp->chgCData(oldp+92,(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__bit_count),3);
        bufp->chgCData(oldp+93,(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__stop_bit_CRT),3);
        bufp->chgCData(oldp+94,(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__stop_bit_NXT),3);
        bufp->chgCData(oldp+95,(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__state_CRT),3);
        bufp->chgBit(oldp+96,((0xfU == (IData)(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__sampling_counter))));
    }
    bufp->chgBit(oldp+97,(vlSelfRef.clk));
    bufp->chgBit(oldp+98,(vlSelfRef.rst_n));
    bufp->chgBit(oldp+99,(vlSelfRef.write_i));
    bufp->chgIData(oldp+100,(vlSelfRef.write_address_i),32);
    bufp->chgIData(oldp+101,(vlSelfRef.write_data_i),32);
    bufp->chgCData(oldp+102,(vlSelfRef.write_strobe_i),4);
    bufp->chgBit(oldp+103,(vlSelfRef.write_done_o));
    bufp->chgBit(oldp+104,(vlSelfRef.write_error_o));
    bufp->chgBit(oldp+105,(vlSelfRef.read_i));
    bufp->chgIData(oldp+106,(vlSelfRef.read_address_i),32);
    bufp->chgIData(oldp+107,(vlSelfRef.read_data_o),32);
    bufp->chgBit(oldp+108,(vlSelfRef.read_done_o));
    bufp->chgBit(oldp+109,(vlSelfRef.read_error_o));
    bufp->chgBit(oldp+110,(vlSelfRef.interrupt_o));
    bufp->chgCData(oldp+111,((3U & vlSelfRef.write_address_i)),2);
    bufp->chgCData(oldp+112,((3U & vlSelfRef.read_address_i)),2);
    bufp->chgCData(oldp+113,((0xffU & vlSelfRef.write_data_i)),8);
    bufp->chgCData(oldp+114,((0xffU & (vlSelfRef.write_data_i 
                                       >> 8U))),8);
    bufp->chgCData(oldp+115,((0xffU & (vlSelfRef.write_data_i 
                                       >> 0x10U))),8);
    bufp->chgCData(oldp+116,((vlSelfRef.write_data_i 
                              >> 0x18U)),8);
    bufp->chgCData(oldp+117,(((2U & vlSelfRef.write_address_i)
                               ? 0U : (3U & ((IData)(1U) 
                                             << (1U 
                                                 & vlSelfRef.write_address_i))))),2);
    bufp->chgBit(oldp+118,((2U == (3U & vlSelfRef.write_address_i))));
    bufp->chgBit(oldp+119,(((2U == (3U & vlSelfRef.read_address_i)) 
                            & (IData)(vlSelfRef.read_i))));
    bufp->chgBit(oldp+120,(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT____Vcellinp__RX_buffer__read_i));
    bufp->chgBit(oldp+121,(vlSelfRef.tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT____Vcellinp__TX_buffer__write_i));
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
}
