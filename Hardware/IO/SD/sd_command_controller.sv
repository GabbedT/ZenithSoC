`ifndef SD_COMMAND_CONTROLLER_SV
    `define SD_COMMAND_CONTROLLER_SV

module sd_command_controller (
    input logic clk_i,
    input logic rst_n_i,
    input logic enable_i,
    input logic shift_i, /* Negedge */
    input logic sample_i, /* Posedge */

    /* Command Interface */
    input logic cmd_valid_i,
    input logic [5:0] cmd_number_i,
    input logic [31:0] cmd_argument_i,

    /* Response Interface */
    output logic resp_valid_o,
    output logic [7:0] resp_o,
    output logic resp_type_o,

    /* Status */
    output logic idle_o,
    output logic status_valid_o,
    output logic crc_error_o,
    output logic timeout_o,

    /* Data Controller Interface */
    output logic force_stop_o,
    output logic data_activate_o,
    output logic data_direction_o, /* 1 = read, 0 = write */
    output logic data_burst_o, /* 1 = burst mode, 0 = single block */

    /* External Interface */
    inout logic sd_cmd_io
);

//====================================================================================
//      DATAPATH REGISTERS
//====================================================================================

    /* Temporary registers to store the command and response */
    logic [5:0] cmd_number_CRT, cmd_number_NXT;
    logic card_app_cmd_CRT, card_app_cmd_NXT;
    logic [31:0] cmd_argument_CRT, cmd_argument_NXT;
    logic [7:0] resp_CRT, resp_NXT;
    logic [6:0] crc7_CRT, crc7_NXT;
    logic rcv136_CRT, rcv136_NXT;
    logic data_burst_CRT, data_burst_NXT;

        always_ff @(posedge clk_i) begin
            cmd_number_CRT <= cmd_number_NXT;
            cmd_argument_CRT <= cmd_argument_NXT;
            card_app_cmd_CRT <= card_app_cmd_NXT;
            resp_CRT <= resp_NXT;
            rcv136_CRT <= rcv136_NXT;
            crc7_CRT <= crc7_NXT;

            if (!rst_n_i) begin
                data_burst_CRT <= 1'b0;
            end else begin
                data_burst_CRT <= data_burst_NXT;
            end
        end 

    assign resp_o = resp_CRT;


    /* Counter to select the payload bits */
    logic [7:0] bit_counter; logic bit_increment, bit_reset;

        always_ff @(posedge clk_i) begin
            if (!rst_n_i) begin 
                bit_counter <= '0;
            end else begin 
                if (bit_reset) begin
                    bit_counter <= '0;
                end else if (bit_increment) begin 
                    bit_counter <= bit_counter + 1'b1;
                end 
            end 
        end

    /* 500ms timeout counter */
    logic [$clog2(100_000_000 / 2) - 1:0] timeout_counter; logic timeout_increment, timeout_reset;

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
//      CRC7 GENERATOR
//====================================================================================

    /* Output CRC7 */
    logic [6:0] crc7_out;

    /* Control signals */
    logic crc7_initialize, crc7_compute;

    /* Data input for CRC7 */
    logic crc7_data;

    sd_crc7 crc7 (
        .clk_i   ( clk_i   ),
        .rst_n_i ( rst_n_i ),

        .initialize_i ( crc7_initialize ),
        .compute_i    ( crc7_compute    ),

        .data_i ( crc7_data ),
        .crc_o  ( crc7_out  )
    );


//====================================================================================
//      FSM LOGIC
//====================================================================================

    localparam CMD_NUMBER_LENGTH = 6;
    localparam CMD_ARGUMENT_LENGTH = 32;
    localparam CRC7_LENGTH = 7;

    localparam DATA_READ = 1'b1;
    localparam DATA_WRITE = 1'b0;

    typedef enum logic [3:0] {IDLE, WAIT_START, START0, START1, CMD_NUMBER, CMD_ARGUMENT, CRC7, END_COMMAND, WAIT_RESP, RESP_DIR, RCV48, RCV136, CHECK_CRC, RECOVERY} cmd_states_t;

    cmd_states_t state_CRT, state_NXT;

        always_ff @(posedge clk_i) begin
            if (!rst_n_i | !enable_i) begin 
                state_CRT <= IDLE;
            end else if (enable_i) begin 
                state_CRT <= state_NXT;
            end 
        end

    assign idle_o = (state_CRT == IDLE);


    /* Output SD command line */
    logic tristate_enable, cmd_bit;

        always_comb begin
            /* Default Values */
            crc7_NXT = crc7_CRT;
            resp_NXT = resp_CRT;
            state_NXT = state_CRT;
            rcv136_NXT = rcv136_CRT;
            data_burst_NXT = data_burst_CRT;
            cmd_number_NXT = cmd_number_CRT;
            cmd_argument_NXT = cmd_argument_CRT;
            card_app_cmd_NXT = card_app_cmd_CRT;

            crc7_initialize = 1'b0;
            crc7_compute = 1'b0;
            crc7_data = 1'b0;

            timeout_increment = 1'b0;
            timeout_reset = 1'b0;

            data_activate_o = 1'b0;
            data_direction_o = 1'b0;
            force_stop_o = 1'b0;

            bit_increment = 1'b0;
            bit_reset = 1'b0;

            resp_valid_o = 1'b0;
            resp_type_o = 1'b0;

            status_valid_o = 1'b0;
            crc_error_o = 1'b0;
            timeout_o = 1'b0;
            
            cmd_bit = 1'b1;
            tristate_enable = 1'b1;

            case (state_CRT)
                /* The IDLE state is the default state of the FSM. The command line is
                 * high and the FSM waits for a command to be issued */
                IDLE: begin
                    if (cmd_valid_i) begin
                        state_NXT = WAIT_START;

                        rcv136_NXT = cmd_number_i == 6'd2 | cmd_number_i == 6'd9 | cmd_number_i == 6'd10;
                    end

                    crc7_initialize = 1'b1;

                    timeout_reset = 1'b1;

                    cmd_argument_NXT = cmd_argument_i;
                    cmd_number_NXT = cmd_number_i;

                    resp_NXT = '0;
                end

                /* Wait to synchronize with the SD CLK */
                WAIT_START: begin
                    if (shift_i) begin
                        state_NXT = START0;
                    end
                end

                /* Every transaction starts with a logic 0, which is the start bit */
                START0: begin
                    cmd_bit = 1'b0;

                    if (shift_i) begin
                        state_NXT = START1;

                        /* Start computing CRC7 */
                        crc7_compute = 1'b1;
                        crc7_data = 1'b0;
                    end
                end

                /* Send the direction of the transaction, the bit
                 * is always 1 in this case, because the host is
                 * sending the command */
                START1: begin
                    cmd_bit = 1'b1;
                    
                    /* Reset bit counter */
                    bit_reset = 1'b1;
                    
                    if (shift_i) begin
                        state_NXT = CMD_NUMBER;

                        crc7_compute = 1'b1;
                        crc7_data = 1'b1;
                    end
                end

                /* Send the command number */
                CMD_NUMBER: begin
                    /* Send MSB first */
                    cmd_bit = cmd_number_CRT[5];

                    if (shift_i) begin
                        /* Shift out most significant bit */
                        cmd_number_NXT = cmd_number_CRT << 1;
                        bit_increment = 1'b1;

                        crc7_compute = 1'b1;
                        crc7_data = cmd_number_CRT[5];

                        /* Once 6 bits have been sent go to the next state */
                        if (bit_counter == (CMD_NUMBER_LENGTH - 1)) begin
                            /* Reset bit counter */
                            bit_reset = 1'b1;

                            state_NXT = CMD_ARGUMENT;
                        end
                    end
                end

                /* Send the command argument */
                CMD_ARGUMENT: begin
                    /* Send MSB first */
                    cmd_bit = cmd_argument_CRT[31];

                    if (shift_i) begin
                        /* Shift out most significant bit */
                        cmd_argument_NXT = cmd_argument_CRT << 1;
                        bit_increment = 1'b1;

                        /* Start computing CRC7 */
                        crc7_compute = 1'b1;
                        crc7_data = cmd_argument_CRT[31];

                        /* Once 32 bits have been sent go to the next state */
                        if (bit_counter == (CMD_ARGUMENT_LENGTH - 1)) begin
                            /* Reset bit counter */
                            bit_reset = 1'b1;

                            state_NXT = CRC7;
                        end
                    end
                end

                /* Send the CRC calculated on the previous command bits */
                CRC7: begin
                    /* Send MSB first */
                    cmd_bit = (bit_counter == '0) ? crc7_out[6] : crc7_CRT[6];

                    if (shift_i) begin
                        if (bit_counter == '0) begin
                            /* Load CRC7 */
                            crc7_NXT = crc7_out << 1;
                        end else begin
                            /* Shift out most significant bit */
                            crc7_NXT = crc7_CRT << 1;
                        end

                        bit_increment = 1'b1;

                        /* Once 7 bits have been sent go to the next state */
                        if (bit_counter == (CRC7_LENGTH - 1)) begin
                            /* Reset bit counter */
                            bit_reset = 1'b1;

                            /* Reset CRC7 */
                            crc7_initialize = 1'b1;

                            state_NXT = END_COMMAND;
                        end
                    end
                end

                /* Send a logic 1 to indicate the end of the command */
                END_COMMAND: begin
                    /* End bit is always 1 */
                    cmd_bit = 1'b1;

                    /* Reset bit counter */
                    bit_reset = 1'b1;

                    if (shift_i) begin
                        state_NXT = (cmd_number_i == '0) ? IDLE : WAIT_RESP;
                    end
                end

                /* Keep the line high impedence until the card sends a response
                 * which starts with a bit 0. If the counter saturates, a timeout 
                 * will be generated */
                WAIT_RESP: begin
                    /* Release command line */
                    tristate_enable = 1'b0;

                    /* Increment timeout counter */
                    timeout_increment = 1'b1;

                    if (timeout_counter == 'd100_000_000) begin
                        /* Timeout activated */
                        timeout_o = 1'b1;
                        status_valid_o = 1'b1;
                        resp_valid_o = 1'b0;

                        state_NXT = RECOVERY;
                    end

                    if (sample_i) begin
                        if (sd_cmd_io == 1'b0) begin
                            /* Response initiated */
                            state_NXT = RESP_DIR;

                            /* Shift in most significant bit */
                            resp_NXT = {resp_CRT[6:0], sd_cmd_io};

                            /* Reset bit counter */
                            bit_reset = 1'b1;

                            /* Start computing CRC7 */
                            crc7_compute = 1'b1;
                            crc7_data = sd_cmd_io;
                        end
                    end
                end

                /* The direction (Host/Card) bit will be sent by the card, the line
                 * is kept high impedence */
                RESP_DIR: begin
                    /* Release command line */
                    tristate_enable = 1'b0;

                    /* Shift in most significant bit */
                    resp_NXT = {resp_CRT[6:0], sd_cmd_io};

                    if (sample_i) begin
                        state_NXT = rcv136_CRT ? RCV136 : RCV48;

                        bit_increment = 1'b1;

                        crc7_compute = 1'b1;
                        crc7_data = sd_cmd_io;
                    end
                end

                /* The FSM is receiving a 48 bit long response. The CRC7 will be calculated on the 
                 * last 8 bits of the response before the end bit */
                RCV48: begin
                    /* Release command line */
                    tristate_enable = 1'b0;

                    if (sample_i) begin
                        /* Shift in most significant bit */
                        resp_NXT = {resp_CRT[6:0], sd_cmd_io};

                        /* Keep shifting in this register and validate it at the end */
                        crc7_NXT = {crc7_CRT[5:0], sd_cmd_io};

                        bit_increment = 1'b1;

                        /* Don't compute CRC on CRC reception */
                        crc7_compute = bit_counter < 'd39;
                        crc7_data = sd_cmd_io;

                        /* Once 8 bits have been received write the byte */
                        if (bit_counter[2:0] == '1) begin
                            /* Load response type */
                            resp_type_o = 1'b0;
                            resp_valid_o = 1'b1;
                        end

                        /* Count one less bit since the first one is not counted in WAIT_RESP */
                        if (bit_counter == 'd46) begin
                            /* Don't check CRC7 */
                            state_NXT = (cmd_number_i == 'd41 & card_app_cmd_CRT) ? RECOVERY : CHECK_CRC;

                            resp_valid_o = 1'b1;

                            /* Don't shift CRC7 on last bit */
                            crc7_NXT = crc7_CRT;
                        end
                    end
                end

                /* The FSM will enter this state if the command is a CMD2, CMD9 or CMD10. The 
                 * response will be 136 bits long and the CRC7 will be calculated on the 
                 * last 8 bits of the response */
                RCV136: begin
                    /* Release command line */
                    tristate_enable = 1'b0;

                    if (sample_i) begin
                        /* Shift in most significant bit */
                        resp_NXT = {resp_CRT[6:0], sd_cmd_io};

                        /* Keep shifting in this register and validate it at the end */
                        crc7_NXT = {crc7_CRT[5:0], sd_cmd_io};

                        bit_increment = 1'b1;

                        crc7_compute = bit_counter < 'd129;
                        crc7_data = sd_cmd_io;

                        /* Once 8 bits have been received write the byte */
                        if (bit_counter[2:0] == '1) begin
                            /* Load response type */
                            resp_type_o = 1'b1;
                            resp_valid_o = 1'b1;
                        end

                        /* Count one less bit since the first one is not counted in WAIT_RESP */
                        if (bit_counter == 'd136) begin
                            /* Don't check CRC7 */
                            state_NXT = (cmd_number_i == 'd2 | cmd_number_i == 'd10) ? RECOVERY : CHECK_CRC;

                            /* Don't shift CRC7 on last bit */
                            crc7_NXT = crc7_CRT;
                        end
                    end
                end

                /* The CHECK_CRC state is responsible for verifying the CRC7 checksum 
                 * of the received command. If the calculated CRC7 (crc7_out) does not 
                 * match the expected CRC7, a CRC error is flagged */
                CHECK_CRC: begin
                    if (crc7_CRT != crc7_out) begin
                        crc_error_o = 1'b1;
                    end

                    status_valid_o = 1'b1;
                    bit_reset = 1'b1;

                    state_NXT = RECOVERY;
                end

                /* The RECOVERY state is responsible for handling the recovery process 
                 * after sending a command. The CMD line will be high for 16 clock cycles
                 * after that, another command can be issued */
                RECOVERY: begin
                    bit_increment = shift_i;

                    /* Tell the data controller to start sending / receiving data */
                    case (cmd_number_i)
                        'd12: begin
                            data_burst_NXT = 1'b0;  

                            /* Stop the data controller */
                            force_stop_o = 1'b1;  
                        end

                        'd17: begin
                            /* Read Single Block */
                            data_activate_o = 1'b1;
                            data_direction_o = DATA_READ;
                            data_burst_NXT = 1'b0; 
                        end

                        'd18: begin
                            /* Read Multiple Block */
                            data_activate_o = 1'b1;
                            data_direction_o = DATA_READ;
                            data_burst_NXT = 1'b1; 
                        end

                        'd24: begin
                            /* Write Single Block */
                            data_activate_o = 1'b1;
                            data_direction_o = DATA_WRITE;
                            data_burst_NXT = 1'b0; 
                        end

                        'd25: begin
                            /* Write Multiple Block */
                            data_activate_o = 1'b1;
                            data_direction_o = DATA_WRITE;
                            data_burst_NXT = 1'b1; 
                        end

                        'd6: begin
                            if (!card_app_cmd_CRT) begin
                                /* Receive data if it's CMD6 */
                                data_activate_o = 1'b1;
                                data_direction_o = DATA_READ;
                            end 
                        end

                        'd13, 'd22, 'd51: begin
                            if (card_app_cmd_CRT) begin
                                /* Receive data if it's ACMD13 / ACMD22 / ACMD51 */
                                data_activate_o = 1'b1;
                                data_direction_o = DATA_READ;
                            end 
                        end
                    endcase

                    if (bit_counter == 'd16) begin
                        state_NXT = IDLE;

                        if (cmd_number_i == 'd55) begin
                            /* If the command is CMD55, set the card_app_cmd flag */
                            card_app_cmd_NXT = 1'b1;
                        end else begin
                            /* Reset the card_app_cmd flag */
                            card_app_cmd_NXT = 1'b0;
                        end
                    end
                end
            endcase
        end

    logic tristate_enable_flop, cmd_bit_flop;

        always_ff @(posedge clk_i) begin
            tristate_enable_flop <= tristate_enable;
            cmd_bit_flop <= cmd_bit;
        end

    assign data_burst_o = data_burst_CRT;

    /* Tristate enable signal */
    assign sd_cmd_io = tristate_enable_flop ? cmd_bit_flop : 1'bz;

endmodule : sd_command_controller

`endif 