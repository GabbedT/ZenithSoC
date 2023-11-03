`ifndef AXI_ROUTER_SV
    `define AXI_ROUTER_SV

`include "../Utility/Interfaces/axi_write_interface.sv"
`include "../Utility/Interfaces/axi_read_interface.sv"

`include "../Utility/Packages/axi_interface_pkg.sv"


module axi_router #(
    /* Number of MASTER / SLAVE */
    parameter N_MASTER = 1,
    parameter N_SLAVE = 2,

    /* Max number of requests pending */
    parameter MAX_PENDING_WRITE = 8,
    parameter MAX_PENDING_READ = 8
) (
    /* Global signals */
    input logic clk_i,
    input logic rst_n_i,

    /* Master interfaces */
    axi_write_interface.slave master_write_channel [N_MASTER - 1:0],
    axi_read_interface.slave master_read_channel [N_MASTER - 1:0],

    /* Slave interfaces */
    axi_write_interface.master slave_write_channel [N_SLAVE - 1:0],
    axi_read_interface.master slave_read_channel [N_SLAVE - 1:0],

    /* Slave connection */
    input logic [N_SLAVE - 1:0] slave_write_match_i,
    input logic [N_SLAVE - 1:0] slave_read_match_i
);

//====================================================================================
//      SIGNALS ASSIGNMENT
//====================================================================================

    `ifdef SV_ASSERTION

        /* Only one or zero slave can be targeted by master */
        assert property (@(posedge clk_i) $onehot0(slave_write_match_i));
        assert property (@(posedge clk_i) $onehot0(slave_read_match_i));

        /* If a master request connection with a slave and that doesn't exist
         * the router must return a bus error */

    `endif 

    /* AXI Master Write signals */
    logic [N_MASTER - 1:0][31:0] m_axi_AWADDR, m_axi_WDATA; 
    logic [N_MASTER - 1:0] m_axi_AWVALID, m_axi_AWREADY, m_axi_WVALID, m_axi_WREADY, m_axi_BVALID, m_axi_BREADY;
    logic [N_MASTER - 1:0][3:0] m_axi_WSTRB;
    axi_response_t [N_MASTER - 1:0] m_axi_BRESP;
    
    genvar i;

    generate 
        for (i = 0; i < N_MASTER; ++i) begin
            /* Input from master */
            assign m_axi_AWADDR[i]  = master_write_channel[i].AWADDR;
            assign m_axi_AWVALID[i] = master_write_channel[i].AWVALID;
            assign m_axi_WDATA[i]   = master_write_channel[i].WDATA;
            assign m_axi_WSTRB[i]   = master_write_channel[i].WSTRB;
            assign m_axi_WVALID[i]  = master_write_channel[i].WVALID;
            assign m_axi_BREADY[i]  = master_write_channel[i].BREADY;

            /* Output to master */
            assign master_write_channel[i].AWREADY = m_axi_AWREADY[i];
            assign master_write_channel[i].WREADY  = m_axi_WREADY[i];
            assign master_write_channel[i].BVALID  = m_axi_BVALID[i];
            assign master_write_channel[i].BRESP   = m_axi_BRESP[i];
        end 
    endgenerate 


    /* AXI Slave Write signals */
    logic [N_SLAVE - 1:0][31:0] s_axi_AWADDR, s_axi_WDATA; 
    logic [N_SLAVE - 1:0] s_axi_AWVALID, s_axi_AWREADY, s_axi_WVALID, s_axi_WREADY, s_axi_BVALID, s_axi_BREADY;
    logic [N_SLAVE - 1:0][3:0] s_axi_WSTRB;
    axi_response_t [N_SLAVE - 1:0] s_axi_BRESP;
    
    generate 
        for (i = 0; i < N_SLAVE; ++i) begin
            /* Output to slave */
            assign slave_write_channel[i].AWADDR  = s_axi_AWADDR[i];
            assign slave_write_channel[i].AWVALID = s_axi_AWVALID[i];
            assign slave_write_channel[i].WDATA   = s_axi_WDATA[i];
            assign slave_write_channel[i].WSTRB   = s_axi_WSTRB[i];
            assign slave_write_channel[i].WVALID  = s_axi_WVALID[i];
            assign slave_write_channel[i].BREADY  = s_axi_BREADY[i];

            /* Input from slave */
            assign s_axi_AWREADY[i] = slave_write_channel[i].AWREADY;
            assign s_axi_WREADY[i]  = slave_write_channel[i].WREADY;
            assign s_axi_BVALID[i]  = slave_write_channel[i].BVALID;
            assign s_axi_BRESP[i]   = slave_write_channel[i].BRESP;
        end 
    endgenerate 


    /* AXI Master Read signals */
    logic [N_MASTER - 1:0][31:0] m_axi_ARADDR, m_axi_RDATA;
    logic [N_MASTER - 1:0] m_axi_ARVALID, m_axi_ARREADY, m_axi_RVALID, m_axi_RREADY;
    axi_response_t [N_MASTER - 1:0] m_axi_RRESP;

    generate
        for (i = 0; i < N_MASTER; ++i) begin
            /* Input from master */
            assign m_axi_ARADDR[i]  = master_read_channel[i].ARADDR;
            assign m_axi_ARVALID[i] = master_read_channel[i].ARVALID;
            assign m_axi_RREADY[i]  = master_read_channel[i].RREADY;

            /* Output to master */
            assign master_read_channel[i].ARREADY = m_axi_ARREADY[i];
            assign master_read_channel[i].RDATA   = m_axi_RDATA[i];
            assign master_read_channel[i].RRESP   = m_axi_RRESP[i];
            assign master_read_channel[i].RVALID  = m_axi_RVALID[i];
        end 
    endgenerate


    /* AXI Slave Read signals */
    logic [N_SLAVE - 1:0][31:0] s_axi_ARADDR, s_axi_RDATA;
    logic [N_SLAVE - 1:0] s_axi_ARVALID, s_axi_ARREADY, s_axi_RVALID, s_axi_RREADY;
    axi_response_t [N_SLAVE - 1:0] s_axi_RRESP;

    generate
        for (i = 0; i < N_SLAVE; ++i) begin
            /* Output to slave */
            assign slave_read_channel[i].ARADDR = s_axi_ARADDR[i];
            assign slave_read_channel[i].ARVALID = s_axi_ARVALID[i];
            assign slave_read_channel[i].RREADY = s_axi_RREADY[i];

            /* Input from slave */
            assign s_axi_ARREADY[i] = slave_read_channel[i].ARREADY;
            assign s_axi_RDATA[i] = slave_read_channel[i].RDATA;
            assign s_axi_RRESP[i] = slave_read_channel[i].RRESP;
            assign s_axi_RVALID[i] = slave_read_channel[i].RVALID;
        end 
    endgenerate


//====================================================================================
//      SHARED SIGNALS 
//====================================================================================

    /* To save logic and reduce routing congestion, we can make some AXI signals global. Data, addresses, strobe are signals
     * that can be shared among all the MASTER / SLAVE in the system, only two of them will be connected with the proper
     * handshaking signals. Given this, only the two point to point connected interfaces will interpred the shared data as valid */

    /* Write signals */
    logic [31:0] write_data2slave, write_address2slave; logic write_addr_valid2slave, write_data_valid2slave, write_ready2slave; logic [3:0] write_strobe2slave;

        always_comb begin 
            /* Drive slaves */
            for (int i = 0; i < N_SLAVE; ++i) begin
                s_axi_WDATA[i] = write_data2slave;
                s_axi_WSTRB[i] = write_strobe2slave;
                s_axi_WVALID[i] = write_data_valid2slave;

                s_axi_AWADDR[i] = write_address2slave;
                s_axi_AWVALID[i] = write_addr_valid2slave;

                s_axi_BREADY[i] = write_ready2slave;
            end
        end


    /* Read signals */
    logic [31:0] read_data2master, read_address2slave; logic read_addr_valid2slave, read_ready2slave;

        always_comb begin 
            for (int i = 0; i < N_MASTER; ++i) begin
                m_axi_RDATA[i] = read_data2master;
            end

            for (int i = 0; i < N_SLAVE; ++i) begin
                s_axi_ARADDR[i] = read_address2slave;
                s_axi_ARVALID[i] = read_addr_valid2slave;
                s_axi_RREADY[i] = read_ready2slave;
            end
        end



    generate if (N_MASTER != 1) begin 

//====================================================================================
//      MULTI MASTER TO SLAVE WRITE ROUTING 
//====================================================================================

    logic write_request, write_response; logic [$clog2(MAX_PENDING_WRITE) - 1:0] pending_writes;

        /* Increment / Decrement the number of requests pending */
        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin : write_request_counter
            if (!rst_n_i) begin 
                pending_writes <= '0;
            end else begin 
                case ({write_request, write_response})
                    2'b10: pending_writes <= pending_writes + 1'b1;
                    2'b01: pending_writes <= pending_writes - 1'b1;
                endcase 
            end 
        end : write_request_counter


    logic [N_MASTER - 1:0] write_bus_granted; logic [$clog2(N_MASTER) - 1:0] write_bus_request; 

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin : master_write_access_decoder
            if (!rst_n_i) begin 
                write_bus_granted <= '1;
            end else begin 
                if (write_request) begin 
                    /* Set only the bit corresponding to the master that has
                    * access to the bus */
                    write_bus_granted <= write_bus_granted & (1 << write_bus_request);
                end else begin
                    if (pending_writes == '0) begin
                        /* Relinquish the bus */
                        write_bus_granted <= '1;
                    end
                end
            end 
        end : master_write_access_decoder


    /* Decode MASTER / SLAVE number */
    logic [$clog2(N_SLAVE) - 1:0] slave_write_connect, slave_write_number; logic [$clog2(N_MASTER) - 1:0] master_write_request;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin : onehot_write_decoder
            if (!rst_n_i) begin 
                slave_write_connect <= '0;
            end else begin 
                if (slave_write_match_i != '0) begin
                    slave_write_connect <= slave_write_number;
                end

                master_write_request <= write_bus_request;
            end 
        end : onehot_write_decoder


    logic [N_MASTER - 1:0] write_request_pending; 

    assign write_request = write_request_pending != '0;

        always_comb begin : write_routing
            /* Default values */
            write_bus_request = '0;
            slave_write_number = '0;
            m_axi_AWREADY = '0;
            m_axi_WREADY = '0;
            m_axi_BVALID = '0;
            m_axi_BRESP = '0;
            s_axi_BREADY = '0;

            for (int i = 0; i < N_MASTER; ++i) begin
                write_request_pending[i] = m_axi_AWVALID[i] & write_bus_granted[i];

                if (write_request_pending[i]) begin 
                    write_bus_request = i;
                end
            end

            for (int i = 0; i < N_SLAVE; ++i) begin
                if (slave_write_match_i[i]) begin
                    slave_write_number = i;
                end
            end
            
            /* MASTER to SLAVE shared address routing */
            write_address2slave = m_axi_AWADDR[write_bus_request];
            write_addr_valid2slave = m_axi_AWVALID[write_bus_request];

            /* MASTER to SLAVE shared data routing */
            write_data2slave = m_axi_WDATA[write_bus_request];
            write_strobe2slave = m_axi_WSTRB[write_bus_request];
            write_data_valid2slave = m_axi_WVALID[write_bus_request];
            write_ready2slave = m_axi_BREADY[master_write_request];

            /* Ready signals on address and data needs to be multiplexed and demultiplexed
             * immediately to ensure good latency */
            m_axi_AWREADY[master_write_request] = s_axi_AWREADY[slave_write_number];
            m_axi_WREADY[master_write_request] = s_axi_WREADY[slave_write_number];

            /* Multiplex from the slave matched and Demultiplex to the master
             * on the next clock cycle after bus acquire */
            m_axi_BVALID[master_write_request] = s_axi_BVALID[slave_write_connect];
            m_axi_BRESP[master_write_request] = s_axi_BRESP[slave_write_connect];

            /* Write transaction response */
            write_response = s_axi_BVALID[slave_write_connect] & m_axi_WREADY[master_write_request];
        end 

    end else begin

//====================================================================================
//      SINGLE MASTER TO SLAVE WRITE ROUTING 
//====================================================================================

    logic [$clog2(N_SLAVE) - 1:0] slave_write_number; 

        always_comb begin
            slave_write_number = '0;

            for (int i = 0; i < N_SLAVE; ++i) begin
                if (slave_write_match_i[i]) begin
                    slave_write_number = i;
                end
            end
        end
        
    /* Decode MASTER / SLAVE number */
    logic [$clog2(N_SLAVE) - 1:0] slave_write_connect;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin : onehot_write_decoder
            if (!rst_n_i) begin 
                slave_write_connect <= '0;
            end else begin 
                if (slave_write_match_i != '0) begin
                    slave_write_connect <= slave_write_number;
                end
            end 
        end : onehot_write_decoder

        
    assign write_address2slave = m_axi_AWADDR[0];
    assign write_addr_valid2slave = m_axi_AWVALID[0];

    assign write_data2slave = m_axi_WDATA[0];
    assign write_strobe2slave = m_axi_WSTRB[0];
    assign write_data_valid2slave = m_axi_WVALID[0];
    
    assign write_ready2slave = m_axi_BREADY[0];

    /* Select only the matched slave */
    assign m_axi_AWREADY[0] = s_axi_AWREADY[slave_write_number];
    assign m_axi_WREADY[0] = s_axi_WREADY[slave_write_number];
    assign m_axi_BVALID[0] = s_axi_BVALID[slave_write_connect];
    assign m_axi_BRESP[0] = s_axi_BRESP[slave_write_connect];

    end endgenerate


    generate if (N_MASTER != 1) begin 

//====================================================================================
//      MULTI MASTER TO SLAVE READ ROUTING 
//====================================================================================

    logic read_request, read_response; logic [$clog2(MAX_PENDING_READ) - 1:0] pending_reads;

        /* Increment / Decrement the number of requests pending */
        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin : read_request_counter
            if (!rst_n_i) begin 
                pending_reads <= '0;
            end else begin 
                case ({read_request, read_response})
                    2'b10: pending_reads <= pending_reads + 1'b1;
                    2'b01: pending_reads <= pending_reads - 1'b1;
                endcase 
            end 
        end : read_request_counter


    logic [N_MASTER - 1:0] read_bus_granted; logic [$clog2(N_MASTER) - 1:0] read_bus_request; 

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin : master_read_access_decoder
            if (!rst_n_i) begin 
                read_bus_granted <= '1;
            end else begin 
                if (read_request) begin 
                    /* Set only the bit corresponding to the master that has
                    * access to the bus */
                    read_bus_granted <= read_bus_granted & (1 << read_bus_request);
                end else begin
                    if (pending_reads == '0) begin
                        /* Relinquish the bus */
                        read_bus_granted <= '1;
                    end
                end
            end 
        end : master_read_access_decoder


    /* Decode MASTER / SLAVE number */
    logic [$clog2(N_SLAVE) - 1:0] slave_read_connect, slave_read_number; logic [$clog2(N_MASTER) - 1:0] master_read_request;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin : onehot_read_decoder
            if (!rst_n_i) begin 
                slave_read_connect <= '0;
            end else begin 
                if (slave_read_match_i != '0) begin
                    /* Save the index of the slave matched */
                    slave_read_connect <= slave_read_number;
                end

                master_read_request <= read_bus_request;
            end 
        end : onehot_read_decoder


    logic [N_MASTER - 1:0] read_request_pending; 

    assign read_request = read_request_pending != '0;

        always_comb begin : write_routing
            /* Default values */
            read_bus_request = '0;
            slave_read_number = '0;
            m_axi_ARREADY = '0;
            m_axi_RVALID = '0;
            m_axi_RRESP = '0;
            s_axi_RREADY = '0;

            for (int i = 0; i < N_MASTER; ++i) begin
                read_request_pending[i] = m_axi_ARVALID[i] & read_bus_granted[i];

                if (read_request_pending[i]) begin 
                    read_bus_request = i;
                end
            end

            for (int i = 0; i < N_SLAVE; ++i) begin
                if (slave_read_match_i[i]) begin
                    slave_read_number = i;
                end
            end
            
            /* Route shared global nets */
            read_address2slave = m_axi_ARADDR[read_bus_request];
            read_addr_valid2slave = m_axi_ARVALID[read_bus_request];
            read_ready2slave = m_axi_RREADY[read_bus_request];
            read_data2master = s_axi_RDATA[slave_read_connect];

            /* Ready signals on address and data needs to be multiplexed and demultiplexed
             * immediately to ensure good latency */
            m_axi_ARREADY[master_read_request] = s_axi_ARREADY[slave_read_number];

            /* Multiplex from the slave matched and Demultiplex to the master
             * on the next clock cycle after bus acquire */
            m_axi_RVALID[master_read_request] = s_axi_RVALID[slave_read_connect];
            m_axi_RRESP[master_read_request] = s_axi_RRESP[slave_read_connect];

            /* Read transaction response */
            read_response = s_axi_RVALID[slave_read_connect] & m_axi_RREADY[master_read_request];
        end 

    end else begin

//====================================================================================
//      SINGLE MASTER TO SLAVE READ ROUTING 
//====================================================================================

    logic [$clog2(N_SLAVE) - 1:0] slave_read_number; 

        always_comb begin
            slave_read_number = '0;

            for (int i = 0; i < N_SLAVE; ++i) begin
                if (slave_read_match_i[i]) begin
                    slave_read_number = i;
                end
            end
        end
        
    /* Decode MASTER / SLAVE number */
    logic [$clog2(N_SLAVE) - 1:0] slave_read_connect;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin : onehot_read_decoder
            if (!rst_n_i) begin 
                slave_read_connect <= '0;
            end else begin 
                if (slave_read_match_i != '0) begin
                    slave_read_connect <= slave_read_number;
                end
            end 
        end : onehot_read_decoder

        
    assign read_address2slave = m_axi_ARADDR[0];
    assign read_addr_valid2slave = m_axi_ARVALID[0];
    assign read_ready2slave = m_axi_RREADY[0];

    /* Select only the matched slave */
    assign m_axi_ARREADY[0] = s_axi_ARREADY[slave_read_number];
    assign m_axi_RVALID[0] = s_axi_RVALID[slave_read_connect];
    assign m_axi_RRESP[0] = s_axi_RRESP[slave_read_connect];
    assign read_data2master = s_axi_RDATA[slave_read_connect];

    end endgenerate

endmodule : axi_router

`endif 