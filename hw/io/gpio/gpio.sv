`ifndef GPIO_SV
    `define GPIO_SV

module gpio (
    /* Global signals */
    input logic clk_i,
    input logic rst_n_i,

    /* SoC external pin */
    inout wire pin_io,

    /* Write interface */
    input logic write_i,
    input logic [2:0] write_address_i,
    input logic write_data_i,

    /* Read interface */
    input logic read_i,
    input logic [2:0] read_address_i,
    output logic read_data_o,

    /* AXI errors */
    output logic write_error_o,
    output logic read_error_o,

    output logic interrupt_o
);

//====================================================================================
//      REGISTERS
//====================================================================================

    logic [5:0] enable_write; 

    assign enable_write = write_i ? (1'b1 << write_address_i) : '0;

    assign write_error_o = write_i & (write_address_i > 5);
    assign read_error_o = read_i & (read_address_i > 5);


    /* Select the direction of the GPIO pin (input / output) */
    logic direction;

    /* Value of the pin */
    logic pin_value, sync_pin;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                pin_value <= 1'b0;
            end else begin
                if (direction) begin 
                    /* In input mode the pin can be set only by external events */
                    pin_value <= sync_pin;  
                end else begin
                    /* In output mode, the pin can be set by the CPU */
                    if (enable_write[0]) begin 
                        pin_value <= write_data_i;
                    end
                end 
            end 
        end


        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                /* Pin set on output */
                direction <= 1'b0;
            end else if (enable_write[1]) begin 
                direction <= write_data_i;
            end 
        end  


    /* Enable interrupt generation while in input mode */
    logic interrupt_enable;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                interrupt_enable <= 1'b0;
            end else if (enable_write[2]) begin 
                interrupt_enable <= write_data_i;
            end 
        end 



    typedef enum logic [1:0] {
        HIGH    = 2'b00,
        POSEDGE = 2'b01,
        NEGEDGE = 2'b10,
        BOTH    = 2'b11
    } triggerLevel_t;

    /* Choose if send an interrupting signal on different transitions */
    triggerLevel_t interrupt_level;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                interrupt_level <= HIGH;
            end else begin
                if (enable_write[3]) begin 
                    interrupt_level[0] <= write_data_i;
                end

                if (enable_write[4]) begin 
                    interrupt_level[1] <= write_data_i;
                end
            end 
        end 

    
//====================================================================================
//      PIN / DEBOUNCING LOGIC
//====================================================================================

    assign pin_io = direction ? 1'bZ : pin_value;

    /* Pin input synchronizer */
    synchronizer #(2, '0) pin_sync (
        .clk_i   ( clk_i   ),
        .rst_n_i ( rst_n_i ),

        .signal_i ( pin_io   ),
        .sync_o   ( sync_pin )
    );


    logic prev_pin_value, interrupt_pending;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                prev_pin_value <= 1'b0;
            end else  if (direction) begin 
                prev_pin_value <= sync_pin;
            end
        end



        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                interrupt_pending <= 1'b0;
            end else if (enable_write[5]) begin 
                interrupt_pending <= write_data_i;
            end else if (interrupt_enable & direction) begin 
                case (interrupt_level)
                    HIGH: begin
                        interrupt_pending <= sync_pin;
                    end

                    POSEDGE: begin
                        if (!prev_pin_value & sync_pin) begin
                            interrupt_pending <= 1'b1;
                        end
                    end

                    NEGEDGE: begin
                        if (prev_pin_value & !sync_pin) begin
                            interrupt_pending <= 1'b1;
                        end
                    end

                    BOTH: begin
                        if (prev_pin_value ^ sync_pin) begin
                            interrupt_pending <= 1'b1;
                        end
                    end
                endcase
            end else begin 
                interrupt_pending <= 1'b0;
            end
        end

    assign interrupt_o = interrupt_pending;


//====================================================================================
//      OUTPUT LOGIC
//====================================================================================

        always_comb begin
            case (read_address_i)
                'd0: read_data_o = pin_value;

                'd1: read_data_o = direction;

                'd2: read_data_o = interrupt_enable;

                'd3: read_data_o = interrupt_level[0];

                'd4: read_data_o = interrupt_level[1];

                'd5: read_data_o = interrupt_pending;

                default: read_data_o = '0;
            endcase 
        end

endmodule : gpio 

`endif 