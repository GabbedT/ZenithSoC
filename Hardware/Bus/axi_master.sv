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
    input logic write_valid_i,
    output logic write_done_o,

    /* Write burst informations */
    input logic write_burst_i,
    input logic write_burst_end_i,
    input logic [7:0] write_lenght_i,
    input logic [1:0] write_size_i,
    input logic [1:0] write_type_i,


    /* Bus read channel AXI connectivity */
    axi_read_interface.master read_channel, 

    /* Read request informations */
    input logic [31:0] read_address_i,
    input logic read_valid_i,
    output logic [31:0] read_data_o,
    output logic [31:0] read_valid_o,
    output logic read_done_o,

    /* Read burst informations */
    input logic read_burst_i,
    input logic [7:0] read_lenght_i,
    input logic [1:0] read_size_i,
    input logic [1:0] read_type_i,


    /* Status */
    input logic stop_read_i,
    output logic stop_write_o
);

//====================================================================================
//      WRITE DATA CHANNEL 
//====================================================================================

    /* Currently a burst operation is taking place */
    logic write_burst_on;

        /* Immediately load the write informations on the interface */
        always_ff @(posedge write_channel.ACLK) begin
            if (write_valid_i) begin 
                write_channel.WDATA <= write_data_i;
                write_channel.AWADDR <= write_address_i;
                write_channel.WSTRB <= write_strobe_i;
            end 
        end 

        /* Write request must be assserted without waiting for READY */
        always_ff @(posedge write_channel.ACLK `ifdef ASYNC or negedge write_channel.ARESETN `endif) begin : write_handshake
            if (!write_channel.ARESETN) begin
                write_channel.AWVALID <= 1'b0;
                write_channel.WVALID <= 1'b0;
            end else begin
                if (write_valid_i) begin
                    /* Address is valid, if a burst has been requested it's valid only 
                     * until the READY signal from the slave */
                    write_channel.AWVALID <= !write_burst_on;

                    /* Data is valid */
                    write_channel.WVALID <= 1'b1;
                end

                /* Complete handshake once both VALID and READY asseted, deassert address valid */
                if (write_channel.AWVALID & write_channel.AWREADY) begin
                    write_channel.AWVALID <= 1'b0;
                end

                if (write_channel.WVALID & write_channel.WREADY) begin
                    if (write_burst_on) begin 
                        /* During a burst the operation finishes once the last
                         * data word has been sent */
                        write_channel.WVALID <= !write_channel.WLAST;
                    end else begin
                        /* During a normal transaction this is the end of it */
                        write_channel.WVALID <= 1'b0;
                    end
                end
            end
        end : write_handshake

    /* In this system a master is always ready for a response */
    assign write_channel.BREADY = 1'b1; 

    /* Write transaction is completed */
    assign write_done_o = write_channel.BVALID & write_channel.BREADY;


        /* Burst writes informations */
        always_ff @(posedge write_channel.ACLK `ifdef ASYNC or negedge write_channel.ARESETN `endif) begin : write_burst
            if (!write_channel.ARESETN) begin 
                write_channel.AWLEN <= 7'b0;
                write_channel.AWSIZE <= WORD; 
                write_channel.AWBURST <= FIXED; 

                write_burst_on <= 1'b0;
            end else begin 
                if (write_burst_i & write_valid_i) begin 
                    /* Set burst informations */
                    write_channel.AWLEN <= write_lenght_i;
                    write_channel.AWSIZE <= axi_burst_size_t'({1'b0, write_size_i});
                    write_channel.AWBURST <= axi_burst_t'(write_type_i);

                    write_burst_on <= 1'b1; 
                end

                if (write_channel.AWVALID & write_channel.AWREADY) begin 
                    /* Back to 1 beat transaction once the handshake ends */
                    write_channel.AWLEN <= 7'b0;
                    write_channel.AWSIZE <= WORD; 
                    write_channel.AWBURST <= FIXED; 
                end 

                if (write_channel.BVALID) begin
                    /* Burst end on slave response */
                    write_burst_on <= 1'b0;
                end
            end
        end : write_burst

        /* Last data transaction signal */
        always_ff @(posedge write_channel.ACLK) begin
            write_channel.WLAST <= write_burst_end_i;
        end 

    /* In a burst transaction, wait if the slave is not ready to accept data */
    assign stop_write_o = !write_channel.WREADY;


//====================================================================================
//      READ DATA CHANNEL 
//====================================================================================

        /* Immediately load the read informations on the interface */
        always_ff @(posedge read_channel.ACLK) begin
            if (read_valid_i) begin 
                read_channel.ARADDR <= read_address_i;
            end 
        end 

        /* Read request must be asserted without waiting for READY */
        always_ff @(posedge read_channel.ACLK `ifdef ASYNC or negedge read_channel.ARESETN `endif) begin : read_handshake
            if (!read_channel.ARESETN) begin
                read_channel.ARVALID <= 1'b0;
                read_channel.RREADY <= 1'b1;
            end else begin 
                if (read_valid_i) begin
                    /* Address is immediately valid */
                    read_channel.ARVALID <= 1'b1;
                end

                /* Complete handshake once both VALID and READY asseted */
                if (read_channel.ARVALID & read_channel.ARREADY) begin
                    read_channel.ARVALID <= 1'b0;
                end

                /* Master is not ready only when an external condition happens 
                 * for example a FIFO is full*/
                read_channel.RREADY <= !stop_read_i;
            end
        end : read_handshake

    /* Data sent by slave is valid */
    assign read_valid_o = read_channel.RVALID & read_channel.RREADY;
    assign read_data_o = read_channel.RDATA; 
    

    /* Currently a burst operation is taking place */
    logic read_burst_on;

        /* Burst read informations */
        always_ff @(posedge read_channel.ACLK `ifdef ASYNC or negedge read_channel.ARESETN `endif) begin : read_burst
            if (!read_channel.ARESETN) begin 
                read_channel.ARLEN <= 7'b0;
                read_channel.ARSIZE <= WORD; 
                read_channel.ARBURST <= FIXED; 

                read_burst_on <= 1'b0;
            end else begin
                if (read_burst_i & read_valid_i) begin 
                    /* Set burst informations */
                    read_channel.ARLEN <= read_lenght_i;
                    read_channel.ARSIZE <= axi_burst_size_t'({1'b0, read_size_i});
                    read_channel.ARBURST <= axi_burst_t'(read_type_i);

                    read_burst_on <= 1'b1;
                end

                if (read_channel.ARVALID & read_channel.ARREADY) begin 
                    /* Back to 1 beat transaction once the handshake ends */
                    read_channel.ARLEN <= 7'b0;
                    read_channel.ARSIZE <= WORD; 
                    read_channel.ARBURST <= FIXED; 
                end 

                if (read_channel.RVALID & read_channel.RREADY & read_channel.RLAST) begin
                    read_burst_on <= 1'b0;
                end
            end
        end : read_burst

    assign read_done_o = read_channel.RVALID & read_channel.RREADY & !(read_channel.RLAST ^ read_burst_on);

endmodule : axi_master 

`endif 