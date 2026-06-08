`ifndef SD_REGISTERS_SV
    `define SD_REGISTERS_SV

`include "../../System/synchronous_buffer.sv"
`include "../../System/edge_detector.sv"

`include "../../Utility/Packages/sd_pkg.sv"

module sd_registers #(
    /* Buffer sizes */
    parameter TX_BUFFER_SIZE = 128,
    parameter RX_BUFFER_SIZE = 128
) (
    input logic clk_i,
    input logic rst_n_i,
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

    /* General Control */
    input logic card_detect_i,
    input logic reset_done_i,
    output logic reset_card_o,
    output logic enable_o,
    output logic clock_speed_o,
    output logic bus_width_o,

    /* Command FSM */
    input logic cmd_response_valid_i,
    input logic [7:0] cmd_response_i,
    input sd_response_type_t cmd_response_type_i,
    input logic cmd_idle_i,
    input logic cmd_crc_error_i,
    input logic cmd_timeout_i,
    output logic [5:0] cmd_number_o,
    output logic [31:0] cmd_argument_o,
    output logic send_command_o,

    /* Data FSM */
    output logic [31:0] data_tx_data_o,
    output logic data_tx_empty_o,
    input logic data_tx_read_i,
    output logic data_rx_full_o,
    input logic [31:0] data_rx_data_i,
    input logic data_rx_valid_i,
    input logic data_idle_i,
    input logic data_timeout_i,
    input logic data_crc_error_i,
    input logic data_error_i
);

//====================================================================================
//      MASK GENERATION
//====================================================================================

    logic [3:0][7:0] mask;

        always_comb begin
            for (int i = 0; i < 4; ++i) begin
                /* Mask certain bytes */
                mask[i] = write_strobe_i[i] ? '1 : '0;
            end
        end


//====================================================================================
//      ERROR CHECK
//====================================================================================

    assign write_error_o = ((write_address_i == SD_DATA_RX_BUFFER) | (write_address_i == SD_CMD_RESPONSE)) & write_i;
    assign read_error_o = (read_address_i == SD_DATA_TX_BUFFER) & read_i;


//====================================================================================
//      CONTROL REGISTER
//====================================================================================  

    sd_control_t control_register;

        always_ff @(posedge clk_i) begin
            if (!rst_n_i) begin 
                control_register.enable <= 1'b0;

                /* 400kHz and 1 bit bus width */
                control_register.clock_speed <= 1'b0;
                control_register.bus_width <= 1'b0;

                control_register.flush_tx <= 1'b0;

                control_register.send_command <= 1'b0;
                control_register.reset_card <= 1'b0;

                /* Interrupts */
                control_register.interrupt_enable <= '0;
            end else begin 
                if ((write_address_i == SD_CTRL) & write_i) begin 
                    control_register <= (write_data_i & mask) | (control_register & ~mask);
                end else begin
                    control_register.send_command <= 1'b0;
                    control_register.flush_tx <= 1'b0;

                    if (reset_done_i) begin 
                        control_register.reset_card <= 1'b0;
                    end
                end
            end 
        end 

    assign clock_speed_o = control_register.clock_speed;
    assign bus_width_o = control_register.bus_width;

    assign enable_o = control_register.enable;

    assign send_command_o = control_register.send_command;
    
    assign reset_card_o = control_register.reset_card & !reset_done_i;


//====================================================================================
//      STATUS REGISTER
//====================================================================================  

    sd_status_t status_register;

    logic tx_buffer_empty, tx_buffer_full;
    logic rx_buffer_empty, rx_buffer_full;
    logic resp_buffer_empty, resp_buffer_full;

    logic cmd_done, data_done;

    edge_detector #(1, 0) cmd_done_detector (
        .clk_i   ( clk_i  ),
        .rst_n_i ( rst_n_i ),

        .signal_i ( cmd_idle_i ),
        .edge_o   ( cmd_done   )
    );

    edge_detector #(1, 0) data_done_detector (
        .clk_i   ( clk_i  ),
        .rst_n_i ( rst_n_i ),

        .signal_i ( data_idle_i ),
        .edge_o   ( data_done   )
    );


        always_ff @(posedge clk_i) begin
            if (!rst_n_i) begin 
                status_register <= '0;
            end else begin 
                if ((write_address_i == SD_STATUS) & write_i) begin 
                    status_register <= (write_data_i & mask) | (status_register & ~mask);
                end else begin
                    /* Card Status */
                    status_register.card_detected <= card_detect_i;

                    /* FIFOS Status */
                    status_register.tx_buffer_empty <= tx_buffer_empty;
                    status_register.tx_buffer_full <= tx_buffer_full;

                    status_register.rx_buffer_empty <= rx_buffer_empty;
                    status_register.rx_buffer_full <= rx_buffer_full;

                    status_register.resp_buffer_empty <= resp_buffer_empty;
                    status_register.resp_buffer_full <= resp_buffer_full;

                    /* CMD Status */
                    status_register.cmd_idle <= cmd_idle_i;

                    if (cmd_crc_error_i) begin
                        status_register.cmd_crc_error <= 1'b1;
                    end

                    if (cmd_timeout_i) begin
                        status_register.cmd_timeout <= 1'b1;
                    end
                    
                    if (cmd_response_valid_i) begin
                        status_register.cmd_response_type <= cmd_response_type_i;
                    end


                    /* Data Status */
                    status_register.data_idle <= data_idle_i;

                    if (data_crc_error_i) begin
                        status_register.data_crc_error <= 1'b1;
                    end

                    if (data_timeout_i) begin
                        status_register.data_timeout <= 1'b1;
                    end

                    if (data_error_i) begin
                        status_register.data_error <= 1'b1;
                    end
                end
            end 
        end 


//====================================================================================
//      NUMBER REGISTER
//====================================================================================  

    logic [5:0] cmd_number;

        always_ff @(posedge clk_i) begin
            if (!rst_n_i) begin 
                cmd_number <= '0;
            end else begin 
                if ((write_address_i == SD_CMD_NUMBER) & write_i) begin
                    cmd_number <= (write_data_i & mask) | (cmd_number & ~mask);
                end
            end 
        end 

    assign cmd_number_o = cmd_number;


//====================================================================================
//      ARGUMENT REGISTER
//====================================================================================  

    logic [31:0] cmd_argument;

        always_ff @(posedge clk_i) begin
            if (!rst_n_i) begin 
                cmd_argument <= '0;
            end else begin 
                if ((write_address_i == SD_CMD_ARGUMENT) & write_i) begin
                    cmd_argument <= (write_data_i & mask) | (cmd_argument & ~mask);
                end
            end 
        end 

    assign cmd_argument_o = cmd_argument;


//====================================================================================
//      EVENT REGISTER
//====================================================================================

    sd_interrupt_status_t event_register; 

    always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
        if (!rst_n_i) begin 
            event_register <= '0;
        end else begin 
            if ((write_address_i == SD_EVENT) & write_i) begin
                event_register <= (write_data_i & mask) | (event_register & ~mask);
            end else begin
                if (tx_buffer_empty) begin
                    event_register.tx_buffer_empty <= control_register.interrupt_enable[0];
                end

                if (rx_buffer_full) begin
                    event_register.rx_buffer_full <= control_register.interrupt_enable[1];
                end

                if (cmd_done) begin
                    event_register.cmd_done <= control_register.interrupt_enable[2];
                end

                if (cmd_crc_error_i) begin
                    event_register.cmd_crc_error <= control_register.interrupt_enable[3];
                end

                if (cmd_timeout_i) begin
                    event_register.cmd_timeout <= control_register.interrupt_enable[4];
                end

                if (data_done) begin
                    event_register.data_done <= control_register.interrupt_enable[5];
                end

                if (data_crc_error_i) begin
                    event_register.data_crc_error <= control_register.interrupt_enable[6];
                end

                if (data_timeout_i) begin
                    event_register.data_timeout <= control_register.interrupt_enable[7];
                end

                if (card_detect_i) begin
                    event_register.card_detected <= control_register.interrupt_enable[8];
                end
            end
        end 
    end

    /* Catch the positive edge of a bit set into the register, otherwise the interrupt bit will stay on */
    logic [8:0] event_edge;

    genvar i;

    generate 
        for (i = 0; i < 9; ++i) begin
            edge_detector #(1, 0) event_detector (
                .clk_i   ( clk_i  ),
                .rst_n_i ( rst_n_i ),

                .signal_i ( event_register[i] ),
                .edge_o   ( event_edge[i]     )
            );
        end
    endgenerate

    assign interrupt_o = (event_edge != '0);


//====================================================================================
//      TX BUFFER
//====================================================================================

    logic write_tx_buffer; assign write_tx_buffer = write_i & (write_address_i == SD_DATA_TX_BUFFER);

    synchronous_buffer #(
        .DATA_WIDTH   ( 32             ),
        .BUFFER_DEPTH ( TX_BUFFER_SIZE )
    ) tx_buffer (
        .clk_i   ( clk_i ),
        .rst_n_i ( rst_n_i & !control_register.flush_tx ),

        .write_i      ( write_tx_buffer ),
        .write_data_i ( write_data_i    ),

        .read_i      ( data_tx_read_i & !tx_buffer_empty ),
        .read_data_o ( data_tx_data_o                    ),

        .empty_o ( tx_buffer_empty ),
        .full_o  ( tx_buffer_full  )
    );

    assign data_tx_empty_o = tx_buffer_empty;


//====================================================================================
//      RX BUFFER
//====================================================================================

    logic [31:0] read_data_rx;

    logic read_rx_buffer; assign read_rx_buffer = read_i & (read_address_i == SD_DATA_RX_BUFFER);

    synchronous_buffer #(
        .DATA_WIDTH   ( 32             ),
        .BUFFER_DEPTH ( RX_BUFFER_SIZE )
    ) rx_buffer (
        .clk_i   ( clk_i   ),
        .rst_n_i ( rst_n_i ),

        .write_i      ( data_rx_valid_i ),
        .write_data_i ( data_rx_data_i  ),

        .read_i      ( read_rx_buffer ),
        .read_data_o ( read_data_rx   ),

        .empty_o ( rx_buffer_empty ),
        .full_o  ( rx_buffer_full )
    );

    assign data_rx_full_o = rx_buffer_full;


//====================================================================================
//      RESPONSE BUFFER
//====================================================================================

    logic [7:0] read_response_data;

    logic read_resp_buffer; assign read_resp_buffer = read_i & (read_address_i == SD_CMD_RESPONSE);

    synchronous_buffer #(
        .DATA_WIDTH   ( 8  ),
        .BUFFER_DEPTH ( 32 )
    ) response_buffer (
        .clk_i   ( clk_i   ),
        .rst_n_i ( rst_n_i ),

        .write_i      ( cmd_response_valid_i ),
        .write_data_i ( cmd_response_i       ),

        .read_i      ( read_resp_buffer   ),
        .read_data_o ( read_response_data ),

        .empty_o ( resp_buffer_empty ),
        .full_o  ( resp_buffer_full  )
    );


//====================================================================================
//      DATA READ
//====================================================================================

        always_comb begin
            /* Default value */
            read_data_o = '0;

            case (read_address_i)
                SD_CTRL: read_data_o = control_register;

                SD_STATUS: read_data_o = status_register;

                SD_CMD_NUMBER: read_data_o = cmd_number;

                SD_CMD_ARGUMENT: read_data_o = cmd_argument;

                SD_EVENT: read_data_o = event_register;

                SD_CMD_RESPONSE: read_data_o = read_response_data;

                SD_DATA_RX_BUFFER: read_data_o = read_data_rx;
            endcase 
        end


//====================================================================================
//      OPERATION DONE LOGIC
//====================================================================================

    assign write_done_o = write_i;


    logic is_delayed;

    assign is_delayed = (sd_registers_t'(read_address_i) == SD_DATA_RX_BUFFER) | 
                        (sd_registers_t'(read_address_i) == SD_CMD_RESPONSE);


    logic read_done_delay; 

        always_ff @(posedge clk_i) begin
            if (!rst_n_i) begin 
                read_done_delay <= 1'b0; 
            end else begin 
                read_done_delay <= is_delayed & read_i;
            end 
        end 

    assign read_done_o = is_delayed ? read_done_delay : read_i;

endmodule : sd_registers

`endif 