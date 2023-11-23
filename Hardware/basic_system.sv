`ifndef BASIC_SYSTEM_SV
    `define BASIC_SYSTEM_SV

`include "CPU/ApogeoRV/Hardware/ApogeoRV.sv"
`include "CPU/ApogeoRV/Hardware/Include/Headers/apogeo_memory_map.svh"
`include "CPU/interrupt_controller.sv"
`include "CPU/on_chip_memory.sv"

`include "Bus/axi_network.sv"

`include "IO/GPIO/gpio.sv"
`include "IO/UART/uart.sv"
`include "IO/Timer/timer.sv"

`include "System/synchronizer.sv"
`include "System/clock_divider.sv"

module basic_system #(
    parameter MEMORY_SIZE = 4096
) (
    input logic clk_i,
    input logic rst_n_i,

    /* GPIO pins */
    inout logic [31:0] pin_io,

    /* UART pins */
    input logic rx_i,
    input logic cts_i,
    output logic tx_o,
    output logic rts_o
);

    /* Reset syncronizer */
    logic reset_n, rst_sync;

        always_ff @(posedge clk_i or negedge rst_n_i) begin 
            if (!rst_n_i) begin
                rst_sync <= 1'b0;
                reset_n <= 1'b0;
            end else begin 
                rst_sync <= rst_n_i;
                reset_n <= rst_sync;
            end 
        end 


//==========================================================
//      CPU SYSTEM
//==========================================================

    /* Channels */
    fetch_interface fetch_channel(); 
    load_interface load_channel();
    store_interface store_channel();
    
    /* Interrupt nets */
    logic interrupt, nmsk_interrupt, t_interrupt, int_acknowledge;
    logic [7:0] int_vector;

    /* Core instantiation */
    ApogeoRV #(1024, 1024, 4, 8) system_cpu (
        .clk_i   ( clk_i   ),
        .rst_n_i ( reset_n ),

        .fetch_channel ( fetch_channel ), 

        .interrupt_i        ( interrupt       ), 
        .non_maskable_int_i ( 1'b0            ),
        .timer_interrupt_i  ( t_interrupt     ), 
        .interrupt_vector_i ( int_vector      ),
        .interrupt_ackn_o   ( int_acknowledge ),

        .load_channel  ( load_channel  ), 
        .store_channel ( store_channel )
    );


    logic uart_interrupt, timer_interrupt, gpio_interrupt, bus_error;

    interrupt_controller #(4) interrupt_controller (
        .clk_i   ( clk_i   ),
        .rst_n_i ( reset_n ),

        .interrupt_i ( {bus_error, timer_interrupt, uart_interrupt, gpio_interrupt} ),

        .acknowledge_i ( int_acknowledge ),
        .interrupt_o   ( interrupt       ),
        .vector_o      ( int_vector      )
    );

    assign t_interrupt = (int_vector == 2) & interrupt;

    assign nmsk_interrupt = (int_vector == 3) & interrupt;


//==========================================================
//      AXI INTERCONNECTION
//==========================================================

    localparam int LOW_SLAVE_ADDRESS [4] = '{
        /* UART */
        `IO_START, 

        /* Timer */
        `IO_START + 4,

        /* GPIO */
        `IO_START + 8,

        /* Memory */
        `USER_MEMORY_REGION_START
    };

    localparam int HIGH_SLAVE_ADDRESS [4] = '{
        /* UART */
        `IO_START + 3, 

        /* Timer */
        `IO_START + 7,

        /* GPIO */
        `IO_START + 11,

        /* Memory */
        '1
    };

    /* Master nets */
    logic write_bus_error, read_bus_error; logic [3:0] master_write_strobe; logic write_cts, read_cts;

    /* Slave nets */
    logic [3:0] write_error, write_done, write_busy, write_ready, write_request; 
    logic [3:0][31:0] write_address, write_data; logic [3:0][3:0] write_strobe;

    logic [3:0] read_error, read_done, read_busy, read_ready, read_request; 
    logic [3:0][31:0] read_address, read_data;

    axi_network #(4, LOW_SLAVE_ADDRESS, HIGH_SLAVE_ADDRESS) interconnection (
        .axi_ACLK    ( clk_i   ),
        .axi_ARESETN ( reset_n ),

        .axi_write_error_o ( write_bus_error ),
        .axi_read_error_o  ( read_bus_error  ),

        .write_start_i   ( store_channel.request ),
        .write_address_i ( store_channel.address ),
        .write_data_i    ( store_channel.data    ),
        .write_strobe_i  ( master_write_strobe   ),
        .write_done_o    ( store_channel.done    ),
        .write_cts_o     ( write_cts             ),

        .read_start_i   ( load_channel.request ),
        .read_address_i ( load_channel.address ),
        .read_data_o    ( load_channel.data    ),
        .read_done_o    ( load_channel.valid   ),
        .read_cts_o     ( read_cts             ),

        .write_error_i   ( write_error   ),
        .write_done_i    ( write_done    ),
        .write_busy_i    ( write_busy    ),
        .write_ready_i   ( write_ready   ),
        .write_address_o ( write_address ),
        .write_data_o    ( write_data    ),
        .write_strobe_o  ( write_strobe  ),
        .write_request_o ( write_request ),

        .read_data_i    ( read_data    ),
        .read_error_i   ( read_error   ),
        .read_done_i    ( read_done    ),
        .read_busy_i    ( read_busy    ),
        .read_ready_i   ( read_ready   ),
        .read_address_o ( read_address ),
        .read_request_o ( read_request )
    );

    assign bus_error = write_bus_error | read_bus_error;


        always_comb begin
            master_write_strobe = '0;

            case (store_channel.width)
                WORD: master_write_strobe = '1;

                HALF_WORD: master_write_strobe = 2'b11 << ({store_channel.address[0], 1'b0});

                BYTE: master_write_strobe = 1'b1 << {store_channel.address[1:0]};
            endcase 
        end


//==========================================================
//      UART
//==========================================================

    localparam _UART_ = 0;

    uart #(512, 512) uart_device (
        .clk_i       ( clk_i   ),
        .rst_n_i     ( reset_n ),

        .interrupt_o ( uart_interrupt ),

        .uart_rx_i  ( rx_i  ),
        .uart_tx_o  ( tx_o  ),
        .uart_rts_o ( rts_o ),
        .uart_cts_i ( cts_i ),

        .write_i         ( write_request[_UART_] ),
        .write_address_i ( write_address[_UART_] ),
        .write_data_i    ( write_data[_UART_]    ),
        .write_error_o   ( write_error[_UART_]   ),
        .write_done_o    ( write_done[_UART_]    ),

        .read_i         ( read_request[_UART_] ),
        .read_address_i ( read_address[_UART_] ),
        .read_data_o    ( read_data[_UART_]    ),
        .read_error_o   ( read_error[_UART_]   ),
        .read_done_o    ( read_done[_UART_]    )
    );

    assign write_busy[_UART_] = 1'b0;
    assign write_ready[_UART_] = 1'b1;
    assign write_strobe[_UART_] = '1;

    assign read_busy[_UART_] = 1'b0;
    assign read_ready[_UART_] = 1'b0;


//==========================================================
//      TIMER
//==========================================================

    localparam _TIMER_ = 1;

    timer timer_device (
        .clk_i   ( clk_i   ),
        .rst_n_i ( reset_n ),

        .write_i         ( write_request[_TIMER_] ),
        .write_data_i    ( write_data[_TIMER_]    ),
        .write_address_i ( write_address[_TIMER_] ),

        .read_i         ( read_request[_TIMER_] ),
        .read_address_i ( read_address[_TIMER_] ),
        .read_data_o    ( read_data[_TIMER_]    ),

        .interrupt_o ( timer_interrupt )
    );

    assign write_busy[_TIMER_] = 1'b0;
    assign write_ready[_TIMER_] = 1'b1;
    assign write_error[_TIMER_] = 1'b0;
    assign write_strobe[_TIMER_] = '1;
    assign write_done[_TIMER_] = write_request[_TIMER_];

    assign read_busy[_TIMER_] = 1'b0;
    assign read_ready[_TIMER_] = 1'b0;
    assign read_error[_TIMER_] = 1'b0;
    assign read_done[_TIMER_] = read_request[_TIMER_];


//==========================================================
//      GPIO
//==========================================================

    logic debounce;

    clock_divider #(62500) debouncer_clock (
        .clk_i   ( clk_i   ),
        .rst_n_i ( reset_n ),

        .tick_o ( debounce )
    );


    localparam _GPIO_ = 2;

    logic [31:0] gpio_group_interrupt;

    genvar i; generate 
        for (i = 0; i < 32; ++i) begin
            gpio gpio_device (
                .clk_i      ( clk_i    ),
                .rst_n_i    ( reset_n  ),
                .debounce_i ( debounce ),

                .pin_io ( pin_io[i] ),

                /* Write interface */
                .write_i         ( write_request[_GPIO_] ),
                .write_address_i ( write_address[_GPIO_] ),
                .write_data_i    ( write_data[_GPIO_][i] ),

                /* Read interface */
                .read_address_i ( read_address[_GPIO_] ),
                .read_data_o    ( read_data[_GPIO_][i] ),

                .interrupt_o ( gpio_group_interrupt[i] )
            );
        end
    endgenerate

    assign gpio_interrupt = gpio_group_interrupt != '0;

    assign write_busy[_GPIO_] = 1'b0;
    assign write_ready[_GPIO_] = 1'b1;
    assign write_error[_GPIO_] = 1'b0;
    assign write_strobe[_GPIO_] = '1;
    assign write_done[_GPIO_] = write_request[_GPIO_];

    assign read_busy[_GPIO_] = 1'b0;
    assign read_ready[_GPIO_] = 1'b0;
    assign read_error[_GPIO_] = 1'b0;
    assign read_done[_GPIO_] = read_request[_GPIO_];


//==========================================================
//      MEMORY 
//==========================================================

    localparam _MEMORY_ = 3;

    on_chip_memory #(MEMORY_SIZE) system_memory (
        .clk_i      ( clk_i    ),
        .rst_n_i    ( reset_n  ),

        .store_i         ( write_request[_MEMORY_] ),
        .store_address_i ( write_address[_MEMORY_] ),
        .store_data_i    ( write_data[_MEMORY_]    ),
        .store_width_i   ( write_strobe[_MEMORY_]  ),
        .store_done_o    ( write_done[_MEMORY_]    ),

        .load_i         ( read_request[_MEMORY_] ),
        .load_address_i ( read_address[_MEMORY_] ),
        .load_data_o    ( read_data[_MEMORY_]    ),
        .load_done_o    ( read_done[_MEMORY_]    ),

        .fetch_channel ( fetch_channel ) 
    );

    assign write_busy[_MEMORY_] = 1'b0;
    assign write_ready[_MEMORY_] = 1'b1;
    assign write_error[_MEMORY_] = 1'b0;

    assign read_busy[_GPIO_] = 1'b0;
    assign read_ready[_GPIO_] = 1'b0;
    assign read_error[_GPIO_] = 1'b0;

endmodule : basic_system

`endif 