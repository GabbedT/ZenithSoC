`ifndef ASYNCHRONOUS_BUFFER_SV
    `define ASYNCHRONOUS_BUFFER_SV 

module asynchronous_buffer #(
    /* Number of entries */
    parameter BUFFER_DEPTH = 1024,

    /* Entries width */
    parameter DATA_WIDTH = 32
) (
    /* Global signals */
    input logic write_clk_i,
    input logic write_rstn_i,
    input logic read_clk_i,
    input logic read_rstn_i,

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
    logic [PTR_SIZE:0] write_ptr, read_ptr;

        /* Write clocked port */
        always_ff @(posedge write_clk_i) begin
            if (write_i) begin
                buffer_memory[write_ptr[PTR_SIZE - 1:0]] <= write_data_i;
            end
        end 

        /* Read clocked port */
        always_ff @(posedge read_clk_i) begin
            if (read_i) begin
                read_data_o <= buffer_memory[read_ptr[PTR_SIZE - 1:0]];
            end
        end 


//====================================================================================
//      POINTERS LOGIC
//====================================================================================

        always_ff @(posedge write_clk_i or negedge write_rstn_i) begin : write_pointer_logic
            if (!write_rstn_i) begin 
                write_ptr <= '0;
            end else if (write_i & !full_o) begin 
                write_ptr <= write_ptr + 1;
            end 
        end : write_pointer_logic


        always_ff @(posedge read_clk_i or negedge read_rstn_i) begin : read_pointer_logic
            if (!read_rstn_i) begin 
                read_ptr <= '0;
            end else if (read_i & !empty_o) begin 
                read_ptr <= read_ptr + 1;
            end 
        end : read_pointer_logic


//====================================================================================
//      POINTERS SYNCRONIZATION LOGIC
//====================================================================================

    logic [PTR_SIZE:0] gray_write_ptr, gray_read_ptr;

    /* Binary to Gray Code conversion of the incremented pointers */
    assign gray_write_ptr = (write_ptr >> 1) ^ (write_ptr);
    assign gray_read_ptr = (read_ptr >> 1) ^ (read_ptr);


    logic [1:0][PTR_SIZE:0] g_write_ptr_sync;

        always_ff @(posedge read_clk_i or negedge read_rstn_i) begin : write_syncronizer
            if (!read_rstn_i) begin 
                g_write_ptr_sync <= '{default: '0};
            end else begin 
                g_write_ptr_sync <= {g_write_ptr_sync[0], gray_write_ptr};
            end 
        end : write_syncronizer 


    logic [1:0][PTR_SIZE:0] g_read_ptr_sync;

        always_ff @(posedge write_clk_i or negedge write_rstn_i) begin : read_syncronizer
            if (!write_rstn_i) begin 
                g_read_ptr_sync <= '{default: '0};
            end else begin 
                g_read_ptr_sync <= {g_read_ptr_sync[0], gray_read_ptr};
            end 
        end : read_syncronizer 


//====================================================================================
//      FIFO STATUS LOGIC
//====================================================================================

    /* Full is generated on the write side, check the wraparound bit and the MSB, if it's equal to the read one and the pointer wrapped around */
    // assign full_o = (gray_write_ptr[PTR_SIZE - 1:0] == g_read_ptr_sync[1][PTR_SIZE - 1:0]) & (gray_write_ptr[PTR_SIZE] != g_read_ptr_sync[1][PTR_SIZE]);
    assign full_o = gray_write_ptr == {~g_read_ptr_sync[1][PTR_SIZE:PTR_SIZE - 1], g_read_ptr_sync[1][PTR_SIZE - 2:0]};

    assign empty_o = gray_read_ptr == g_write_ptr_sync[1];

endmodule : asynchronous_buffer

`endif