`ifndef BASIC_SYSTEM_SV
    `define BASIC_SYSTEM_SV

`include "CPU/ApogeoRV/Hardware/ApogeoRV.sv"
`include "CPU/ApogeoRV/Hardware/Include/Headers/apogeo_memory_map.svh"
`include "CPU/interrupt_controller.sv"

`include "Memory/on_chip_memory.sv"

`include "Bus/axi_network.sv"

`include "IO/GPIO/gpio.sv"
`include "IO/UART/uart.sv"
`include "IO/Timer/timer.sv"
`include "IO/SPI/spi.sv"

`include "System/synchronizer.sv"

module basic_system #(
    /* Predictor table size */ 
    parameter PREDICTOR_SIZE = 512, 

    /* Branch target buffer cache size */
    parameter BTB_SIZE = 512, 

    /* Store buffer entries number */
    parameter STORE_BUFFER_SIZE = 4, 

    /* Maximum number of instruction held by the buffer */
    parameter INSTRUCTION_BUFFER_SIZE = 8,

    /* System memory bytes */
    parameter MEMORY_SIZE = 2 ** 14,

    /* Boot memory bytes */
    parameter BOOT_SIZE = 2 ** 11,

    /* Reorder Buffer entries */
    parameter ROB_DEPTH = 32,

    /* SPI Slave devices */
    parameter SLAVES = 1
) (
    input logic clk_i,
    input logic rst_n_i,

    /* GPIO pins */
    inout logic [7:0] pin_io,

    /* UART pins */
    input logic rx_i,
    input logic cts_i,
    output logic tx_o,
    output logic rts_o,

    /* SPI pins */
    output logic sclk_o,
    output logic [SLAVES - 1:0] cs_n_o,
    output logic mosi_o,
    input logic miso_i 
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
    ApogeoRV #(PREDICTOR_SIZE, BTB_SIZE, STORE_BUFFER_SIZE, INSTRUCTION_BUFFER_SIZE, ROB_DEPTH) system_cpu (
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


    localparam INTERRUPT_SOURCES = 5;
    
    logic [INTERRUPT_SOURCES - 1:0] interrupt_source;

    interrupt_controller #(INTERRUPT_SOURCES) interrupt_controller (
        .clk_i   ( clk_i   ),
        .rst_n_i ( reset_n ),

        .interrupt_i ( interrupt_source ),

        .acknowledge_i ( int_acknowledge                             ),
        .interrupt_o   ( interrupt                                   ),
        .vector_o      ( int_vector[$clog2(INTERRUPT_SOURCES) - 1:0] )
    );

    assign t_interrupt = (int_vector[$clog2(INTERRUPT_SOURCES) - 1:0] == 2) & interrupt;

    assign nmsk_interrupt = (int_vector[$clog2(INTERRUPT_SOURCES) - 1:0] == 3) & interrupt;

    assign int_vector[7:$clog2(INTERRUPT_SOURCES)] = '0;


//==========================================================
//      AXI INTERCONNECTION
//==========================================================

    localparam NETWORK_DEVICES = 6;

    localparam int LOW_SLAVE_ADDRESS [NETWORK_DEVICES] = '{
        /* Boot ROM */
        `BOOT_START, 

        /* UART */
        `IO_START, 

        /* Timer */
        `IO_START + 'd128,

        /* GPIO */
        `IO_START + 'd256,

        /* SPI */
        `IO_START + 'd384,

        /* Memory */
        `USER_MEMORY_REGION_START
    };

    localparam int HIGH_SLAVE_ADDRESS [NETWORK_DEVICES] = '{
        /* Boot ROM */
        `BOOT_END, 

        /* UART */
        LOW_SLAVE_ADDRESS[1] + (3 << 2), 

        /* Timer */
        LOW_SLAVE_ADDRESS[2] + (4 << 2),

        /* GPIO */
        LOW_SLAVE_ADDRESS[3] + (3 << 2),

        /* SPI */
        LOW_SLAVE_ADDRESS[4] + (4 << 2),

        /* Memory */
        `USER_MEMORY_REGION_END
    };

    /* Master nets */
    logic write_bus_error, read_bus_error; logic [3:0] master_write_strobe; logic write_cts, read_cts;

    /* Slave nets */
    logic [NETWORK_DEVICES - 1:0] write_error, write_done, write_busy, write_ready, write_request; 
    logic [NETWORK_DEVICES - 1:0][31:0] write_address, write_data; logic [NETWORK_DEVICES - 1:0][3:0] write_strobe;

    logic [NETWORK_DEVICES - 1:0] read_error, read_done, read_busy, read_ready, read_request; 
    logic [NETWORK_DEVICES - 1:0][31:0] read_address, read_data;

    axi_network #(NETWORK_DEVICES, LOW_SLAVE_ADDRESS, HIGH_SLAVE_ADDRESS) interconnection (
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

        .read_start_i   ( load_channel.request    ),
        .read_invalid_i ( load_channel.invalidate ),
        .read_address_i ( load_channel.address    ),
        .read_data_o    ( load_channel.data       ),
        .read_done_o    ( load_channel.valid      ),
        .read_cts_o     ( read_cts                ),

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

    assign interrupt_source[INTERRUPT_SOURCES - 1] = write_bus_error | read_bus_error;

        always_comb begin
            /* Default values */
            master_write_strobe = '0;

            case (store_channel.width)
                WORD: begin 
                    master_write_strobe = '1;
                end

                HALF_WORD: begin 
                    master_write_strobe = 2'b11 << ({store_channel.address[0], 1'b0});
                end 

                BYTE: begin
                    master_write_strobe = 1'b1 << {store_channel.address[1:0]};
                end 
            endcase 
        end


//==========================================================
//      UART
//==========================================================

    localparam _UART_ = 1;

    uart #(512, 512) uart_device (
        .clk_i       ( clk_i   ),
        .rst_n_i     ( reset_n ),

        .interrupt_o ( interrupt_source[INTERRUPT_SOURCES - 3] ),

        .uart_rx_i  ( rx_i  ),
        .uart_tx_o  ( tx_o  ),
        .uart_rts_o ( rts_o ),
        .uart_cts_i ( cts_i ),

        .write_i         ( write_request[_UART_]      ),
        .write_address_i ( write_address[_UART_] >> 2 ),
        .write_data_i    ( write_data[_UART_]         ),
        .write_strobe_i  ( write_strobe[_UART_]       ),
        .write_error_o   ( write_error[_UART_]        ),
        .write_done_o    ( write_done[_UART_]         ),

        .read_i         ( read_request[_UART_]      ),
        .read_address_i ( read_address[_UART_] >> 2 ),
        .read_data_o    ( read_data[_UART_]         ),
        .read_error_o   ( read_error[_UART_]        ),
        .read_done_o    ( read_done[_UART_]         )
    );

    assign write_busy[_UART_] = 1'b0;
    assign write_ready[_UART_] = 1'b1;

    assign read_busy[_UART_] = 1'b0;
    assign read_ready[_UART_] = 1'b1;


//==========================================================
//      TIMER
//==========================================================

    localparam _TIMER_ = 2;

    timer timer_device (
        .clk_i   ( clk_i   ),
        .rst_n_i ( reset_n ),

        .write_i         ( write_request[_TIMER_]      ),
        .write_data_i    ( write_data[_TIMER_]         ),
        .write_address_i ( write_address[_TIMER_] >> 2 ),
        .write_error_o   ( write_error[_TIMER_]        ),
        .write_strobe_i  ( write_strobe[_TIMER_]       ),

        .read_i         ( read_request[_TIMER_]      ),
        .read_address_i ( read_address[_TIMER_] >> 2 ),
        .read_data_o    ( read_data[_TIMER_]         ),
        .read_error_o   ( read_error[_TIMER_]        ),

        .interrupt_o ( interrupt_source[INTERRUPT_SOURCES - 2] )
    );

    assign write_busy[_TIMER_] = 1'b0;
    assign write_ready[_TIMER_] = 1'b1;
    assign write_done[_TIMER_] = write_request[_TIMER_];

    assign read_busy[_TIMER_] = 1'b0;
    assign read_ready[_TIMER_] = 1'b1;
    assign read_done[_TIMER_] = read_request[_TIMER_];


//==========================================================
//      GPIO
//==========================================================

    localparam _GPIO_ = 3;

    logic [7:0] gpio_group_interrupt;

    genvar i; generate 
        for (i = 0; i < 8; ++i) begin : gpio_gen
            gpio gpio_device (
                .clk_i      ( clk_i    ),
                .rst_n_i    ( reset_n  ),

                .pin_io ( pin_io[i] ),

                /* Write interface */
                .write_i         ( write_request[_GPIO_]      ),
                .write_address_i ( write_address[_GPIO_] >> 2 ),
                .write_data_i    ( write_data[_GPIO_][i]      ),

                /* Read interface */
                .read_address_i ( read_address[_GPIO_] >> 2 ),
                .read_data_o    ( read_data[_GPIO_][i]      ),

                .interrupt_o ( gpio_group_interrupt[i] )
            );
        end : gpio_gen
    endgenerate

    assign read_data[_GPIO_][31:8] = '0;

    assign interrupt_source[INTERRUPT_SOURCES - 4] = gpio_group_interrupt != '0;

    assign write_busy[_GPIO_] = 1'b0;
    assign write_ready[_GPIO_] = 1'b1;
    assign write_error[_GPIO_] = 1'b0;
    assign write_done[_GPIO_] = write_request[_GPIO_];

    assign read_busy[_GPIO_] = 1'b0;
    assign read_ready[_GPIO_] = 1'b1;
    assign read_error[_GPIO_] = 1'b0;
    assign read_done[_GPIO_] = read_request[_GPIO_];


//==========================================================
//      MEMORY 
//==========================================================

    localparam _MEMORY_ = 5;

    logic sys_memory_fetch, sys_memory_done;
    logic [31:0] sys_memory_instr;

    logic [31:0] store_address, load_address, fetch_address;

    /* Address translation */
    assign store_address = write_address[_MEMORY_] - LOW_SLAVE_ADDRESS[NETWORK_DEVICES - 1];
    assign load_address = read_address[_MEMORY_] - LOW_SLAVE_ADDRESS[NETWORK_DEVICES - 1];
    assign fetch_address = fetch_channel.address - LOW_SLAVE_ADDRESS[NETWORK_DEVICES - 1];
    
    on_chip_memory #(MEMORY_SIZE, "/home/gabbed/Projects/ZenithSoC/Software/Test/program.hex") system_memory (
        .clk_i      ( clk_i    ),
        .rst_n_i    ( reset_n  ),

        .store_i         ( write_request[_MEMORY_] ),
        .store_address_i ( store_address           ),
        .store_data_i    ( write_data[_MEMORY_]    ),
        .store_width_i   ( write_strobe[_MEMORY_]  ),
        .store_done_o    ( write_done[_MEMORY_]    ),

        .load_i         ( read_request[_MEMORY_]  ),
        .load_address_i ( load_address >> 2       ),
        .load_data_o    ( read_data[_MEMORY_]     ),
        .load_done_o    ( read_done[_MEMORY_]     ),
        .load_invalid_i ( load_channel.invalidate ),

        .fetch_i         ( sys_memory_fetch         ),
        .invalidate_i    ( fetch_channel.invalidate ),
        .fetch_address_i ( fetch_address >> 2       ),
        .instruction_o   ( sys_memory_instr         ),
        .fetch_done_o    ( sys_memory_done          )
    );

    assign write_busy[_MEMORY_] = 1'b0;
    assign write_ready[_MEMORY_] = 1'b1;
    assign write_error[_MEMORY_] = 1'b0;

    assign read_busy[_MEMORY_] = 1'b0;
    assign read_ready[_MEMORY_] = 1'b1;
    assign read_error[_MEMORY_] = 1'b0;


//==========================================================
//      SPI
//==========================================================

    localparam _SPI_ = 4;

    spi #(512, 512, SLAVES) spi_device (
        .clk_i       ( clk_i   ),
        .rst_n_i     ( reset_n ),

        .interrupt_o ( interrupt_source[INTERRUPT_SOURCES - 5] ),

        .sclk_o ( sclk_o ),
        .cs_n_o ( cs_n_o ),
        .mosi_o ( mosi_o ),
        .miso_i ( miso_i ),

        .write_i         ( write_request[_SPI_]      ),
        .write_address_i ( write_address[_SPI_] >> 2 ),
        .write_data_i    ( write_data[_SPI_]         ),
        .write_strobe_i  ( write_strobe[_SPI_]       ),
        .write_error_o   ( write_error[_SPI_]        ),
        .write_done_o    ( write_done[_SPI_]         ),

        .read_i         ( read_request[_SPI_]      ),
        .read_address_i ( read_address[_SPI_] >> 2 ),
        .read_data_o    ( read_data[_SPI_]         ),
        .read_error_o   ( read_error[_SPI_]        ),
        .read_done_o    ( read_done[_SPI_]         )
    );

    assign write_busy[_SPI_] = 1'b0;
    assign write_ready[_SPI_] = 1'b1;

    assign read_busy[_SPI_] = 1'b0;
    assign read_ready[_SPI_] = 1'b1;


//==========================================================
//      BOOT RAM
//==========================================================

    localparam _BOOT_ = 0;

    logic boot_rom_fetch, boot_rom_done;
    logic [31:0] boot_rom_instr;

    on_chip_memory #(BOOT_SIZE, "/home/gabbed/Projects/ZenithSoC/Software/Test/boot.hex") boot_memory (
        .clk_i      ( clk_i    ),
        .rst_n_i    ( reset_n  ),

        .store_i         ( write_request[_BOOT_] ),
        .store_address_i ( write_address[_BOOT_] ),
        .store_data_i    ( write_data[_BOOT_]    ),
        .store_width_i   ( write_strobe[_BOOT_]  ),
        .store_done_o    ( write_done[_BOOT_]    ),

        .load_i         ( read_request[_BOOT_]      ),
        .load_address_i ( read_address[_BOOT_] >> 2 ),
        .load_data_o    ( read_data[_BOOT_]         ),
        .load_done_o    ( read_done[_BOOT_]         ),
        .load_invalid_i ( load_channel.invalidate ),

        .fetch_i         ( boot_rom_fetch             ),
        .invalidate_i    ( fetch_channel.invalidate   ),
        .fetch_address_i ( fetch_channel.address >> 2 ),
        .instruction_o   ( boot_rom_instr             ),
        .fetch_done_o    ( boot_rom_done              )
    );

    assign write_busy[_BOOT_] = 1'b0;
    assign write_ready[_BOOT_] = 1'b1;
    assign write_error[_BOOT_] = 1'b0;

    assign read_busy[_BOOT_] = 1'b0;
    assign read_ready[_BOOT_] = 1'b1;
    assign read_error[_BOOT_] = 1'b0;


    /* Decode Boot ROM / System Memory signals */
    assign boot_rom_fetch = (fetch_channel.address <= `BOOT_END) & fetch_channel.fetch; 
    assign sys_memory_fetch = (fetch_channel.address >= `USER_MEMORY_REGION_START) & fetch_channel.fetch; 

    assign fetch_channel.valid = boot_rom_done | sys_memory_done;

        always_comb begin
            /* Default value */
            fetch_channel.instruction = '0;

            case ({boot_rom_done, sys_memory_done})
                2'b01: fetch_channel.instruction = sys_memory_instr;

                2'b10: fetch_channel.instruction = boot_rom_instr;
            endcase 
        end

endmodule : basic_system

`endif 