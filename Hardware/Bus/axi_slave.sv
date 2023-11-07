`ifndef AXI_SLAVE_SV
    `define AXI_SLAVE_SV

`include "../Utility/Interfaces/axi_write_interface.sv"
`include "../Utility/Interfaces/axi_read_interface.sv"


module axi_slave #(
    /* Low and High addresses to compare against 
     * the master's address request */
    parameter LOW_SLAVE_ADDRESS = 32'h0000_0000,
    parameter HIGH_SLAVE_ADDRESS = 32'hFFFF_FFFF
) (
    /* Global signals */
    input logic axi_ACLK, 
    input logic axi_ARESETN, 


    /* AXI connectivity */
    axi_write_interface.slave write_channel,
    axi_read_interface.slave read_channel,


    /* Router control */
    input logic write_bus_taken_i,
    output logic write_match_o,
    output logic write_busy_o,
    input logic read_bus_taken_i,
    output logic read_match_o,
    output logic read_busy_o,


    /* Write signals */
    input logic write_error_i,
    input logic write_done_i,
    input logic write_busy_i,
    input logic write_ready_i,

    output logic [31:0] write_address_o,
    output logic [31:0] write_data_o,
    output logic [3:0] write_strobe_o,
    output logic write_request_o,


    /* Read signals */
    input logic [31:0] read_data_i,
    input logic read_error_i,
    input logic read_done_i,
    input logic read_busy_i,
    input logic read_ready_i,

    output logic [31:0] read_address_o,
    output logic read_request_o
);

//====================================================================================
//      ADDRESS DECODING LOGIC 
//====================================================================================

    assign write_match_o = (write_channel.AWADDR >= LOW_SLAVE_ADDRESS) & (write_channel.AWADDR <= HIGH_SLAVE_ADDRESS) & write_channel.AWVALID;

    assign read_match_o = (read_channel.ARADDR >= LOW_SLAVE_ADDRESS) & (read_channel.ARADDR <= HIGH_SLAVE_ADDRESS) & read_channel.ARVALID;


//====================================================================================
//      WRITE DATA LOGIC 
//====================================================================================

        always_ff @(posedge axi_ACLK `ifdef ASYNC or negedge axi_ARESETN `endif) begin : write_handshake
            if (!axi_ARESETN) begin 
                write_channel.AWREADY <= 1'b1;
                write_channel.WREADY <= 1'b1;
                write_channel.BVALID <= 1'b0;

                write_channel.BRESP <= OKAY;
            end else begin 
                /* Ready if no external condition require a stall */
                write_channel.AWREADY <= write_ready_i;
                write_channel.WREADY <= write_ready_i;

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

    /* Slave is busy if it's processing a transaction */
    assign write_busy_o = write_busy_i | write_request_o;

    /* If the Master has presented valid data and the Slave is ready to accept a transaction */
    assign write_request_o = write_channel.WVALID & write_channel.WREADY & write_match_o & !write_bus_taken_i;

    /* Transaction informations */
    assign write_data_o = write_channel.WDATA;
    assign write_strobe_o = write_channel.WSTRB;
    assign write_address_o = write_channel.AWADDR;


    `ifdef SV_ASSERTION

        /* Check that SLAVE does not lower VALID if READY is not asserted by MASTER */
        assert property (@(posedge axi_ACLK) (write_channel.BVALID & !write_channel.BREADY) |=> write_channel.BVALID);

    `endif 


//====================================================================================
//      READ DATA LOGIC 
//====================================================================================

        always_ff @(posedge axi_ACLK) begin : read_transaction_info
            read_channel.RDATA <= read_data_i;
        end : read_transaction_info


        always_ff @(posedge axi_ACLK `ifdef ASYNC or negedge axi_ARESETN `endif) begin : read_handshake
            if (!axi_ARESETN) begin 
                read_channel.ARREADY <= 1'b1;
                read_channel.RVALID <= 1'b0;

                read_channel.RRESP <= OKAY;
            end else begin 
                /* Ready to accept address if no external condition require a stall */
                read_channel.ARREADY <= read_ready_i;

                read_channel.RRESP <= read_error_i ? SLVERR : OKAY;
                
                if (read_done_i) begin 
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

    /* Slave is busy if it's processing a transaction */
    assign read_busy_o = read_busy_i | read_request_o;

    /* If the Master has presented a valid address and the Slave is ready to accept a transaction */
    assign read_request_o = read_channel.ARVALID & read_channel.ARREADY & read_match_o & !read_bus_taken_i;

    /* Transaction informations */
    assign read_address_o = read_channel.ARADDR;


    `ifdef SV_ASSERTION

        /* Check that SLAVE does not lower VALID if READY is not asserted by MASTER */
        assert property (@(posedge axi_ACLK) (read_channel.RVALID & !read_channel.RREADY) |=> read_channel.RVALID);

    `endif 


endmodule : axi_slave 

`endif 