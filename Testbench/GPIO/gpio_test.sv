`ifndef GPIO_TEST_SV
    `define GPIO_TEST_SV

module gpio_test();

    /* Global signals */
    logic clk_i = 1;
    logic rst_n_i = 0;
    logic debounce_i;

    /* SoC external pin */
    wire pin_io;

    /* Write interface */
    logic write_i = 0;
    logic [1:0] write_address_i = 0;
    logic write_data_i = 0;

    /* Read interface */
    logic [1:0] read_address_i = 0;
    logic read_data_o;

    logic interrupt_o;

    gpio dut (.*);

    clock_divider #(62500) clk_div (
        .clk_i   ( clk_i   ),
        .rst_n_i ( rst_n_i ),

        .tick_o ( debounce_i )
    );

    always #5 clk_i <= !clk_i;


    logic pin; assign pin_io = pin;

    initial begin
        @(posedge clk_i);
        rst_n_i <= 1'b1;
        pin <= 1'bZ;

        /* Output mode */
        write_i <= 1'b1;
        write_address_i <= 'd1;
        write_data_i <= 1'b0;
        @(posedge clk_i);

        /* Drive 1 - 0 sequence */
        for (int i = 0; i < 10; ++i) begin
            write_i <= 1'b1;
            write_address_i <= 'd0;
            write_data_i <= ((i % 2) == '0);
            @(posedge clk_i);

            read_address_i <= 'd0;
            @(posedge clk_i);
        end

        /* Input mode */
        write_i <= 1'b1;
        write_address_i <= 'd1;
        write_data_i <= 1'b1;
        @(posedge clk_i);

        /* Enable interrupt */
        write_i <= 1'b1;
        write_address_i <= 'd2;
        write_data_i <= 1'b1;
        pin <= 1'b0;
        @(posedge clk_i);

        repeat(50) @(posedge clk_i); 

        for (int i = 0; i < 10000; ++i) begin
            pin <= !pin;
            @(posedge clk_i); 
        end

        pin <= 1'b1;
        repeat (1000000) @(posedge clk_i); 

        read_address_i <= 'd0;
        @(posedge clk_i);

        $finish;
    end

endmodule : gpio_test 

`endif 