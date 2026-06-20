`ifndef SYNCHRONOUS_BUFFER_SV
    `define SYNCHRONOUS_BUFFER_SV 

module synchronous_buffer #(
    /* Number of entries */
    parameter BUFFER_DEPTH = 1024,

    /* Entries width */
    parameter DATA_WIDTH = 32,

    /* Word is immediately available */
    parameter FIRST_WORD_FALL_TROUGH = 0
) (
    /* Global signals */
    input logic clk_i,
    input logic rst_n_i,

    /* Control signals */
    input logic write_i,
    input logic read_i,

    /* Status signals */
    output logic empty_o,
    output logic full_o,

    /* Data */
    input logic [DATA_WIDTH - 1:0] write_data_i,
    output logic [DATA_WIDTH - 1:0] read_data_o
);

//====================================================================================
//      MEMORY LOGIC
//====================================================================================

    localparam PTR_SIZE = $clog2(BUFFER_DEPTH);

    logic [DATA_WIDTH - 1:0] buffer_memory [BUFFER_DEPTH - 1:0];

    /* Operation pointers, one more bit is required for wraparound condition */
    logic [PTR_SIZE - 1:0] write_ptr, read_ptr;

        /* Write clocked port */
        always_ff @(posedge clk_i) begin
            if (write_i) begin
                buffer_memory[write_ptr] <= write_data_i;
            end
        end 

        if (!FIRST_WORD_FALL_TROUGH) begin
            /* Read clocked port */
            always_ff @(posedge clk_i) begin
                if (read_i) begin
                    read_data_o <= buffer_memory[read_ptr];
                end
            end 
        end else begin
            assign read_data_o = buffer_memory[read_ptr];
        end


//====================================================================================
//      POINTERS LOGIC
//====================================================================================

    logic [PTR_SIZE - 1:0] inc_write_ptr;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin : write_pointer_logic
            if (!rst_n_i) begin 
                write_ptr <= '0;
            end else if (write_i) begin 
                write_ptr <= inc_write_ptr;
            end 
        end : write_pointer_logic

    assign inc_write_ptr = write_ptr + 1;


    logic [PTR_SIZE - 1:0] inc_read_ptr;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin : read_pointer_logic
            if (!rst_n_i) begin 
                read_ptr <= '0;
            end else if (read_i) begin 
                read_ptr <= inc_read_ptr;
            end 
        end : read_pointer_logic

    assign inc_read_ptr = read_ptr + 1;


//====================================================================================
//      FIFO STATUS LOGIC
//====================================================================================

    /* FIFO access mode */
    localparam logic [1:0] PULL_OPERATION = 2'b01;
    localparam logic [1:0] PUSH_OPERATION = 2'b10;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin : status_register
            if (!rst_n_i) begin 
                full_o <= 1'b0;
                empty_o <= 1'b1;
            end else begin 
                case ({write_i, read_i})
                    PULL_OPERATION: begin
                        full_o <= 1'b0;
                        empty_o <= (write_ptr == inc_read_ptr);
                    end

                    PUSH_OPERATION: begin
                        full_o <= (read_ptr == inc_write_ptr);
                        empty_o <= 1'b0; 
                    end
                endcase 
            end
        end : status_register

endmodule : synchronous_buffer

`endif