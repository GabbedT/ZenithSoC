 `ifndef TRACE_UNIT_PKG_SV
    `define TRACE_UNIT_PKG_SV

package trace_unit_pkg;
    
    /* Register map */
    typedef enum logic [1:0] { TU_STATUS, TU_CONTROL, TU_EVENT_ENABLE, TU_PC_TRIGGER } trace_unit_registers_t;


    /* Status Register */
    typedef struct packed {
        /* Packet buffer status */
        logic buffer_full;
        logic buffer_empty;
    } trace_unit_status_t;


    /* Control Register */
    typedef struct packed {
        /* Enable tracing PC and Events */
        logic enable_event_tracing;
        logic enable_branch_tracing;

        /* Enable timestamp to send as packet */
        logic enable_timestamp_event;
        logic enable_timestamp_branch;

        /* Enable trace trigger */
        logic enable_trigger;

        /* Clear packet buffer */
        logic clear_buffer;

        /* Enable interrupts */
        logic enable_int_full;
        logic enable_int_empty;
    } trace_unit_control_t;


    /* Control Register */
    typedef struct packed {
        /* Instruction fault events */
        logic instr_misaligned;
        logic instr_access_fault;
        logic instr_illegal;

        /* Breakpoint debug event */
        logic breakpoint;

        /* Memory ops event */
        logic load_operation;
        logic load_misaligned;
        logic load_access_fault;
        logic store_operation;
        logic store_misaligned;
        logic store_access_fault;

        /* System call events */
        logic u_system_call;
        logic m_system_call;

        /* Core sleep WFI */
        logic sleep;

        /* Branching */
        logic branch;
        logic jump;

        /* CSR */
        logic csr;
    } trace_unit_event_enable_t;


    /* Event Packet (2/6 Bytes) */
    /* SYNC (1 BYTE) | TYPE EVENT + EVENT_NUMBER (1 BYTE) | TIMESTAMP (IF ENABLED) (3 BYTE) */

    /* Branch Packet (10/14 Bytes) */
    /* SYNC (1 BYTE) | TYPE BRANCH (1 BYTE) | START PC (4 BYTE) | END PC (4 BYTE) | TIMESTAMP (IF ENABLED) (3 BYTE) */

    /* Time overflow Packet (2 Bytes) */
    /* SYNC (1 BYTE) | TYPE OVERFLOW + OV_FLAG (1 BYTE) | */

endpackage : trace_unit_pkg

import trace_unit_pkg::*;

`endif