#ifndef TRACE_UNIT_H
#define TRACE_UNIT_H

#include <inttypes.h>

#include "../mmio.h"

class TraceUnit {

public:

    /* Trace Control Register */
    struct traceControl_s {
        /* Enable interrupts */
        unsigned int enableIntEmpty : 1;
        unsigned int enableIntFull : 1;
        
        /* Clear timestamp counter */
        unsigned int clearTimestamp : 1;

        /* Enable PC trace trigger */
        unsigned int enableTrigger : 1;

        /* Enable timestamp to send as packet */
        unsigned int enableTimestampBranch : 1;
        unsigned int enableTimestampEvent : 1;

        /* Enable tracing PC and Events */
        unsigned int enableBranchTrace : 1;
        unsigned int enableEventTrace : 1;

        /* Padding */
        unsigned int padding : 24;
    };

    /* Trace Status Register */
    struct traceStatus_s {
        /* Packet buffer status */
        unsigned int bufferEmpty : 1;
        unsigned int bufferFull : 1;

        unsigned int padding : 30;
    };

    /* Event Enable Register */
    struct eventEnable_s {
        /* CSR event */
        unsigned int csr : 1;

        /* Jump event */
        unsigned int jump : 1;

        /* Branch event */
        unsigned int branch : 1;

        /* Sleep event */
        unsigned int sleep : 1;
        
        /* Interrupt return event */
        unsigned int handlerReturn : 1;

        /* System call events */
        unsigned int mSysCall : 1;
        unsigned int uSysCall : 1;

        /* Store-related events */
        unsigned int storeFault : 1;
        unsigned int storeMisaligned : 1;
        unsigned int storeOperation : 1;

        /* Load-related events */
        unsigned int loadFault : 1;
        unsigned int loadMisaligned : 1;
        unsigned int loadOperation : 1;

        /* Breakpoint event */
        unsigned int breakpoint : 1;

        /* Instruction-related events */
        unsigned int instrIllegal : 1;
        unsigned int instrFault : 1;
        unsigned int instrMisaligned : 1;

        /* Padding */
        unsigned int padding : 15;
    };


/*****************************************************************/
/*                         MEMORY MAP                            */
/*****************************************************************/

    /* Base memory address */
    uint32_t* const baseAddress;

    /* Trace status register */
    volatile traceStatus_s* const status;

    /* Control register */
    volatile traceControl_s* const control;

    volatile eventEnable_s* const eventEnable;

    /* PC Trigger */
    volatile uint32_t* const triggerPC;


/*****************************************************************/
/*                           BUILDER                             */
/*****************************************************************/

    /**
     * @brief Construct a new TraceUnit object, with the specified configuration.
     * 
     * @param config Configuration for the control register, all functions are disabled by default, the user can enable them as needed.
     * @param event Event types to be traced, all events are disabled by default, the user can enable them as needed.
     */
    TraceUnit(traceControl_s config, eventEnable_s event);

    /**
     * @brief Construct a new TraceUnit object, with all functions disabled.
     */
    TraceUnit();

/*****************************************************************/
/*                         CONFIGURATION                         */
/*****************************************************************/

    /**
     * @brief Set the control register
     * 
     * @param config Configuration for the control register
     * @return TraceUnit& Reference to the Trace Unit object to chain the function call.
     */
    TraceUnit& setControl(traceControl_s config);

    /**
     * @brief Set the event enable register
     * 
     * @param event Event types to be traced
     * @return TraceUnit& Reference to the Trace Unit object to chain the function call.
     */
    TraceUnit& setEventEnable(eventEnable_s event);

    /**
     * @brief Clear the timestamp counter, the timestamp will be reset to 0 and the next packet will have a timestamp of 0.
     * 
     * @return TraceUnit& Reference to the Trace Unit object to chain the function call.
     */
    TraceUnit& clearTimestamp();


/*****************************************************************/
/*                         BRANCH TRACING                        */
/*****************************************************************/

    /**
     * @brief Start tracing banch divergences, the Trace Unit will send a packet every time a 
     * branch instruction is executed, the packet will contain the PC of the branch instruction, 
     * the target address and a timestamp.
     * 
     * @return TraceUnit& Reference to the Trace Unit object to chain the function call.
     */
    TraceUnit& startBranchTracing();

    /**
     * @brief Stop branch tracing.
     * 
     * @return TraceUnit& Reference to the Trace Unit object to chain the function call.
     */
    TraceUnit& stopBranchTracing();

    /**
     * @brief Enable branch timestamp, the Trace Unit will include a timestamp in the packet every time a branch instruction is executed.
     * 
     * @param enable Enable or disable branch timestamp
     * @return TraceUnit& Reference to the Trace Unit object to chain the function call.
     */
    TraceUnit& enableBranchTimestamp(bool enable);


/*****************************************************************/
/*                         EVENT TRACING                         */
/*****************************************************************/
    
    /**
     * @brief Start event tracing, the Trace Unit will send a packet every time an event of the enabled 
     * types is triggered, the packet will contain the event type and a timestamp.
     * 
     * @return TraceUnit& Reference to the Trace Unit object to chain the function call.
     */
    TraceUnit& startEventTracing();

    /**
     * @brief Stop event tracing.
     * 
     * @return TraceUnit& Reference to the TraceUnit object to chain the function call.
     */
    TraceUnit& stopEventTracing();

    /**
     * @brief Enable event timestamp, the Trace Unit will include a timestamp in the packet every time an event is traced.
     * 
     * @param enable Enable or disable event timestamp
     * @return TraceUnit& Reference to the TraceUnit object to chain the function call.
     */
    TraceUnit& enableEventTimestamp(bool enable);
    

/*****************************************************************/
/*                         INTERRUPT                             */
/*****************************************************************/

    /**
     * @brief Enable interrupt, the Trace Unit will generate an interrupt when the buffer is full or empty.
     * 
     * @param bufferFull Enable or disable buffer full interrupt
     * @param bufferEmpty Enable or disable buffer empty interrupt
     * @return TraceUnit& Reference to the Trace Unit object to chain the function call.
     */
    TraceUnit& enableInterrupt(bool bufferFull, bool bufferEmpty);
    

/*****************************************************************/
/*                         TRIGGER PC                            */
/*****************************************************************/

    /**
     * @brief Enable trigger PC, the Trace Unit will start tracing when the PC matches the trigger address,
     * this works only if branch tracing is enabled.
     * 
     * @param enable Enable or disable trigger PC
     * @return TraceUnit& Reference to the Trace Unit object to chain the function call.
     */
    TraceUnit& enableTriggerPC(bool enable);

    /**
     * @brief Set the trigger PC address, the Trace Unit will start tracing when the PC matches this address.
     * 
     * @param address The trigger PC address
     * @return TraceUnit& Reference to the Trace Unit object to chain the function call.
     */
    TraceUnit& setTriggerPC(uint32_t address);


/*****************************************************************/
/*                           STATUS                              */
/*****************************************************************/

    /**
     * @brief Check if the buffer is full.
     * 
     * @return bool True if the buffer is full, false otherwise.
     */
    bool isBufferFull() const;

    /**
     * @brief Check if the buffer is empty.
     * 
     * @return bool True if the buffer is empty, false otherwise.
     */
    bool isBufferEmpty() const;

};

#endif