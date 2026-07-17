#ifndef IO_INTERRUPTS_H
#define IO_INTERRUPTS_H

class UART;

bool testUARTInterrupts(UART& uart);
bool testUARTTXEmptyInterrupt(UART& uart);
bool testUARTTXDoneInterrupt(UART& uart);
bool testUARTRXDoneInterrupt(UART& uart);
bool testUARTFullInterrupt(UART& uart);

bool testTimerInterrupt(UART& uart);

bool testGPIOInterrupts(UART& uart);
bool testGPIOPosedgeInterrupt(UART& uart);
bool testGPIONegedgeInterrupt(UART& uart);
bool testGPIOBothEdgeInterrupt(UART& uart);
bool testGPIOHighLevelInterrupt(UART& uart);

bool testSPIInterrupt(UART& uart);

bool testAPUInterrupt(UART& uart);

bool testTraceInterrupt(UART& uart);

/* These tests need external protocol models and are intentionally not called by main */
bool testEthernetInterrupt(UART& uart);
bool testSDInterrupt(UART& uart);

#endif
