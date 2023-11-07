`ifndef AXI_NETWORK_TEST_SV
    `define AXI_NETWORK_TEST_SV

`include "../../Hardware/Bus/axi_network.sv"

`include "axi_master_driver.sv"
`include "axi_slave_memory.sv"

module axi_network_test (); 

    localparam SLAVES = 2; 

    localparam int LOW_BOUNDARIES [SLAVES] = '{0, 32};
    localparam int HIGH_BOUNDARIES [SLAVES] = '{31, 63};


    /* Global signals */
    logic axi_ACLK = 0;
    logic axi_ARESETN = 0;

    always #5 axi_ACLK <= !axi_ACLK;

    /* Master write interface */
    logic write_start_i;
    logic [31:0] write_address_i;
    logic [31:0] write_data_i;
    logic [3:0] write_strobe_i;
    logic write_done_o;
    logic write_cts_o;
    axi_response_t read_response_o;

    /* Master read interface */
    logic read_start_i;
    logic [31:0] read_address_i;
    logic [31:0] read_data_o;
    logic read_done_o;
    logic read_cts_o;
    axi_response_t write_response_o;

    /* Slave write interface */
    logic [SLAVES - 1:0] write_error_i = '0;
    logic [SLAVES - 1:0] write_done_i;
    logic [SLAVES - 1:0] write_busy_i;
    logic [SLAVES - 1:0] write_ready_i = '1;
    logic [SLAVES - 1:0][31:0] write_address_o;
    logic [SLAVES - 1:0][31:0] write_data_o;
    logic [SLAVES - 1:0][3:0] write_strobe_o;
    logic [SLAVES - 1:0] write_request_o;

    /* Slave read interface */
    logic [SLAVES - 1:0] read_error_i = '0;
    logic [SLAVES - 1:0] read_done_i;
    logic [SLAVES - 1:0] read_busy_i;
    logic [SLAVES - 1:0] read_ready_i = '1;
    logic [SLAVES - 1:0][31:0] read_data_i;
    logic [SLAVES - 1:0][31:0] read_address_o;
    logic [SLAVES - 1:0] read_request_o;



    /* Transaction infos */
    logic [31:0] write_trx_address, write_trx_data, read_trx_address;
    logic [3:0] write_trx_strobe; logic write_trx_load, read_trx_load;

    /* Master */
    axi_master_driver master (
        .clk_i   ( axi_ACLK ), 
        .rst_n_i ( axi_ARESETN ),  

        .write_address_i ( write_trx_address ),
        .write_data_i    ( write_trx_data    ),
        .write_strobe_i  ( write_trx_strobe  ),
        .write_load_i    ( write_trx_load    ),
        .write_stop_i    (                   ),
        .write_address_o ( write_address_i   ),
        .write_data_o    ( write_data_i      ),
        .write_strobe_o  ( write_strobe_i    ),
        .write_start_o   ( write_start_i     ),
        .write_cts_i     ( write_cts_o       ),

        .read_address_i ( read_trx_address ),
        .read_load_i    ( read_trx_load    ),
        .read_stop_i    (                  ),
        .read_address_o ( read_address_i   ),
        .read_start_o   ( read_start_i     ),  
        .read_cts_i     ( read_cts_o       )  
    );


    /* Network */
    axi_network #(SLAVES, LOW_BOUNDARIES, HIGH_BOUNDARIES) network (
        .axi_ACLK    ( axi_ACLK    ),
        .axi_ARESETN ( axi_ARESETN ),

        .write_start_i   ( write_start_i   ),
        .write_address_i ( write_address_i ),
        .write_data_i    ( write_data_i    ),
        .write_strobe_i  ( write_strobe_i  ),
        .write_done_o    ( write_done_o    ),
        .write_cts_o     ( write_cts_o     ),
        .read_response_o ( read_response_o ),

        .read_start_i     ( read_start_i     ),
        .read_address_i   ( read_address_i   ),
        .read_data_o      ( read_data_o      ),
        .read_done_o      ( read_done_o      ),
        .read_cts_o       ( read_cts_o       ),
        .write_response_o ( write_response_o ),


        .write_error_i   ( write_error_i   ),
        .write_done_i    ( write_done_i    ),
        .write_busy_i    ( write_done_i    ),
        .write_ready_i   ( write_ready_i   ),
        .write_address_o ( write_address_o ),
        .write_data_o    ( write_data_o    ),
        .write_strobe_o  ( write_strobe_o  ),
        .write_request_o ( write_request_o ),

        .read_data_i    ( read_data_i    ),
        .read_error_i   ( read_error_i   ),
        .read_done_i    ( read_done_i    ),
        .read_busy_i    ( read_done_i    ),
        .read_ready_i   ( read_ready_i   ),
        .read_address_o ( read_address_o ),
        .read_request_o ( read_request_o )
    );

    
    genvar i; generate
        for (i = 0; i < SLAVES; ++i) begin 
            axi_slave_memory #(8) memory (
                .clk_i   ( axi_ACLK    ), 
                .rst_n_i ( axi_ARESETN ),
    
                .load_address_i ( read_address_o[i] ),
                .load_i         ( read_request_o[i] ),
                .load_data_o    ( read_data_i[i]    ),
                .load_done_o    ( read_done_i[i]    ),
                
                .store_address_i ( write_address_o[i] ),
                .store_data_i    ( write_data_o[i]    ),
                .store_strobe_i  ( write_strobe_o[i]  ),
                .store_i         ( write_request_o[i] ),
                .store_done_o    ( write_done_i[i]    )
            );
        end
    endgenerate

    task axi_read(input logic [31:0] address);
        read_trx_address <= address;
        read_trx_load <= 1'b1;
        @(posedge axi_ACLK);
        read_trx_load <= 1'b0;
    endtask 

    task axi_write(input logic [31:0] address, input logic [31:0] data, input logic [3:0] strobe);
        write_trx_address <= address;
        write_trx_data <= data;
        write_trx_strobe <= strobe;
        write_trx_load <= 1'b1;
        @(posedge axi_ACLK);
        write_trx_load <= 1'b0;
    endtask 

    initial begin
        @(posedge axi_ACLK);
        axi_ARESETN <= 1'b1;

        for (int i = 0; i < 8 * SLAVES; ++i) begin 
            axi_read(i * 4);
        end

        for (int i = 0; i < 8 * SLAVES; ++i) begin 
            axi_write(i * 4, '1, '1);
        end

        for (int i = 0; i < 8 * SLAVES; ++i) begin 
            axi_read(i * 4);
        end

        repeat(5) @(posedge axi_ACLK);

        for (int i = 0; i < 8 * SLAVES; ++i) begin 
            axi_read(i * 4);
            axi_write(i * 4, i, '1);
        end

        for (int i = 0; i < 8 * SLAVES; ++i) begin 
            axi_read(i * 4);
        end

        repeat(5) @(posedge axi_ACLK);

        for (int i = 0; i < 8; ++i) begin 
            axi_read(60 - (i * 4));
            axi_read(i * 4);
        end

        repeat(10) @(posedge axi_ACLK);

        for (int i = 0; i < 8 * SLAVES; ++i) begin 
            axi_write(i * 4, '1, '1);
            axi_read(i * 4);
        end

        repeat(100) @(posedge axi_ACLK);

        fork
            begin
                for (int i = 0; i < 64; ++i) begin 
                    repeat ($urandom_range(0, 5)) @(posedge axi_ACLK);
                    axi_write($urandom_range(0, 15) * 4, $random(), '1);
                end
            end

            begin
                for (int i = 0; i < 64; ++i) begin 
                    repeat ($urandom_range(0, 5)) @(posedge axi_ACLK);
                    axi_read($urandom_range(0, 15) * 4);
                end
            end
        join

        repeat(5) @(posedge axi_ACLK); $finish;
    end

endmodule 

`endif 