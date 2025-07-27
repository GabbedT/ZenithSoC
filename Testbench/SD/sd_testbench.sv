`include "Model/sd_top.v"

`timescale 1ns/1ps

module sd_testbench;

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
//      TX_BUFFER
//====================================================================================

    logic tx_write, tx_full, tx_empty;
    logic [7:0] tx_data_in, tx_data_out;

    synchronous_buffer #(
        .BUFFER_DEPTH           ( 512 ),
        .DATA_WIDTH             ( 8   ),
        .FIRST_WORD_FALL_TROUGH ( 0   )
    ) tx_buffer (
        .clk_i   ( clk_100MHz ),
        .rst_n_i ( rst_n_i    ),

        .write_i ( tx_write ),
        .read_i  ( tx_read  ),

        .empty_o  ( tx_empty ),
        .full_o   ( tx_full  ),

        .write_data_i ( tx_data_in  ),
        .read_data_o  ( tx_data_out )
    );

    logic rx_write, rx_full, rx_read;
    logic [7:0] rx_data_in, rx_data_out;

    synchronous_buffer #(
        .BUFFER_DEPTH           ( 512 ),
        .DATA_WIDTH             ( 8   ),
        .FIRST_WORD_FALL_TROUGH ( 0   )
    ) rx_buffer (
        .clk_i   ( clk_100MHz ),
        .rst_n_i ( rst_n_i    ),

        .write_i ( rx_write ),
        .read_i  ( rx_read  ),

        .empty_o  ( rx_empty ),
        .full_o   ( rx_full  ),

        .write_data_i ( rx_data_in  ),
        .read_data_o  ( rx_data_out )
    );
    
//====================================================================================
//      DUT
//====================================================================================

    wire sd_cmd;

    logic rst_n_i = 1'b0;
    logic shift_i;
    logic sample_i;

    logic cmd_valid_i;  
    logic [5:0] cmd_number_i;
    logic [31:0] cmd_argument_i;

    logic resp_valid_o;
    logic [7:0] resp_o;
    logic resp_type_o;
    logic status_valid_o;
    logic burst_o;

    logic idle_o;
    logic crc_error_o;
    logic timeout_o;

    logic force_stop;

    sd_command_controller dut1 (
      .clk_i   ( clk_100MHz ),
      .rst_n_i ( rst_n_i    ),

      .enable_i ( 1'b1     ),
      .shift_i  ( shift_i  ),
      .sample_i ( sample_i ),

      /* Command Interface */
      .cmd_valid_i   ( cmd_valid_i    ),
      .cmd_number_i  ( cmd_number_i   ),
      .cmd_argument_i( cmd_argument_i ),

      /* Response Interface */
      .resp_valid_o  ( resp_valid_o ),
      .resp_o        ( resp_o       ),
      .resp_type_o   ( resp_type_o  ),

      /* Status */
      .idle_o        ( idle_o      ),
      .crc_error_o   ( crc_error_o ),
      .timeout_o     ( timeout_o   ),

      .force_stop_o     ( force_stop  ),
      .data_activate_o  ( transfer_en ),
      .data_direction_o ( direction   ), /* 1 = read, 0 = write */
      .data_burst_o     ( burst_o     ), /* 1 = burst mode, 0 = single block */

      /* External Interface */
      .sd_cmd_io     (sd_cmd)
    );


    logic [7:0] tx_data, rx_data, tx_data_stimulus; logic rx_valid, crc_valid;
    logic direction, wide_bus, transfer_en, data_idle, crc_error, write_error, status_valid, write_ok;
    wire [3:0] sd_data; logic [7:0] token;

    logic drive_buffer;

    sd_data_controller dut2 (
        .clk_i   ( clk_100MHz ),
        .rst_n_i ( rst_n_i    ),

        .enable_i ( 1'b1       ),
        .shift_i  ( shift_i    ),
        .sample_i ( sample_i   ),

        .tx_data_i  ( drive_buffer ? tx_data_out : tx_data_stimulus ),
        .tx_empty_i ( tx_empty    ),
        .tx_read_o  ( tx_read     ),

        .rx_full_i   ( rx_full ),
        .rx_data_o   ( rx_data   ),
        .rx_valid_o  ( rx_valid  ),

        .force_stop_i ( force_stop ),
        .direction_i   ( direction   ),
        .wide_bus_i    ( wide_bus    ),
        .burst_i       ( burst_o      ),
        .transfer_en_i ( transfer_en ),

        .timeout_o     (  ),
        .idle_o         ( data_idle ),
        .crc_error_o    ( crc_error ),
        .token_o       ( token     ),

        .sd_data_io ( sd_data )
    );

    assign rx_write = rx_valid & !rx_full;
    assign rx_data_in = rx_data;

    logic reset_stim;

        always_ff @(posedge clk_100MHz) begin
            if (!rst_n_i | reset_stim) begin
                tx_data_stimulus <= 0;
            end else if (tx_read) begin
                tx_data_stimulus <= tx_data_stimulus + 1;
            end
        end

//====================================================================================
//      PULSE GENERATOR
//====================================================================================

    localparam FREQ_400KHZ = 125;
    localparam FREQ_25MHZ = 2;

    int divisor, counter; logic clk_previous, reset_counter; logic sd_clk;

    always_ff @(posedge clk_100MHz) begin
        if (!rst_n_i | reset_counter) begin
            counter <= 0;

            clk_previous <= 0;
            sd_clk <= 0;
        end else begin
            counter <= counter + 1;
            clk_previous <= sd_clk;

            if (counter == (divisor - 1)) begin
                sd_clk <= !sd_clk;
                counter <= 0;
            end
        end
    end 

    assign shift_i = !sd_clk & clk_previous;
    assign sample_i = sd_clk & !clk_previous;



//====================================================================================
//      SD CARD MEMORY
//====================================================================================

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

    /* SD Connection Command to DUT  */
    assign sd_cmd = sd_cmd_t ? dut1.cmd_bit_flop : sd_cmd_o;

    /* SD Connection Data to DUT */
    assign sd_data = (sd_dat_t == '1) ? dut2.data_line_flop : sd_dat_o;

    assign sd_cmd_wire = (sd_cmd_t == 1'b0) ? sd_cmd_o :  
                         (dut1.tristate_enable_flop) ? dut1.cmd_bit_flop : 1'bz;
    
    assign sd_cmd_i = sd_cmd_wire;

    assign sd_dat_wire = (sd_dat_t == '0) ? sd_dat_o :  
                         (dut2.tristate_enable_flop) ? dut2.data_line_flop : 4'bz;

    assign sd_dat_i = sd_dat_wire;
    

//====================================================================================
//      STIMULI
//====================================================================================

    task automatic sendCommand(input logic [5:0] cmd_number, input logic [31:0] cmd_argument);
        @(posedge clk_100MHz);
        cmd_valid_i = 1'b1;
        cmd_number_i = cmd_number;
        cmd_argument_i = cmd_argument;

        @(posedge clk_100MHz);
        cmd_valid_i = 1'b0;
        @(posedge clk_100MHz);
    endtask : sendCommand

    task automatic fillData();
        for (int i = 0; i < 512; i++) begin
            @(posedge clk_100MHz);
            tx_write = 1'b1;
            tx_data_in = i[7:0];
            @(posedge clk_100MHz);
            tx_write = 1'b0;
        end

        tx_write = 1'b0;
    endtask : fillData
    

    bit card_ready; bit [47:0] response; bit [15:0] rca_card;

        always_ff @(posedge clk_100MHz) begin
            if (resp_valid_o) begin
                response <= {response[39:0], resp_o};
            end
        end

    bit [31:0] expected_response;

    initial begin
        automatic int attempts = 0;
        drive_buffer = 1'b1; reset_stim = 1'b0;
        
        /* Initial reset */
        rst_n_i <= 0;
        divisor = FREQ_400KHZ; 
        repeat (10) @(posedge clk_100MHz);
        rst_n_i <= 1;

        /* Clock stabilization */
        repeat (100) @(posedge sd_clk);
        repeat (10) @(posedge clk_100MHz);

        /* CMD0: GO_IDLE_STATE */
        sendCommand(6'd0, 32'h00000000);
        wait (idle_o);
        $display("CMD0 Done! Response: 0x%h\n", response);


        /* CMD8: SEND_IF_COND */
        sendCommand(6'd8, 32'h000001AA);
        wait (idle_o);
        $display("CMD8 Done! Response: 0x%h\n", response);

        /* Initialization loop ACMD41 */
        do begin
            $display("Attempt %0d to initialize card...\n", attempts);
            /* CMD55: APP_CMD */
            sendCommand(6'd55, 32'h00000000);
            wait (idle_o);

            /* ACMD41: SEND_OP_COND */
            sendCommand(6'd41, 32'h40300000); /* HCS = 1 */
            wait (idle_o);

            /* Check if the card is ready */
            card_ready = (response[39] == 1'b1);
            attempts++;
        end while (!card_ready && attempts < 20);

        $display("Card ready!\n");

        /* Change frequency to 25 MHz */
        repeat (20) @(posedge clk_100MHz);
        divisor <= FREQ_25MHZ;
        reset_counter <= 1'b1;
        @(posedge clk_100MHz);
        reset_counter <= 1'b0;
        repeat (20) @(posedge clk_100MHz);

        /* Set wide_bus to 0 */
        wide_bus <= 1'b0;

        /* CMD2: ALL_SEND_CID */
        sendCommand(6'd2, 32'h00000000);
        wait (idle_o);
        $display("CMD2 Done! Response: 0x%h\n", response);


        /* CMD3: SEND_RELATIVE_ADDR */
        sendCommand(6'd3, 32'h00000000);
        wait (idle_o);
        $display("CMD3 Done! Relative Card Address: 0x%h assigned. Response: 0x%h\n", response[39:24], response);

        /* CMD7: SELECT_CARD, Send RCA again */
        rca_card = response[39:24];
        sendCommand(6'd7, {response[39:24], 16'b0});
        wait (idle_o);
        $display("CMD7 Done! Response: 0x%h\n", response);

        /* CMD6: SWITCH_FUNC, go into serial mode */
        sendCommand(6'd6, 32'h00000000);
        wait (idle_o);
        repeat(2) @(posedge clk_100MHz);
        wait (data_idle);
        repeat(100) @(posedge clk_100MHz);
        $display("CMD6 Done! Serial mode, response: 0x%h\n", response);


        /* Testing blocks */
        for (int i = 0; i < 32; ++i) begin
            /* Write i-th block in serial mode */
            $display("Writing to Block %0d in Serial Mode\n", i);

            fillData();
            sendCommand(6'd24, i);
            wait (idle_o);

            repeat(2) @(posedge clk_100MHz);
            wait (data_idle);
            $display("Write done on Block 0 in Serial Mode\n");

            /* Write i-th block in serial mode */
            $display("Reading Block %0d in Serial Mode\n", i);

            /* Read from block 0 in serial mode */
            sendCommand(6'd17, 32'h00000000);
            wait (dut1.state_CRT == dut1.RECOVERY);
            repeat(2) @(posedge clk_100MHz);
            wait (data_idle);
            $display("Read done on Block %0d in Serial Mode\n", i);

            expected_response = 0;

            while (expected_response <= 512) begin
                rx_read = 1'b1;
                @(posedge clk_100MHz);

                if (rx_data_out != expected_response[7:0]) begin
                    $display("Data mismatch at Block %0d: Expected 0x%h, Received 0x%h\n", i, expected_response[7:0], rx_data_out);
                end

                expected_response++;
            end

            rx_read = 1'b0;
        end


        /* ACMD6 go into wide bus mode */
        sendCommand(6'd55, {rca_card, 16'b0});
        wait (idle_o);
        repeat(2) @(posedge clk_100MHz);
        sendCommand(6'd6, 32'd2);
        wait (idle_o);
        $display("CMD6 Done! Wide bus mode, response: 0x%h\n", response);
        wait (data_idle);
        repeat(100) @(posedge clk_100MHz);
        transfer_en = 1'b0;

        /* Set wide_bus to 1 */
        wide_bus <= 1'b1;

        /* Testing blocks */
        for (int i = 0; i < 32; ++i) begin
            /* Write i-th block in serial mode */
            $display("Writing to Block %0d in Wide Bus Mode\n", i);

            fillData();
            sendCommand(6'd24, i);
            wait (idle_o);

            repeat(2) @(posedge clk_100MHz);
            wait (data_idle);
            $display("Write done on Block 0 in Wide Bus Mode\n");

            /* Write i-th block in serial mode */
            $display("Reading Block %0d in Wide Bus Mode\n", i);

            /* Read from block 0 in serial mode */
            sendCommand(6'd17, 32'h00000000);
            wait (dut1.state_CRT == dut1.RECOVERY);
            repeat(2) @(posedge clk_100MHz);
            wait (data_idle);
            $display("Read done on Block %0d in Wide Bus Mode\n", i);

            expected_response = 0;
            while (expected_response <= 512) begin
                rx_read = 1'b1;
                @(posedge clk_100MHz);

                if (rx_data_out != expected_response[7:0]) begin
                    $display("Data mismatch at Block %0d: Expected 0x%h, Received 0x%h\n", i, expected_response[7:0], rx_data_out);
                end

                expected_response++;
            end

            rx_read = 1'b0;
        end


        /* Burst Write Test */
        $display("CMD25: Burst Write Test\n");
        sendCommand(6'd25, 32'h00000000);

        expected_response = 0; reset_stim = 1'b1;
        tx_empty = 1'b0; drive_buffer = 1'b0;
        @(posedge clk_100MHz);
        reset_stim = 1'b0;

        while (expected_response <= 2048) begin
            wait (tx_read);
            ++expected_response;
            @(posedge clk_100MHz);
        end

        $display("CMD12: Stopping Burst Write\n");
        sendCommand(6'd12, 32'h00000000);
        wait(idle_o); 
        @(posedge clk_100MHz);


        $display("CMD18: Burst Read Test\n");
        sendCommand(6'd18, 32'h00000000);
        #500us;

        $display("CMD12: Stopping Burst Read\n");
        sendCommand(6'd12, 32'h00000000);
        wait(idle_o); 

        $finish;
    end

endmodule : sd_testbench