#ifndef TRACE_UNIT_CPP
#define TRACE_UNIT_CPP

#include "../lib/driver/TraceUnit.h"
#include "../lib/mmio.h"
#include "../lib/platform.h"

#include <inttypes.h>


/****************************************************************/
/*                         CONSTRUCTORS                         */
/****************************************************************/

TraceUnit::TraceUnit() : 
    baseAddress ( ( uint32_t *       )  ( TRACE_UNIT_BASE  ) ),
    status      ( ( traceStatus_s *  )  ( baseAddress      ) ),
    control     ( ( traceControl_s * )  ( baseAddress + 1  ) ),
    eventEnable ( ( eventEnable_s *  )  ( baseAddress + 2  ) ),
    triggerPC   ( ( uint32_t *       )  ( baseAddress + 3  ) ) {
    
    /* Disable all trace functionality */
    control->enableBranchTrace = false;
    control->enableTimestampBranch = false;
    control->enableEventTrace = false;
    control->enableTimestampEvent = false;

    /* Disable all interrupts */
    control->enableIntEmpty = false;
    control->enableIntFull = false;
    
    /* Disable trigger */
    control->enableTrigger = false;


    /* Set the event enable register */
    eventEnable->csr = false;
    eventEnable->jump = false;
    eventEnable->branch = false;
    eventEnable->sleep = false;
    eventEnable->handlerReturn = false;
    eventEnable->mSysCall = false;
    eventEnable->uSysCall = false;
    eventEnable->storeFault = false;
    eventEnable->storeMisaligned = false;
    eventEnable->storeOperation = false;
    eventEnable->loadFault = false;
    eventEnable->loadMisaligned = false;
    eventEnable->loadOperation = false;
    eventEnable->breakpoint = false;
    eventEnable->instrIllegal = false;
    eventEnable->instrFault = false;
    eventEnable->instrMisaligned = false;
};


TraceUnit::TraceUnit(traceControl_s config, eventEnable_s event) : 
    baseAddress ( ( uint32_t *       )  ( TRACE_UNIT_BASE  ) ),
    status      ( ( traceStatus_s *  )  ( baseAddress      ) ),
    control     ( ( traceControl_s * )  ( baseAddress + 1  ) ),
    eventEnable ( ( eventEnable_s *  )  ( baseAddress + 2  ) ),
    triggerPC   ( ( uint32_t *       )  ( baseAddress + 3  ) ) {

    control->enableBranchTrace = config.enableBranchTrace;
    control->enableTimestampBranch = config.enableTimestampBranch;
    control->enableEventTrace = config.enableEventTrace;
    control->enableTimestampEvent = config.enableTimestampEvent;

    control->enableIntEmpty = config.enableIntEmpty;
    control->enableIntFull = config.enableIntFull;
    
    control->enableTrigger = config.enableTrigger;


    eventEnable->csr = event.csr;
    eventEnable->jump = event.jump;
    eventEnable->branch = event.branch;
    eventEnable->sleep = event.sleep;
    eventEnable->handlerReturn = event.handlerReturn;
    eventEnable->mSysCall = event.mSysCall;
    eventEnable->uSysCall = event.uSysCall;
    eventEnable->storeFault = event.storeFault;
    eventEnable->storeMisaligned = event.storeMisaligned;
    eventEnable->storeOperation = event.storeOperation;
    eventEnable->loadFault = event.loadFault;
    eventEnable->loadMisaligned = event.loadMisaligned;
    eventEnable->loadOperation = event.loadOperation;
    eventEnable->breakpoint = event.breakpoint;
    eventEnable->instrIllegal = event.instrIllegal;
    eventEnable->instrFault = event.instrFault;
    eventEnable->instrMisaligned = event.instrMisaligned;
};


/*****************************************************************/
/*                         CONFIGURATION                         */
/*****************************************************************/

TraceUnit& TraceUnit::setControl(traceControl_s config) {
    control->enableBranchTrace = config.enableBranchTrace;
    control->enableTimestampBranch = config.enableTimestampBranch;
    control->enableEventTrace = config.enableEventTrace;
    control->enableTimestampEvent = config.enableTimestampEvent;

    control->enableIntEmpty = config.enableIntEmpty;
    control->enableIntFull = config.enableIntFull;
    
    control->enableTrigger = config.enableTrigger;

    control->clearTimestamp = config.clearTimestamp;

    return *this;
};


TraceUnit& TraceUnit::setEventEnable(eventEnable_s config) {
    eventEnable->csr = config.csr;
    eventEnable->jump = config.jump;
    eventEnable->branch = config.branch;
    eventEnable->sleep = config.sleep;
    eventEnable->handlerReturn = config.handlerReturn;
    eventEnable->mSysCall = config.mSysCall;
    eventEnable->uSysCall = config.uSysCall;
    eventEnable->storeFault = config.storeFault;
    eventEnable->storeMisaligned = config.storeMisaligned;
    eventEnable->storeOperation = config.storeOperation;
    eventEnable->loadFault = config.loadFault;
    eventEnable->loadMisaligned = config.loadMisaligned;
    eventEnable->loadOperation = config.loadOperation;
    eventEnable->breakpoint = config.breakpoint;
    eventEnable->instrIllegal = config.instrIllegal;
    eventEnable->instrFault = config.instrFault;
    eventEnable->instrMisaligned = config.instrMisaligned;

    return *this;
};


TraceUnit& TraceUnit::clearTimestamp() {
    control->clearTimestamp = true;

    return *this;
};


/*****************************************************************/
/*                         BRANCH TRACING                        */
/*****************************************************************/

TraceUnit& TraceUnit::startBranchTracing() {
    control->enableBranchTrace = true;

    return *this;
};


TraceUnit& TraceUnit::stopBranchTracing() {
    control->enableBranchTrace = false;

    return *this;
};


TraceUnit& TraceUnit::enableBranchTimestamp(bool enable) {
    control->enableTimestampBranch = enable;

    return *this;
};


/*****************************************************************/
/*                         EVENT TRACING                         */
/*****************************************************************/

TraceUnit& TraceUnit::startEventTracing() {
    control->enableEventTrace = true;

    return *this;
};


TraceUnit& TraceUnit::stopEventTracing() {
    control->enableEventTrace = false;

    return *this;
};


TraceUnit& TraceUnit::enableEventTimestamp(bool enable) {
    control->enableTimestampEvent = enable;

    return *this;
};


/*****************************************************************/
/*                         EVENT TRACING                         */
/*****************************************************************/

TraceUnit& TraceUnit::enableInterrupt(bool bufferFull, bool bufferEmpty) {
    control->enableIntFull = bufferFull;
    control->enableIntEmpty = bufferEmpty;

    return *this;
};


/*****************************************************************/
/*                          TRIGGER PC                           */
/*****************************************************************/

TraceUnit& TraceUnit::enableTriggerPC(bool enable) {
    control->enableTrigger = enable;

    return *this;
};

TraceUnit& TraceUnit::setTriggerPC(uint32_t address) {
    *triggerPC = address;

    return *this;
};


/*****************************************************************/
/*                            STATUS                             */
/*****************************************************************/

bool TraceUnit::isBufferFull() const {
    return status->bufferFull;
};

bool TraceUnit::isBufferEmpty() const {
    return status->bufferEmpty;
};

#endif /* TRACE_UNIT_CPP */
