`ifndef VGA_PIXEL_SEQUENCER_SV
    `define VGA_PIXEL_SEQUENCER_SV

module vga_pixel_sequencer #(
    /* Width of a pixel */
    parameter PIXEL_WIDTH = 12,

    /* Depth of pixel buffer */
    parameter PIXEL_BUFFER_SIZE = 512
) (
    input logic clk_i,
    input logic rst_n_i,
    input logic display_i,

    /* Frame buffer */
    input logic [26:0] base_address_i,
    input logic [18:0] size_i,

    /* Line buffer status */
    input logic full_i,

    /* Line buffer write */
    output logic write_o,
    output pixel_t pixel_o,

    /* Interface with master */
    input logic ddr_ready_i,
    input logic ddr_valid_i,
    input logic [127:0] ddr_data_i,
    output logic [26:0] ddr_address_o,
    output logic ddr_read_o
);

//====================================================================================
//      DDR REQUEST
//====================================================================================

    logic [18:0] frame_buffer_offset; logic buffer_full;

        always_ff @(posedge clk_i) begin
            if (!rst_n_i || !display_i) begin
                frame_buffer_offset <= '0;
            end else if (ddr_read_o) begin
                if (frame_buffer_offset == ((size_i >> 4) - 1'b1)) begin
                    frame_buffer_offset <= '0;
                end else begin
                    frame_buffer_offset <= frame_buffer_offset + 1'b1;
                end
            end
        end

    assign ddr_read_o = ddr_ready_i & display_i & !buffer_full &
                        (size_i >= 19'd16);

    assign ddr_address_o = base_address_i + (frame_buffer_offset << 4);


//====================================================================================
//      DDR DATA FIFO
//====================================================================================

    logic read_bundle, buffer_empty; logic [127:0] pixel_bundle;

    synchronous_buffer #(
        .BUFFER_DEPTH           ( 8   ), 
        .DATA_WIDTH             ( 128 ), 
        .FIRST_WORD_FALL_TROUGH ( 1   ) 
    ) ddr_data_buffer (
        .clk_i   ( clk_i               ),
        .rst_n_i ( rst_n_i & display_i ),

        .write_i ( ddr_valid_i ),
        .read_i  ( read_bundle ),

        .empty_o ( buffer_empty ),
        .full_o  ( buffer_full  ),

        .write_data_i ( ddr_data_i   ),
        .read_data_o  ( pixel_bundle )
    );


//====================================================================================
//      GEARBOX
//====================================================================================

    localparam int RESERVOIR_WIDTH = 256;
    localparam int RESERVOIR_COUNT_WIDTH = $clog2(RESERVOIR_WIDTH + 1);
    localparam logic [RESERVOIR_COUNT_WIDTH - 1:0] RESERVOIR_LOAD_LIMIT = 128;


    logic [RESERVOIR_WIDTH - 1:0] reservoir, reservoir_next;
    logic [RESERVOIR_COUNT_WIDTH - 1:0] reservoir_size, reservoir_size_next;
    logic load_reservoir, extract_pixel;
    logic [7:0] reservoir_insert_index;


    assign extract_pixel = display_i & (reservoir_size >= PIXEL_WIDTH) & !full_i;

    assign load_reservoir = display_i & !buffer_empty & (reservoir_size <= RESERVOIR_LOAD_LIMIT);

    /* The FIFO uses first-word fall-through, so pixel_bundle is valid before
     * this read pulse advances the FIFO pointer. */
    assign read_bundle = load_reservoir;

    assign reservoir_insert_index = reservoir_size_next[7:0];

        /*
         * A new 128-bit word is accepted when, after an optional pixel
         * extraction, there is room for all 128 bits.  With a 256-bit reservoir
         * this permits the following useful cases:
         *
         *   0   bits: load the first word
         *   128 bits: append the next word
         *   124 bits: extract 12 bits and append at bit 112
         */
        always_comb begin
            reservoir_next = reservoir;
            reservoir_size_next = reservoir_size;

            /* Remove the pixel at the bottom of the bit stream first. */
            if (extract_pixel) begin
                reservoir_next = reservoir_next >> PIXEL_WIDTH;
                reservoir_size_next = reservoir_size_next - PIXEL_WIDTH;
            end

            /* Append after the optional extraction.  This handles all byte and
             * nibble misalignment cases without special-case logic. */
            if (load_reservoir) begin
                reservoir_next[reservoir_insert_index +: 128] = pixel_bundle;
                reservoir_size_next = reservoir_size_next + 128;
            end
        end


        always_ff @(posedge clk_i) begin
            if (!rst_n_i | !display_i) begin
                reservoir_size <= '0;
                reservoir <= '0;
            end else begin
                reservoir_size <= reservoir_size_next;
                reservoir <= reservoir_next;
            end
        end


//====================================================================================
//      OUTPUT
//====================================================================================

    assign pixel_o = pixel_t'(reservoir[11:0]);

    assign write_o = extract_pixel;


//====================================================================================
//      ASSERTIONS
//====================================================================================

`ifndef SYNTHESIS

    localparam logic [RESERVOIR_COUNT_WIDTH - 1:0] RESERVOIR_CAPACITY = 9'd256;

    /* Parameter and protocol assumptions. */
    initial begin
        assert (PIXEL_WIDTH > 0) else $error("PIXEL_WIDTH must be greater than zero");
        
        assert (PIXEL_WIDTH <= RESERVOIR_WIDTH) else $error("PIXEL_WIDTH must fit in the reservoir");

        assert (PIXEL_BUFFER_SIZE > 0) else $error("PIXEL_BUFFER_SIZE must be greater than zero");
    end

    /* The reservoir count describes valid bits only. */
    assert property (@(posedge clk_i)
        disable iff (!rst_n_i || !display_i)
        reservoir_size <= RESERVOIR_CAPACITY)
        else $error("VGA reservoir count exceeded its capacity");

    /* A pixel is emitted only when a complete pixel is available and the
     * downstream line buffer can accept it. */
    assert property (@(posedge clk_i)
        disable iff (!rst_n_i || !display_i)
        write_o |-> (reservoir_size >= PIXEL_WIDTH && !full_i))
        else $error("VGA sequencer emitted a pixel without space/data");

    /* The dynamic part-select must always remain inside the 256-bit reservoir. */
    assert property (@(posedge clk_i)
        disable iff (!rst_n_i || !display_i)
        load_reservoir |-> (reservoir_size_next <= RESERVOIR_CAPACITY))
        else $error("VGA reservoir append exceeded its capacity");

    assert property (@(posedge clk_i)
        disable iff (!rst_n_i || !display_i)
        load_reservoir |-> (reservoir_insert_index <= RESERVOIR_LOAD_LIMIT[7:0]))
        else $error("VGA dynamic reservoir part-select is out of range");

    /* The response FIFO has no ready input from this block. Therefore the
     * producer must not send data while the FIFO is full, unless this cycle
     * simultaneously consumes the FIFO head. */
    assert property (@(posedge clk_i)
        disable iff (!rst_n_i || !display_i)
        ddr_valid_i |-> (!buffer_full || read_bundle))
        else $error("DDR response FIFO overflow");

    /* 128-bit requests are 16-byte requests and must be aligned accordingly. */
    assert property (@(posedge clk_i)
        disable iff (!rst_n_i || !display_i)
        ddr_read_o |-> (ddr_ready_i && ddr_address_o[3:0] == 4'b0))
        else $error("Unaligned or unaccepted VGA DDR request");

    assert property (@(posedge clk_i)
        disable iff (!rst_n_i || !display_i)
        base_address_i[3:0] == 4'b0)
        else $error("VGA framebuffer base address is not 16-byte aligned");

    assert property (@(posedge clk_i)
        disable iff (!rst_n_i || !display_i)
        size_i[3:0] == 4'b0)
        else $error("VGA framebuffer size is not a multiple of 16 bytes");

    /* A non-empty, non-zero-sized framebuffer must not produce an address
     * beyond its final 128-bit word. */
    assert property (@(posedge clk_i)
        disable iff (!rst_n_i || !display_i)
        (ddr_read_o && size_i != 0) |->
        (frame_buffer_offset < (size_i >> 4)))
        else $error("VGA framebuffer offset is outside the requested range");

`endif


endmodule : vga_pixel_sequencer

`endif
