`ifndef ETHERNET_TX_SV
    `define ETHERNET_TX_SV

`include "../../Utility/Packages/ethernet_pkg.sv"

`include "ethernet_crc32.sv"

module ethernet_tx #(
    parameter logic [5:0][7:0] MAC_ADDRESS = 48'h00_00_00_00_00_00
) (
    /* Global signals */
    input logic clk_i,
    input logic rst_n_i,
    input logic transmit_i,

    /* Packet info */
    input logic [5:0][7:0] dest_address_i,
    input logic [1:0][7:0] payload_length_i,

    /* Data */
    output logic read_data_o,
    output logic read_descriptor_o,
    input logic data_ready_i,
    input logic [7:0] payload_data_i,

    /* RMII Interface */
    output logic [1:0] rmii_txd_o,
    output logic rmii_txen_o,

    /* Status */
    output logic idle_o
);

//====================================================================================
//      DATAPATH REGISTERS
//====================================================================================

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


    /* Holds the next byte to send */
    logic [7:0] data_shift, load_data, saved_data; logic next, load;

        always_ff @(posedge clk_i) begin
            if (load) begin
                data_shift <= load_data;
            end else if (next) begin
                data_shift <= data_shift >> 2;
            end

            if (load) begin
                saved_data <= load_data;
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

    typedef enum logic [3:0] {IDLE, PREAMBLE, START_FRAME_DELIMITER, MAC_DESTINATION, MAC_SOURCE, ETH_TYPE, PAYLOAD, FRAME_CHECK_SEQUENCE, INTER_PACKET_GAP} ethernet_tx_states_t;

    ethernet_tx_states_t state_CRT, state_NXT;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                state_CRT <= IDLE;
            end else begin 
                state_CRT <= state_NXT;
            end 
        end 


    logic [5:0][7:0] inv_dest_address, inv_src_address; logic [3:0][7:0] final_crc;

    /* Final CRC32 calculation */
    assign final_crc = crc32 ^ '1;

        /* Invert bytes */
        always_comb begin
            for (int i = 0; i < 6; ++i) begin
                inv_dest_address[i] = dest_address_i[5 - i];
                inv_src_address[i] = MAC_ADDRESS[5 - i];
            end
        end


        always_comb begin
            /* Default values */
            state_NXT = state_CRT;

            idle_o = 1'b0;
            read_data_o = 1'b0;
            read_descriptor_o = 1'b0;

            rmii_txen_o = 1'b1;
            rmii_txd_o = '0;

            crc32_init = 1'b0;
            crc32_compute = 1'b0;
            crc32_data = '0;
            byte_reset = 1'b0;
            byte_increment = 1'b0;
            bit_increment = 1'b0;
            bit_reset = 1'b0;
            load_data = '0;
            load = 1'b0;
            next = 1'b0;

            case (state_CRT)
                /* Reset counters and CRC, if the carrier is detected
                 * the PHY is receiving data */
                IDLE: begin
                    idle_o = 1'b1;
                    rmii_txen_o = 1'b0;

                    byte_reset = 1'b1;
                    bit_reset = 1'b1;

                    crc32_init = 1'b1;

                    if (data_ready_i & transmit_i) begin
                        idle_o = 1'b0;

                        state_NXT = PREAMBLE;

                        read_descriptor_o = 1'b1;
                        read_data_o = 1'b1;
                    end
                end


                /* Transmit 7 bytes of alternating 1 and 0 */
                PREAMBLE: begin
                    rmii_txd_o = 2'b01;

                    if (transmit_i) begin
                        bit_increment = 1'b1;

                        if (bit_counter == '1) begin
                            if (bytes_counter == (PREAMBLE_BYTES - 1)) begin
                                state_NXT = START_FRAME_DELIMITER; 

                                byte_reset = 1'b1;
                                bit_reset = 1'b1;
                            end else begin 
                                byte_increment = 1'b1;

                                bit_reset = 1'b1;
                            end
                        end
                    end
                end


                /* Last byte before actual useful data, it marks the 
                 * start of a frame */
                START_FRAME_DELIMITER: begin
                    if (bit_counter == '1) begin
                        rmii_txd_o = 2'b11;
                    end else begin
                        rmii_txd_o = 2'b01;
                    end

                    if (transmit_i) begin
                        bit_increment = 1'b1;

                        if (bit_counter == '1) begin 
                            state_NXT = MAC_DESTINATION;

                            byte_reset = 1'b1;
                            bit_reset = 1'b1;

                            /* Load the destination address */
                            load_data = inv_dest_address[0];
                            load = 1'b1;
                        end 
                    end
                end


                /* Transmit the destination MAC address */
                MAC_DESTINATION: begin
                    rmii_txd_o = data_shift[1:0];

                    if (transmit_i) begin
                        bit_increment = 1'b1;

                        next = 1'b1;

                        if (bit_counter == '1) begin
                            if (bytes_counter == (MAC_ADDR_BYTES - 1)) begin
                                state_NXT = MAC_SOURCE; 

                                byte_reset = 1'b1;
                                bit_reset = 1'b1;

                                /* Load the source address */
                                load_data = inv_src_address[0];
                                load = 1'b1;
                            end else begin 
                                byte_increment = 1'b1;

                                bit_reset = 1'b1;

                                /* Load the n-th byte of the destination address */
                                load_data = inv_dest_address[bytes_counter[2:0] + 1];
                                load = 1'b1;
                            end

                            /* Compute the CRC */
                            crc32_compute = 1'b1;
                            crc32_data = inv_dest_address[bytes_counter[2:0]];
                        end
                    end
                end


                /* Transmit the source MAC address */
                MAC_SOURCE: begin
                    rmii_txd_o = data_shift[1:0];

                    if (transmit_i) begin
                        bit_increment = 1'b1;

                        next = 1'b1;

                        if (bit_counter == '1) begin
                            if (bytes_counter == (MAC_ADDR_BYTES - 1)) begin
                                state_NXT = ETH_TYPE; 

                                byte_reset = 1'b1;
                                bit_reset = 1'b1;

                                /* Load the payload lenght info */
                                load_data = payload_length_i[1];
                                load = 1'b1;
                            end else begin 
                                byte_increment = 1'b1;

                                bit_reset = 1'b1;

                                /* Load the n-th byte of the source address */
                                load_data = inv_src_address[bytes_counter[2:0] + 1];
                                load = 1'b1;
                            end

                            /* Compute the CRC */
                            crc32_compute = 1'b1;
                            crc32_data = inv_src_address[bytes_counter[2:0]];
                        end
                    end
                end


                /* Transmit the ethernet type / payload lenght information. */
                ETH_TYPE: begin
                    rmii_txd_o = data_shift[1:0];

                    if (transmit_i) begin
                        bit_increment = 1'b1;
                        
                        next = 1'b1;

                        if (bit_counter == '1) begin
                            if (bytes_counter == (ETH_TYPE_BYTES - 1)) begin
                                state_NXT = PAYLOAD; 

                                byte_reset = 1'b1;
                                bit_reset = 1'b1;

                                /* Load payload */
                                load_data = payload_data_i;
                                load = 1'b1;

                                read_data_o = 1'b1;
                            end else begin 
                                byte_increment = 1'b1;

                                bit_reset = 1'b1;

                                /* Load next byte of the payload */
                                load_data = payload_length_i[0];
                                load = 1'b1;
                            end

                            /* Compute the CRC */
                            crc32_compute = 1'b1;
                            crc32_data = payload_length_i[!bytes_counter[0]];
                        end
                    end
                end

                
                /* Actual data carried by the Ethernet frame */
                PAYLOAD: begin
                    rmii_txd_o = data_shift[1:0];

                    if (transmit_i) begin
                        bit_increment = 1'b1;
                        
                        next = 1'b1;

                        if (bit_counter == '1) begin
                            if (bytes_counter == (payload_length_i - 1)) begin
                                state_NXT = FRAME_CHECK_SEQUENCE; 

                                byte_reset = 1'b1;
                                bit_reset = 1'b1;
                            end else begin 
                                byte_increment = 1'b1;

                                bit_reset = 1'b1;

                                load_data = payload_data_i;
                                load = 1'b1;

                                read_data_o = 1'b1;
                            end

                            crc32_compute = 1'b1;
                            crc32_data = saved_data;
                        end 
                    end
                end


                /* Transmit computed CRC sequence */
                FRAME_CHECK_SEQUENCE: begin
                    if ((bit_counter == '0) & (bytes_counter[1:0] == '0)) begin
                        rmii_txd_o = final_crc[0][1:0];

                        /* Load CRC data */
                        load_data = {2'b0, final_crc[0][7:2]};
                        load = 1'b1;
                    end else begin 
                        rmii_txd_o = data_shift[1:0];
                    end

                    if (transmit_i) begin
                        bit_increment = 1'b1;

                        next = 1'b1;

                        if (bit_counter == '1) begin
                            if (bytes_counter[1:0] == (CRC_BYTES - 1)) begin
                                state_NXT = INTER_PACKET_GAP; 

                                byte_reset = 1'b1;
                                bit_reset = 1'b1;
                            end else begin
                                byte_increment = 1'b1;

                                bit_reset = 1'b1;

                                load_data = final_crc[bytes_counter + 1];
                                load = 1'b1;
                            end
                        end 
                    end
                end


                /* Between two packets there are 12 bytes of time. */
                INTER_PACKET_GAP: begin
                    rmii_txen_o = 1'b0;
                    
                    if (transmit_i) begin
                        bit_increment = 1'b1;

                        if (bit_counter == '1) begin
                            if (bytes_counter[3:0] == (IPG_BYTES - 1)) begin
                                state_NXT = IDLE; 

                                idle_o = 1'b1;

                                byte_reset = 1'b1;
                                bit_reset = 1'b1;
                            end else begin 
                                byte_increment = 1'b1;

                                bit_reset = 1'b1;
                            end
                        end 
                    end
                end
            endcase 
        end

endmodule : ethernet_tx

`endif 