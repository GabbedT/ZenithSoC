`ifndef AXI_READ_INTERFACE_SV 
    `define AXI_READ_INTERFACE_SV

`include "../Packages/axi_interface_pkg.sv"

interface axi_read_interface(input logic ACLK, input logic ARESETN);

//====================================================================================
//      READ ADDRESS CHANNEL 
//====================================================================================
    
    /* Read address */
    logic [AXI_ADDR_SIZE - 1:0] ARADDR;

    /* Master address valid to be sampled */
    logic ARVALID;

    /* Slave is ready to accept the address */
    logic ARREADY;

    


//====================================================================================
//      READ DATA CHANNEL 
//====================================================================================

    /* Read data */
    logic [AXI_DATA_SIZE - 1:0][7:0] RDATA;

    /* Data is valid */
    logic RVALID;

    /* Master is ready to accept the data */
    logic RREADY;

    /* Read response */
    axi_response_t RRESP;


//====================================================================================
//      MODPORTS
//====================================================================================

    modport master (
        input ACLK,
        input ARESETN,

        /* Read Address Channel */
        output ARADDR,
        output ARVALID,

        input ARREADY,

        /* Read Data Channel */
        output RREADY,

        input RDATA,
        input RRESP,
        input RVALID
    );

    modport slave (
        input ACLK,
        input ARESETN,

        /* Read Address Channel */
        output ARREADY,

        input ARADDR,
        input ARVALID,

        /* Read Data Channel */
        output RDATA,
        output RRESP,
        output RVALID,

        input RREADY
    );

endinterface : axi_read_interface

`endif 