/*
 * How this test works:
 * Start in M-mode, enter U-mode with MRET, and try privileged operations.
 * The trap handler checks each rejection and uses ECALL to return to M-mode.
 * A second U-mode pass verifies that mcounteren grants cycle-counter access.
 */
#include <stdint.h>

extern "C" void exception_trap_entry();
extern "C" void exception_enter_user(void (*)());
extern "C" void exception_load_access();
extern "C" void exception_load_access_site();
extern "C" void exception_load_access_resume();
extern "C" void exception_store_access();
extern "C" void exception_store_access_site();
extern "C" void exception_store_access_resume();
extern "C" void exception_ecall_u();
extern "C" void exception_ecall_u_site();
extern "C" void exception_ecall_u_resume();
extern "C" void priv_machine_csr();
extern "C" void priv_machine_csr_site();
extern "C" void priv_machine_csr_resume();
extern "C" void priv_wfi();
extern "C" void priv_wfi_site();
extern "C" void priv_wfi_resume();
extern "C" void priv_mret();
extern "C" void priv_mret_site();
extern "C" void priv_mret_resume();
extern "C" void priv_cycle_blocked();
extern "C" void priv_cycle_blocked_site();
extern "C" void priv_cycle_blocked_resume();
extern "C" uint32_t priv_read_cycle();

extern "C" {
volatile uint32_t exception_resume_in_machine = 0xffffffff;
volatile uint32_t expectedCause = 0xffffffff;
volatile uint32_t expectedEpc = 0xffffffff;
volatile uint32_t expectedMpp = 0xffffffff;
volatile uint32_t recoveryPc = 0xffffffff;
volatile uint32_t trapCount = 0x100;
volatile uint32_t failures = 0x80000000;
}

namespace {

constexpr uint32_t TRAP_COUNT_INITIAL = 0x100;
constexpr uint32_t FAILURES_INITIAL = 0x80000000;

uint32_t address(void (*symbol)()) {
    return reinterpret_cast<uint32_t>(symbol);
}

__attribute__((noinline)) void expectUserTrap(
    uint32_t cause, void (*site)(), void (*resume)(), bool resumeInMachine,
    void (*trigger)()) {
    expectedCause = cause;
    expectedEpc = address(site);
    recoveryPc = address(resume);
    expectedMpp = 0;
    exception_resume_in_machine = resumeInMachine;

    const uint32_t before = trapCount;
    asm volatile ("" ::: "memory");

    trigger();

    asm volatile ("" ::: "memory");

    failures += trapCount != before + 1;
    exception_resume_in_machine = 0;
}

__attribute__((noinline)) void firstUserPhase() {
    volatile uint32_t userData = 0x13579bdf;
    failures += userData != 0x13579bdf;

    /* U-mode cannot access machine CSRs or the private boot/IO region. */
    expectUserTrap(2, priv_machine_csr_site, priv_machine_csr_resume, false,
                   priv_machine_csr);
    expectUserTrap(5, exception_load_access_site, exception_load_access_resume,
                   false, exception_load_access);
    expectUserTrap(7, exception_store_access_site, exception_store_access_resume,
                   false, exception_store_access);

    /* WFI is specified as illegal in U-mode by this implementation (TW is not
     * implemented). MRET is always illegal outside M-mode. */
    expectUserTrap(2, priv_wfi_site, priv_wfi_resume, false, priv_wfi);
    expectUserTrap(2, priv_mret_site, priv_mret_resume, false, priv_mret);

    /* User counters must trap while mcounteren.CY is clear after reset. */
    expectUserTrap(2, priv_cycle_blocked_site, priv_cycle_blocked_resume, false,
                   priv_cycle_blocked);

    /* ECALL is the architected U -> M transition used by this test. */
    expectUserTrap(8, exception_ecall_u_site, exception_ecall_u_resume, true,
                   exception_ecall_u);
}

__attribute__((noinline)) void counterEnabledUserPhase() {
    const uint32_t before = trapCount;
    const uint32_t cycle = priv_read_cycle();
    failures += trapCount != before;
    failures += cycle == 0;

    expectUserTrap(8, exception_ecall_u_site, exception_ecall_u_resume, true,
                   exception_ecall_u);
}

}

extern "C" int main() {
    asm volatile ("csrc mstatus, %0" :: "r"(1 << 3) : "memory");
    asm volatile ("csrw mtvec, %0"
                  :: "r"(address(exception_trap_entry)) : "memory");

    uint32_t misa;
    uint32_t mstatus;
    asm volatile ("csrr %0, misa" : "=r"(misa));
    asm volatile ("csrr %0, mstatus" : "=r"(mstatus));
    failures += (misa & (1 << ('U' - 'A'))) == 0;
    failures += ((mstatus >> 11) & 3) != 0;

    /* The first MRET uses reset's saved U privilege. */
    exception_enter_user(firstUserPhase);

    /* The ECALL handler deliberately continued in M-mode. A successful read
     * of mstatus proves that the upward transition happened. */
    asm volatile ("csrr %0, mstatus" : "=r"(mstatus));

    /* Grant only cycle access, return to U again, and verify the grant. */
    asm volatile ("csrw mcounteren, %0" :: "r"(1) : "memory");
    exception_enter_user(counterEnabledUserPhase);

    asm volatile ("csrr %0, mstatus" : "=r"(mstatus));

    return failures == FAILURES_INITIAL &&
           trapCount == TRAP_COUNT_INITIAL + 8 ? 0 : 1;
}
