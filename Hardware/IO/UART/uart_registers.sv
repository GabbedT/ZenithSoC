`ifndef UART_REGISTERS_SV
    `define UART_REGISTERS_SV

`include "../../System/synchronous_buffer.sv"
`include "../../System/edge_detector.sv"
 
`include "../../Utility/Packages/uart_pkg.sv"

module uart_registers #(
    /* Buffer depth */
    parameter RX_BUFFER_SIZE = 512,
    parameter TX_BUFFER_SIZE = 512
) (
    /* Global signals */
    input logic clk_i,
    input logic rst_n_i,
    output logic interrupt_o,

    /* Transmission and reception */
    input logic [7:0] rx_data_i,
    output logic [7:0] tx_data_o,
    input logic tx_cts_i,
    output logic rx_rts_o,
    output logic tx_empty_o,

    /* FSMs signals */
    input logic tx_done_i,
    input logic rx_done_i,
    input logic rx_error_i,

    /* Configuration to UART */
    output uart_data_lenght_t data_lenght_o,
    output uart_stop_bits_t stop_bits_o,
    output uart_parity_mode_t parity_mode_o,
    output logic [14:0] divider_o,
    output logic parity_enable_o, 
    output logic tx_enable_o, 
    output logic rx_enable_o, 
    output logic flow_control_o,

    /* Write interface */
    input logic write_i,
    input uart_registers_t write_address_i,
    input logic [3:0][7:0] write_data_i,
    input logic [3:0] write_strobe_i,
    output logic write_error_o,

    /* Read interface */
    input logic read_i,
    input uart_registers_t read_address_i,
    output logic [31:0] read_data_o,
    output logic read_error_o
);

//====================================================================================
//      ADDRESS DECODING
//====================================================================================

    logic [1:0] write_enable; logic write_error; 
    
    /* Error checking */
    assign write_error = write_address_i == UART_RX_BUFFER;
    assign write_error_o = write_error & write_i;

    assign write_enable = write_address_i[1] ? '0 : (1 << write_address_i[0]);


    /* Error checking */
    assign read_error_o = (read_address_i == UART_TX_BUFFER) & read_i;


//====================================================================================
//      STATUS REGISTER
//====================================================================================  

    uart_status_t status_register;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                /* 115200 Bauds */
                status_register.clock_divider <= 'd53;

                /* Communication setup */
                status_register.flow_control <= 1'b0;
                status_register.data_bits <= DBIT8;
                status_register.stop_bits <= STOP1;
                status_register.parity_mode <= EVEN;
                status_register.parity_enable <= 1'b1;

                /* Operation disabled on reset */
                status_register.enable_TX <= 1'b0;
                status_register.enable_RX <= 1'b0;
                status_register.interrupt_enable <= '0;
            end else begin 
                if (write_enable[0] & write_i) begin 
                    if (write_strobe_i[0]) begin
                        status_register.interrupt_enable[3:0] <= write_data_i[0][7:4];
                    end

                    if (write_strobe_i[1]) begin
                        status_register.interrupt_enable[4] <= write_data_i[1][0];

                        status_register.enable_RX <= write_data_i[1][1];
                        status_register.enable_TX <= write_data_i[1][2];

                        status_register.parity_enable <= write_data_i[1][3];
                        status_register.parity_mode <= uart_parity_mode_t'(write_data_i[1][4]);

                        status_register.stop_bits <= uart_stop_bits_t'(write_data_i[1][5]);

                        status_register.data_bits <= uart_data_lenght_t'(write_data_i[1][7:6]);
                    end

                    if (write_strobe_i[2]) begin
                        status_register.flow_control <= write_data_i[2][0];

                        status_register.clock_divider[6:0] <= write_data_i[2][7:1];
                    end

                    if (write_strobe_i[3]) begin
                        status_register.clock_divider[14:7] <= write_data_i[3][7:0]; 
                    end
                end
            end 
        end 

    assign flow_control_o = status_register.flow_control;
    
    assign tx_enable_o = status_register.enable_TX;
    assign rx_enable_o = status_register.enable_RX;

    assign data_lenght_o = status_register.data_bits;
    assign stop_bits_o = status_register.stop_bits;
    assign parity_mode_o = status_register.parity_mode;
    assign parity_enable_o = status_register.parity_enable;

    assign divider_o = status_register.clock_divider;


//====================================================================================
//      TX BUFFER REGISTER
//====================================================================================  

    logic tx_empty, tx_full;

    /* TX Buffer asyncronous FIFO instantiation */
    synchronous_buffer #(TX_BUFFER_SIZE, 8) TX_buffer (
        .clk_i   ( clk_i   ),
        .rst_n_i ( rst_n_i ),

        .write_i ( write_enable[1] & write_i & !tx_full ),
        .read_i  ( tx_cts_i & !tx_empty                 ),

        .empty_o ( tx_empty ),
        .full_o  ( tx_full  ),

        .write_data_i ( write_data_i[0] ),
        .read_data_o  ( tx_data_o       )
    );
    
    assign tx_empty_o = tx_empty;

    assign status_register.TX_empty = tx_empty;
    assign status_register.TX_full = tx_full;

//====================================================================================
//      RX BUFFER REGISTER
//====================================================================================  

    logic read_rx_buffer; logic [7:0] rx_buffer_read; 

    assign read_rx_buffer = (read_address_i == UART_RX_BUFFER) & read_i;


    logic rx_empty, rx_full;

    /* RX Buffer asyncronous FIFO instantiation */
    synchronous_buffer #(RX_BUFFER_SIZE, 8) RX_buffer (
        .clk_i   ( clk_i   ),
        .rst_n_i ( rst_n_i ),

        .write_i ( rx_done_i & !rx_full       ),
        .read_i  ( read_rx_buffer & !rx_empty ),

        .empty_o ( rx_empty ),
        .full_o  ( rx_full  ),

        .write_data_i ( rx_data_i      ),
        .read_data_o  ( rx_buffer_read )
    );

    assign rx_rts_o = !rx_full & status_register.flow_control;

    assign status_register.RX_empty = rx_empty;
    assign status_register.RX_full = rx_full;


//====================================================================================
//      EVENT REGISTER
//====================================================================================

    logic [4:0] event_register; logic [2:0] uart_event; 

    always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
        if (!rst_n_i) begin 
            event_register <= '0;
        end else begin 
            if ((write_address_i == UART_EVENT) & write_i) begin
                event_register <= write_data_i[0][4:0];
            end else begin
                if (rx_done_i & status_register.interrupt_enable[0]) begin
                    event_register[0] <= 1'b1;
                end

                if (tx_done_i & status_register.interrupt_enable[1]) begin
                    event_register[1] <= 1'b1;
                end

                if (rx_full & status_register.interrupt_enable[2]) begin
                    event_register[2] <= 1'b1;
                end

                if (tx_empty & status_register.interrupt_enable[3]) begin
                    event_register[3] <= 1'b1;
                end

                if (rx_error_i & status_register.interrupt_enable[4]) begin
                    event_register[4] <= 1'b1;
                end
            end
        end 
    end 


    /* Catch the positive edge of a bit set into the register, otherwise the interrupt bit will stay on */
    logic [4:0] event_edge;

    genvar i; generate
        for (i = 0; i < 5; ++i) begin
            edge_detector #(1, 0) event_detector (
                .clk_i   ( clk_i  ),
                .rst_n_i ( rst_n_i ),

                .signal_i ( event_register[i] ),
                .edge_o   ( event_edge[i]     )
            );
        end 
    endgenerate

    assign interrupt_o = (event_edge != '0);


//====================================================================================
//      DATA READ
//====================================================================================

        always_comb begin
            /* Default value */
            read_data_o = '0;

            case (read_address_i)
                UART_STATUS: read_data_o = status_register;

                UART_RX_BUFFER: read_data_o = rx_buffer_read;

                UART_EVENT: read_data_o = event_register;
            endcase 
        end

endmodule : uart_registers 

`endif 