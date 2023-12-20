`ifndef SPI_REGISTERS_SV
    `define SPI_REGISTERS_SV

`include "../../System/synchronous_buffer.sv"
`include "../../System/edge_detector.sv"
 
`include "../../Utility/Packages/spi_pkg.sv"

module spi_registers #(
    /* Buffer depth */
    parameter RX_BUFFER_SIZE = 512,
    parameter TX_BUFFER_SIZE = 512,

    /* Max number of slaves supported */
    parameter SLAVES = 1
) (
    /* Global signals */
    input logic clk_i,
    input logic rst_n_i,
    output logic interrupt_o,

    /* Transmission and reception */
    input logic idle_i,
    input logic done_i,
    input logic [7:0] rx_data_i,
    output logic [7:0] tx_data_o,
    output logic [SLAVES - 1:0] slave_select_o,

    /* Buffers */
    output logic data_ready_o,
    input logic tx_read_i,
    input logic rx_write_i,

    /* Configuration to SPI */
    output logic [15:0] divisor_o,
    output logic bit_order_o,
    output logic clock_pol_o,
    output logic clock_pha_o,

    /* Write interface */
    input logic write_i,
    input spi_registers_t write_address_i,
    input logic [3:0][7:0] write_data_i,
    input logic [3:0] write_strobe_i,
    output logic write_error_o,

    /* Read interface */
    input logic read_i,
    input spi_registers_t read_address_i,
    output logic [31:0] read_data_o,
    output logic read_error_o
);

//====================================================================================
//      ERROR CHECK
//====================================================================================

    /* Error checking */
    assign write_error_o = (write_address_i == SPI_RX_BUFFER) & write_i;
    assign read_error_o = (read_address_i == SPI_TX_BUFFER) & read_i;


//====================================================================================
//      STATUS REGISTER
//====================================================================================  

    spi_status_t status_register;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                status_register.clock_divisor <= '0;

                /* Communication setup */
                status_register.bit_order <= 1'b0;

                /* SPI Mode */
                status_register.cpol <= 1'b0;
                status_register.cpha <= 1'b0;

                /* Operation disabled on reset */
                status_register.interrupt_enable <= 1'b0;
            end else begin 
                if ((write_address_i == SPI_STATUS) & write_i) begin 
                    if (write_strobe_i[0]) begin
                        status_register.interrupt_enable <= write_data_i[0][5];
                        status_register.cpha <= write_data_i[0][6];
                        status_register.cpol <= write_data_i[0][7];
                    end

                    if (write_strobe_i[1]) begin
                        status_register.bit_order <= write_data_i[1][0];
                        status_register.clock_divisor[6:0] <= write_data_i[1][7:1];
                    end

                    if (write_strobe_i[2]) begin
                        status_register.clock_divisor[14:7] <= write_data_i[2][7:0];
                    end

                    if (write_strobe_i[3]) begin
                        status_register.clock_divisor[15] <= write_data_i[3][0]; 
                    end
                end
            end 
        end 


    assign bit_order_o = status_register.bit_order;

    assign divisor_o = status_register.clock_divisor;

    assign clock_pol_o = status_register.cpol;
    assign clock_pha_o = status_register.cpha;

    assign status_register.idle = idle_i;


//====================================================================================
//      TX BUFFER REGISTER
//====================================================================================  

    logic tx_empty, tx_full, write_tx_buffer;

    assign write_tx_buffer = (write_address_i == SPI_TX_BUFFER) & write_i;

    /* TX Buffer asyncronous FIFO instantiation */
    synchronous_buffer #(TX_BUFFER_SIZE, 8) TX_buffer (
        .clk_i   ( clk_i   ),
        .rst_n_i ( rst_n_i ),

        .write_i ( write_tx_buffer & !tx_full ),
        .read_i  ( !tx_empty & tx_read_i      ),

        .empty_o ( tx_empty ),
        .full_o  ( tx_full  ),

        .write_data_i ( write_data_i[0] ),
        .read_data_o  ( tx_data_o       )
    );
    
    assign data_ready_o = !tx_empty;

    assign status_register.TX_empty = tx_empty;
    assign status_register.TX_full = tx_full;


//====================================================================================
//      RX BUFFER REGISTER
//====================================================================================  

    logic read_rx_buffer; logic [7:0] rx_buffer_read; 

    assign read_rx_buffer = (read_address_i == SPI_RX_BUFFER) & read_i;


    logic rx_empty, rx_full;

    /* RX Buffer asyncronous FIFO instantiation */
    synchronous_buffer #(RX_BUFFER_SIZE, 8) RX_buffer (
        .clk_i   ( clk_i   ),
        .rst_n_i ( rst_n_i ),

        .write_i ( rx_write_i & !rx_full      ),
        .read_i  ( read_rx_buffer & !rx_empty ),

        .empty_o ( rx_empty ),
        .full_o  ( rx_full  ),

        .write_data_i ( rx_data_i      ),
        .read_data_o  ( rx_buffer_read )
    );

    assign status_register.RX_empty = rx_empty;
    assign status_register.RX_full = rx_full;


//====================================================================================
//      EVENT REGISTER
//====================================================================================

    logic  event_register; 

    always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
        if (!rst_n_i) begin 
            event_register <= 1'b0;
        end else begin 
            if ((write_address_i == SPI_EVENT) & write_i) begin
                event_register <= write_data_i[0][0];
            end else begin
                if (done_i & status_register.interrupt_enable) begin
                    event_register <= 1'b1;
                end
            end
        end 
    end 


    /* Catch the positive edge of a bit set into the register, otherwise the interrupt bit will stay on */
    logic event_edge;

    edge_detector #(1, 0) event_detector (
        .clk_i   ( clk_i  ),
        .rst_n_i ( rst_n_i ),

        .signal_i ( event_register ),
        .edge_o   ( event_edge     )
    );

    assign interrupt_o = (event_edge != '0);


//====================================================================================
//      SLAVE SELECT REGISTER
//====================================================================================

    logic [31:0] slave_select;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                slave_select <= '0;
            end else begin 
                if ((write_address_i == SPI_SLAVE_SELECT) & write_i) begin 
                    if (write_strobe_i[0]) begin
                        slave_select[7:0] <= write_data_i[0][7:0]; 
                    end

                    if (write_strobe_i[1]) begin
                        slave_select[15:8] <= write_data_i[1][7:0]; 
                    end

                    if (write_strobe_i[2]) begin
                        slave_select[23:16] <= write_data_i[2][7:0]; 
                    end

                    if (write_strobe_i[3]) begin
                        slave_select[31:24] <= write_data_i[3][7:0]; 
                    end
                end
            end 
        end 

    assign slave_select_o[SLAVES - 1:0] = ~slave_select[SLAVES - 1:0];


//====================================================================================
//      DATA READ
//====================================================================================

        always_comb begin
            /* Default value */
            read_data_o = '0;

            case (read_address_i)
                SPI_STATUS: read_data_o = status_register;

                SPI_RX_BUFFER: read_data_o = rx_buffer_read;

                SPI_EVENT: read_data_o = event_register;

                SPI_SLAVE_SELECT: read_data_o = slave_select;
            endcase 
        end

endmodule : spi_registers 

`endif 