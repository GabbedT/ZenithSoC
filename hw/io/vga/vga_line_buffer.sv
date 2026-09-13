`ifndef VGA_LINE_BUFFER_SV
    `define VGA_LINE_BUFFER_SV

module vga_line_buffer #(
    /* How many pixels */
    parameter DEPTH = 512,

    /* Width of a pixel */
    parameter WIDTH = 12

) (
    input logic clk_i,
    input logic rst_n_i,
    input logic enable_video_i,
    input resolution_t resolution_i,

    /* Write interface */
    input logic write_i,
    input pixel_t pixel_i,

    /* Read interface */
    input logic read_i,
    output pixel_t pixel_o,

    /* Status */
    output logic full_o,
    output logic empty_o
);

//====================================================================================
//      MEMORY
//====================================================================================

    /* Memory */
    logic [WIDTH - 1:0] buffer [DEPTH - 1:0];

    localparam PTR_SIZE = (DEPTH > 1) ? $clog2(DEPTH) : 1;
    localparam logic [PTR_SIZE - 1:0] LAST_POINTER = PTR_SIZE'(DEPTH - 1);

    localparam SIZE_WIDTH = $clog2(DEPTH + 1);
    localparam LOW_RESOLUTION_SIZE = 320;

    /* Buffer pointers */
    logic [PTR_SIZE - 1:0] write_ptr, read_ptr, line_start_ptr;

    /* Buffer size */
    logic [SIZE_WIDTH - 1:0] size;


//====================================================================================
//      MEMORY ACCESS
//====================================================================================

    logic write_enable, read_enable, pop_enable;

    /* Write when not full and buffer is able to pop */
    assign write_enable = enable_video_i & write_i & (!full_o | pop_enable);

    /* Read is NOT popping, this is used for low resolution logic */
    assign read_enable = enable_video_i & read_i & !empty_o;

    /* In 320x240 mode, a line must be read two times since the same pixel will be 
     * written to positions: (X, Y), (X+1, Y) (X, Y+1) (X+1, Y+1) */
    assign pop_enable = read_enable & ((resolution_i != _320x240_) | low_resolution_second_pass);


        always_ff @(posedge clk_i) begin
            if (write_enable) begin
                buffer[write_ptr] <= pixel_i;
            end
        end

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin
                pixel_o <= '0;
            end else if (read_enable) begin
                pixel_o <= buffer[read_ptr];
            end
        end


//====================================================================================
//      POINTERS LOGIC
//====================================================================================

    /* Next pointer */
    logic [PTR_SIZE - 1:0] inc_write_ptr, inc_read_ptr;

    /* 320x240 logic */
    logic [$clog2(LOW_RESOLUTION_SIZE) - 1:0] low_resolution_read_count;
    logic low_resolution_second_pass;


    /* Wrap around or increment */
    assign inc_write_ptr = (write_ptr == LAST_POINTER) ? '0 : write_ptr + 1'b1;
    assign inc_read_ptr = (read_ptr == LAST_POINTER) ? '0 : read_ptr + 1'b1;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin
                write_ptr <= '0;
            end else if (write_enable) begin
                write_ptr <= inc_write_ptr;
            end
        end

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin
                read_ptr <= '0;
                
                line_start_ptr <= '0;

                low_resolution_read_count <= '0;
                low_resolution_second_pass <= 1'b0;
            end else if (read_enable) begin
                if (resolution_i == _320x240_) begin
                    if (low_resolution_read_count == (LOW_RESOLUTION_SIZE - 1)) begin
                        /* First pass: goes back to the line start 
                         * Second pass: advance FIFO pointer */
                        read_ptr <= low_resolution_second_pass ? inc_read_ptr : line_start_ptr;
                        line_start_ptr <= low_resolution_second_pass ? inc_read_ptr : line_start_ptr;
                        
                        low_resolution_read_count <= '0;
                        low_resolution_second_pass <= !low_resolution_second_pass;
                    end else begin
                        read_ptr <= inc_read_ptr;

                        low_resolution_read_count <= low_resolution_read_count + 1'b1;
                    end
                end else begin
                    /* Normal FIFO operation */
                    read_ptr <= inc_read_ptr;
                    line_start_ptr <= inc_read_ptr;

                    low_resolution_read_count <= '0;
                    low_resolution_second_pass <= 1'b0;
                end
            end else if (resolution_i != _320x240_) begin
                line_start_ptr <= read_ptr;

                low_resolution_read_count <= '0;
                low_resolution_second_pass <= 1'b0;
            end
        end


//====================================================================================
//      FIFO STATUS LOGIC
//====================================================================================

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin
                size <= '0;
            end else begin
                case ({write_enable, pop_enable})
                    2'b10: size <= size + 1'b1;
                    2'b01: size <= size - 1'b1;
                    default: size <= size;
                endcase
            end
        end

    assign full_o = size == DEPTH;
    assign empty_o = size == '0;


//====================================================================================
//      ASSERTIONS
//====================================================================================

`ifndef SYNTHESIS

    initial begin
        assert (DEPTH > 1)
            else $error("VGA line buffer depth must be greater than one");

        assert (DEPTH >= LOW_RESOLUTION_SIZE)
            else $error("VGA line buffer cannot contain a 320-pixel line");

    end

    assert property (@(posedge clk_i)
        disable iff (!rst_n_i)
        size <= DEPTH)
        else $error("VGA line buffer count exceeded its capacity");

    assert property (@(posedge clk_i)
        disable iff (!rst_n_i)
        write_enable |-> (!full_o || pop_enable))
        else $error("VGA line buffer write was issued while full");

    assert property (@(posedge clk_i)
        disable iff (!rst_n_i)
        read_enable |-> !empty_o)
        else $error("VGA line buffer read was issued while empty");

    assert property (@(posedge clk_i)
        disable iff (!rst_n_i)
        pop_enable |-> read_enable)
        else $error("VGA line buffer pop was issued without a read");

`endif

endmodule 

`endif
