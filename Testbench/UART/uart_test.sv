`ifndef UART_TEST_SV
    `define UART_TEST_SV

module uart_test ();

    /* Global signals */
    logic clk_i = 1;
    logic rst_n_i = 1;
    logic uart_clk_i = 1;
    logic uart_rstn_i = 1;
    logic interrupt_o;

    /* UART interface */
    logic uart_rx_i;
    logic uart_tx_o;
    logic uart_rts_o;
    logic uart_cts_i = 1;

    /* Write interface */
    logic write_i = 0;
    uart_registers_t write_address_i = STATUS;
    logic [31:0] write_data_i = 0;
    logic write_error_o;
    logic write_done_o;

    /* Read interface */
    logic read_i = 0;
    uart_registers_t read_address_i = STATUS;
    logic [31:0] read_data_o;
    logic read_error_o;
    logic read_done_o;


    uart #(8, 8) dut (.*); 

    always #5 clk_i <= !clk_i;

    assign uart_rx_i = uart_tx_o;


    task write_status(input logic [31:0] data);
        write_address_i <= STATUS;
        write_data_i <= data;
        write_i <= 1'b1;
        @(posedge clk_i);
        write_i <= 1'b0;
    endtask 

    task send_data(input logic [7:0] data);
        write_address_i <= TX_BUFFER;
        write_data_i <= {'0, data};
        write_i <= 1'b1;
        @(posedge clk_i);
        write_i <= 1'b0;
    endtask 
    
    task write_event(input logic [4:0] data);
        write_address_i <= EVENT;
        write_data_i <= {'0, data};
        write_i <= 1'b1;
        @(posedge clk_i);
        write_i <= 1'b0;
    endtask 

    task read_status();
        read_address_i <= STATUS;
        read_i <= 1'b1;
        @(posedge clk_i);
        read_i <= 1'b0;
    endtask 

    task read_data();
        read_address_i <= RX_BUFFER;
        read_i <= 1'b1;
        @(posedge clk_i);
        read_i <= 1'b0;
    endtask 

    task read_cause();
        read_address_i <= EVENT;
        read_i <= 1'b1;
        @(posedge clk_i);
        read_i <= 1'b0;
    endtask


    uart_status_t status;

    initial begin
        status <= '0;
        rst_n_i <= '0;
        @(posedge clk_i);
        rst_n_i <= '1;

        @(posedge clk_i);

        read_status();
        read_cause();

        status.interrupt_enable <= 5'b00001;
        status.clock_divider <= 'd53;
        status.parity_enable <= 1'b1;

        @(posedge clk_i);

        write_status({status, 4'b0});

        fork 
            begin 
                for (int i = 0; i < 8; ++i) begin
                    send_data(i);
                end
            end

            begin 
                for (int i = 0; i < 8; ++i) begin
                    wait (interrupt_o);
                    write_event('0);
                    @(posedge clk_i);
                    read_data();
                end
            end 
        join
        
        for (int i = 0; i < 8; ++i) begin
            send_data(i);
        end
        
        for (int i = 0; i < 8; ++i) begin
            wait (interrupt_o);
            write_event('0);
            @(posedge clk_i);
            read_status();
        end     
        
        for (int i = 0; i < 8; ++i) begin
            @(posedge clk_i);
            read_data();
        end       
        
        read_status();
        repeat(5) @(posedge clk_i);
        
        
        $finish;
    end

endmodule : uart_test 

`endif 