`include "Model/sd_top.v"

`include "../../Hardware/Utility/Packages/sd_pkg.sv"

`timescale 1ns/1ps

module sd_interface_testbench;

//====================================================================================
//      CLOCKING
//====================================================================================

    /* Clocks for the SD Card */
    logic clk_50MHz = 0;
    logic clk_100MHz = 0;
    logic clk_200MHz = 0;
    
    /* Clock generation */
    always #10ns clk_50MHz  <= ~clk_50MHz;
    always #5ns  clk_100MHz <= ~clk_100MHz;
    always #2.5ns clk_200MHz <= ~clk_200MHz; 

    
//====================================================================================
//      DUT
//====================================================================================

    logic rst_n_i = 1'b0;

    logic interrupt_o;

    sd_registers_t write_address_i;
    logic write_i;
    logic [3:0][7:0] write_data_i;
    logic [3:0] write_strobe_i;
    logic write_done_o, write_done;
    logic write_error_o;

    sd_registers_t read_address_i;
    logic read_i;
    logic read_done_o, read_done;
    logic [31:0] read_data_o;
    logic read_error_o;

    logic sd_reset, sd_detect;
    wire sd_cmd;
    wire [3:0] sd_data;
    logic sd_clk;

    always_ff @(posedge clk_100MHz) begin
        write_done <= write_done_o;
        read_done <= read_done_o;
    end


    sd dut (
        .clk_i   ( clk_100MHz),
        .rst_n_i ( rst_n_i),

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

        .sd_cd_n_i    ( sd_detect ), 
        .sd_reset_n_o ( sd_reset  ),

        .sd_cmd_io  (sd_cmd),
        .sd_data_io (sd_data),
        .sd_clk_o   (sd_clk)
    );


//====================================================================================
//      SD CARD MEMORY
//====================================================================================

    /* Wishbone Interface */
    logic        wbm_clk_o;
    logic [31:0] wbm_adr_o;
    logic [31:0] wbm_dat_i = 32'h0;
    logic [31:0] wbm_dat_o;
    logic [3:0]  wbm_sel_o;
    logic        wbm_cyc_o;
    logic        wbm_stb_o;
    logic        wbm_we_o;
    logic        wbm_ack_i = 1'b0;
    logic [2:0]  wbm_cti_o;
    logic [1:0]  wbm_bte_o;
        
    wishbone_ram ram_inst (
        .clk ( wbm_clk_o ),
        .rst ( rst_n_i   ),

        .wbm_adr_i  ( wbm_adr_o[11:2] ),
        .wbm_dat_i  ( wbm_dat_o       ),
        .wbm_dat_o  ( wbm_dat_i       ),
        .wbm_we_i   ( wbm_we_o        ),
        .wbm_stb_i  ( wbm_stb_o       ),
        .wbm_cyc_i  ( wbm_cyc_o       ),
        .wbm_sel_i  ( wbm_sel_o       ),
        .wbm_ack_o  ( wbm_ack_i       ),
        .wbm_cti_i  ( wbm_cti_o       ),
        .wbm_bte_i  ( wbm_bte_o       )
    );


//====================================================================================
//      SD CARD MODEL
//====================================================================================

    /* Card SD Interface */
    logic sd_cmd_i, sd_cmd_o, sd_cmd_t;
    logic [3:0] sd_dat_i, sd_dat_o, sd_dat_t;

    wire sd_cmd_wire; wire [3:0] sd_dat_wire;

    sd_top model (
        .clk_50      ( clk_50MHz  ),
        .clk_100     ( clk_100MHz ),
        .clk_200     ( clk_200MHz ),
        .reset_n     ( rst_n_i    ),

        .sd_clk      ( sd_clk ),

        .sd_cmd_i    ( sd_cmd_i ),
        .sd_cmd_o    ( sd_cmd_o ),
        .sd_cmd_t    ( sd_cmd_t ),

        .sd_dat_i    ( sd_dat_i ),
        .sd_dat_o    ( sd_dat_o ),
        .sd_dat_t    ( sd_dat_t ),

        .wbm_clk_o ( wbm_clk_o ),
        .wbm_adr_o ( wbm_adr_o ),
        .wbm_dat_i ( wbm_dat_i ),
        .wbm_dat_o ( wbm_dat_o ),
        .wbm_sel_o ( wbm_sel_o ),
        .wbm_cyc_o ( wbm_cyc_o ),
        .wbm_stb_o ( wbm_stb_o ),
        .wbm_we_o  ( wbm_we_o  ),
        .wbm_ack_i ( wbm_ack_i ),
        .wbm_cti_o ( wbm_cti_o ),
        .wbm_bte_o ( wbm_bte_o ),

        .opt_enable_hs ( '0 )
    );

    // TODO: CHANGE

    /* SD Connection Command to DUT  */
    assign sd_cmd = sd_cmd_t ? dut.command_controller.cmd_bit_flop : sd_cmd_o;

    /* SD Connection Data to DUT */
    assign sd_data = (sd_dat_t == '1) ? dut.data_controller.data_line_flop : sd_dat_o;

    assign sd_cmd_wire = (sd_cmd_t == 1'b0) ? sd_cmd_o :  
                         (dut.command_controller.tristate_enable_flop) ? dut.command_controller.cmd_bit_flop : 1'bz;
    
    assign sd_cmd_i = sd_cmd_wire;

    assign sd_dat_wire = (sd_dat_t == '0) ? sd_dat_o :  
                         (dut.data_controller.tristate_enable_flop) ? dut.data_controller.data_line_flop : 4'bz;

    assign sd_dat_i = sd_dat_wire;
    

//====================================================================================
//      STIMULI
//====================================================================================

    sd_status_t status_register;

    task automatic writeRegister(input sd_registers_t address, input logic [31:0] data);
        write_address_i <= address;
        write_data_i <= data;
        write_strobe_i <= '1;
        write_i <= 1'b1;
        @(posedge clk_100MHz);
        write_i <= 1'b0;
        wait(write_done);
    endtask : writeRegister 

    task automatic readRegister(input sd_registers_t address, output logic [31:0] data);
        read_address_i <= address;
        read_i <= 1'b1;
        @(posedge clk_100MHz);
        read_i <= 1'b0;
        wait(read_done);
        data = read_data_o;
    endtask : readRegister

    task automatic waitDataFSM();
        automatic sd_status_t status = 0; 

        while (!status.data_idle) begin
            @(posedge clk_100MHz);
            readRegister(SD_STATUS, status);
            @(posedge clk_100MHz);
        end
    endtask : waitDataFSM


    task automatic readResponse(output logic [135:0] response, output logic resp_type);
        automatic sd_status_t status = '0;
        automatic logic [16:0][7:0] temp_response = '0;
        automatic logic [7:0] byte_resp;
        @(posedge clk_100MHz);
        $display("Reading Response Buffer...\n");

        readRegister(SD_STATUS, status);
        while (!status.cmd_idle) begin
            readRegister(SD_STATUS, status);
            @(posedge clk_100MHz);
        end

        for (int i = 0; status.resp_buffer_empty == 1'b0; ++i) begin
            readRegister(SD_CMD_RESPONSE, byte_resp);
            @(posedge clk_100MHz);
            temp_response = {temp_response[15:0], byte_resp};

            readRegister(SD_STATUS, status);
            @(posedge clk_100MHz);
        end

        response = temp_response;
        readRegister(SD_STATUS, status);
        resp_type = status.cmd_response_type;
        $display("Response Buffer read: 0x%h, type: %s!\n", resp_type ? response : response[47:0], resp_type ? "RESP136" : "RESP48");
    endtask : readResponse


    task automatic sendCommand(input logic [5:0] cmd_number, input logic [31:0] cmd_argument, output logic [135:0] response, output logic resp_type);
        automatic logic [31:0] temp_data;
        @(posedge clk_100MHz);
        $display("Sending Command %0d with argument 0x%h\n", cmd_number, cmd_argument);

        writeRegister(SD_CMD_NUMBER, cmd_number);
        @(posedge clk_100MHz);
        writeRegister(SD_CMD_ARGUMENT, cmd_argument);
        @(posedge clk_100MHz);

        readRegister(SD_CTRL, temp_data);
        temp_data = temp_data | (1 << 9);
        @(posedge clk_100MHz);
        writeRegister(SD_CTRL, temp_data);
        repeat (10) @(posedge clk_100MHz);

        readResponse(response, response_type);
    endtask : sendCommand

    task automatic fillData();
        automatic sd_status_t status = '0;
        automatic logic [7:0] b0, b1, b2, b3;
        @(posedge clk_100MHz);
        $display("Filling Data Buffer...\n");

        readRegister(SD_STATUS, status);
        if (!status.tx_buffer_empty) begin
            $display("WARNING: Data buffer is not empty!\n");
        end

        for (logic [7:0] i = 0; status.tx_buffer_full == 1'b0; i = i + 4) begin
            b0 = i;
            b1 = i + 1;
            b2 = i + 2;
            b3 = i + 3;
            writeRegister(SD_DATA_TX_BUFFER, {b3, b2, b1, b0});
            
            @(posedge clk_100MHz);
            readRegister(SD_STATUS, status);
            @(posedge clk_100MHz);
        end

        $display("Data Buffer filled!\n");
    endtask : fillData
    
    task automatic readData();
        automatic logic [127:0][31:0] data_response = '0;
        automatic sd_status_t status = '0;
        @(posedge clk_100MHz);
        $display("Reading Data Buffer...\n");

        readRegister(SD_STATUS, status);
        while (!status.data_idle | !status.cmd_idle) begin
            readRegister(SD_STATUS, status);
            @(posedge clk_100MHz);
        end

        readRegister(SD_STATUS, status);
        if (status.data_crc_error) begin
            $display("WARNING: Data CRC Error!\n");
        end

        for (int i = 0; status.rx_buffer_empty == 1'b0; ++i) begin
            readRegister(SD_DATA_RX_BUFFER, data_response[i]);
            @(posedge clk_100MHz);
            readRegister(SD_STATUS, status);
            @(posedge clk_100MHz);
        end

        $display("Data Buffer read: 0x%h!\n", data_response);
    endtask : readData
    

    bit card_ready, response_type; bit [135:0] response; bit [15:0] rca_card; sd_status_t status;
    bit [31:0] temp_register;

    initial begin
        automatic int attempts = 0;
        
        /* Initial reset */
        rst_n_i <= 0;
        sd_detect <= 1;
        repeat (10) @(posedge clk_100MHz);
        rst_n_i <= 1;

        /* Clock stabilization */
        repeat (100) @(posedge sd_clk);
        repeat (10) @(posedge clk_100MHz);
        sd_detect <= 0; /* Simulate card detection */
        repeat (10) @(posedge clk_100MHz);


        writeRegister(SD_CTRL, 1 << 13); /* Reset card */
        #1ms;

        /* Enable FSMs */
        writeRegister(SD_CTRL, 1 << 12);

        /* CMD0: GO_IDLE_STATE */
        sendCommand(6'd0, 32'h00000000, response, response_type);

        /* CMD8: SEND_IF_COND */
        sendCommand(6'd8, 32'h000001AA, response, response_type);

        /* Initialization loop ACMD41 */
        do begin
            $display("Attempt %0d to initialize card...\n", attempts);
            /* CMD55: APP_CMD */
            sendCommand(6'd55, 32'h00000000, response, response_type);

            /* ACMD41: SEND_OP_COND */
            sendCommand(6'd41, 32'h40300000, response, response_type); /* HCS = 1 */

            /* Check if the card is ready */
            card_ready = response[39];
            attempts++;
        end while (!card_ready && attempts < 20);

        $display("Card ready!\n");

        /* Change frequency to 25 MHz */
        readRegister(SD_CTRL, temp_register);
        writeRegister(SD_CTRL, temp_register | (1 << 11));

        /* Wait to stabilize the clock */
        repeat (256) @(posedge clk_100MHz);

        /* CMD2: ALL_SEND_CID */
        sendCommand(6'd2, 32'h00000000, response, response_type);

        /* CMD3: SEND_RELATIVE_ADDR */
        sendCommand(6'd3, 32'h00000000, response, response_type);
        $display("CMD3 Done! Relative Card Address: 0x%h assigned. Response: 0x%h\n", response[39:24], response);

        /* CMD7: SELECT_CARD, Send RCA again */
        rca_card = response[39:24];
        sendCommand(6'd7, {response[39:24], 16'b0}, response, response_type);

        /* CMD6: SWITCH_FUNC, go into serial mode */
        sendCommand(6'd6, 32'h00000000, response, response_type);
        readData();


        /* Testing blocks */
        for (int i = 0; i < 32; ++i) begin
            /* Write i-th block in serial mode */
            $display("Writing to Block %0d in Serial Mode\n", i);

            fillData();
            sendCommand(6'd24, i, response, response_type);
            waitDataFSM();
            $display("Write done on Block %d in Serial Mode\n", i);

            /* Write i-th block in serial mode */
            $display("Reading Block %0d in Serial Mode\n", i);

            /* Read from block 0 in serial mode */
            sendCommand(6'd17, 32'h00000000, response, response_type);
            $display("Read done on Block %0d in Serial Mode\n", i);

            readData();
            @(posedge clk_100MHz);
        end


        /* ACMD6 go into wide bus mode */
        sendCommand(6'd55, {rca_card, 16'b0}, response, response_type);
        sendCommand(6'd6, 32'd2, response, response_type);
        readData();

        /* Set wide_bus to 1 */
        readRegister(SD_CTRL, temp_register);
        writeRegister(SD_CTRL, temp_register | (1 << 10));

        /* Testing blocks */
        for (int i = 0; i < 32; ++i) begin
            /* Write i-th block in wide mode */
            $display("Writing to Block %0d in Wide Bus Mode\n", i);

            fillData();
            sendCommand(6'd24, i, response, response_type);
            waitDataFSM();
            $display("Write done on Block %d in Wide Bus Mode\n", i);

            /* Write i-th block in serial mode */
            $display("Reading Block %0d in Wide Bus Mode\n", i);

            /* Read from block 0 in wide mode */
            sendCommand(6'd17, 32'h00000000, response, response_type);
            $display("Read done on Block %0d in Wide Bus Mode\n", i);
            readData();
        end


        /* Burst Write Test */
        $display("CMD25: Burst Write Test\n");
        fillData();
        sendCommand(6'd25, 32'h00000000, response, response_type);

        for (int i = 0; i < 4; i = i) begin
            readRegister(SD_STATUS, status_register);
            @(posedge clk_100MHz);


            if (status_register.tx_buffer_empty) begin
                $display("Filling Data Buffer for Burst Write...\n");
                fillData();
                ++i;
                $display("Data Buffer filled for Burst Write, iteration %0d\n", i);
            end
            
            @(posedge clk_100MHz);
        end

        $display("CMD12: Stopping Burst Write\n");
        sendCommand(6'd12, 32'h00000000, response, response_type);

        $display("CMD18: Burst Read Test\n");
        sendCommand(6'd18, 32'h00000000, response, response_type);
        
        for (int i = 0; i < 4; i = i) begin
            readRegister(SD_STATUS, status_register);
            @(posedge clk_100MHz);


            if (status_register.rx_buffer_full) begin
                $display("Reading Data Buffer for Burst Read...\n");

                for (int j = 0; j < 128; ++j) begin
                    readRegister(SD_DATA_RX_BUFFER, temp_register);
                    @(posedge clk_100MHz);
                    $display("Data Buffer read: 0x%h", temp_register);
                end

                ++i;
            end
            
            @(posedge clk_100MHz);
        end

        $display("CMD12: Stopping Burst Read\n");
        sendCommand(6'd12, 32'h00000000, response, response_type);
        waitDataFSM();

        $finish;
    end

endmodule : sd_interface_testbench