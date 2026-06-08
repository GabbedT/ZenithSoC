`ifndef SD_SV 
    `define SD_SV 

`include "sd_registers.sv"
`include "sd_data_controller.sv"
`include "sd_command_controller.sv"

`include "../../System/edge_detector.sv"
`include "../../System/synchronizer.sv"

`include "../../Utility/Packages/sd_pkg.sv"

module sd (
    input logic clk_i,
    input logic rst_n_i,

    /* IRQ Output */
    output logic interrupt_o,

    /* Write register interface */
    input sd_registers_t write_address_i,
    input logic write_i,
    input logic [3:0][7:0] write_data_i,
    input logic [3:0] write_strobe_i,
    output logic write_done_o,
    output logic write_error_o,

    /* Read register interface */
    input sd_registers_t read_address_i,
    input logic read_i,
    output logic read_done_o,
    output logic [31:0] read_data_o,
    output logic read_error_o,

    /* PHY Interface */
    input logic sd_cd_n_i,
    inout logic sd_cmd_io,
    inout logic [3:0] sd_data_io,
    output logic sd_reset_n_o,
    output logic sd_clk_o
);

//====================================================================================
//      SYNCHRONIZATION
//====================================================================================

    logic cd_n_sync;

    synchronizer #(
        .FLOP_NUMBER ( 2 ),
        .RESET_VALUE ( 0 )
    ) card_detect_synchronizer (
        /* Global signals */
        .clk_i   ( clk_i   ),
        .rst_n_i ( rst_n_i ),

        /* Sync signal */
        .signal_i ( sd_cd_n_i ),
        .sync_o   ( cd_n_sync )
    );


//====================================================================================
//      REGISTER INTERFACE
//====================================================================================

    /* Interconnect wires */
    logic sd_card_detect; 
    logic sd_reset; 
    logic enable;
    logic clock_speed;
    logic bus_width;
    logic reset_done;

    /* Command signals */
    logic [5:0] cmd_number;
    logic [31:0] cmd_argument;
    logic send_command;
    logic cmd_idle;
    logic cmd_crc_error, cmd_timeout, cmd_crc_error_ff, cmd_timeout_ff;
    logic [7:0] cmd_response;
    logic cmd_response_valid;
    sd_response_type_t cmd_response_type;

    /* Data signals */
    logic [3:0][7:0] data_tx;
    logic data_tx_empty, data_rx_full;
    logic data_buffer_tx_read;
    logic [3:0][7:0] data_rx;
    logic data_buffer_rx_write;
    logic data_idle;
    logic data_timeout, data_crc_error, data_error, data_timeout_ff, data_crc_error_ff, data_error_ff;

    /* Control from Command to Data */
    logic force_stop;
    logic data_direction;
    logic data_burst;

    /* Output for IRQ */
    logic interrupt;


    always_ff @(posedge clk_i) begin
        data_timeout_ff <= data_timeout;
        data_crc_error_ff <= data_crc_error;
        data_error_ff <= data_error;

        cmd_timeout_ff <= cmd_timeout;
        cmd_crc_error_ff <= cmd_crc_error;

        sd_card_detect <= !cd_n_sync;
    end

    sd_registers register_interface (
        .clk_i       ( clk_i       ),
        .rst_n_i     ( rst_n_i     ),
        .interrupt_o ( interrupt_o ),

        .write_address_i ( write_address_i ),
        .write_i         ( write_i         ),
        .write_data_i    ( write_data_i    ),
        .write_strobe_i  ( write_strobe_i  ),
        .write_done_o    ( write_done_o    ),
        .write_error_o   ( write_error_o   ),

        .read_address_i ( read_address_i ),
        .read_i         ( read_i         ),
        .read_done_o    ( read_done_o    ),
        .read_data_o    ( read_data_o    ),
        .read_error_o   ( read_error_o   ),

        .card_detect_i ( sd_card_detect ),
        .reset_card_o  ( sd_reset       ),
        .enable_o      ( enable         ),
        .reset_done_i  ( reset_done     ),

        .clock_speed_o ( clock_speed ),
        .bus_width_o   ( bus_width   ),

        .cmd_response_valid_i ( cmd_response_valid ),
        .cmd_response_i       ( cmd_response       ),
        .cmd_response_type_i  ( cmd_response_type  ),
        .cmd_idle_i           ( cmd_idle           ),
        .cmd_crc_error_i      ( cmd_crc_error_ff   ),
        .cmd_timeout_i        ( cmd_timeout_ff     ),
        .cmd_number_o         ( cmd_number         ),
        .cmd_argument_o       ( cmd_argument       ),
        .send_command_o       ( send_command       ),

        .data_tx_data_o     ( data_tx              ),
        .data_tx_empty_o    ( data_tx_empty        ),
        .data_tx_read_i     ( data_buffer_tx_read  ),
        .data_rx_full_o     ( data_rx_full         ),
        .data_rx_data_i     ( data_rx              ),
        .data_rx_valid_i    ( data_buffer_rx_write ),
        .data_idle_i        ( data_idle            ),
        .data_timeout_i     ( data_timeout_ff      ),
        .data_crc_error_i   ( data_crc_error_ff    ),
        .data_error_i       ( data_error_ff        )

    );


//====================================================================================
//      RESET GENERATION
//====================================================================================

    localparam RESET_CYCLES = 100_000;

    /* 1 ms reset counter */
    logic [$clog2(RESET_CYCLES) - 1:0] reset_counter; logic active;

        always_ff @(posedge clk_i) begin
            if (!rst_n_i) begin
                reset_counter <= '0;
            end else begin
                if (reset_counter == (RESET_CYCLES - 1)) begin
                    reset_counter <= '0;
                end else if (active) begin
                    reset_counter <= reset_counter + 1;
                end
            end
        end

    assign sd_reset_n_o = !active;


        always_ff @(posedge clk_i) begin
            if (!rst_n_i) begin
                active <= 1'b0;
                reset_done <= 1'b0;
            end else begin
                if (reset_counter == (RESET_CYCLES - 1)) begin
                    active <= 1'b0;

                    reset_done <= 1'b1;
                end else begin
                    if (sd_reset) begin
                        active <= 1'b1;
                    end

                    reset_done <= 1'b0;
                end
            end
        end


//====================================================================================
//      CLOCK GENERATION
//====================================================================================

    localparam FREQ_400KHZ = 125;
    localparam FREQ_25MHZ = 2;

    logic [7:0] divisor, counter; logic clk_previous, sd_clk;

    assign divisor = clock_speed ? (FREQ_25MHZ - 1) : (FREQ_400KHZ - 1);

    always_ff @(posedge clk_i) begin
        if (!rst_n_i) begin
            counter <= '0;

            clk_previous <= 1'b0;
            sd_clk <= 1'b0;
        end else if (enable) begin
            counter <= counter + 1;
            clk_previous <= sd_clk;

            /* Reset counter */
            if (counter == (divisor - 1)) begin
                sd_clk <= !sd_clk;
                counter <= '0;
            end
        end
    end 


    logic shift, sample;

    assign shift = !sd_clk & clk_previous;
    assign sample = sd_clk & !clk_previous;

    assign sd_clk_o = sd_clk;


//====================================================================================
//      DATA LOGIC
//====================================================================================

    logic data_activate, data_activate_edge;

        edge_detector #(1, 0) data_fsm_activation_detector (
            .clk_i   ( clk_i  ),
            .rst_n_i ( rst_n_i ),

            .signal_i ( data_activate      ),
            .edge_o   ( data_activate_edge )
        );


    logic [1:0] tx_data_selector;

        always_ff @(posedge clk_i) begin
            if (data_activate_edge) begin
                tx_data_selector <= '1;
            end else begin
                if (data_tx_read) begin
                    tx_data_selector <= tx_data_selector + 1'b1;
                end
            end
        end

    /* Select a byte out of the four */
    logic [7:0] tx_data_byte; assign tx_data_byte = data_tx[tx_data_selector];

    /* Read buffer every four bytes */
    assign data_buffer_tx_read = data_tx_read & (tx_data_selector == '1) & !data_tx_empty;


    logic [7:0] data_received; logic data_rx_write;

    always_ff @(posedge clk_i) begin
        if (data_rx_write) begin
            data_rx <= {data_rx[2:0], data_received};
        end
    end

    logic [4:0] write_selector; logic buffer_rx_write;

        always_ff @(posedge clk_i) begin
            if (!rst_n_i | data_activate_edge) begin
                write_selector <= 5'b10000;
            end else begin
                if (data_rx_write) begin
                    if (write_selector[4]) begin 
                        write_selector <= 5'b00001;
                    end else begin
                        write_selector <= {1'b0, write_selector[2:0], write_selector[3]};
                    end
                end
            end
        end

    /* Write 32 bits every 4 bytes received */
    assign buffer_rx_write = write_selector[3];

        edge_detector #(1, 0) data_rx_write_detector (
            .clk_i   ( clk_i   ),
            .rst_n_i ( rst_n_i ),

            .signal_i ( buffer_rx_write      ),
            .edge_o   ( data_buffer_rx_write )
        );


//====================================================================================
//      COMMAND CONTROLLER
//====================================================================================
    
    sd_command_controller command_controller (
        .clk_i    ( clk_i   ),
        .rst_n_i  ( rst_n_i ),
        .enable_i ( enable  ),

        .shift_i  ( shift  ),
        .sample_i ( sample ),

        .cmd_valid_i    ( send_command ),
        .cmd_number_i   ( cmd_number   ),
        .cmd_argument_i ( cmd_argument ),

        .resp_valid_o ( cmd_response_valid ),
        .resp_o       ( cmd_response       ),
        .resp_type_o  ( cmd_response_type  ),

        .idle_o      ( cmd_idle      ),
        .crc_error_o ( cmd_crc_error ),
        .timeout_o   ( cmd_timeout   ),

        .force_stop_o     ( force_stop     ),
        .data_activate_o  ( data_activate  ), 
        .data_direction_o ( data_direction ),
        .data_burst_o     ( data_burst     ),

        .sd_cmd_io ( sd_cmd_io )
    );


//====================================================================================
//      DATA CONTROLLER
//====================================================================================

    sd_data_controller data_controller (
        .clk_i    ( clk_i   ),
        .rst_n_i  ( rst_n_i ),
        .enable_i ( enable  ),

        .shift_i  ( shift  ),
        .sample_i ( sample ),

        .tx_data_i  ( tx_data_byte  ),
        .tx_read_o  ( data_tx_read  ),

        .rx_full_i  ( data_rx_full  ),
        .rx_data_o  ( data_received ),
        .rx_valid_o ( data_rx_write ),

        .force_stop_i  ( force_stop     ),
        .direction_i   ( data_direction ),
        .wide_bus_i    ( bus_width      ),
        .burst_i       ( data_burst     ),
        .transfer_en_i ( data_activate  ),

        .idle_o      ( data_idle      ),
        .timeout_o   ( data_timeout   ),
        .crc_error_o ( data_crc_error ),
        .error_o     ( data_error     ),

        .sd_data_io ( sd_data_io )
    );

endmodule : sd

`endif 