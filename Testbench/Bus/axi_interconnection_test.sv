`ifndef AXI_INTERCONNECTION_TEST_SV
    `define AXI_INTERCONNECTION_TEST_SV

`include "../../Hardware/Bus/axi_master.sv"
`include "../../Hardware/Bus/axi_slave.sv"
`include "../../Hardware/Bus/axi_router.sv"

`include "../../Hardware/Utility/Interfaces/axi_read_interface.sv"
`include "../../Hardware/Utility/Interfaces/axi_write_interface.sv"

`include "../../Hardware/Utility/Packages/axi_interface_pkg.sv"

module axi_interconnection_test ();

    localparam AXI_MASTER_NUMBER = 1;
    localparam AXI_SLAVE_NUMBER = 2;

    genvar i;

    /* Global signals */
    logic axi_ACLK = 0; 
    logic axi_ARESETN = 1; 

    always #5 axi_ACLK <= !axi_ACLK;

    axi_write_interface master_write_channel[AXI_MASTER_NUMBER - 1:0](); 
    axi_read_interface master_read_channel[AXI_MASTER_NUMBER - 1:0](); 
    axi_write_interface slave_write_channel[AXI_SLAVE_NUMBER - 1:0]();
    axi_read_interface slave_read_channel[AXI_SLAVE_NUMBER - 1:0]();

    /* Master nets */
    logic [AXI_MASTER_NUMBER - 1:0][31:0] master_write_address_i = 0;
    logic [AXI_MASTER_NUMBER - 1:0][31:0] master_write_data_i = 0;
    logic [AXI_MASTER_NUMBER - 1:0][3:0] master_write_strobe_i = 0;
    logic [AXI_MASTER_NUMBER - 1:0] master_write_start_i = 0;
    logic [AXI_MASTER_NUMBER - 1:0] master_write_done_o;
    logic [AXI_MASTER_NUMBER - 1:0] master_write_cts_o;
    logic [AXI_MASTER_NUMBER - 1:0][31:0] master_read_address_i = 0;
    logic [AXI_MASTER_NUMBER - 1:0] master_read_start_i = 0;
    logic [AXI_MASTER_NUMBER - 1:0][31:0] master_read_data_o;
    logic [AXI_MASTER_NUMBER - 1:0] master_read_done_o;
    logic [AXI_MASTER_NUMBER - 1:0] master_read_cts_o;
    axi_response_t [AXI_MASTER_NUMBER - 1:0] master_write_response_o;
    axi_response_t [AXI_MASTER_NUMBER - 1:0] master_read_response_o;

    generate
        for (i = 0; i < AXI_MASTER_NUMBER; ++i) begin
            axi_master master (
                .axi_ACLK    ( axi_ACLK    ),
                .axi_ARESETN ( axi_ARESETN ),

                .write_channel ( master_write_channel[i] ), 

                .write_address_i ( master_write_address_i[i] ),
                .write_data_i    ( master_write_data_i[i]    ),
                .write_strobe_i  ( master_write_strobe_i[i]  ),
                .write_start_i   ( master_write_start_i[i]   ),
                .write_done_o    ( master_write_done_o[i]    ),
                .write_cts_o     ( master_write_cts_o[i]     ),

                .read_channel ( master_read_channel[i] ), 

                .read_address_i ( master_read_address_i ),
                .read_start_i   ( master_read_start_i   ),
                .read_data_o    ( master_read_data_o    ),
                .read_done_o    ( master_read_done_o    ),
                .read_cts_o     ( master_read_cts_o     ),

                .write_response_o ( master_write_response_o ),
                .read_response_o  ( master_read_response_o  )
            );
        end
    endgenerate


    /* Slave nets */
    logic [AXI_SLAVE_NUMBER - 1:0] slave_write_match_o;
    logic [AXI_SLAVE_NUMBER - 1:0] slave_read_match_o;
    logic [AXI_SLAVE_NUMBER - 1:0] slave_write_error_i = 0;
    logic [AXI_SLAVE_NUMBER - 1:0] slave_write_done_i = 0;
    logic [AXI_SLAVE_NUMBER - 1:0][31:0] slave_write_address_o;
    logic [AXI_SLAVE_NUMBER - 1:0][31:0] slave_write_data_o;
    logic [AXI_SLAVE_NUMBER - 1:0][3:0] slave_write_strobe_o;
    logic [AXI_SLAVE_NUMBER - 1:0] slave_write_request_o;
    logic [AXI_SLAVE_NUMBER - 1:0][31:0] slave_read_data_i = 0;
    logic [AXI_SLAVE_NUMBER - 1:0] slave_read_done_i = 0;
    logic [AXI_SLAVE_NUMBER - 1:0] slave_read_error_i = 0;
    logic [AXI_SLAVE_NUMBER - 1:0][31:0] slave_read_address_o;
    logic [AXI_SLAVE_NUMBER - 1:0] slave_read_request_o;
    logic [AXI_SLAVE_NUMBER - 1:0] slave_stop_write_i = 0;
    logic [AXI_SLAVE_NUMBER - 1:0] slave_stop_read_i = 0;

    generate
        for (i = 0; i < AXI_SLAVE_NUMBER; ++i) begin
            axi_slave #(i * 32, ((i + 1) * 32) - 1) slave (
                .axi_ACLK    ( axi_ACLK    ),
                .axi_ARESETN ( axi_ARESETN ),

                .write_match_o ( slave_write_match_o[i] ),
                .read_match_o  ( slave_read_match_o[i]  ),

                .write_channel ( slave_write_channel[i] ),

                .write_error_i   ( slave_write_error_i[i]   ),
                .write_done_i    ( slave_write_done_i[i]    ),
                .write_address_o ( slave_write_address_o[i] ),
                .write_data_o    ( slave_write_data_o[i]    ),
                .write_strobe_o  ( slave_write_strobe_o[i]  ),
                .write_request_o ( slave_write_request_o[i] ),

                .read_channel ( slave_read_channel[i] ),

                .read_data_i    ( slave_read_data_i[i]    ),
                .read_done_i    ( slave_read_done_i[i]   ),
                .read_error_i   ( slave_read_error_i[i]   ),
                .read_address_o ( slave_read_address_o[i] ),
                .read_request_o ( slave_read_request_o[i] ),

                .stop_write_i ( slave_stop_write_i[i] ),
                .stop_read_i  ( slave_stop_read_i[i]  )
            );

            slave_memory #(8) mem (
                .clk_i   ( axi_ACLK ), 
                .rst_n_i ( axi_ARESETN ),
                
                .load_address_i ( slave_read_address_o[i] ),
                .load_i         ( slave_read_request_o[i] ),
                .load_data_o    ( slave_read_data_i[i]    ),
                .load_done_o    ( slave_read_done_i[i]    ),
                
                .store_address_i ( slave_write_address_o[i] ),
                .store_data_i    ( slave_write_data_o[i]    ),
                .store_strobe_i  ( slave_write_strobe_o[i]  ),
                .store_i         ( slave_write_request_o[i] ),
                .store_done_o    ( slave_write_done_i[i]    )
            ); 
        end
    endgenerate


    axi_router #(AXI_MASTER_NUMBER, AXI_SLAVE_NUMBER, 8, 8) dut (
        .clk_i   ( axi_ACLK ), 
        .rst_n_i ( axi_ARESETN ),

        .master_write_channel ( master_write_channel ),
        .master_read_channel  ( master_read_channel  ),

        .slave_write_channel ( slave_write_channel ),
        .slave_read_channel  ( slave_read_channel  ),

        .slave_write_match_i ( slave_write_match_o ),
        .slave_read_match_i  ( slave_read_match_o  )
    );

    task start_write_transaction(input int master, input logic [31:0] address, input logic [31:0] data, input logic [3:0] strobe);
        master_write_address_i[master] <= address;
        master_write_data_i[master] <= data;
        master_write_strobe_i[master] <= strobe;
        master_write_start_i[master] <= 1'b1;
        @(posedge axi_ACLK);
        master_write_start_i[master] <= 1'b0;
    endtask 
    
    task start_read_transaction(input int master, input logic [31:0] address);
        master_read_address_i[master] <= address;
        master_read_start_i[master] <= 1'b1;
        @(posedge axi_ACLK);
        master_read_start_i[master] <= 1'b0;
    endtask 

    /* 1. Start by reading the initial values from slaves, overwrite them and
     * read them again. Test read / write transactions from one slave to another.
     *
     * 2. Then initiate read and write transactions back to back to a single slave, switch slave.
     * Read again to check result correctness.
     * 
     * 3. Do again but write before read. 
     * 
     * 4. Read and write at the same time to the same slave to the same address */
    task basic_test_case();
        /* 1 Step */
        for (int i = 0; i < 8 * AXI_SLAVE_NUMBER; ++i) begin
            start_read_transaction(0, i * 4);
        end
        
        for (int i = 0; i < 8 * AXI_SLAVE_NUMBER; ++i) begin
            start_write_transaction(0, i * 4, '1, '1);
        end

        for (int i = 0; i < 8 * AXI_SLAVE_NUMBER; ++i) begin
            start_read_transaction(0, i * 4);
        end

        /* 2 Step */
        for (int i = 0; i < 8 * AXI_SLAVE_NUMBER; ++i) begin
            /* Back to back read / write */
            start_read_transaction(0, i * 4);
            start_write_transaction(0, i * 4, i, '1);
        end

        for (int i = 0; i < 8 * AXI_SLAVE_NUMBER; ++i) begin
            start_read_transaction(0, i * 4);
        end

        /* 3 Step */
        for (int i = 0; i < 8 * AXI_SLAVE_NUMBER; ++i) begin
            start_write_transaction(0, i * 4, i, '1);
            start_read_transaction(0, i * 4);
        end

        for (int i = 0; i < 8 * AXI_SLAVE_NUMBER; ++i) begin
            start_read_transaction(0, i * 4);
        end

        /* 4 Step */
        for (int i = 0; i < 8 * AXI_SLAVE_NUMBER; ++i) begin
            master_write_address_i[0] <= i * 4;
            master_write_data_i[0] <= 32'hDEADBEEF;
            master_write_strobe_i[0] <= '1;
            master_write_start_i[0] <= 1'b1;

            master_read_address_i[0] <= i * 4;
            master_read_start_i[0] <= 1'b1;
            @(posedge axi_ACLK);
        end

        master_read_start_i[0] <= 1'b0;
        master_write_start_i[0] <= 1'b0;
    endtask

    initial begin
        axi_ARESETN <= '0;
        @(posedge axi_ACLK);
        axi_ARESETN <= '1;
        
        basic_test_case();

        repeat(5) @(posedge axi_ACLK);
        $finish;
    end

endmodule : axi_interconnection_test

module slave_memory #(parameter MEMORY_SIZE = 32) (
    input clk_i, rst_n_i,
    
    input logic [31:0] load_address_i,
    input logic load_i,
    output logic [31:0] load_data_o,
    output logic load_done_o,
    
    input logic [31:0] store_address_i,
    input logic [3:0][7:0] store_data_i,
    input logic [3:0] store_strobe_i,
    input logic store_i,
    output logic store_done_o
); 

    logic [3:0][7:0] memory [MEMORY_SIZE];
    
    initial begin
       for (int i = 0; i < MEMORY_SIZE; ++i) begin 
           memory[i] = i;
       end 
    end

        always_ff @(posedge clk_i) begin
            if (!rst_n_i) begin 
                load_done_o <= 1'b0;
                store_done_o <= 1'b0;
            end else begin 
                load_done_o <= load_i;
                store_done_o <= store_i;
            end 
        end
        
        
        always_ff @(posedge clk_i) begin
            if (load_i) begin 
                load_data_o <= memory[load_address_i[$clog2(MEMORY_SIZE) + 1:2]];
            end 
            
            if (store_i) begin 
                for (int i = 0; i < 4; ++i) begin 
                    if (store_strobe_i[i]) begin 
                        memory[store_address_i[$clog2(MEMORY_SIZE) + 1:2]][i] <= store_data_i[i];
                    end 
                end 
            end 
        end
        
endmodule 

`endif 