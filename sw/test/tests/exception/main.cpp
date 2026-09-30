/*
 * How this test works:
 * Enter U-mode for user-only faults, then return to M-mode for the rest.
 * Every trap checks mcause, mepc, previous privilege, and interrupt masking.
 * The test passes only when all eight reachable exceptions are correct.
 */
#include <stdint.h>

extern "C" void exception_trap_entry();
extern "C" void exception_enter_user(void (*)());
extern "C" void exception_illegal();
extern "C" void exception_illegal_site();
extern "C" void exception_illegal_resume();
extern "C" void exception_breakpoint();
extern "C" void exception_breakpoint_site();
extern "C" void exception_breakpoint_resume();
extern "C" void exception_load_misaligned();
extern "C" void exception_load_misaligned_site();
extern "C" void exception_load_misaligned_resume();
extern "C" void exception_store_misaligned();
extern "C" void exception_store_misaligned_site();
extern "C" void exception_store_misaligned_resume();
extern "C" void exception_load_access();
extern "C" void exception_load_access_site();
extern "C" void exception_load_access_resume();
extern "C" void exception_store_access();
extern "C" void exception_store_access_site();
extern "C" void exception_store_access_resume();
extern "C" void exception_ecall_u();
extern "C" void exception_ecall_u_site();
extern "C" void exception_ecall_u_resume();
extern "C" void exception_ecall_m();
extern "C" void exception_ecall_m_site();
extern "C" void exception_ecall_m_resume();

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

/* Non-zero initializers keep trap state out of BSS. This matters because the
 * current store buffer cannot merge a new write with a still-visible BSS-clear
 * entry for the same word. */
constexpr uint32_t TRAP_COUNT_INITIAL = 0x100;
constexpr uint32_t FAILURES_INITIAL = 0x80000000;

uint32_t address(void (*symbol)()) {
    return reinterpret_cast<uint32_t>(symbol);
}

__attribute__((noinline)) void expect(uint32_t cause, void (*site)(),
                                      void (*resume)(), uint32_t mpp,
                                      bool resumeInMachine,
                                      void (*trigger)()) {
    expectedCause = cause;
    expectedEpc = address(site);
    recoveryPc = address(resume);
    expectedMpp = mpp;
    exception_resume_in_machine = resumeInMachine;
    const uint32_t before = trapCount;
    asm volatile ("" ::: "memory");
    trigger();
    asm volatile ("" ::: "memory");
    failures += trapCount != before + 1;
    exception_resume_in_machine = 0;
}

__attribute__((noinline)) void userExceptions() {
    expect(5, exception_load_access_site, exception_load_access_resume, 0, false,
           exception_load_access);
    expect(7, exception_store_access_site, exception_store_access_resume, 0, false,
           exception_store_access);
    expect(8, exception_ecall_u_site, exception_ecall_u_resume, 0, true,
           exception_ecall_u);
}

}

extern "C" int main() {
    asm volatile ("csrc mstatus, %0" :: "r"(1 << 3) : "memory");
    asm volatile ("csrw mtvec, %0" :: "r"(address(exception_trap_entry)) : "memory");

    /* U-mode is needed to generate the core's access-fault causes. */
    exception_enter_user(userExceptions);

    expect(2, exception_illegal_site, exception_illegal_resume, 3, false,
           exception_illegal);
    expect(3, exception_breakpoint_site, exception_breakpoint_resume, 3, false,
           exception_breakpoint);
    expect(4, exception_load_misaligned_site, exception_load_misaligned_resume, 3,
           false, exception_load_misaligned);
    expect(6, exception_store_misaligned_site, exception_store_misaligned_resume,
           3, false, exception_store_misaligned);
    expect(11, exception_ecall_m_site, exception_ecall_m_resume, 3, false,
           exception_ecall_m);

    /* Software-reachable synchronous causes: 2,3,4,5,6,7,8,11. */
    return failures == FAILURES_INITIAL &&
           trapCount == TRAP_COUNT_INITIAL + 8 ? 0 : 1;
}
