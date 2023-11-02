`ifndef AXI_MASTER_SV
    `define AXI_MASTER_SV

`include "../Utility/Interfaces/axi_write_interface.sv"
`include "../Utility/Interfaces/axi_read_interface.sv"


module axi_master (
    /* Bus write channel AXI connectivity */
    axi_write_interface.master write_channel, 

    /* Write request informations */
    input logic [31:0] write_address_i,
    input logic [31:0] write_data_i,
    input logic [3:0] write_strobe_i,
    input logic write_start_i,
    output logic write_done_o,
    output logic write_cts_o,


    /* Bus read channel AXI connectivity */
    axi_read_interface.master read_channel, 

    /* Read request informations */
    input logic [31:0] read_address_i,
    input logic read_start_i,
    output logic [31:0] read_data_o,
    output logic read_valid_o,
    output logic read_done_o,
    output logic read_cts_o,


    /* Status */
    output axi_response_t write_response_o,
    output axi_response_t read_response_o
);

//====================================================================================
//      WRITE DATA LOGIC 
//====================================================================================

    /* Flush all the ongoing transactions */
    logic write_flush; assign write_flush = (write_channel.BRESP == SLVERR);


        /* Immediately load the write transaction informations on the interface */
        always_ff @(posedge write_channel.ACLK) begin
            if (write_start_i & !write_flush) begin 
                write_channel.WDATA <= write_data_i;
                write_channel.AWADDR <= write_address_i;
                write_channel.WSTRB <= write_strobe_i;
            end 
        end 


        always_ff @(posedge write_channel.ACLK `ifdef ASYNC or negedge write_channel.ARESETN `endif) begin : write_handshake
            if (!write_channel.ARESETN) begin
                write_channel.AWVALID <= 1'b0;
                write_channel.WVALID <= 1'b0;
            end else begin
                if (write_start_i & !write_flush) begin
                    /* Start handshake */
                    write_channel.AWVALID <= 1'b1;
                    write_channel.WVALID <= 1'b1;
                end else begin 
                    if ((write_channel.AWVALID & write_channel.AWREADY) | write_flush) begin
                        /* Handshake completed once the slave accept the address */
                        write_channel.AWVALID <= 1'b0;
                    end

                    if ((write_channel.WVALID & write_channel.WREADY) | write_flush) begin
                        /* End of transaction */
                        write_channel.WVALID <= 1'b0;
                    end
                end
            end
        end : write_handshake

    /* In this system a master is always ready for a response */
    assign write_channel.BREADY = 1'b1; 

    /* Write transaction is completed */
    assign write_done_o = write_channel.BVALID & write_channel.BREADY;

    /* AXI slave response only valid when the transaction ended */
    assign write_response_o = write_channel.BRESP;

    /* Clear to send another transaction */
    assign write_cts_o = write_channel.AWREADY & write_channel.WREADY;


//====================================================================================
//      READ DATA LOGIC 
//====================================================================================

    /* Flush all the ongoing transactions */
    logic read_flush; assign read_flush = (read_channel.RRESP == SLVERR);


        /* Immediately load the read transaction informations on the interface */
        always_ff @(posedge read_channel.ACLK) begin
            if (read_start_i & !read_flush) begin 
                read_channel.ARADDR <= read_address_i;
            end 
        end 


        always_ff @(posedge read_channel.ACLK `ifdef ASYNC or negedge read_channel.ARESETN `endif) begin : read_handshake
            if (!read_channel.ARESETN) begin
                read_channel.ARVALID <= 1'b0;
                read_channel.RREADY <= 1'b1;
            end else begin 
                if (read_start_i & !read_flush) begin
                    /* Start handshake */
                    read_channel.ARVALID <= 1'b1;
                end else begin 
                    if ((read_channel.ARVALID & read_channel.ARREADY) | read_flush) begin
                        /* Handshake completed once the slave accept the address */
                        read_channel.ARVALID <= 1'b0;
                    end
                end 
            end
        end : read_handshake

    /* In this system a master is always ready for a response */
    assign read_channel.RREADY = 1'b1;

    /* Data sent by slave is valid */
    assign read_valid_o = read_channel.RVALID & read_channel.RREADY;
    assign read_data_o = read_channel.RDATA; 
    assign read_response_o = read_channel.RRESP;

    /* Write transaction is completed */
    assign read_done_o = read_channel.RVALID & read_channel.RREADY;

    /* Clear to send another transaction */
    assign read_cts_o = read_channel.ARREADY;

endmodule : axi_master 

`endif 