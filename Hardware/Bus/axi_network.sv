`ifndef AXI_NETWORK_SV
    `define AXI_NETWORK_SV

`include "../Utility/Interfaces/axi_write_interface.sv"
`include "../Utility/Interfaces/axi_read_interface.sv"

`include "../Utility/Packages/axi_interface_pkg.sv"

`include "axi_router.sv"
`include "axi_master.sv"
`include "axi_slave.sv"


module axi_network #(
    /* Number of masters and slaves */
    parameter AXI_SLAVE_NUMBER = 2,

    /* Slave's address boundaries */
    parameter int LOW_SLAVE_ADDRESS [AXI_SLAVE_NUMBER] = '{AXI_SLAVE_NUMBER{'0}},
    parameter int HIGH_SLAVE_ADDRESS [AXI_SLAVE_NUMBER] = '{AXI_SLAVE_NUMBER{'1}}
) (
    /* Global signals */
    input logic axi_ACLK,
    input logic axi_ARESETN,
    output logic axi_write_error_o,
    output logic axi_read_error_o,

    /* Master write interface */
    input logic write_start_i,
    input logic [31:0] write_address_i,
    input logic [31:0] write_data_i,
    input logic [3:0] write_strobe_i,
    output logic write_done_o,
    output logic write_cts_o,

    /* Master read interface */
    input logic read_start_i,
    input logic read_invalid_i,
    input logic [31:0] read_address_i,
    output logic [31:0] read_data_o,
    output logic read_done_o,
    output logic read_cts_o,


    /* Slave write interface */
    input logic [AXI_SLAVE_NUMBER - 1:0] write_error_i,
    input logic [AXI_SLAVE_NUMBER - 1:0] write_done_i,
    input logic [AXI_SLAVE_NUMBER - 1:0] write_busy_i,
    input logic [AXI_SLAVE_NUMBER - 1:0] write_ready_i,
    output logic [AXI_SLAVE_NUMBER - 1:0][31:0] write_address_o,
    output logic [AXI_SLAVE_NUMBER - 1:0][31:0] write_data_o,
    output logic [AXI_SLAVE_NUMBER - 1:0][3:0] write_strobe_o,
    output logic [AXI_SLAVE_NUMBER - 1:0] write_request_o,

    /* Slave read interface */
    input logic [AXI_SLAVE_NUMBER - 1:0][31:0] read_data_i,
    input logic [AXI_SLAVE_NUMBER - 1:0] read_error_i,
    input logic [AXI_SLAVE_NUMBER - 1:0] read_done_i,
    input logic [AXI_SLAVE_NUMBER - 1:0] read_busy_i,
    input logic [AXI_SLAVE_NUMBER - 1:0] read_ready_i,
    output logic [AXI_SLAVE_NUMBER - 1:0][31:0] read_address_o,
    output logic [AXI_SLAVE_NUMBER - 1:0] read_request_o
);

//====================================================================================
//      AXI MASTER INTERFACE LOGIC
//====================================================================================

    /* AXI Interconnection */
    axi_write_interface axi_master_write_channel();
    axi_read_interface axi_master_read_channel();

    /* Router flow control signals */
    logic clear2send_write_trx, clear2send_read_trx; 
    axi_response_t write_response, read_response;

    axi_master axi_master_interface (
        .axi_ACLK    ( axi_ACLK    ), 
        .axi_ARESETN ( axi_ARESETN ),  

        .write_channel ( axi_master_write_channel ), 
        .read_channel  ( axi_master_read_channel  ), 

        .router_write_cts_i ( clear2send_write_trx ),
        .router_read_cts_i  ( clear2send_read_trx  ),

        .write_start_i    ( write_start_i   ),
        .write_address_i  ( write_address_i ),
        .write_data_i     ( write_data_i    ),
        .write_strobe_i   ( write_strobe_i  ),
        .write_done_o     ( write_done_o    ),
        .write_cts_o      ( write_cts_o     ),
        .write_response_o ( write_response  ),

        .read_start_i    ( read_start_i   ),
        .read_invalid_i  ( read_invalid_i ),
        .read_address_i  ( read_address_i ),
        .read_data_o     ( read_data_o    ),
        .read_done_o     ( read_done_o    ),
        .read_cts_o      ( read_cts_o     ),
        .read_response_o ( read_response  ) 
    );



//====================================================================================
//      AXI SLAVE INTERFACE LOGIC
//====================================================================================

    /* AXI Interconnection */
    axi_write_interface axi_slave_write_channel[AXI_SLAVE_NUMBER - 1:0]();
    axi_read_interface axi_slave_read_channel[AXI_SLAVE_NUMBER - 1:0]();
    
    logic [AXI_SLAVE_NUMBER - 1:0] slave_write_match, slave_read_match, slave_write_busy, slave_read_busy, write_bus_taken, read_bus_taken;

    genvar i; generate

        for (i = 0; i < AXI_SLAVE_NUMBER; ++i) begin 
            axi_slave #(LOW_SLAVE_ADDRESS[i], HIGH_SLAVE_ADDRESS[i]) axi_slave_interface (
                .axi_ACLK    ( axi_ACLK    ), 
                .axi_ARESETN ( axi_ARESETN ), 

                .write_channel ( axi_slave_write_channel[i] ),
                .read_channel  ( axi_slave_read_channel[i]  ),

                .write_bus_taken_i ( write_bus_taken[i]   ),
                .write_match_o     ( slave_write_match[i] ),
                .write_busy_o      ( slave_write_busy[i]  ),
                .read_bus_taken_i  ( read_bus_taken[i]    ),
                .read_match_o      ( slave_read_match[i]  ),
                .read_busy_o       ( slave_read_busy[i]   ),

                .write_error_i   ( write_error_i[i]   ),
                .write_done_i    ( write_done_i[i]    ),
                .write_busy_i    ( write_busy_i[i]    ),
                .write_ready_i   ( write_ready_i[i]   ),
                .write_address_o ( write_address_o[i] ),
                .write_data_o    ( write_data_o[i]    ),
                .write_strobe_o  ( write_strobe_o[i]  ),
                .write_request_o ( write_request_o[i] ), 

                .read_data_i    ( read_data_i[i]    ),
                .read_error_i   ( read_error_i[i]   ),
                .read_done_i    ( read_done_i[i]    ),
                .read_busy_i    ( read_busy_i[i]    ),
                .read_ready_i   ( read_ready_i[i]   ),
                .read_address_o ( read_address_o[i] ),
                .read_request_o ( read_request_o[i] )
            );
        end

    endgenerate


//====================================================================================
//      AXI ROUTER LOGIC
//====================================================================================

    logic axi_write_error, axi_read_error;

    axi_router #(AXI_SLAVE_NUMBER) router (
        .clk_i   ( axi_ACLK    ),
        .rst_n_i ( axi_ARESETN ),

        .master_write_channel ( axi_master_write_channel ),
        .master_read_channel  ( axi_master_read_channel  ),

        .slave_write_channel ( axi_slave_write_channel ),
        .slave_read_channel  ( axi_slave_read_channel  ),

        .slave_write_match_i ( slave_write_match ),
        .slave_write_busy_i  ( slave_write_busy  ),
        .slave_read_match_i  ( slave_read_match  ),
        .slave_read_busy_i   ( slave_read_busy   ),

        .slave_write_bus_taken_o ( write_bus_taken ),
        .slave_read_bus_taken_o  ( read_bus_taken  ),

        .axi_write_error_o ( axi_write_error ),
        .axi_read_error_o  ( axi_read_error  ),

        .master_write_cts_o ( clear2send_write_trx ),
        .master_read_cts_o  ( clear2send_read_trx  )
    );

    assign axi_write_error_o = axi_write_error | ((write_response == DECERR) | (write_response == SLVERR));
    assign axi_read_error_o = axi_read_error | ((read_response == DECERR) | (read_response == SLVERR));

endmodule : axi_network 

`endif 