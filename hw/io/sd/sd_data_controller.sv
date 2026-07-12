`ifndef SD_DATA_CONTROLLER_SV
    `define SD_DATA_CONTROLLER_SV

module sd_data_controller (
    input logic clk_i,
    input logic rst_n_i,
    input logic enable_i,
    input logic shift_i, /* Negedge */
    input logic sample_i, /* Posedge */

    /* Data Interface */
    input logic [7:0] tx_data_i,
    output logic tx_read_o,
    
    input logic rx_full_i,
    output logic [7:0] rx_data_o,
    output logic rx_valid_o,

    /* Control Interface */
    input logic force_stop_i,
    input logic direction_i, /* 1 = read, 0 = write */
    input logic wide_bus_i,
    input logic burst_i,
    input logic transfer_en_i, 
    input logic [12:0] data_length_i,

    /* Status */
    output logic idle_o,
    output logic timeout_o,
    output logic crc_error_o, 
    output logic error_o, 

    /* PHY Interface */
    inout logic [3:0] sd_data_io
);

//====================================================================================
//      DATAPATH REGISTERS
//====================================================================================

    /* Temporary registers to data and CRC */
    logic [7:0] data_CRT, data_NXT;
    logic [15:0] crc16_CRT, crc16_NXT;
    logic [3:0][15:0] crc16_wide_CRT, crc16_wide_NXT;
    logic [12:0] data_length_CRT;

        always_ff @(posedge clk_i) begin
            if (!rst_n_i) begin
                data_length_CRT <= 13'd4096;
            end else if (transfer_en_i) begin
                data_length_CRT <= data_length_i;
            end

            data_CRT <= data_NXT;
            crc16_CRT <= crc16_NXT;
            crc16_wide_CRT <= crc16_wide_NXT;
        end 

    assign rx_data_o = data_CRT;


    /* Bit counter */
    logic [11:0] bit_counter; logic bit_increment, bit_reset;

        always_ff @(posedge clk_i) begin
            if (!rst_n_i) begin 
                bit_counter <= '0;
            end else begin 
                if (bit_reset) begin
                    bit_counter <= '0;
                end else if (bit_increment) begin 
                    bit_counter <= bit_counter + 'd1;
                end 
            end 
        end


    /* Timeout counter */
    logic [$clog2(1_000_000) - 1:0] timeout_counter; logic timeout_increment, timeout_reset;

        always_ff @(posedge clk_i) begin
            if (!rst_n_i) begin 
                timeout_counter <= '0;
            end else begin 
                if (timeout_reset) begin
                    timeout_counter <= '0;
                end else if (timeout_increment) begin 
                    timeout_counter <= timeout_counter + 1'b1;
                end 
            end 
        end

//====================================================================================
//      CRC16 GENERATOR
//====================================================================================

    /* Output CRC16 */
    logic [15:0] crc16_out; logic [3:0][15:0] crc16_wide_out;

    /* Control signals */
    logic crc16_initialize, crc16_compute;

    /* Data input for CRC15 */
    logic [3:0] crc16_data;

    sd_crc16 crc16 (
        .clk_i   ( clk_i   ),
        .rst_n_i ( rst_n_i ),

        .initialize_i ( crc16_initialize ),
        .compute_i    ( crc16_compute    ), 

        .data_i     ( crc16_data     ),
        .crc_o      ( crc16_out      ),
        .crc_wide_o ( crc16_wide_out )
    );


//====================================================================================
//      FSM LOGIC
//====================================================================================

    localparam CRC16_LENGTH = 16; // TODO: know why SEND_DATA blocks, modify interface so the software doesn't need to read the tokens but just a bit (then reset it)
    /* Write response */
    localparam WRITE_OK_RESP = 8'h05;
    localparam WRITE_ERROR_RESP = 8'h0D;
    localparam WRITE_CRC_ERROR_RESP = 8'h0B;

    /* Write recovery after a single block in a burst */
    localparam WRITE_RECOVERY_CYCLES = 16;


    typedef enum logic [3:0] {IDLE, WAIT_RX, WAIT_TX, START_BIT, RCV_DATA, 
                              RCV_CRC, CHECK_CRC, SEND_DATA, SEND_CRC, STOP_BIT, 
                              WAIT_END, WAIT_TOKEN, CARD_RESPONSE, READ_TOKEN, RECOVERY_CYCLE} data_states_t;

    data_states_t state_CRT, state_NXT; 

        always_ff @(posedge clk_i) begin
            if (!rst_n_i | !enable_i | force_stop_i) begin 
                state_CRT <= IDLE;
            end else if (enable_i) begin 
                state_CRT <= state_NXT;
            end 
        end

    assign idle_o = (state_CRT == IDLE);

    /* Output SD data line */
    logic tristate_enable; logic [3:0] data_line;

    /* Temporary variable */
    logic rx_valid;

        always_comb begin
            /* Default values */
            crc16_wide_NXT = crc16_wide_CRT;
            state_NXT = state_CRT;
            crc16_NXT = crc16_CRT;
            data_NXT = data_CRT;

            crc16_initialize = 1'b0;
            crc16_compute = 1'b0;
            crc16_data = '0;

            bit_increment = 1'b0;
            bit_reset = 1'b0;

            tx_read_o = 1'b0;
            rx_valid = 1'b0;

            timeout_o = 1'b0;
            timeout_increment = 1'b0;
            timeout_reset = 1'b0;

            crc_error_o = 1'b0;
            error_o = 1'b0;

            tristate_enable = 1'b1;
            data_line = '1;

            case (state_CRT) 
                IDLE: begin
                    /* Start transaction once the Command FSM is done 
                     * and data can be moved */
                    if (transfer_en_i) begin
                        if (direction_i) begin
                            state_NXT = WAIT_RX;

                            tristate_enable = 1'b0;
                        end else begin
                            /* Wait pulse to synchronize */
                            state_NXT = WAIT_TX;

                            tristate_enable = 1'b1;
                        end

                        timeout_reset = 1'b1;
                        bit_reset = 1'b1;
                        crc16_initialize = 1'b1;
                    end
                end

                WAIT_RX: begin
                    /* Wait for the data to be received (start bit) */
                    if (sample_i) begin
                        if (wide_bus_i) begin
                            if (sd_data_io == '0) begin
                                state_NXT = RCV_DATA;

                                timeout_reset = 1'b1;

                                crc16_data = sd_data_io;
                                crc16_compute = 1'b1;
                            end
                        end else begin
                            if (sd_data_io[0] == 1'b0) begin
                                state_NXT = RCV_DATA;

                                timeout_reset = 1'b1;

                                crc16_data = sd_data_io;
                                crc16_compute = 1'b1;
                            end
                        end
                    end

                    timeout_increment = 1'b1;

                    if (timeout_counter == 'd10_000) begin
                        /* Timeout, reset the state machine */
                        state_NXT = IDLE;
                    end

                    /* Reset data */
                    data_NXT = '0;

                    /* Keep high impedence */
                    tristate_enable = 1'b0;
                end

                RCV_DATA: begin
                    /* Receive data from the SD card */
                    if (sample_i) begin
                        if (wide_bus_i) begin
                            /* Shift nibble in */
                            data_NXT = {data_CRT[3:0], sd_data_io};

                            if (bit_counter == ((data_length_CRT / 4) - 1)) begin
                                state_NXT = RCV_CRC;

                                bit_reset = 1'b1;
                            end

                            /* Check if 8 bits have been received with modulo 8 operator and
                             * assert that the counter is not zero (to not have a false data) */
                            if ((bit_counter[0] == '1) && (bit_counter != '0)) begin
                                rx_valid = !rx_full_i;
                            end
                        end else begin
                            /* Shift bit in */
                            data_NXT = {data_CRT[6:0], sd_data_io[0]};

                            if (bit_counter == data_length_CRT - 1'b1) begin
                                state_NXT = RCV_CRC;

                                bit_reset = 1'b1;
                            end

                            /* A byte has been received */
                            if (bit_counter[2:0] == 3'd7) begin
                                rx_valid = !rx_full_i;
                            end
                        end

                        crc16_data = sd_data_io;
                        crc16_compute = 1'b1;

                        bit_increment = 1'b1;
                    end

                    /* Keep high impedence */
                    tristate_enable = 1'b0;
                end

                RCV_CRC: begin
                    /* Receive CRC from the SD card */
                    if (sample_i) begin
                        /* CRC is sent to each line, we only check the first one */
                        if (wide_bus_i) begin
                            /* Shift nibble in */
                            for (int i = 0; i < 4; ++i) begin
                                crc16_wide_NXT[i] = {crc16_wide_CRT[i][14:0], sd_data_io[i]};
                            end
                        end else begin
                            /* Shift bit in */
                            crc16_NXT = {crc16_CRT[14:0], sd_data_io[0]};
                        end

                        bit_increment = 1'b1;

                        if (bit_counter == CRC16_LENGTH - 1) begin
                            state_NXT = CHECK_CRC;

                            bit_reset = 1'b1;
                        end
                    end

                    /* Keep high impedence */
                    tristate_enable = 1'b0;
                end

                CHECK_CRC: begin
                    /* Check CRC */
                    crc_error_o = wide_bus_i ? (crc16_wide_out != crc16_wide_CRT) : (crc16_out != crc16_CRT);

                    state_NXT = burst_i ? WAIT_RX : IDLE;

                    timeout_reset = 1'b1;
                    bit_reset = 1'b1;
                    crc16_initialize = 1'b1;
                end

                RECOVERY_CYCLE: begin
                    tristate_enable = 1'b0;

                    if (shift_i) begin
                        if (bit_counter == WRITE_RECOVERY_CYCLES - 1) begin
                            state_NXT = WAIT_TX;

                            bit_reset = 1'b1;
                        end else begin
                            bit_increment = 1'b1;
                        end
                    end
                end

                WAIT_TX: begin
                    if (shift_i) begin
                        /* Wait for the clock pulse to send the data */
                        state_NXT = START_BIT;

                        tx_read_o = 1'b1;
                    end
                end

                START_BIT: begin
                    /* Send start bit */
                    if (wide_bus_i) begin
                        data_line = '0;
                    end else begin
                        data_line[0] = 1'b0;
                    end

                    if (shift_i) begin
                        state_NXT = SEND_DATA;
                    end

                    /* Reset the bit counter */
                    bit_reset = 1'b1;

                    /* Drive bit bus */
                    tristate_enable = 1'b1;
                end

                SEND_DATA: begin
                    /* Send data to the SD card */
                    if (wide_bus_i) begin
                        if (bit_counter[0] == '0) begin
                            /* Always MSB first */
                            data_line = tx_data_i[7:4];
                        end else begin
                            data_line = tx_data_i[3:0];
                        end

                        if (shift_i) begin
                            /* Check if 8 bits have been transmitted, then request another packet.
                             * Consider that the FIFO will take 1 cycle to output the data */
                            if (bit_counter[0] == 'b1) begin
                                /* Don't read the last data to avoid FIFO problem */
                                tx_read_o = bit_counter != ((data_length_CRT / 4) - 1);
                            end

                            if (bit_counter == ((data_length_CRT / 4) - 1)) begin
                                state_NXT = SEND_CRC;
                                crc16_NXT = crc16_out;

                                bit_reset = 1'b1;
                            end
                        end
                    end else begin
                        /* Always MSB first */
                        data_line[0] = tx_data_i[7 - bit_counter[2:0]];

                        if (shift_i) begin
                            /* A byte has been transmitted */
                            if (bit_counter[2:0] == 3'd7) begin
                                /* Don't read the last data to avoid FIFO problem */
                                tx_read_o = bit_counter != (data_length_CRT - 1'b1);
                            end

                            if (bit_counter == (data_length_CRT - 1'b1)) begin
                                state_NXT = SEND_CRC;

                                crc16_NXT = crc16_out;
                                crc16_wide_NXT = crc16_wide_out;

                                bit_reset = 1'b1;
                            end
                        end
                    end

                    /* Compute CRC each time the bits are sent */
                    crc16_compute = shift_i;
                    crc16_data = data_line;

                    /* Increase the bit counter */
                    bit_increment = shift_i;

                    /* Drive bit bus */
                    tristate_enable = 1'b1;
                end

                SEND_CRC: begin
                    if (bit_counter == '0) begin
                        /* Load CRC7 */
                        if (wide_bus_i) begin
                            /* Send CRC7 in 4 bits, each channel has a CRC calculated */
                            data_line = {crc16_wide_out[3][15], crc16_wide_out[2][15], crc16_wide_out[1][15], crc16_wide_out[0][15]};

                            if (shift_i) begin
                                /* Load CRC16, shift already since data is already on the bus */
                                for (int i = 0; i < 4; ++i) begin
                                    crc16_wide_NXT[i] = crc16_wide_out[i] << 1;
                                end
                            end
                        end else begin
                            /* Send CRC7 in 1 bit */
                            data_line[0] = crc16_out[15];

                            if (shift_i) begin
                                /* Load CRC16, shift already since data is already on the bus */
                                crc16_NXT = crc16_out << 1;
                            end
                        end
                    end else begin
                        /* Load CRC7 */
                        if (wide_bus_i) begin
                            /* Send CRC7 in 4 bits, each channel has a CRC calculated */
                            data_line = {crc16_wide_CRT[3][15], crc16_wide_CRT[2][15], crc16_wide_CRT[1][15], crc16_wide_CRT[0][15]};

                            if (shift_i) begin
                                /* Load CRC16, shift already since data is already on the bus */
                                for (int i = 0; i < 4; ++i) begin
                                    crc16_wide_NXT[i] = crc16_wide_CRT[i] << 1;
                                end
                            end
                        end else begin
                            /* Send CRC7 in 1 bit */
                            data_line[0] = crc16_CRT[15];

                            if (shift_i) begin
                                /* Load CRC16, shift already since data is already on the bus */
                                crc16_NXT = crc16_CRT << 1;
                            end
                        end
                    end

                    bit_increment = shift_i;

                    /* Once 16 bits have been sent go to the next state */
                    if (shift_i) begin
                        if (bit_counter == (CRC16_LENGTH - 1)) begin
                            /* Reset bit counter */
                            bit_reset = 1'b1;

                            /* Reset CRC16 */
                            crc16_initialize = 1'b1;

                            state_NXT = STOP_BIT;
                        end
                    end

                    /* Drive bit bus */
                    tristate_enable = 1'b1;
                end

                STOP_BIT: begin
                    /* Send stop bit */
                    if (shift_i) begin
                        /* Send stop bit */
                        if (wide_bus_i) begin
                            data_line = '1;
                        end else begin
                            data_line[0] = 1'b1;
                        end

                        /* Reset the state machine */
                        state_NXT = WAIT_TOKEN;
                        data_NXT = '0;

                        bit_reset = 1'b1;
                    end

                    /* Drive bit bus */
                    tristate_enable = 1'b1;
                end

                WAIT_TOKEN: begin
                    if (sample_i & !sd_data_io[0]) begin
                        data_NXT = '0;

                        bit_reset = 1'b1;

                        state_NXT = READ_TOKEN;
                    end else if (timeout_counter >= 'd1_000_000) begin
                        state_NXT = IDLE;
                        timeout_o = 1'b1;
                    end

                    timeout_increment = 1'b1;

                    /* Keep high impedence */
                    tristate_enable = 1'b0;
                end

                READ_TOKEN: begin
                    tristate_enable = 1'b0;

                    if (sample_i) begin
                        data_NXT = {data_CRT[6:0], sd_data_io[0]};

                        /* We already stored the first token bit in WAIT_TOKEN (1'b0).
                         * Now we need 4 more samples. At bit_counter == 3, data_NXT[4:0] contains:
                         * 0 s2 s1 s0 1 */
                        if (bit_counter == 'd3) begin
                            /* Write CRC Error Token */
                            crc_error_o = data_NXT[3:1] == WRITE_CRC_ERROR_RESP[3:1];
                            error_o = data_NXT[3:1] == WRITE_ERROR_RESP[3:1];

                            if (data_NXT[3:1] != WRITE_OK_RESP[3:1]) begin
                                state_NXT = IDLE;
                            end else begin
                                state_NXT = WAIT_END;
                            end

                            timeout_reset = 1'b1;
                            bit_reset = 1'b1;
                        end else begin
                            bit_increment = 1'b1;
                        end
                    end
                end

                WAIT_END: begin
                    /* Wait for the card to send stop bit */
                    if (sample_i) begin
                        /* Wait until the card is not busy anymore (bit is high) */
                        if (sd_data_io[0] == 1'b1) begin
                            state_NXT = burst_i ? RECOVERY_CYCLE : IDLE;

                            timeout_reset = 1'b1;
                            bit_reset = 1'b1;
                            crc16_initialize = 1'b1;
                        end else if (timeout_counter >= 'd1_000_000) begin
                            /* Timeout, reset the state machine */
                            state_NXT = IDLE;

                            timeout_o = 1'b1;
                        end
                    end

                    timeout_increment = 1'b1;

                    /* Keep high impedence */
                    tristate_enable = 1'b0;
                end
            endcase
        end

    logic tristate_enable_flop; logic [3:0] data_line_flop;

        always_ff @(posedge clk_i) begin
            tristate_enable_flop <= tristate_enable;
            data_line_flop <= data_line;

            rx_valid_o <= rx_valid;
        end


    /* Tristate enable signal */
    assign sd_data_io = tristate_enable_flop ? data_line_flop : 4'bz;

endmodule : sd_data_controller

`endif
