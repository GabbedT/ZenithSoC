`ifndef SPI_SV
    `define SPI_SV

`include "spi_transmitter.sv"
`include "spi_receiver.sv"
`include "spi_registers.sv"

`include "../../System/synchronizer.sv"

`include "../../Utility/Packages/spi_pkg.sv"

module spi #(
    /* Buffer depth */
    parameter RX_BUFFER_SIZE = 16,
    parameter TX_BUFFER_SIZE = 16,

    /* Max number of slaves supported */
    parameter SLAVES = 1
) (
    /* Global signals */
    input logic clk_i,
    input logic rst_n_i,
    output logic interrupt_o,

    /* SPI interface */
    output logic sclk_o,
    output logic [SLAVES - 1:0] cs_n_o,
    output logic mosi_o,
    input logic miso_i, 

    /* Write interface */
    input logic write_i,
    input logic [2:0] write_address_i,
    input logic [3:0][7:0] write_data_i,
    input logic [3:0] write_strobe_i,
    output logic write_error_o,
    output logic write_done_o,

    /* Read interface */
    input logic read_i,
    input logic [2:0] read_address_i,
    output logic [31:0] read_data_o,
    output logic read_error_o,
    output logic read_done_o
);

//====================================================================================
//      CONTROL REGISTERS
//====================================================================================

    logic [7:0] rx_data, tx_data; logic data_ready, tx_read, rx_write; logic [15:0] divisor;
    logic bit_order, clock_pol, clock_pha; logic [SLAVES - 1:0] chip_select; logic trx_idle, trx_done;

    spi_registers #(RX_BUFFER_SIZE, TX_BUFFER_SIZE, SLAVES) registers (
        /* Global signals */
        .clk_i       ( clk_i       ),
        .rst_n_i     ( rst_n_i     ),
        .interrupt_o ( interrupt_o ),

        .idle_i    ( trx_idle ),
        .done_i    ( trx_done ),
        .rx_data_i ( rx_data  ),
        .tx_data_o ( tx_data  ),

        .slave_select_o ( chip_select ),

        .data_ready_o ( data_ready ),
        .tx_read_i    ( tx_read    ),
        .rx_write_i   ( rx_write   ),

        .divisor_o    ( divisor    ),
        .bit_order_o  ( bit_order  ),
        .clock_pol_o  ( clock_pol  ),
        .clock_pha_o  ( clock_pha  ), 

        .write_i         ( write_i                           ),
        .write_address_i ( spi_registers_t'(write_address_i) ),
        .write_data_i    ( write_data_i                      ),
        .write_strobe_i  ( write_strobe_i                    ),
        .write_error_o   ( write_error_o                     ),

        .read_i         ( read_i                           ),
        .read_address_i ( spi_registers_t'(read_address_i) ),
        .read_data_o    ( read_data_o                      ),
        .read_error_o   ( read_error_o                     )
    );


    assign write_done_o = write_i;

    logic read_done_delay; 

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                read_done_delay <= 1'b0; 
            end else begin 
                read_done_delay <= (spi_registers_t'(read_address_i) == SPI_RX_BUFFER) & read_i;
            end 
        end 

    assign read_done_o = (spi_registers_t'(read_address_i) == SPI_RX_BUFFER) ? read_done_delay : read_i;


//====================================================================================
//      TRANSACTION TRACKER
//====================================================================================

    logic clock_CRT, clock_NXT;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            clock_CRT <= clock_NXT;
        end 

    assign spi_clock_o = clock_CRT;

    
    logic [15:0] counter; logic [2:0] bit_counter; logic transition, count, bit_sent;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                counter <= '0;
                bit_counter <= '0;
            end else begin 
                if (count) begin
                    counter <= counter + 1'b1;

                    if (transition) begin
                        counter <= '0;
                    end
                end else begin
                    counter <= '0;
                end

                if (bit_sent) begin
                    bit_counter <= bit_counter + 1'b1;
                end
            end 
        end 

    assign transition = (counter == divisor);


//====================================================================================
//      FSM LOGIC
//====================================================================================

    typedef enum logic [1:0] { IDLE, PHASE_DELAY, CLOCK_LOW, CLOCK_HIGH } clock_states_t;

    clock_states_t state_CRT, state_NXT;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                state_CRT <= IDLE;
            end else if (!data_ready & rx_write) begin
                state_CRT <= IDLE;
            end else begin 
                state_CRT <= state_NXT;
            end 
        end 

    /* Increment clock divisor counter once the FSM is not idle */ 
    assign count = state_CRT != IDLE;

    /* Transaction is done when FSM goes idle */
    assign trx_done = !data_ready & rx_write;


    logic start_transaction, tx_read_delay;

    /* A new transaction starts if data is ready and the bits are not transmitted yet.
     * Once this signal is asserted the TX shift register is loaded with data from TX buffer */
    assign start_transaction = tx_read_delay;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                tx_read_delay <= 1'b0;
            end else begin 
                tx_read_delay <= tx_read;
            end 
        end 

    assign tx_read = ((bit_counter == '1) & data_ready & bit_sent) | (data_ready & (state_CRT == IDLE));
    assign rx_write = (bit_counter == '1) & bit_sent;


    logic sample_data, shift_data;

        always_comb begin
            /* Default Values */
            state_NXT = state_CRT;
            clock_NXT = clock_CRT;

            trx_idle = 1'b0;
            bit_sent = 1'b0;
            sample_data = 1'b0;
            shift_data = 1'b0;
            cs_n_o = chip_select;

            case (state_CRT)

                IDLE: begin
                    clock_NXT = clock_pol; 
                    cs_n_o = '1;

                    trx_idle = 1'b1;

                    if (data_ready) begin 
                        if (clock_pha) begin
                            state_NXT = PHASE_DELAY;

                            clock_NXT = !clock_pol;
                        end else begin
                            state_NXT = clock_pol ? CLOCK_HIGH : CLOCK_LOW;

                            sample_data = clock_pol;
                        end
                    end 
                end

                PHASE_DELAY: begin
                    if (transition) begin
                        state_NXT = clock_pol ? CLOCK_HIGH : CLOCK_LOW;

                        clock_NXT = clock_pol;

                        sample_data = 1'b1;
                    end
                end

                CLOCK_LOW: begin
                    if (transition) begin
                        state_NXT = CLOCK_HIGH;

                        clock_NXT = (!data_ready & rx_write) ? clock_pol : 1'b1;

                        /* Rising edge */
                        if (clock_pha == clock_pol) begin
                            sample_data = 1'b1;
                        end else begin
                            shift_data = 1'b1;
                            bit_sent = 1'b1;
                        end
                    end
                end

                CLOCK_HIGH: begin
                    if (transition) begin
                        state_NXT = CLOCK_LOW;

                        clock_NXT = (!data_ready & rx_write) ? clock_pol : 1'b0;

                        /* Falling edge */
                        if (clock_pha != clock_pol) begin
                            sample_data = 1'b1;
                        end else begin
                            shift_data = 1'b1;
                            bit_sent = 1'b1;
                        end
                    end
                end

            endcase 
        end

    assign sclk_o = clock_CRT;


//====================================================================================
//      RECEIVER
//====================================================================================

    logic miso_sync;

    synchronizer #(2, '1) miso_line_sync (
        .clk_i   ( clk_i   ),
        .rst_n_i ( rst_n_i ),

        .signal_i ( miso_i    ),
        .sync_o   ( miso_sync )
    );


    spi_receiver receiver (
        .clk_i   ( clk_i   ),

        .sample_i  ( sample_data ),
        .data_o    ( rx_data     ),

        .spi_miso_i ( miso_sync ),

        .bit_order_i ( bit_order_t'(bit_order) )
    );


//====================================================================================
//      TRANSMITTER
//====================================================================================

    spi_transmitter transmitter (
        .clk_i   ( clk_i   ),
        .rst_n_i ( rst_n_i ),

        .transmit_i ( start_transaction ),

        .shift_i    ( shift_data ),
        .data_i     ( tx_data    ),

        .spi_mosi_o ( mosi_o ),

        .bit_order_i ( bit_order_t'(bit_order) )
    );

endmodule : spi 

`endif 