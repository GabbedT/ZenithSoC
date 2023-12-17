`ifndef UART_SV
    `define UART_SV

`include "uart_registers.sv"
`include "uart_baud_generator.sv"
`include "uart_transmitter.sv"
`include "uart_receiver.sv"
 
`include "../../System/synchronizer.sv"

`include "../../Utility/Packages/uart_pkg.sv"

module uart #(
    /* Buffer depth */
    parameter RX_BUFFER_SIZE = 512,
    parameter TX_BUFFER_SIZE = 512
) (
    /* Global signals */
    input logic clk_i,
    input logic rst_n_i,
    output logic interrupt_o,

    /* UART interface */
    input logic uart_rx_i,
    output logic uart_tx_o,
    output logic uart_rts_o,
    input logic uart_cts_i,

    /* Write interface */
    input logic write_i,
    input logic [1:0] write_address_i,
    input logic [31:0] write_data_i,
    input logic [3:0] write_strobe_i,
    output logic write_error_o,
    output logic write_done_o,

    /* Read interface */
    input logic read_i,
    input logic [1:0] read_address_i,
    output logic [31:0] read_data_o,
    output logic read_error_o,
    output logic read_done_o
);

//====================================================================================
//      CONTROL REGISTERS
//====================================================================================

    /* TX / RX Nets */
    logic [7:0] rx_data, tx_data; logic uart_rts, transmit, tx_done, tx_idle, rx_done, rx_idle, parity_error, tx_empty;

    /* Configuration */
    uart_data_lenght_t data_lenght;
    uart_stop_bits_t stop_bits;
    uart_parity_mode_t parity_mode;
    logic parity_enable;
    logic tx_enable; 
    logic rx_enable; 
    logic [14:0] divider;
    logic flow_control;

    uart_registers #(RX_BUFFER_SIZE, TX_BUFFER_SIZE) registers (
        /* Global signals */
        .clk_i       ( clk_i       ),
        .rst_n_i     ( rst_n_i     ),
        .interrupt_o ( interrupt_o ),

        .rx_data_i  ( rx_data  ),
        .tx_data_o  ( tx_data  ),
        .tx_cts_i   ( transmit ),
        .rx_rts_o   ( uart_rts ),
        .tx_empty_o ( tx_empty ),

        .tx_done_i  ( tx_done      ),
        .rx_done_i  ( rx_done      ),
        .rx_error_i ( parity_error ),

        .data_lenght_o   ( data_lenght   ),
        .stop_bits_o     ( stop_bits     ),
        .parity_mode_o   ( parity_mode   ),
        .parity_enable_o ( parity_enable ), 
        .divider_o       ( divider       ),
        .tx_enable_o     ( tx_enable     ), 
        .rx_enable_o     ( rx_enable     ), 
        .flow_control_o  ( flow_control  ),

        .write_i         ( write_i                            ),
        .write_address_i ( uart_registers_t'(write_address_i) ),
        .write_data_i    ( write_data_i                       ),
        .write_strobe_i  ( write_strobe_i                     ),
        .write_error_o   ( write_error_o                      ),

        .read_i         ( read_i                            ),
        .read_address_i ( uart_registers_t'(read_address_i) ),
        .read_data_o    ( read_data_o                       ),
        .read_error_o   ( read_error_o                      )
    );

    assign uart_rts_o = flow_control & uart_rts;
    
    assign write_done_o = write_i;


    logic read_done_delay;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                read_done_delay <= 1'b0; 
            end else begin 
                read_done_delay <= (uart_registers_t'(read_address_i) == RX_BUFFER) & read_i;
            end 
        end 

    assign read_done_o = (uart_registers_t'(read_address_i) == RX_BUFFER) ? read_done_delay : read_i;
    

//====================================================================================
//      BAUD RATE GENERATOR
//====================================================================================

    logic sample;

    uart_baud_generator baud_rate_generator (
        .clk_i   ( clk_i  ),
        .rst_n_i ( rst_n_i ),

        .divider_i ( divider ),

        .sample_o ( sample )
    );


//====================================================================================
//      TRANSMITTER
//====================================================================================

    logic uart_tx; 

    assign transmit = (flow_control ? uart_cts_i : 1'b1) & tx_idle & !tx_empty;

    uart_transmitter transmitter (
        .clk_i    ( clk_i     ),
        .rst_n_i  ( rst_n_i   ),
        .enable_i ( tx_enable ),
        .sample_i ( sample    ),

        .uart_tx_o ( uart_tx ),

        .transmit_i ( transmit ),
        .tx_data_i  ( tx_data  ),
        .tx_done_o  ( tx_done  ),
        .tx_idle_o  ( tx_idle  ),

        .data_lenght_i   ( data_lenght   ),
        .stop_bits_i     ( stop_bits     ),
        .parity_mode_i   ( parity_mode   ),
        .parity_enable_i ( parity_enable )
    );

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                uart_tx_o <= 1'b1;
            end else begin 
                uart_tx_o <= uart_tx;
            end 
        end 


//====================================================================================
//      RECEIVER
//====================================================================================

    logic rx_sync;

    synchronizer #(2, '1) rx_line_sync (
        .clk_i   ( clk_i   ),
        .rst_n_i ( rst_n_i ),

        .signal_i ( uart_rx_i ),
        .sync_o   ( rx_sync   )
    );

    uart_receiver receiver (
        .clk_i    ( clk_i     ),
        .rst_n_i  ( rst_n_i   ),
        .enable_i ( rx_enable ),
        .sample_i ( sample    ),

        .uart_rx_i ( rx_sync ),

        .rx_data_o      ( rx_data      ),
        .rx_done_o      ( rx_done      ),
        .rx_idle_o      ( rx_idle      ),
        .parity_error_o ( parity_error ),

        .data_lenght_i   ( data_lenght   ),
        .stop_bits_i     ( stop_bits     ),
        .parity_mode_i   ( parity_mode   ),
        .parity_enable_i ( parity_enable )
    );


endmodule : uart

`endif 