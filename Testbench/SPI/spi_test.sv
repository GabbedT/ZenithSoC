`ifndef SPI_TEST_SV
    `define SPI_TEST_SV

module spi_test();

    localparam RX_BUFFER_SIZE = 16;
    localparam TX_BUFFER_SIZE = 16;
    localparam SLAVES = 1;

    /* Global signals */
    logic clk_i = 1;
    logic rst_n_i = 0;
    logic interrupt_o;

    /* SPI interface */
    logic sclk_o;
    logic [SLAVES - 1:0] cs_n_o;
    logic mosi_o;
    logic miso_i; 

    /* Write interface */
    logic write_i = 0;
    spi_registers_t write_address_i = SPI_STATUS;
    logic [3:0][7:0] write_data_i = 0;
    logic [3:0] write_strobe_i = '1;
    logic write_error_o;

    /* Read interface */
    logic read_i = 0;
    spi_registers_t read_address_i = SPI_STATUS;
    logic [31:0] read_data_o;
    logic read_error_o;

    /* Clock */
    always #5 clk_i <= !clk_i;

    assign miso_i = mosi_o;

    spi #(RX_BUFFER_SIZE, TX_BUFFER_SIZE, SLAVES) dut ( .* );




    task write_status(input logic [31:0] data);
        write_address_i <= SPI_STATUS;
        write_data_i <= data;
        write_i <= 1'b1;
        @(posedge clk_i);
        write_i <= 1'b0;
    endtask 

    task send_data(input logic [7:0] data);
        write_address_i <= SPI_TX_BUFFER;
        write_data_i <= {'0, data};
        write_i <= 1'b1;
        @(posedge clk_i);
        write_i <= 1'b0;
    endtask 
    
    task write_event(input logic [4:0] data);
        write_address_i <= SPI_EVENT;
        write_data_i <= {'0, data};
        write_i <= 1'b1;
        @(posedge clk_i);
        write_i <= 1'b0;
    endtask 

    task write_slave_select(input logic [SLAVES - 1:0] slv_sel);
        write_address_i <= SPI_SLAVE_SELECT;
        write_data_i <= {'0, slv_sel};
        write_i <= 1'b1;
        @(posedge clk_i);
        write_i <= 1'b0;
    endtask

    task read_slave_select();
        read_address_i <= SPI_SLAVE_SELECT;
        read_i <= 1'b1;
        @(posedge clk_i);
        read_i <= 1'b0;
    endtask 

    task read_status(output logic [31:0] status);
        read_address_i <= SPI_STATUS;
        read_i <= 1'b1;
        status <= read_data_o;
        @(posedge clk_i);
        read_i <= 1'b0;
    endtask 

    task read_data();
        read_address_i <= SPI_RX_BUFFER;
        read_i <= 1'b1;
        @(posedge clk_i);
        read_i <= 1'b0;
    endtask 

    task read_cause();
        read_address_i <= SPI_EVENT;
        read_i <= 1'b1;
        @(posedge clk_i);
        read_i <= 1'b0;
    endtask


    task test_mode(input bit cpol, input bit cpha, input int scale);
        read_status(status);
        read_cause();

        status.cpol <= cpol;
        status.cpha <= cpha;
        status.interrupt_enable <= 4'b0001;
        status.clock_divisor <= 'd4;
        status.enable_TX <= 1'b1;
        status.enable_RX <= 1'b1;
        @(posedge clk_i);

        write_status({'0, status});

        repeat(10) @(posedge clk_i);

        write_slave_select(1);

        fork 
            begin 
                for (int i = 0; i < 8; ++i) begin
                    send_data(i + scale);
                end
            end

            begin 
                wait (interrupt_o);
                write_event('0);
                @(posedge clk_i);

                for (int i = 0; i < 8; ++i) begin
                    read_data();
                end
            end 
        join
    endtask : test_mode 

    spi_status_t status;

    initial begin
        status <= '0;
        rst_n_i <= '0;
        @(posedge clk_i);
        rst_n_i <= '1;

        @(posedge clk_i);
        test_mode(0, 0, int'("A"));
        @(posedge clk_i);
        test_mode(0, 1, int'("B"));
        @(posedge clk_i);
        test_mode(1, 0, int'("C"));
        @(posedge clk_i);
        test_mode(1, 1, int'("D"));
        @(posedge clk_i);
        
        for (int i = 0; i < 8; ++i) begin
            send_data(i);
        end
        
        wait (interrupt_o);
        write_event('0);

        for (int i = 0; i < 8; ++i) begin
            read_status(status);
        end     
        
        for (int i = 0; i < 8; ++i) begin
            @(posedge clk_i);
            read_data();
        end       
        
        read_status(status);
        repeat(100) @(posedge clk_i);

        $finish;
    end

endmodule : spi_test 

`endif 