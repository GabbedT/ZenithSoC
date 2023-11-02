`ifndef AXI_SLAVE_SV
    `define AXI_SLAVE_SV

`include "../Utility/Interfaces/axi_write_interface.sv"
`include "../Utility/Interfaces/axi_read_interface.sv"


module axi_slave (
    /* Bus write channel AXI connectivity */
    axi_write_interface.slave write_channel, 

    /* Write request from master */
    input logic write_error_i,
    input logic write_done_i,

    output logic [31:0] write_address_o,
    output logic [31:0] write_data_o,
    output logic [3:0] write_strobe_o,
    output logic write_request_o,


    /* Bus read channel AXI connectivity */
    axi_read_interface.slave read_channel,

    /* Read request informations */
    input logic [31:0] read_data_i,
    input logic read_valid_i,
    input logic read_error_i,

    output logic [31:0] read_address_o,
    output logic read_request_o,


    /* Status */
    input logic stop_write_i,
    input logic stop_read_i
);

//====================================================================================
//      WRITE DATA LOGIC 
//====================================================================================

        always_ff @(posedge write_channel.ACLK `ifdef ASYNC or negedge write_channel.ARESETN `endif) begin : write_handshake
            if (!write_channel.ARESETN) begin 
                write_channel.AWREADY <= 1'b1;
                write_channel.WREADY <= 1'b1;
                write_channel.BVALID <= 1'b0;

                write_channel.BRESP <= OKAY;

                write_request_o <= 1'b0;
            end else begin 
                /* Ready if no external condition require a stall */
                write_channel.AWREADY <= !stop_write_i;
                write_channel.WREADY <= !stop_write_i;

                if (write_done_i) begin 
                    /* Response is valid if data stream ended */
                    write_channel.BVALID <= 1'b1;
                    write_channel.BRESP <= write_error_i ? SLVERR : OKAY;
                end else begin 
                    if (write_channel.BVALID & write_channel.BREADY) begin
                        /* Transaction end */
                        write_channel.BVALID <= 1'b0;
                        write_channel.BRESP <= OKAY;
                    end
                end
            end 
        end : write_handshake

    /* If the Master has presented valid data and the Slave is ready to accept a transaction */
    assign write_request_o = write_channel.WVALID & write_channel.WREADY;

    /* Transaction informations */
    assign write_data_o = write_channel.WDATA;
    assign write_strobe_o = write_channel.WSTRB;
    assign write_address_o = write_channel.AWADDR;


//====================================================================================
//      READ DATA LOGIC 
//====================================================================================

        always_ff @(posedge read_channel.ACLK) begin : read_transaction_info
            read_channel.RDATA <= read_data_i;
        end : read_transaction_info


        always_ff @(posedge read_channel.ACLK `ifdef ASYNC or negedge read_channel.ARESETN `endif) begin : read_handshake
            if (!read_channel.ARESETN) begin 
                read_channel.ARREADY <= 1'b1;
                read_channel.RVALID <= 1'b0;

                read_channel.RRESP <= OKAY;
            end else begin 
                /* Ready to accept address if no external condition require a stall */
                read_channel.ARREADY <= !stop_read_i;

                read_channel.RRESP <= read_error_i ? SLVERR : OKAY;
                
                if (read_valid_i) begin 
                    /* Data on bus is valid */
                    read_channel.RVALID <= 1'b1;
                end else begin 
                    if (read_channel.RVALID & read_channel.RREADY) begin
                        /* End transaction */
                        read_channel.RVALID <= 1'b0;
                    end
                end 
            end 
        end : read_handshake

    /* If the Master has presented a valid address and the Slave is ready to accept a transaction */
    assign read_request_o = read_channel.ARVALID & read_channel.ARREADY;

    /* Transaction informations */
    assign read_address_o = read_channel.ARADDR;

endmodule : axi_slave 

`endif 