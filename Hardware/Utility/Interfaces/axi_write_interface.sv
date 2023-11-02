`ifndef AXI_WRITE_INTERFACE_SV 
    `define AXI_WRITE_INTERFACE_SV

`include "../Packages/axi_interface_pkg.sv"

interface axi_write_interface(input logic ACLK, input logic ARESETN);

//====================================================================================
//      WRITE ADDRESS CHANNEL 
//====================================================================================
    
    /* Write address */
    logic [AXI_ADDR_SIZE - 1:0] AWADDR;

    /* Master address valid to be sampled */
    logic AWVALID;

    /* Slave is ready to accept the address */
    logic AWREADY;


//====================================================================================
//      WRITE DATA CHANNEL 
//====================================================================================

    /* Write data */
    logic [AXI_DATA_SIZE - 1:0][7:0] WDATA;

    /* Indicates which byte lane hold valid data */
    logic [AXI_DATA_SIZE - 1:0] WSTRB;

    /* Data and strobe signals are valid */
    logic WVALID;

    /* Slave is ready to accept the data */
    logic WREADY;


//====================================================================================
//      WRITE RESPONSE CHANNEL 
//====================================================================================

    /* Write response is valid */
    logic BVALID; 

    /* The master can accept a write response */
    logic BREADY;

    /* Write response code */
    axi_response_t BRESP;


//====================================================================================
//      MODPORTS
//====================================================================================

    modport master (
        input ACLK,
        input ARESETN,

        /* Write Address Channel */
        output AWADDR,
        output AWVALID,

        input AWREADY,

        /* Write Data Channel */
        output WDATA,
        output WSTRB,
        output WVALID,

        input WREADY,

        /* Write Response Channel */
        output BREADY,

        input BRESP,
        input BVALID
    );

    modport slave (
        input ACLK,
        input ARESETN,

        /* Write Address Channel */
        output AWREADY,

        input AWADDR,
        input AWVALID,

        /* Write Data Channel */
        output WREADY,

        input WDATA,
        input WSTRB,
        input WVALID,

        /* Write Response Channel */
        output BRESP,
        output BVALID,

        input BREADY
    );

endinterface : axi_write_interface

`endif 