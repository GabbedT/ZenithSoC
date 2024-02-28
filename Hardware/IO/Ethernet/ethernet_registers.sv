`ifndef ETHERNET_REGISTERS_SV
    `define ETHERNET_REGISTERS_SV

`include "../../System/synchronous_buffer.sv"
`include "../../System/edge_detector.sv"

`include "../../Utility/Packages/ethernet_pkg.sv"

module ethernet_registers #(
    /* Buffer sizes */
    parameter TX_BUFFER_SIZE = 512,
    parameter RX_BUFFER_SIZE = 512,

    /* Number of packets in a descriptor buffer */
    parameter TX_PACKETS = 32,
    parameter RX_PACKETS = 32
) (
    /* Global signals */
    input logic clk_i,
    input logic rst_n_i,
    output logic interrupt_o,
    output eth_speed_t eth_speed_o,

    /* Write register interface */
    input eth_registers_t write_address_i,
    input logic write_i,
    input logic [31:0] write_data_i,
    output logic write_done_o,
    output logic write_error_o,

    /* Read register interface */
    input eth_registers_t read_address_i,
    input logic read_i,
    output logic read_done_o,
    output logic [31:0] data_o,
    output logic read_error_o,

    /* TX interface */
    input logic read_payload_i,
    input logic read_descriptor_i,
    input logic tx_idle_i,
    output logic [5:0][7:0] dest_address_o,
    output logic [1:0][7:0] payload_length_o,
    output logic [7:0] payload_o,
    output logic data_ready_o,

    /* RX interface */
    input logic write_payload_i,
    input logic write_descriptor_i,
    input logic rx_idle_i,
    input logic rx_error_i,
    input logic [5:0][7:0] source_address_i,
    input logic [1:0][7:0] payload_length_i,
    input logic [7:0] payload_i,

    /* SMII interface */
    output logic [4:0] smii_address_o,
    output logic smii_write_o,
    output logic smii_read_o,
    output logic [15:0] smii_data_o,
    input logic smii_interrupt_i,
    input logic [15:0] smii_data_i,
    input logic smii_done_i
);

//====================================================================================
//      ERROR CHECK
//====================================================================================

    assign write_error_o = (write_address_i.select == MAC) & (write_address_i.registers >= ETH_RX_DESC_LOW) & (write_address_i.registers <= ETH_RX_BUFFER) & write_i;
    assign read_error_o = (read_address_i.select == MAC) & (read_address_i.registers >= ETH_TX_DESC_LOW) & (read_address_i.registers <= ETH_TX_BUFFER) & read_i;


//====================================================================================
//      SMII INTERFACE
//====================================================================================  

        always_comb begin
            /* Default value */
            smii_address_o = '0;
            
            case ({write_i, read_i})
                2'b01: smii_address_o = read_address_i[4:0];

                2'b10: smii_address_o = write_address_i[4:0];
            endcase 
        end

    assign smii_write_o = write_i & (write_address_i.select == PHY);
    assign smii_read_o = read_i & (read_address_i.select == PHY);

    assign smii_data_o = write_data_i[15:0];
    

//====================================================================================
//      STATUS REGISTER
//====================================================================================

    eth_status_t status_register;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                status_register.interrupt_enable <= 4'b0;
                status_register.speed <= MBPS10;
            end else begin 
                if ((write_address_i.select == MAC) & (write_address_i.registers == ETH_MAC_STATUS) & write_i) begin
                    status_register.interrupt_enable <= write_data_i[13:10];
                    status_register.speed <= eth_speed_t'(write_data_i[14]);
                end
            end 
        end 

    assign status_register.TX_idle = tx_idle_i;
    assign status_register.RX_idle = rx_idle_i;

    assign eth_speed_o = status_register.speed;


//====================================================================================
//      TX DESCRIPTOR
//====================================================================================

    logic tx_descl_empty, tx_descl_full;

    /* Control */
    logic tx_descl_write;

    assign tx_descl_write = (write_address_i.select == MAC) & (write_address_i.registers == ETH_TX_DESC_LOW) & write_i;

    synchronous_buffer #(TX_PACKETS, 32) TX_descriptor_low (
        .clk_i   ( clk_i   ),
        .rst_n_i ( rst_n_i ),

        .write_i ( !tx_descl_full & tx_descl_write     ),
        .read_i  ( !tx_descl_empty & read_descriptor_i ),

        .empty_o ( tx_descl_empty ),
        .full_o  ( tx_descl_full  ),

        .write_data_i ( write_data_i        ),
        .read_data_o  ( tx_descriptor[31:0] )
    );


    /* Read from buffers */
    eth_descriptor_t tx_descriptor;

    /* Status */
    logic tx_desch_empty, tx_desch_full;

    /* Control */
    logic tx_desch_write;

    assign tx_desch_write = (write_address_i.select == MAC) & (write_address_i.registers == ETH_TX_DESC_HIGH) & write_i;

    synchronous_buffer #(TX_PACKETS, 32) TX_descriptor_high (
        .clk_i   ( clk_i   ),
        .rst_n_i ( rst_n_i ),

        .write_i ( !tx_desch_full & tx_desch_write     ),
        .read_i  ( !tx_desch_empty & read_descriptor_i ),

        .empty_o ( tx_desch_empty ),
        .full_o  ( tx_desch_full  ),

        .write_data_i ( write_data_i         ),
        .read_data_o  ( tx_descriptor[63:32] )
    );


    assign dest_address_o = tx_descriptor.mac_address;

    assign payload_length_o = tx_descriptor.lenght;

    assign data_ready_o = !tx_descl_empty & !tx_desch_empty;


    assign status_register.TX_packet_empty = tx_desch_empty & tx_descl_empty;
    assign status_register.TX_packet_full = tx_desch_full & tx_descl_full;


//====================================================================================
//      TX PAYLOAD BUFFER
//====================================================================================

    /* Status */
    logic tx_buf_empty, tx_buf_full;

    /* Control */
    logic tx_buf_write;

    assign tx_buf_write = (write_address_i.select == MAC) & (write_address_i.registers == ETH_TX_BUFFER) & write_i;

    synchronous_buffer #(TX_BUFFER_SIZE, 8) TX_payload_buffer (
        .clk_i   ( clk_i   ),
        .rst_n_i ( rst_n_i ),

        .write_i ( !tx_buf_full & tx_buf_write    ),
        .read_i  ( !tx_buf_empty & read_payload_i ),

        .empty_o ( tx_buf_empty ),
        .full_o  ( tx_buf_full  ),

        .write_data_i ( write_data_i[7:0] ),
        .read_data_o  ( payload_o         )
    );

    assign status_register.TX_payload_full = tx_buf_full;
    assign status_register.TX_payload_empty = tx_buf_empty;


//====================================================================================
//      RX DESCRIPTOR
//====================================================================================

    /* Read from buffers */
    eth_descriptor_t rx_descriptor;

    logic rx_descl_empty, rx_descl_full;

    /* Control */
    logic rx_descl_read;

    assign rx_descl_read = (read_address_i.select == MAC) & (read_address_i.registers == ETH_RX_DESC_LOW) & read_i;

    synchronous_buffer #(RX_PACKETS, 32) RX_descriptor_low (
        .clk_i   ( clk_i   ),
        .rst_n_i ( rst_n_i ),

        .write_i ( !rx_descl_full & write_descriptor_i ),
        .read_i  ( !rx_descl_empty & rx_descl_read     ),

        .empty_o ( rx_descl_empty ),
        .full_o  ( rx_descl_full  ),

        .write_data_i ( data_write_rx_buf[31:0] ),
        .read_data_o  ( rx_descriptor[31:0]     )
    );


    /* Status */
    logic rx_desch_empty, rx_desch_full;

    /* Control */
    logic rx_desch_read;

    assign rx_desch_read = (read_address_i.select == MAC) & (read_address_i.registers == ETH_RX_DESC_HIGH) & read_i;


    logic [63:0] data_write_rx_buf; assign data_write_rx_buf = {source_address_i, payload_length_i};

    synchronous_buffer #(RX_PACKETS, 32) RX_descriptor_high (
        .clk_i   ( clk_i   ),
        .rst_n_i ( rst_n_i ),

        .write_i ( !rx_desch_full & write_descriptor_i ),
        .read_i  ( !rx_desch_empty & rx_desch_read     ),

        .empty_o ( rx_desch_empty ),
        .full_o  ( rx_desch_full  ),

        .write_data_i ( data_write_rx_buf[63:32] ),
        .read_data_o  ( rx_descriptor[63:32]     )
    );


    assign status_register.RX_packet_empty = rx_desch_empty & rx_descl_empty;
    assign status_register.RX_packet_full = rx_desch_full & rx_descl_full;


//====================================================================================
//      RX PAYLOAD BUFFER
//====================================================================================

    /* Read from buffers */
    logic [7:0] rx_payload;

    /* Status */
    logic rx_buf_empty, rx_buf_full;

    /* Control */
    logic rx_buf_read;

    assign rx_buf_read = (read_address_i.select == MAC) & (read_address_i.registers == ETH_RX_BUFFER) & read_i;

    synchronous_buffer #(RX_BUFFER_SIZE, 8) RX_payload_buffer (
        .clk_i   ( clk_i   ),
        .rst_n_i ( rst_n_i ),

        .write_i ( !rx_buf_full & write_payload_i ),
        .read_i  ( !rx_buf_empty & rx_buf_read    ),

        .empty_o ( rx_buf_empty ),
        .full_o  ( rx_buf_full  ),

        .write_data_i ( payload_i  ),
        .read_data_o  ( rx_payload )
    );

    assign status_register.RX_payload_full = rx_buf_full;
    assign status_register.RX_payload_empty = rx_buf_empty;


//====================================================================================
//      INTERRUPT REGISTER
//====================================================================================

    eth_interrupt_t interrupt_register; logic phy_interrupt_edge, rx_done_edge, tx_done_edge;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                interrupt_register <= '0;
            end else if ((write_address_i.select == MAC) & (write_address_i.registers == ETH_INTERRUPT) & write_i) begin 
                interrupt_register <= '0;
            end else begin 
                if (rx_error_i & status_register.interrupt_enable[3]) begin
                    interrupt_register.rx_error <= 1'b1;
                end

                if (rx_done_edge & status_register.interrupt_enable[2]) begin
                    interrupt_register.rx_done <= 1'b1;
                end

                if (tx_done_edge & status_register.interrupt_enable[1]) begin
                    interrupt_register.tx_done <= 1'b1;
                end

                if (phy_interrupt_edge & status_register.interrupt_enable[0]) begin
                    interrupt_register.phy_interrupt <= 1'b1;
                end
            end 
        end 


    edge_detector #(1, 0) phy_interrupt_detector (
        .clk_i   ( clk_i  ),
        .rst_n_i ( rst_n_i ),

        .signal_i ( smii_interrupt_i   ),
        .edge_o   ( phy_interrupt_edge )
    );

    edge_detector #(1, 0) rx_done_detector (
        .clk_i   ( clk_i  ),
        .rst_n_i ( rst_n_i ),

        .signal_i ( rx_idle_i    ),
        .edge_o   ( rx_done_edge )
    );

    edge_detector #(1, 0) tx_done_detector (
        .clk_i   ( clk_i  ),
        .rst_n_i ( rst_n_i ),

        .signal_i ( tx_idle_i    ),
        .edge_o   ( tx_done_edge )
    );

    edge_detector #(1, 0) interrupt_detector (
        .clk_i   ( clk_i  ),
        .rst_n_i ( rst_n_i ),

        .signal_i ( interrupt_register != '0 ),
        .edge_o   ( interrupt_o              )
    );


//====================================================================================
//      READ LOGIC
//====================================================================================

        always_comb begin
            /* Default value */
            data_o = '0;

            if (smii_done_i) begin
                data_o = {'0, smii_data_i};
            end else begin 
                case (read_address_i.registers)
                    ETH_MAC_STATUS: data_o = {'0, status_register};

                    ETH_RX_DESC_LOW: data_o = {'0, rx_descriptor[31:0]};

                    ETH_RX_DESC_HIGH: data_o = {'0, rx_descriptor[63:32]};

                    ETH_RX_BUFFER: data_o = {'0, rx_payload};

                    ETH_INTERRUPT: data_o = {'0, interrupt_register};
                endcase 
            end 
        end


//====================================================================================
//      DONE LOGIC
//====================================================================================

    logic write_wait, read_wait; 

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                write_wait <= 1'b0;
                read_wait <= 1'b0;
            end else if (smii_done_i) begin
                write_wait <= 1'b0;
                read_wait <= 1'b0;
            end else begin 
                if (write_i & (write_address_i.select == PHY)) begin
                    write_wait <= 1'b1;
                end 

                if (read_i & (read_address_i.select == PHY)) begin
                    read_wait <= 1'b1;
                end
            end 
        end 


    logic read_delay;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                read_delay <= 1'b0;
            end else begin 
                read_delay <= read_i & (read_address_i.registers >= ETH_RX_DESC_LOW) & (read_address_i.registers <= ETH_RX_BUFFER);
            end 
        end 


        always_comb begin
            if (write_wait) begin
                write_done_o = smii_done_i;
            end else begin
                write_done_o = (write_address_i.select == MAC) & write_i;
            end

            if (read_wait) begin
                read_done_o = smii_done_i;
            end else begin
                if ((read_address_i.registers >= ETH_RX_DESC_LOW) & (read_address_i.registers <= ETH_RX_BUFFER)) begin
                    read_done_o = read_delay & (read_address_i.select == MAC);
                end else begin 
                    read_done_o = read_i & (read_address_i.select == MAC);
                end 
            end
        end

endmodule : ethernet_registers

`endif 