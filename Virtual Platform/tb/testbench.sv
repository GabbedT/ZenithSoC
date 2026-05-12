module testbench (
    input logic clk_i,
    input logic rst_n_i

    /* Write request informations */
    input logic write_start_i,
    input logic [31:0] write_address_i,
    input logic [31:0] write_data_i,
    input logic [3:0] write_strobe_i,

    output logic write_done_o,


    /* Read request informations */
    input logic read_start_i,
    input logic read_invalid_i,
    input logic [31:0] read_address_i,
    
    output logic [31:0] read_data_o,
    output logic read_done_o,
);


//==============================================================
//      CONNECTOR
//==============================================================

    /* AXI connectivity to Slave */
    axi_write_interface write_channel();
    axi_read_interface read_channel();

    axi_response_t write_response_o;
    axi_response_t read_response_o;

    axi_master master (
        /* Global signals */
        .axi_ACLK    ( clk_i   ),
        .axi_ARESETN ( rst_n_i ),

        /* AXI connectivity */
        .write_channel ( write_channel ),
        .read_channel  ( read_channel  ),

        /* Router flow control */
        .router_write_cts_i ( 1'b1 ),
        .router_read_cts_i  ( 1'b1 ),

        /* Write request informations */
        .write_start_i      ( write_start_i   ),
        .write_address_i    ( write_address_i ),
        .write_data_i       ( write_data_i    ),
        .write_strobe_i     ( write_strobe_i  ),

        .write_done_o       ( write_done_o ),
        .write_cts_o        (              ),

        /* Read request informations */
        .read_start_i       ( read_start_i   ),
        .read_invalid_i     ( read_invalid_i ),
        .read_address_i     ( read_address_i ),
        
        .read_data_o        ( read_data_o ),
        .read_done_o        ( read_done_o ),
        .read_cts_o         (             ),

        /* Status */
        .write_response_o   ( write_response_o ),
        .read_response_o    ( read_response_o  )
    );


//==============================================================
//      IO BLOCK TO TEST
//==============================================================


endmodule : testbench 