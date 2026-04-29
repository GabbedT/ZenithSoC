`ifndef TRACE_UNIT_SV
    `define TRACE_UNIT_SV

`include "../../CPU/ApogeoRV/Hardware/Include/Interfaces/trace_interface.sv"

module trace_unit (
    /* Global signals */
    input logic clk_i,
    input logic rst_n_i,

    /* CPU Signals */
    output logic interrupt_o,
    output logic halt_core_o,

    /* Write interface */
    input logic write_i,
    input logic [1:0] write_address_i,
    input logic [31:0] write_data_i,
    input logic [3:0] write_strobe_i,
    output logic write_error_o,
    output logic write_done_o,

    /* Read interface */
    input logic read_i,
    input logic [1:0] read_address_i,
    output logic [31:0] read_data_o,
    output logic read_error_o,
    output logic read_done_o

    /* Trace channel from CPU */
    trace_interface.slave trace_interface_i,

    /* Trace packets to UART */
    output logic [7:0] trace_chunk_o,
    output logic write_chunk_o
);


endmodule : trace_unit

`endif 