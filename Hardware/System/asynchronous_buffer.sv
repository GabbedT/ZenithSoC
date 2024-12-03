`ifndef ASYNCHRONOUS_BUFFER_SV
    `define ASYNCHRONOUS_BUFFER_SV 

`define _VIVADO_

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

    `ifndef _VIVADO_ /* IF VIVADO MACRO IS NOT DEFINED */

//====================================================================================
//      MEMORY LOGIC
//====================================================================================

    localparam PTR_SIZE = $clog2(BUFFER_DEPTH);

    logic [DATA_WIDTH - 1:0] buffer_memory [BUFFER_DEPTH - 1:0];

    /* Operation pointers, one more bit is required for wraparound condition */
    logic [PTR_SIZE:0] write_ptr, read_ptr;

        /* Write clocked port */
        always_ff @(posedge write_clk_i) begin
            if (write_i & !full_o) begin
                buffer_memory[write_ptr[PTR_SIZE - 1:0]] <= write_data_i;
            end
        end 

        /* Read clocked port */
        always_ff @(posedge read_clk_i) begin
            if (read_i & !empty_o) begin
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
    assign full_o = gray_write_ptr == {~g_read_ptr_sync[1][PTR_SIZE:PTR_SIZE - 1], g_read_ptr_sync[1][PTR_SIZE - 2:0]};

    assign empty_o = gray_read_ptr == g_write_ptr_sync[1];

    `else /* IF VIVADO MACRO IS DEFINED */

//====================================================================================
//      VIVADO FIFO
//====================================================================================

    xpm_fifo_async #(
        .CASCADE_HEIGHT      ( 0            ),
        .CDC_SYNC_STAGES     ( 2            ),
        .DOUT_RESET_VALUE    ( "0"          ),
        .ECC_MODE            ( "no_ecc"     ),
        .EN_SIM_ASSERT_ERR   ( "warning"    ),
        .FIFO_MEMORY_TYPE    ( "auto"       ),
        .FIFO_READ_LATENCY   ( 1            ),
        .FIFO_WRITE_DEPTH    ( BUFFER_DEPTH ),
        .FULL_RESET_VALUE    ( 0            ),
        .PROG_EMPTY_THRESH   ( 10           ),
        .PROG_FULL_THRESH    ( 10           ),
        .RD_DATA_COUNT_WIDTH ( 1            ),
        .READ_DATA_WIDTH     ( DATA_WIDTH   ),
        .READ_MODE           ( "std"        ),
        .RELATED_CLOCKS      ( 0            ),
        .SIM_ASSERT_CHK      ( 0            ),
        .USE_ADV_FEATURES    ( "0707"       ),
        .WAKEUP_TIME         ( 0            ),
        .WRITE_DATA_WIDTH    ( DATA_WIDTH   ),
        .WR_DATA_COUNT_WIDTH ( 1            )
    ) vivado_async_fifo (
        .almost_empty  (              ),
        .almost_full   (              ),
        .data_valid    (              ),
        .dbiterr       (              ),
        .dout          ( read_data_o  ),
        .empty         ( empty_o      ),
        .full          ( full_o       ),
        .overflow      (              ),
        .prog_empty    (              ),
        .prog_full     (              ),
        .rd_data_count (              ),
        .rd_rst_busy   (              ),
        .sbiterr       (              ),
        .underflow     (              ),
        .wr_ack        (              ),
        .wr_data_count (              ),
        .wr_rst_busy   (              ),
        .din           ( write_data_i ),
        .injectdbiterr (              ),
        .injectsbiterr (              ),
        .rd_clk        ( read_clk_i   ),
        .rd_en         ( read_i       ),
        .rst           ( write_rstn_i ),
        .sleep         (              ),
        .wr_clk        ( write_clk_i  ),
        .wr_en         ( write_i      )
    );

    `endif /* VIVADO */

endmodule : asynchronous_buffer

`endif