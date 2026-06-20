`ifndef INTERRUPT_CONTROLLER_SV
    `define INTERRUPT_CONTROLLER_SV

module interrupt_controller #(
    parameter INTERRUPTS = 4
) (
    /* Global signals */
    input logic clk_i,
    input logic rst_n_i,

    /* Interrupts */
    input logic [INTERRUPTS - 1:0] interrupt_i,

    /* CPU interface */
    input logic acknowledge_i,
    output logic interrupt_o,
    output logic [$clog2(INTERRUPTS) - 1:0] vector_o
);

//====================================================================================
//      INTERRUPT LOGIC
//====================================================================================

    /* Save pending interrupts */
    logic [INTERRUPTS - 1:0] interrupt_pending, clear_interrupt;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                interrupt_pending <= '0;
            end else begin 
                /* Each interrupt should clear and set independently */
                for (int i = 0; i < INTERRUPTS; ++i) begin
                    if (clear_interrupt[i]) begin  
                        interrupt_pending[i] <= 1'b0;
                    end else begin
                        interrupt_pending[i] <= interrupt_pending[i] | interrupt_i[i];
                    end
                end 
            end 
        end 


    /* Extract the most priority interrupt bit */
    logic [INTERRUPTS - 1:0] priority_interrupt;

        always_comb begin
            priority_interrupt = '0;

            for (int i = 0; i < INTERRUPTS; ++i) begin
                if (interrupt_pending[i]) begin
                    priority_interrupt = '0;
                    
                    /* Set only the corresponding bit */
                    priority_interrupt[i] = 1'b1;
                end
            end
        end


    logic [INTERRUPTS - 1:0] interrupt_request;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                interrupt_request <= '0;
            end else begin 
                if (interrupt_request == '0) begin 
                    /* If an interrupt has already been requested, stop 
                     * the incoming other */
                    interrupt_request <= priority_interrupt;
                end else begin
                    if (acknowledge_i) begin
                        /* Clear request on acknowledge */
                        interrupt_request <= '0;
                    end 
                end
            end 
        end

    assign clear_interrupt = interrupt_request;


        /* Assign each interrupt to a vector */
        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                vector_o <= '0;
            end else begin 
                for (int i = 0; i < INTERRUPTS; ++i) begin
                    if (priority_interrupt[i]) begin
                        /* Generate interrupt vector based on interrupt index */
                        vector_o = i;
                    end
                end
            end 
        end 

    assign interrupt_o = interrupt_request != '0;

endmodule : interrupt_controller

`endif 