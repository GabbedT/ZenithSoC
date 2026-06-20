`ifndef TRACE_UNIT_REGISTERS_SV
    `define TRACE_UNIT_REGISTERS_SV

`include "../../System/edge_detector.sv"
 
`include "../../Utility/Packages/trace_unit_pkg.sv"

module trace_unit_registers (
    /* Global signals */
    input logic clk_i,
    input logic rst_n_i,
    output logic interrupt_o,

    /* Configuration Tracing */
    output logic enable_event_tracing_o,
    output logic enable_branch_tracing_o,

    /* Configuration Timestamp */
    output logic enable_timestamp_event_o,
    output logic enable_timestamp_branch_o,

    /* Configuration Trigger PC */
    output logic enable_trigger_o,
    output logic [31:0] trigger_pc_o,

    /* From Packetizer buffer */
    input logic trace_buffer_empty_i,
    input logic trace_buffer_full_i,

    /* Timer control */
    output logic clear_timestamp_o,

    /* Enable interrupts */
    output trace_unit_event_t enable_event_o,

    /* Write interface */
    input logic write_i,
    input trace_unit_registers_t write_address_i,
    input logic [3:0][7:0] write_data_i,
    input logic [3:0] write_strobe_i,
    output logic write_error_o,

    /* Read interface */
    input logic read_i,
    input trace_unit_registers_t read_address_i,
    output logic [31:0] read_data_o,
    output logic read_error_o
);

//====================================================================================
//      ADDRESS DECODING
//====================================================================================

    logic [3:0] write_enable; 

    assign write_enable = 1 << write_address_i;


    assign read_error_o = 1'b0;
    assign write_error_o = write_enable[TU_STATUS] & write_i;


    logic [3:0][7:0] mask;

        always_comb begin
            for (int i = 0; i < 4; ++i) begin
                /* Mask certain bytes */
                mask[i] = write_strobe_i[i] ? '1 : '0;
            end
        end


//====================================================================================
//      STATUS REGISTER
//====================================================================================  

    trace_unit_status_t status_register;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                status_register.buffer_full <= 1'b0;
                status_register.buffer_empty <= 1'b1;
            end else begin 
                status_register.buffer_full <= trace_buffer_full_i;
                status_register.buffer_empty <= trace_buffer_empty_i;
            end 
        end 


//====================================================================================
//      CONTROL REGISTER
//====================================================================================  

    trace_unit_control_t control_register;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                control_register <= '0;
            end else begin 
                if (write_enable[TU_CONTROL_] & write_i) begin 
                    control_register <= (write_data_i & mask) | (control_register & ~mask);
                end

                control_register.clear_timestamp <= 1'b0;
            end 
        end

    assign enable_event_tracing_o = control_register.enable_event_tracing;
    assign enable_branch_tracing_o = control_register.enable_branch_tracing;

    assign enable_timestamp_event_o = control_register.enable_timestamp_event;
    assign enable_timestamp_branch_o = control_register.enable_timestamp_branch;

    assign enable_trigger_o = control_register.enable_trigger;

    assign clear_timestamp_o = control_register.clear_timestamp;


//====================================================================================
//      EVENT CONTROL REGISTER
//====================================================================================  

    trace_unit_event_t event_enable_register;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                event_enable_register <= '0;
            end else begin 
                if (write_enable[TU_EVENT_ENABLE_] & write_i) begin 
                    event_enable_register <= (write_data_i & mask) | (event_enable_register & ~mask);
                end
            end 
        end

    assign enable_event_o = event_enable_register;


//====================================================================================
//      PC TRIGGER REGISTER
//====================================================================================

    logic [31:0] pc_trigger_register;

    always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
        if (!rst_n_i) begin
            pc_trigger_register <= '0; 
        end else begin 
            if (write_enable[TU_PC_TRIGGER_] & write_i) begin 
                pc_trigger_register <= (write_data_i & mask) | (pc_trigger_register & ~mask);
            end
        end 
    end 

    assign trigger_pc_o = pc_trigger_register;


//====================================================================================
//      INTERRUPT LOGIC
//====================================================================================

    logic event_triggered;

    assign event_triggered = (control_register.enable_int_full & status_register.buffer_full) |
                             (control_register.enable_int_empty & status_register.buffer_empty);
    
    
    /* Catch the positive edge of a bit set into the register, otherwise the interrupt bit will stay on */
    edge_detector #(1, 0) event_detector (
        .clk_i   ( clk_i  ),
        .rst_n_i ( rst_n_i ),

        .signal_i ( event_triggered ),
        .edge_o   ( interrupt_o     )
    );


//====================================================================================
//      DATA READ
//====================================================================================

        always_comb begin
            /* Default value */
            read_data_o = '0;

            case (read_address_i)
                TU_STATUS: read_data_o = status_register;

                TU_CONTROL: read_data_o = control_register;

                TU_EVENT_ENABLE: read_data_o = event_enable_register;
                
                TU_PC_TRIGGER: read_data_o = pc_trigger_register;
            endcase 
        end

endmodule : trace_unit_registers 

`endif 