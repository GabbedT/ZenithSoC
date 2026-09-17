`ifndef DEV2DDR_INTERFACE_SV 
    `define DEV2DDR_INTERFACE_SV

interface dev2ddr_interface();

    /* Transaction request */
    logic trx_req;

    /* Transaction type: 0 = read, 1 = write */
    logic trx_type;

    /* Transaction address shared between read / write */
    logic [26:0] trx_addr;

    /* Write data and byte strobe */
    logic [15:0] wstrobe;
    logic [127:0] wdata;

    /* Read transaction response */
    logic read_valid;
    logic read_error;

    /* Write acceptance by the DDR controller */
    logic write_valid;
    logic write_error;

    /* Target can accept transaction */
    logic ready;

    /* Data read */
    logic [127:0] rdata;


//====================================================================================
//      MODPORTS
//====================================================================================

    modport master (
        /* General transaction info */
        output trx_req,
        output trx_type,
        output trx_addr,

        input read_valid,
        input read_error,
        input write_valid,
        input write_error,

        /* Slave status */
        input ready,

        /* Write path */
        output wdata,
        output wstrobe,

        /* Read path */
        input rdata
    );

    modport slave (
        /* General transaction info */
        input trx_req,
        input trx_type,
        input trx_addr,

        output read_valid,
        output read_error,
        output write_valid,
        output write_error,

        /* Slave status */
        output ready,

        /* Write path */
        input wdata,
        input wstrobe,

        /* Read path */
        output rdata
    );

endinterface : dev2ddr_interface

`endif
