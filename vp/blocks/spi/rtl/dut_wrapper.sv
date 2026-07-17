`timescale 1ns/1ns

/*
 * A minimal full-duplex SPI slave used by the VP test.  The first byte after
 * CS asserts is the slave ID.  Each following byte is the byte it saw on
 * MOSI in the preceding byte time, so software observes { ID, TX[0], ... }.
 */
module spi_test_slave #(
    parameter logic [7:0] ID = 8'h00
) (
    input  logic selected_n_i,
    input  logic cs_n_i,
    input  logic sclk_i,
    input  logic mosi_i,
    input  logic cpol_i,
    input  logic cpha_i,
    input  logic lsb_first_i,
    output logic miso_o
);
    logic [7:0] reply_shift;
    logic [7:0] request_shift;
    logic [3:0] received_bits;
    logic transaction_active;
    logic first_shift;

    always @(negedge selected_n_i or negedge cs_n_i) begin
        if (!selected_n_i) begin
            reply_shift   = ID;
            request_shift = '0;
            received_bits = '0;
            first_shift   = cpha_i;
        end
    end

    always @(negedge cs_n_i or posedge cs_n_i) begin
        transaction_active <= !cs_n_i;
    end

    /* MOSI is sampled on the standard capture edge.  On the opposite edge
     * advance MISO; once a byte is complete, queue that byte as the reply. */
    always @(posedge sclk_i or negedge sclk_i) begin : serial_engine
        logic leading_edge;
        logic sample_edge;

        if (!cs_n_i & transaction_active) begin
            leading_edge = (sclk_i != cpol_i);
            sample_edge = cpha_i ? !leading_edge : leading_edge;

            if (sample_edge) begin
                if (lsb_first_i)
                    request_shift = {mosi_i, request_shift[7:1]};
                else
                    request_shift = {request_shift[6:0], mosi_i};
                received_bits = received_bits + 1'b1;
            end else begin
                if (first_shift) begin
                    first_shift = 1'b0;
                end else if (received_bits == 8) begin
                    reply_shift   = request_shift;
                    received_bits = '0;
                end else if (lsb_first_i) begin
                    reply_shift = {1'b0, reply_shift[7:1]};
                end else begin
                    reply_shift = {reply_shift[6:0], 1'b0};
                end
            end
        end
    end

    assign miso_o = lsb_first_i ? reply_shift[0] : reply_shift[7];
endmodule

module dut_wrapper (
    input  logic        clk,
    input  logic        rst_n,

    // Standard ZenithSoC peripheral write interface
    input  logic        write_i,
    input  logic [31:0] write_address_i,
    input  logic [31:0] write_data_i,
    input  logic [3:0]  write_strobe_i,
    output logic        write_done_o,
    output logic        write_error_o,

    // Standard ZenithSoC peripheral read interface
    input  logic        read_i,
    input  logic [31:0] read_address_i,
    output logic [31:0] read_data_o,
    output logic        read_done_o,
    output logic        read_error_o,

    // Interrupt
    output logic        interrupt_o
);

    localparam int SLAVES = 4;

    logic sclk;
    logic [SLAVES - 1:0] cs_n;
    logic mosi;
    logic [SLAVES - 1:0] slave_miso;
    logic miso;

    spi #(
        .RX_BUFFER_SIZE ( 512    ),
        .TX_BUFFER_SIZE ( 512    ),
        .SLAVES         ( SLAVES )
    ) u_spi (
        .clk_i       (clk),
        .rst_n_i     (rst_n),
        .interrupt_o (interrupt_o),
        .sclk_o      (sclk),
        .cs_n_o      (cs_n),
        .mosi_o      (mosi),
        .miso_i      (miso),
        .write_i         (write_i),
        .write_address_i (write_address_i[2:0]),
        .write_data_i    (write_data_i),
        .write_strobe_i  (write_strobe_i),
        .write_error_o   (write_error_o),
        .write_done_o    (write_done_o),
        .read_i         (read_i),
        .read_address_i (read_address_i[2:0]),
        .read_data_o    (read_data_o),
        .read_error_o   (read_error_o),
        .read_done_o    (read_done_o)
    );

    for (genvar i = 0; i < SLAVES; ++i) begin : slaves
        spi_test_slave #(.ID(i)) u_slave (
            .selected_n_i ( u_spi.chip_select[i] ),
            .cs_n_i      ( cs_n[i]         ),
            .sclk_i      ( sclk            ),
            .mosi_i      ( mosi            ),
            .cpol_i      ( u_spi.clock_pol ),
            .cpha_i      ( u_spi.clock_pha ),
            .lsb_first_i ( u_spi.bit_order ),
            .miso_o      ( slave_miso[i]   )
        );
    end


    always_comb begin
        miso = 1'b0;

        /* The selected slave drives RX before the first SCLK edge. */
        for (int i = 0; i < SLAVES; ++i) begin
            if (!u_spi.chip_select[i]) miso = slave_miso[i];
        end
    end

endmodule
