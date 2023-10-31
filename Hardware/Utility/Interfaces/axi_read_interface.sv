`ifndef AXI_READ_INTERFACE_SV 
    `define AXI_READ_INTERFACE_SV

`include "../Packages/axi_interface_pkg.sv"

interface axi_read_interface(input logic ACLK, input logic ARESETN);

//====================================================================================
//      READ ADDRESS CHANNEL 
//====================================================================================
    
    /* Read address */
    logic [AXI_ADDR_SIZE - 1:0] ARADDR;

    /* Read burst lenght */
    logic [7:0] ARLEN;

    /* Maximum number of bytes in each transfer during a burst */
    axi_burst_size_t ARSIZE;

    /* Burst type */
    axi_burst_t ARBURST;   

    /* Master address valid to be sampled */
    logic ARVALID;

    /* Slave is ready to accept the address */
    logic ARREADY;


//====================================================================================
//      READ DATA CHANNEL 
//====================================================================================

    /* Read data */
    logic [AXI_DATA_SIZE - 1:0][7:0] RDATA;

    /* Indicates the last transfer in a read burst */
    logic RLAST;

    /* Data is valid */
    logic RVALID;

    /* Master is ready to accept the data */
    logic RREADY;


//====================================================================================
//      MODPORTS
//====================================================================================

    modport master (
        input ACLK,
        input ARESETN,

        /* Read Address Channel */
        output ARADDR,
        output ARLEN,
        output ARSIZE,
        output ARBURST,
        output ARVALID,

        input ARREADY,

        /* Read Data Channel */
        output RREADY,

        input RDATA,
        input RLAST,
        input RVALID
    );

    modport slave (
        input ACLK,
        input ARESETN,

        /* Read Address Channel */
        output ARREADY,

        input ARADDR,
        input ARLEN,
        input ARSIZE,
        input ARBURST,
        input ARVALID,

        /* Read Data Channel */
        output RDATA,
        output RLAST,
        output RVALID,

        input RREADY
    );


//====================================================================================
//      TASK
//====================================================================================

    task automatic reset();
        ARADDR <= '0;
        ARLEN <= '0;
        ARSIZE <= WORD;
        ARBURST <= FIXED;
        ARVALID <= '0;
        ARREADY <= '0;
        RDATA <= '0;
        RLAST <= '0;
        RVALID <= '0;
        RREADY <= '0;
    endtask : reset

endinterface : axi_read_interface

`endif 