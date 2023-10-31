`ifndef AXI_WRITE_INTERFACE_SV 
    `define AXI_WRITE_INTERFACE_SV

`include "../Packages/axi_interface_pkg.sv"

interface axi_write_interface(input logic ACLK, input logic ARESETN);

//====================================================================================
//      WRITE ADDRESS CHANNEL 
//====================================================================================
    
    /* Write address */
    logic [AXI_ADDR_SIZE - 1:0] AWADDR;

    /* Write burst lenght */
    logic [7:0] AWLEN;

    /* Maximum number of bytes in each transfer during a burst */
    axi_burst_size_t AWSIZE;

    /* Burst type */
    axi_burst_t AWBURST;   

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

    /* Indicates the last transfer in a write burst */
    logic WLAST;

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


//====================================================================================
//      MODPORTS
//====================================================================================

    modport master (
        input ACLK,
        input ARESETN,

        /* Write Address Channel */
        output AWADDR,
        output AWLEN,
        output AWSIZE,
        output AWBURST,
        output AWVALID,

        input AWREADY,

        /* Write Data Channel */
        output WDATA,
        output WSTRB,
        output WLAST,
        output WVALID,

        input WREADY,

        /* Write Response Channel */
        output BREADY,

        input BVALID
    );

    modport slave (
        input ACLK,
        input ARESETN,

        /* Write Address Channel */
        output AWREADY,

        input AWADDR,
        input AWLEN,
        input AWSIZE,
        input AWBURST,
        input AWVALID,

        /* Write Data Channel */
        output WREADY,

        input WDATA,
        input WSTRB,
        input WLAST,
        input WVALID,

        /* Write Response Channel */
        output BVALID,

        input BREADY
    );


//====================================================================================
//      TASK
//====================================================================================

    task automatic reset();
        AWADDR <= '0;
        AWLEN <= '0;
        AWSIZE <= WORD;
        AWBURST <= FIXED;
        AWVALID <= '0;
        AWREADY <= '0;
        WDATA <= '0;
        WSTRB <= '0;
        WLAST <= '0;
        WVALID <= '0;
        WREADY <= '0;
        BREADY <= '1;
        BVALID <= '0;
    endtask : reset

endinterface : axi_write_interface

`endif 