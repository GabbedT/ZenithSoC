`ifndef ETHERNET_RX_SV
    `define ETHERNET_RX_SV

`include "../../Utility/Packages/ethernet_pkg.sv"

`include "ethernet_crc32.sv"

module ethernet_rx #(
    parameter logic [5:0][7:0] MAC_ADDRESS = 48'hFF_FF_FF_FF_FF_FF
) (
    /* Global signals */
    input logic clk_i,
    input logic rst_n_i,
    input logic sample_i,

    /* Packet info */
    output logic [5:0][7:0] source_address_o,
    output logic [1:0][7:0] lenght_type_o,
    output logic [7:0] payload_o,
    output logic packet_error_o,

    /* RMII Interface */
    input logic [1:0] rmii_rxd_i,
    input logic rmii_crsdv_i,
    input logic rmii_rxer_i,

    /* FSM Status */
    output logic idle_o,
    output logic packet_valid_o,
    output logic payload_valid_o
);

//====================================================================================
//      DATAPATH REGISTERS
//====================================================================================

    /* Save the value of Ethernet frame fields */
    logic [47:0] address_CRT, address_NXT;
    logic [15:0] length_CRT, length_NXT;
    logic [7:0] payload_CRT, payload_NXT;
    logic [31:0] frame_check_CRT, frame_check_NXT;

        always_ff @(posedge clk_i) begin
            frame_check_CRT <= frame_check_NXT;
            address_CRT <= address_NXT;
            payload_CRT <= payload_NXT;
            length_CRT <= length_NXT;
        end 

    assign payload_o = payload_CRT;


    logic [1:0][7:0] length; logic [5:0][7:0] address;

        /* Invert bytes */
        always_comb begin
            length = length_CRT;
            address = address_NXT;

            for (int i = 0; i < 6; ++i) begin
                source_address_o[i] = address[5 - i];
            end

            for (int i = 0; i < 2; ++i) begin
                lenght_type_o[i] = length[1 - i];
            end
        end


    /* Counter to select the payload bits */
    logic [1:0] bit_counter; logic bit_increment, bit_reset;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                bit_counter <= '0;
            end else begin 
                if (bit_reset) begin
                    bit_counter <= '0;
                end if (bit_increment) begin 
                    bit_counter <= bit_counter + 1'b1;
                end 
            end 
        end 


    /* Counter to count the bytes sent */
    logic [10:0] bytes_counter; logic byte_increment, byte_reset;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                bytes_counter <= '0;
            end else begin 
                if (byte_reset) begin
                    bytes_counter <= '0;
                end else if (byte_increment) begin
                    bytes_counter <= bytes_counter + 1'b1;
                end
            end 
        end 


    logic crc32_init, crc32_compute; logic [7:0] crc32_data; logic [3:0][7:0] crc32;

    ethernet_crc32 crc32_engine (
        .clk_i   ( clk_i   ),
        .rst_n_i ( rst_n_i ),

        .initialize_i ( crc32_init    ),
        .compute_i    ( crc32_compute ),
        .data_i       ( crc32_data    ),

        .crc32_o ( crc32 )
    );


//====================================================================================
//      FSM LOGIC
//====================================================================================

    typedef enum logic [3:0] {IDLE, PREAMBLE, START_FRAME_DELIMITER, MAC_DESTINATION, MAC_SOURCE, ETH_TYPE, PAYLOAD, FRAME_CHECK_SEQUENCE, INTER_PACKET_GAP, WAIT_END} ethernet_tx_states_t;

    ethernet_tx_states_t state_CRT, state_NXT;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                state_CRT <= IDLE;
            end else if (rmii_rxer_i) begin
                state_CRT <= IDLE;
            end else begin 
                state_CRT <= state_NXT;
            end 
        end 


        always_comb begin
            /* Default values */
            state_NXT = state_CRT;
            address_NXT = address_CRT;
            payload_NXT = payload_CRT;
            length_NXT = length_CRT;
            frame_check_NXT = frame_check_CRT;

            idle_o = 1'b0;
            packet_valid_o = 1'b0;
            payload_valid_o = 1'b0;
            packet_error_o = 1'b0;

            crc32_init = 1'b0;
            crc32_compute = 1'b0;
            crc32_data = '0;
            byte_reset = 1'b0;
            byte_increment = 1'b0;
            bit_increment = 1'b0;
            bit_reset = 1'b0;

            case (state_CRT)
                IDLE: begin
                    idle_o = 1'b1;

                    byte_reset = 1'b1;
                    bit_reset = 1'b1;

                    crc32_init = 1'b1;

                    if (rmii_crsdv_i) begin
                        state_NXT = PREAMBLE;

                        bit_reset = 1'b0;
                    end
                end


                PREAMBLE: begin
                    if (sample_i) begin
                        bit_increment = 1'b1;

                        if (bit_counter == '1) begin
                            if (bytes_counter == (PREAMBLE_BYTES - 1)) begin
                                state_NXT = START_FRAME_DELIMITER; 

                                byte_reset = 1'b1;
                                bit_reset = 1'b1;
                            end

                            byte_increment = 1'b1;
                        end
                    end
                end


                START_FRAME_DELIMITER: begin
                    if (sample_i) begin
                        bit_increment = 1'b1;

                        if (bit_counter == '1) begin 
                            state_NXT = MAC_DESTINATION;

                            byte_reset = 1'b1;
                            bit_reset = 1'b1;
                        end 
                    end
                end


                MAC_DESTINATION: begin
                    if (sample_i) begin
                        bit_increment = 1'b1;

                        address_NXT = {rmii_rxd_i, address_CRT[47:2]}; 

                        if (bit_counter == '1) begin 
                            if (bytes_counter == (MAC_ADDR_BYTES - 1)) begin
                                if (source_address_o == MAC_ADDRESS) begin
                                    state_NXT = MAC_SOURCE;
                                end else begin
                                    state_NXT = WAIT_END;
                                end

                                byte_reset = 1'b1;
                                bit_reset = 1'b1;
                            end

                            crc32_compute = 1'b1;
                            crc32_data = address_NXT[47:40];

                            byte_increment = 1'b1;
                        end 
                    end
                end


                MAC_SOURCE: begin
                    if (sample_i) begin
                        bit_increment = 1'b1;

                        address_NXT = {rmii_rxd_i, address_CRT[47:2]}; 

                        if (bit_counter == '1) begin 
                            if (bytes_counter == (MAC_ADDR_BYTES - 1)) begin
                                state_NXT = ETH_TYPE;   

                                byte_reset = 1'b1;
                                bit_reset = 1'b1;     
                            end                    

                            crc32_compute = 1'b1;
                            crc32_data = address_NXT[47:40];

                            byte_increment = 1'b1;
                        end 
                    end
                end


                ETH_TYPE: begin
                    if (sample_i) begin
                        bit_increment = 1'b1;

                        length_NXT = {rmii_rxd_i, length_CRT[15:2]}; 

                        if (bit_counter == '1) begin 
                            if (bytes_counter == (ETH_TYPE_BYTES - 1)) begin
                                state_NXT = PAYLOAD;   

                                byte_reset = 1'b1;
                                bit_reset = 1'b1;                        
                            end 

                            crc32_compute = 1'b1;
                            crc32_data = length_NXT[15:8];

                            byte_increment = 1'b1;
                        end 
                    end
                end


                PAYLOAD: begin
                    if (sample_i) begin
                        bit_increment = 1'b1;

                        payload_NXT = {rmii_rxd_i, payload_CRT[7:2]}; 

                        if (bit_counter == '1) begin 
                            if (bytes_counter == (lenght_type_o - 1)) begin
                                state_NXT = FRAME_CHECK_SEQUENCE; 

                                byte_reset = 1'b1;
                                bit_reset = 1'b1;                          
                            end 

                            crc32_compute = 1'b1;
                            crc32_data = payload_NXT[7:0];

                            payload_valid_o = 1'b1;

                            byte_increment = 1'b1;
                        end 
                    end
                end


                FRAME_CHECK_SEQUENCE: begin
                    if (sample_i) begin
                        bit_increment = 1'b1;

                        frame_check_NXT = {rmii_rxd_i, frame_check_CRT[31:2]}; 

                        if (bit_counter == '1) begin 
                            if (bytes_counter == (CRC_BYTES - 1)) begin
                                state_NXT = INTER_PACKET_GAP;    

                                byte_reset = 1'b1;
                                bit_reset = 1'b1;                       
                            end 

                            byte_increment = 1'b1;
                        end 
                    end
                end


                INTER_PACKET_GAP: begin
                    if (sample_i) begin
                        bit_increment = 1'b1;

                        if (bit_counter == '1) begin 
                            if (bytes_counter == (IPG_BYTES - 1)) begin
                                state_NXT = IDLE;

                                packet_valid_o = 1'b1;  
                                packet_error_o = frame_check_CRT != crc32;

                                idle_o = 1'b1;       

                                byte_reset = 1'b1;
                                bit_reset = 1'b1;                  
                            end 

                            byte_increment = 1'b1;
                        end 
                    end
                end


                WAIT_END: begin
                    idle_o = 1'b1;
                    
                    if (!rmii_crsdv_i) begin
                        state_NXT = IDLE;
                    end
                end
            endcase 
        end

endmodule : ethernet_rx 

`endif