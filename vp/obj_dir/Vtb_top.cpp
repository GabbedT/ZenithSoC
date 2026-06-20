// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Model implementation (design independent parts)

#include "Vtb_top__pch.h"
#include "verilated_fst_c.h"

//============================================================
// Constructors

Vtb_top::Vtb_top(VerilatedContext* _vcontextp__, const char* _vcname__)
    : VerilatedModel{*_vcontextp__}
    , vlSymsp{new Vtb_top__Syms(contextp(), _vcname__, this)}
    , clk{vlSymsp->TOP.clk}
    , rst_n{vlSymsp->TOP.rst_n}
    , write_i{vlSymsp->TOP.write_i}
    , write_strobe_i{vlSymsp->TOP.write_strobe_i}
    , write_done_o{vlSymsp->TOP.write_done_o}
    , write_error_o{vlSymsp->TOP.write_error_o}
    , read_i{vlSymsp->TOP.read_i}
    , read_done_o{vlSymsp->TOP.read_done_o}
    , read_error_o{vlSymsp->TOP.read_error_o}
    , interrupt_o{vlSymsp->TOP.interrupt_o}
    , write_address_i{vlSymsp->TOP.write_address_i}
    , write_data_i{vlSymsp->TOP.write_data_i}
    , read_address_i{vlSymsp->TOP.read_address_i}
    , read_data_o{vlSymsp->TOP.read_data_o}
    , rootp{&(vlSymsp->TOP)}
{
    // Register model with the context
    contextp()->addModel(this);
    contextp()->traceBaseModelCbAdd(
        [this](VerilatedTraceBaseC* tfp, int levels, int options) { traceBaseModel(tfp, levels, options); });
}

Vtb_top::Vtb_top(const char* _vcname__)
    : Vtb_top(Verilated::threadContextp(), _vcname__)
{
}

//============================================================
// Destructor

Vtb_top::~Vtb_top() {
    delete vlSymsp;
}

//============================================================
// Evaluation function

#ifdef VL_DEBUG
void Vtb_top___024root___eval_debug_assertions(Vtb_top___024root* vlSelf);
#endif  // VL_DEBUG
void Vtb_top___024root___eval_static(Vtb_top___024root* vlSelf);
void Vtb_top___024root___eval_initial(Vtb_top___024root* vlSelf);
void Vtb_top___024root___eval_settle(Vtb_top___024root* vlSelf);
void Vtb_top___024root___eval(Vtb_top___024root* vlSelf);

void Vtb_top::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate Vtb_top::eval_step\n"); );
#ifdef VL_DEBUG
    // Debug assertions
    Vtb_top___024root___eval_debug_assertions(&(vlSymsp->TOP));
#endif  // VL_DEBUG
    vlSymsp->__Vm_activity = true;
    vlSymsp->__Vm_deleter.deleteAll();
    if (VL_UNLIKELY(!vlSymsp->__Vm_didInit)) {
        vlSymsp->__Vm_didInit = true;
        VL_DEBUG_IF(VL_DBG_MSGF("+ Initial\n"););
        Vtb_top___024root___eval_static(&(vlSymsp->TOP));
        Vtb_top___024root___eval_initial(&(vlSymsp->TOP));
        Vtb_top___024root___eval_settle(&(vlSymsp->TOP));
    }
    VL_DEBUG_IF(VL_DBG_MSGF("+ Eval\n"););
    Vtb_top___024root___eval(&(vlSymsp->TOP));
    // Evaluate cleanup
    Verilated::endOfEval(vlSymsp->__Vm_evalMsgQp);
}

//============================================================
// Events and timing
bool Vtb_top::eventsPending() { return !vlSymsp->TOP.__VdlySched.empty(); }

uint64_t Vtb_top::nextTimeSlot() { return vlSymsp->TOP.__VdlySched.nextTimeSlot(); }

//============================================================
// Utilities

const char* Vtb_top::name() const {
    return vlSymsp->name();
}

//============================================================
// Invoke final blocks

void Vtb_top___024root___eval_final(Vtb_top___024root* vlSelf);

VL_ATTR_COLD void Vtb_top::final() {
    Vtb_top___024root___eval_final(&(vlSymsp->TOP));
}

//============================================================
// Implementations of abstract methods from VerilatedModel

const char* Vtb_top::hierName() const { return vlSymsp->name(); }
const char* Vtb_top::modelName() const { return "Vtb_top"; }
unsigned Vtb_top::threads() const { return 1; }
void Vtb_top::prepareClone() const { contextp()->prepareClone(); }
void Vtb_top::atClone() const {
    contextp()->threadPoolpOnClone();
}
std::unique_ptr<VerilatedTraceConfig> Vtb_top::traceConfig() const {
    return std::unique_ptr<VerilatedTraceConfig>{new VerilatedTraceConfig{false, false, false}};
};

//============================================================
// Trace configuration

void Vtb_top___024root__trace_decl_types(VerilatedFst* tracep);

void Vtb_top___024root__trace_init_top(Vtb_top___024root* vlSelf, VerilatedFst* tracep);

VL_ATTR_COLD static void trace_init(void* voidSelf, VerilatedFst* tracep, uint32_t code) {
    // Callback from tracep->open()
    Vtb_top___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vtb_top___024root*>(voidSelf);
    Vtb_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    if (!vlSymsp->_vm_contextp__->calcUnusedSigs()) {
        VL_FATAL_MT(__FILE__, __LINE__, __FILE__,
            "Turning on wave traces requires Verilated::traceEverOn(true) call before time 0.");
    }
    vlSymsp->__Vm_baseCode = code;
    tracep->pushPrefix(std::string{vlSymsp->name()}, VerilatedTracePrefixType::SCOPE_MODULE);
    Vtb_top___024root__trace_decl_types(tracep);
    Vtb_top___024root__trace_init_top(vlSelf, tracep);
    tracep->popPrefix();
}

VL_ATTR_COLD void Vtb_top___024root__trace_register(Vtb_top___024root* vlSelf, VerilatedFst* tracep);

VL_ATTR_COLD void Vtb_top::traceBaseModel(VerilatedTraceBaseC* tfp, int levels, int options) {
    (void)levels; (void)options;
    VerilatedFstC* const stfp = dynamic_cast<VerilatedFstC*>(tfp);
    if (VL_UNLIKELY(!stfp)) {
        vl_fatal(__FILE__, __LINE__, __FILE__,"'Vtb_top::trace()' called on non-VerilatedFstC object;"
            " use --trace-fst with VerilatedFst object, and --trace with VerilatedVcd object");
    }
    stfp->spTrace()->addModel(this);
    stfp->spTrace()->addInitCb(&trace_init, &(vlSymsp->TOP));
    Vtb_top___024root__trace_register(&(vlSymsp->TOP), stfp->spTrace());
}
