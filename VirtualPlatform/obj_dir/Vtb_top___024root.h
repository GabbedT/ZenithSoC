// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vtb_top.h for the primary calling header

#ifndef VERILATED_VTB_TOP___024ROOT_H_
#define VERILATED_VTB_TOP___024ROOT_H_  // guard

#include "verilated.h"


class Vtb_top__Syms;

class alignas(VL_CACHE_LINE_BYTES) Vtb_top___024root final : public VerilatedModule {
  public:

    // DESIGN SPECIFIC STATE
    // Anonymous structures to workaround compiler member-count bugs
    struct {
        VL_IN8(clk,0,0);
        VL_IN8(rst_n,0,0);
        VL_IN8(write_i,0,0);
        VL_IN8(write_strobe_i,3,0);
        VL_OUT8(write_done_o,0,0);
        VL_OUT8(write_error_o,0,0);
        VL_IN8(read_i,0,0);
        VL_OUT8(read_done_o,0,0);
        VL_OUT8(read_error_o,0,0);
        VL_OUT8(interrupt_o,0,0);
        CData/*0:0*/ tb_top__DOT__u_dut__DOT__uart_tx;
        CData/*7:0*/ tb_top__DOT__u_dut__DOT__u_uart__DOT__rx_data;
        CData/*7:0*/ tb_top__DOT__u_dut__DOT__u_uart__DOT__tx_data;
        CData/*0:0*/ tb_top__DOT__u_dut__DOT__u_uart__DOT__transmit;
        CData/*0:0*/ tb_top__DOT__u_dut__DOT__u_uart__DOT__tx_done;
        CData/*0:0*/ tb_top__DOT__u_dut__DOT__u_uart__DOT__tx_idle;
        CData/*0:0*/ tb_top__DOT__u_dut__DOT__u_uart__DOT__rx_done;
        CData/*0:0*/ tb_top__DOT__u_dut__DOT__u_uart__DOT__rx_idle;
        CData/*0:0*/ tb_top__DOT__u_dut__DOT__u_uart__DOT__parity_error;
        CData/*0:0*/ tb_top__DOT__u_dut__DOT__u_uart__DOT__read_done_delay;
        CData/*0:0*/ tb_top__DOT__u_dut__DOT__u_uart__DOT__sample;
        CData/*0:0*/ tb_top__DOT__u_dut__DOT__u_uart__DOT__uart_tx;
        CData/*0:0*/ tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__tx_empty;
        CData/*0:0*/ tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__tx_full;
        CData/*0:0*/ tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT____Vcellinp__TX_buffer__read_i;
        CData/*0:0*/ tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT____Vcellinp__TX_buffer__write_i;
        CData/*7:0*/ tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__rx_buffer_read;
        CData/*0:0*/ tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__rx_empty;
        CData/*0:0*/ tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__rx_full;
        CData/*0:0*/ tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT____Vcellinp__RX_buffer__read_i;
        CData/*0:0*/ tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT____Vcellinp__RX_buffer__write_i;
        CData/*4:0*/ tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__event_register;
        CData/*2:0*/ tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__uart_event;
        CData/*0:0*/ tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__genblk1__BRA__0__KET____DOT__event_detector__DOT__previous_signal;
        CData/*0:0*/ tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__genblk1__BRA__1__KET____DOT__event_detector__DOT__previous_signal;
        CData/*0:0*/ tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__genblk1__BRA__2__KET____DOT__event_detector__DOT__previous_signal;
        CData/*0:0*/ tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__genblk1__BRA__3__KET____DOT__event_detector__DOT__previous_signal;
        CData/*0:0*/ tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__genblk1__BRA__4__KET____DOT__event_detector__DOT__previous_signal;
        CData/*3:0*/ tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__sampling_counter;
        CData/*0:0*/ tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__counter_enable;
        CData/*0:0*/ tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__counter_reset;
        CData/*7:0*/ tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__tx_data;
        CData/*0:0*/ tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__shift_data;
        CData/*0:0*/ tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__load_data;
        CData/*0:0*/ tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__parity_bit;
        CData/*0:0*/ tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__update_parity;
        CData/*2:0*/ tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__bit_count;
        CData/*2:0*/ tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__stop_bit_CRT;
        CData/*2:0*/ tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__stop_bit_NXT;
        CData/*0:0*/ tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__reset_bit_count;
        CData/*2:0*/ tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__state_CRT;
        CData/*2:0*/ tb_top__DOT__u_dut__DOT__u_uart__DOT__transmitter__DOT__state_NXT;
        CData/*1:0*/ tb_top__DOT__u_dut__DOT__u_uart__DOT__rx_line_sync__DOT__sync_signal;
        CData/*3:0*/ tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__sampling_counter;
        CData/*0:0*/ tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__counter_enable;
        CData/*0:0*/ tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__counter_reset;
        CData/*7:0*/ tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__rx_data;
        CData/*0:0*/ tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__shift_data;
        CData/*0:0*/ tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__start_receiving;
        CData/*0:0*/ tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__parity_bit;
        CData/*0:0*/ tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__parity_error;
        CData/*0:0*/ tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__update_parity;
        CData/*0:0*/ tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__check_parity;
        CData/*2:0*/ tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__bit_count;
    };
    struct {
        CData/*2:0*/ tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__stop_bit_CRT;
        CData/*2:0*/ tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__stop_bit_NXT;
        CData/*0:0*/ tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__reset_bit_count;
        CData/*2:0*/ tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__state_CRT;
        CData/*2:0*/ tb_top__DOT__u_dut__DOT__u_uart__DOT__receiver__DOT__state_NXT;
        CData/*0:0*/ __Vtrigprevexpr___TOP__tb_top__DOT__u_dut__DOT__u_uart__DOT__tx_idle__0;
        CData/*0:0*/ __VstlDidInit;
        CData/*0:0*/ __VstlFirstIteration;
        CData/*0:0*/ __VicoFirstIteration;
        CData/*0:0*/ __Vtrigprevexpr___TOP__tb_top__DOT__u_dut__DOT__u_uart__DOT__tx_idle__1;
        CData/*0:0*/ __Vtrigprevexpr___TOP__clk__0;
        CData/*0:0*/ __VactDidInit;
        CData/*0:0*/ __VactContinue;
        SData/*8:0*/ tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__TX_buffer__DOT__write_ptr;
        SData/*8:0*/ tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__TX_buffer__DOT__read_ptr;
        SData/*8:0*/ tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__TX_buffer__DOT__inc_write_ptr;
        SData/*8:0*/ tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__TX_buffer__DOT__inc_read_ptr;
        SData/*8:0*/ tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__RX_buffer__DOT__write_ptr;
        SData/*8:0*/ tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__RX_buffer__DOT__read_ptr;
        SData/*8:0*/ tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__RX_buffer__DOT__inc_write_ptr;
        SData/*8:0*/ tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__RX_buffer__DOT__inc_read_ptr;
        SData/*14:0*/ tb_top__DOT__u_dut__DOT__u_uart__DOT__baud_rate_generator__DOT__counter;
        VL_IN(write_address_i,31,0);
        VL_IN(write_data_i,31,0);
        VL_IN(read_address_i,31,0);
        VL_OUT(read_data_o,31,0);
        IData/*31:0*/ tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__status_register;
        IData/*31:0*/ __VactIterCount;
        VlUnpacked<CData/*7:0*/, 512> tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__TX_buffer__DOT__buffer_memory;
        VlUnpacked<CData/*7:0*/, 512> tb_top__DOT__u_dut__DOT__u_uart__DOT__registers__DOT__RX_buffer__DOT__buffer_memory;
        VlUnpacked<CData/*0:0*/, 4> __Vm_traceActivity;
    };
    VlTriggerVec<2> __VstlTriggered;
    VlTriggerVec<1> __VicoTriggered;
    VlTriggerVec<2> __VactTriggered;
    VlTriggerVec<2> __VnbaTriggered;

    // INTERNAL VARIABLES
    Vtb_top__Syms* const vlSymsp;

    // CONSTRUCTORS
    Vtb_top___024root(Vtb_top__Syms* symsp, const char* v__name);
    ~Vtb_top___024root();
    VL_UNCOPYABLE(Vtb_top___024root);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
};


#endif  // guard
