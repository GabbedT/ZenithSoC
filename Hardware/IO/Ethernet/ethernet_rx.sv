`ifndef ETHERNET_RX_SV
    `define ETHERNET_RX_SV

`include "../../Utility/Packages/ethernet_pkg.sv"

`include "ethernet_crc32.sv"

module ethernet_rx #(
    parameter logic [5:0][7:0] MAC_ADDRESS = 48'h00_00_00_00_00_00
) (
    /* Global signals */
    input logic clk_i,
    input logic rst_n_i,
    input logic sample_i,
    input logic enable_i, 
    input logic ethernet_II_i,
    input eth_speed_t speed_i, 

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


    /* Counter to select the payload bits */
    logic [1:0] bit_counter; logic bit_increment, bit_reset;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
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


    logic [1:0][7:0] length; logic [5:0][7:0] address;

        /* Invert bytes */
        always_comb begin
            if (ethernet_II_i) begin
                length = {'0, bytes_counter};
            end else begin
                length = length_CRT;
            end
            address = address_NXT;

            for (int i = 0; i < 6; ++i) begin
                source_address_o[i] = address[5 - i];
            end

            for (int i = 0; i < 2; ++i) begin
                lenght_type_o[i] = length[1 - i];
            end
        end


    /* Counter for IPG */
    logic [1:0] ipg_bit_counter; logic ipg_bit_increment, ipg_bit_reset;
    logic [3:0] ipg_bytes_counter; logic ipg_byte_increment, ipg_byte_reset;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                ipg_bit_counter <= '0;
                ipg_bytes_counter <= '0;
            end else begin 
                if (ipg_bit_reset) begin
                    ipg_bit_counter <= '0;
                end else if (ipg_bit_increment) begin 
                    ipg_bit_counter <= ipg_bit_counter + 1'b1;
                end 

                if (ipg_byte_reset) begin
                    ipg_bytes_counter <= '0;
                end else if (ipg_byte_increment) begin 
                    ipg_bytes_counter <= ipg_bytes_counter + 1'b1;
                end 
            end 
        end


    logic crsdv_deasserted;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                crsdv_deasserted <= 1'b0;
            end else if (sample_i) begin 
                crsdv_deasserted <= !rmii_crsdv_i;
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

    /* Save the last 4 CRCs */
    logic [3:0][31:0] crc32_saved; logic shift_crc, shift_crc_nxt; logic [3:0][7:0] final_crc;

        always_ff @(posedge clk_i) begin
            shift_crc_nxt <= shift_crc;

            if (shift_crc_nxt) begin
                crc32_saved <= {crc32_saved[2:0], crc32};
            end
        end 

    assign final_crc = crc32_saved[3] ^ '1;


//====================================================================================
//      FSM LOGIC
//====================================================================================

    typedef enum logic [3:0] {IDLE, PREAMBLE, SDF, MAC_DESTINATION, MAC_SOURCE, ETH_TYPE, PAYLOAD, WAIT_IPG, WAIT_END} ethernet_tx_states_t;

    ethernet_tx_states_t state_CRT, state_NXT;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                state_CRT <= IDLE;
            end else if (rmii_rxer_i | (!rmii_crsdv_i & (state_CRT != PAYLOAD & state_CRT != WAIT_IPG))) begin
                state_CRT <= IDLE;
            end else if (enable_i) begin 
                state_CRT <= state_NXT;
            end 
        end 


    logic payload_write, packet_write;

        always_comb begin
            /* Default values */
            state_NXT = state_CRT;
            length_NXT = length_CRT;
            address_NXT = address_CRT;
            payload_NXT = payload_CRT;
            frame_check_NXT = frame_check_CRT;

            idle_o = 1'b0;
            packet_write = 1'b0;
            payload_write = 1'b0;
            packet_error_o = 1'b0;
            
            shift_crc = 1'b0;
            crc32_init = 1'b0;
            crc32_compute = 1'b0;
            crc32_data = '0;
            byte_reset = 1'b0;
            byte_increment = 1'b0;
            bit_increment = 1'b0;
            bit_reset = 1'b0;
            ipg_byte_reset = 1'b0;
            ipg_byte_increment = 1'b0;
            ipg_bit_increment = 1'b0;
            ipg_bit_reset = 1'b0;


            case (state_CRT)
                /* Reset counters and CRC, if the carrier is detected
                 * the PHY is receiving data */
                IDLE: begin
                    idle_o = 1'b1;

                    byte_reset = 1'b1;
                    bit_reset = 1'b1;

                    crc32_init = 1'b1;

                    if (rmii_crsdv_i) begin
                        state_NXT = (speed_i == MBPS10) ? SDF : PREAMBLE;

                        bit_reset = 1'b0;
                    end
                end


                /* Check for 7 bytes of alternating 1 and 0 */
                PREAMBLE: begin
                    idle_o = 1'b1;
                    
                    if (sample_i & (rmii_rxd_i == 2'b01)) begin
                        bit_increment = 1'b1;

                        if (bit_counter == '1) begin
                            if (bytes_counter == (PREAMBLE_BYTES - 1)) begin
                                state_NXT = SDF; 

                                byte_reset = 1'b1;
                                bit_reset = 1'b1;
                            end

                            byte_increment = 1'b1;
                            bit_reset = 1'b1;
                        end
                    end
                end


                /* Last byte before actual useful data, it marks the 
                 * start of a frame */
                SDF: begin
                    idle_o = 1'b1;

                    if (sample_i & ((rmii_rxd_i == 2'b01) | ((rmii_rxd_i == 2'b11)))) begin
                        bit_increment = 1'b1;

                        if (bit_counter == '1) begin 
                            state_NXT = MAC_DESTINATION;

                            byte_reset = 1'b1;
                            bit_reset = 1'b1;
                        end 
                    end
                end


                /* Shift bits into the address register, once all the
                 * bits are received compare the MAC address with the 
                 * received MAC, if they don't match, wait for the 
                 * end of the frame */
                MAC_DESTINATION: begin
                    idle_o = 1'b1;

                    if (sample_i) begin
                        bit_increment = 1'b1;

                        address_NXT = {rmii_rxd_i, address_CRT[47:2]}; 

                        if (bit_counter == '1) begin 
                            if (bytes_counter == (MAC_ADDR_BYTES - 1)) begin
                                if ((source_address_o == MAC_ADDRESS) | (source_address_o == BROADCAST_ADDRESS)) begin
                                    state_NXT = MAC_SOURCE;
                                end else begin
                                    state_NXT = WAIT_END;
                                end

                                byte_reset = 1'b1;
                                bit_reset = 1'b1;
                            end

                            /* Compute the CRC */
                            crc32_compute = 1'b1;
                            crc32_data = address_NXT[47:40];

                            byte_increment = 1'b1;
                            bit_reset = 1'b1;
                        end 
                    end
                end


                /* Save the source MAC address inside the address
                 * shift register */
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

                            /* Compute the CRC */
                            crc32_compute = 1'b1;
                            crc32_data = address_NXT[47:40];

                            byte_increment = 1'b1;
                            bit_reset = 1'b1;
                        end 
                    end
                end


                /* Receive the ethernet type / payload lenght information, 
                 * the FSM will use it to determine the end of the payload. */
                ETH_TYPE: begin
                    if (sample_i) begin
                        bit_increment = 1'b1;

                        length_NXT = {rmii_rxd_i, length_CRT[15:2]}; 
                        payload_NXT = {rmii_rxd_i, payload_CRT[7:2]}; 

                        if (bit_counter == '1) begin 
                            if (bytes_counter == (ETH_TYPE_BYTES - 1)) begin
                                state_NXT = PAYLOAD;   

                                byte_reset = 1'b1;
                                bit_reset = 1'b1;                        
                            end 

                            if (ethernet_II_i) begin
                                payload_write = 1'b1;
                            end

                            /* Compute the CRC */
                            crc32_compute = 1'b1;
                            crc32_data = length_NXT[15:8];

                            byte_increment = 1'b1;
                            bit_reset = 1'b1;
                        end 
                    end
                end


                /* Actual data carried by the Ethernet frame. The payload 
                 * contains also the CRC computed by the transmitter, which
                 * is compared at the end of the reception. */
                PAYLOAD: begin
                    if (rmii_crsdv_i) begin
                        if (sample_i) begin
                            bit_increment = 1'b1;

                            payload_NXT = {rmii_rxd_i, payload_CRT[7:2]}; 
                            frame_check_NXT = {frame_check_CRT[29:0], rmii_rxd_i[0], rmii_rxd_i[1]}; 

                            if (bit_counter == '1) begin 
                                /* Compute the CRC */
                                crc32_compute = 1'b1;
                                crc32_data = payload_NXT[7:0];
                                shift_crc = 1'b1;

                                payload_write = 1'b1;

                                byte_increment = 1'b1;
                                bit_reset = 1'b1;
                            end 
                        end
                    end else begin
                        state_NXT = WAIT_IPG;

                        packet_write = 1'b1;  
                    end
                end


                WAIT_IPG: begin
                    if (rmii_crsdv_i) begin
                        state_NXT = PAYLOAD;

                        ipg_byte_reset = 1'b1;
                        ipg_bit_reset = 1'b1;
                    end else begin
                        if (sample_i) begin
                            bit_increment = 1'b1;

                            /* Keep saving data in case of a temporary loss of CRSDV signal */
                            payload_NXT = {rmii_rxd_i, payload_CRT[7:2]};

                            if (bit_counter == '1) begin 
                                /* Compute the CRC */
                                crc32_compute = 1'b1;
                                crc32_data = payload_NXT[7:0];
                                shift_crc = 1'b1;

                                byte_increment = 1'b1;
                                bit_reset = 1'b1;
                            end 


                            ipg_bit_increment = 1'b1;

                            if (ipg_bit_counter == '1) begin
                                if (ipg_bytes_counter == (IPG_BYTES - 1)) begin
                                    state_NXT = IDLE; 

                                    idle_o = 1'b1;

                                    packet_error_o = frame_check_CRT != final_crc;

                                    ipg_byte_reset = 1'b1;
                                    ipg_bit_reset = 1'b1;
                                end else begin 
                                    ipg_byte_increment = 1'b1;

                                    ipg_bit_reset = 1'b1;
                                end
                            end 
                        end
                    end
                end


                WAIT_END: begin
                    idle_o = 1'b1;
                end
            endcase 
        end


        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                payload_valid_o <= 1'b0;
                packet_valid_o <= 1'b0;
            end else begin 
                payload_valid_o <= payload_write;
                packet_valid_o <= packet_write;
            end 
        end 

endmodule : ethernet_rx 

`endif