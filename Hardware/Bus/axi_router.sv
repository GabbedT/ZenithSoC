`ifndef AXI_ROUTER_SV
    `define AXI_ROUTER_SV

`include "../Utility/Interfaces/axi_write_interface.sv"
`include "../Utility/Interfaces/axi_read_interface.sv"

`include "../Utility/Packages/axi_interface_pkg.sv"


module axi_router #(
    /* Number of slaves */
    parameter N_SLAVE = 2
) (
    /* Global signals */
    input logic clk_i,
    input logic rst_n_i,

    /* Master interfaces */
    axi_write_interface.slave master_write_channel,
    axi_read_interface.slave master_read_channel,

    /* Slave interfaces */
    axi_write_interface.master slave_write_channel [N_SLAVE - 1:0],
    axi_read_interface.master slave_read_channel [N_SLAVE - 1:0],

    /* Slave connection */
    input logic [N_SLAVE - 1:0] slave_write_match_i,
    input logic [N_SLAVE - 1:0] slave_write_busy_i,
    input logic [N_SLAVE - 1:0] slave_read_match_i,
    input logic [N_SLAVE - 1:0] slave_read_busy_i,
    output logic [N_SLAVE - 1:0] slave_write_bus_taken_o,
    output logic [N_SLAVE - 1:0] slave_read_bus_taken_o,

    /* Master error */
    output logic axi_write_error_o,
    output logic axi_read_error_o,

    /* Master flow control */
    output logic master_write_cts_o,
    output logic master_read_cts_o
);

//====================================================================================
//      SIGNALS ASSIGNMENT
//====================================================================================

    `ifdef SV_ASSERTION

        /* Only one or zero slave can be targeted by master */
        assert property (@(posedge clk_i) $onehot0(slave_write_match_i));
        assert property (@(posedge clk_i) $onehot0(slave_read_match_i));

    `endif 

    /* AXI Slave Write signals */
    logic [N_SLAVE - 1:0][31:0] s_axi_AWADDR, s_axi_WDATA; 
    logic [N_SLAVE - 1:0] s_axi_AWVALID, s_axi_AWREADY, s_axi_WVALID, s_axi_WREADY, s_axi_BVALID, s_axi_BREADY;
    logic [N_SLAVE - 1:0][3:0] s_axi_WSTRB;
    axi_response_t [N_SLAVE - 1:0] s_axi_BRESP;
    
    genvar i;

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
            for (int i = 0; i < N_SLAVE; ++i) begin
                s_axi_ARADDR[i] = read_address2slave;
                s_axi_ARVALID[i] = read_addr_valid2slave;
                s_axi_RREADY[i] = read_ready2slave;
            end
        end

    assign master_read_channel.RDATA = read_data2master;


//====================================================================================
//      MASTER TO SLAVE WRITE ROUTING 
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
                if (slave_write_match_i != '0 & master_write_cts_o) begin
                    slave_write_connect <= slave_write_number;
                end
            end 
        end : onehot_write_decoder

        
    assign write_address2slave = master_write_channel.AWADDR;
    assign write_addr_valid2slave = master_write_channel.AWVALID;

    assign write_data2slave = master_write_channel.WDATA;
    assign write_strobe2slave = master_write_channel.WSTRB;
    assign write_data_valid2slave = master_write_channel.WVALID;
    
    assign write_ready2slave = master_write_channel.BREADY;

    /* Select only the matched slave */
    assign master_write_channel.AWREADY = s_axi_AWREADY[slave_write_number];
    assign master_write_channel.WREADY = s_axi_WREADY[slave_write_number];
    assign master_write_channel.BVALID = s_axi_BVALID[slave_write_connect];
    assign master_write_channel.BRESP = s_axi_BRESP[slave_write_connect];

    /* Master is clear to send another transaction if its destined to the same slave of the previous transaction or 
     * if no slave is busy and the master is changing slave connection */
    assign master_write_cts_o = (slave_write_busy_i == '0) ? 1'b1 : !(slave_write_busy_i & slave_write_match_i);


    /* Foward the busy signal to other slaves */
    always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
        if (!rst_n_i) begin 
            slave_write_bus_taken_o <= '0;
        end else begin 
            slave_write_bus_taken_o <= (slave_write_busy_i == '0) ? '0 : ~slave_write_busy_i;
        end 
    end 
    

    /* If a request is sent and no slave respond, meaning that no valid slaves exist at that address, 
     * a bus error is generated */
    assign axi_write_error_o = (slave_write_match_i == '0) & master_write_channel.AWVALID;


//====================================================================================
//      MASTER TO SLAVE READ ROUTING 
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
                if (slave_read_match_i != '0 & master_read_cts_o) begin
                    slave_read_connect <= slave_read_number;
                end
            end 
        end : onehot_read_decoder

        
    assign read_address2slave = master_read_channel.ARADDR;
    assign read_addr_valid2slave = master_read_channel.ARVALID;
    assign read_ready2slave = master_read_channel.RREADY;

    /* Select only the matched slave */
    assign master_read_channel.ARREADY = s_axi_ARREADY[slave_read_number];
    assign master_read_channel.RVALID = s_axi_RVALID[slave_read_connect];
    assign master_read_channel.RRESP = s_axi_RRESP[slave_read_connect];
    assign read_data2master = s_axi_RDATA[slave_read_connect];


    /* Master is clear to send another transaction if its destined to the same slave of the previous transaction or 
     * if no slave is busy and the master is changing slave connection */
    assign master_read_cts_o = (slave_read_busy_i == '0) ? 1'b1 : !(slave_read_busy_i & slave_read_match_i);


    /* Foward the busy signal to other slaves */
    always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
        if (!rst_n_i) begin 
            slave_read_bus_taken_o <= '0;
        end else begin 
            slave_read_bus_taken_o <= (slave_read_busy_i == '0) ? '0 : ~slave_read_busy_i;
        end 
    end 

    /* If a request is sent and no slave respond, meaning that no valid slaves exist at that address, 
     * a bus error is generated */
    assign axi_read_error_o = (slave_read_match_i == '0) & master_read_channel.ARVALID;

endmodule : axi_router

`endif 