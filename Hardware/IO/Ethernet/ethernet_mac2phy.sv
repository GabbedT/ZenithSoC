`ifndef ETHERNET_MAC2PHY_SV
    `define ETHERNET_MAC2PHY_SV

module ethernet_mac2phy #(
    parameter logic [4:0] CHIP_PHY_ADDRESS = 5'b00001
) (
    /* Global signals */
    input logic clk_i,
    input logic rst_n_i,
    input logic pulse_i,

    /* Register interface */
    input logic [4:0] address_i,
    input logic write_i,
    input logic read_i,
    input logic [15:0] data_i,
    output logic [15:0] data_o,

    /* SMII interface */
    output logic smii_mdc_o,
    inout logic smii_mdio_io,

    output logic done_o
);

//====================================================================================
//      DATAPATH REGISTERS
//====================================================================================

    logic opcode_CRT, opcode_NXT; logic [15:0] data_CRT, data_NXT;

        always_ff @(posedge clk_i) begin
            opcode_CRT <= opcode_NXT;
            data_CRT <= data_NXT;
        end 
    
    assign data_o = data_CRT;


    /* Counter to select the payload bits */
    logic [3:0] bit_counter; logic bit_increment, bit_reset;

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


//====================================================================================
//      FSM LOGIC
//====================================================================================

    typedef enum logic [2:0] {IDLE, PREAMBLE, START_OF_FRAME, OPCODE, PHY_ADDRESS, REG_ADDRESS, TURN_AROUND, DATA} smii_states_t;

    smii_states_t state_CRT, state_NXT;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                state_CRT <= IDLE;
            end else begin 
                state_CRT <= state_NXT;
            end 
        end 


    localparam WRITE = 0;
    localparam READ = 1;

    logic smii_mdio, enable;

        always_comb begin
            /* Default Values */
            data_NXT = data_CRT;
            state_NXT = state_CRT;
            opcode_NXT = opcode_CRT;

            bit_increment = 1'b0;
            bit_reset = 1'b0;
            smii_mdio = 1'b0;

            done_o = 1'b0;
            enable = 1'b0;
            
            case (state_CRT)
                IDLE: begin
                    if (write_i | read_i) begin
                        state_NXT = PREAMBLE;
                    end

                    case ({write_i, read_i})
                        2'b10: opcode_NXT = WRITE;

                        2'b01: opcode_NXT = READ;
                    endcase 
                end

                PREAMBLE: begin
                    smii_mdio = 1'b1;
                    enable = 1'b1;

                    if (pulse_i) begin
                        bit_increment = 1'b1;

                        if (bit_counter == '1) begin
                            bit_reset = 1'b1;

                            state_NXT = START_OF_FRAME;
                        end
                    end 
                end

                START_OF_FRAME: begin
                    smii_mdio = bit_counter[0];
                    enable = 1'b1;

                    if (pulse_i) begin
                        bit_increment = 1'b1;

                        if (bit_counter == 'd1) begin
                            bit_reset = 1'b1;

                            state_NXT = OPCODE;
                        end
                    end 
                end

                OPCODE: begin
                    /* WRITE = 01 --- READ = 10 */
                    smii_mdio = opcode_CRT ? !bit_counter[0] : bit_counter[0];
                    enable = 1'b1;

                    if (pulse_i) begin
                        bit_increment = 1'b1;

                        if (bit_counter == 'd1) begin
                            bit_reset = 1'b1;

                            state_NXT = PHY_ADDRESS;

                            data_NXT = {CHIP_PHY_ADDRESS, 11'b0};
                        end
                    end 
                end

                PHY_ADDRESS: begin
                    smii_mdio = data_CRT[15];
                    enable = 1'b1;

                    if (pulse_i) begin
                        bit_increment = 1'b1;
                        
                        data_NXT = data_CRT << 1;

                        if (bit_counter == 'd5) begin
                            bit_reset = 1'b1;

                            state_NXT = PHY_ADDRESS;

                            data_NXT = {address_i, 11'b0};
                        end
                    end 
                end

                REG_ADDRESS: begin
                    smii_mdio = data_CRT[15];
                    enable = 1'b1;

                    if (pulse_i) begin
                        bit_increment = 1'b1;
                        
                        data_NXT = data_CRT << 1;

                        if (bit_counter == 'd5) begin
                            bit_reset = 1'b1;

                            state_NXT = PHY_ADDRESS;

                            data_NXT = data_i;
                        end
                    end 
                end

                TURN_AROUND: begin
                    if (opcode_CRT == WRITE) begin
                        smii_mdio = !bit_counter[0];
                        enable = 1'b1;
                    end else begin
                        enable = 1'b0; 
                    end

                    if (pulse_i) begin
                        bit_increment = 1'b1;

                        if (bit_counter == 'd1) begin
                            bit_reset = 1'b1;

                            state_NXT = DATA;
                        end
                    end 
                end

                DATA: begin
                    if (opcode_CRT == WRITE) begin
                        smii_mdio = !bit_counter[0];
                        enable = 1'b1;
                    end else begin
                        enable = 1'b0; 
                    end

                    if (pulse_i) begin
                        bit_increment = 1'b1;
                        
                        if (opcode_CRT == WRITE) begin 
                            data_NXT = data_CRT << 1;
                        end else begin
                            data_NXT = {data_CRT[14:0], smii_mdio_io};
                        end

                        if (bit_counter == 'd15) begin
                            bit_reset = 1'b1;

                            state_NXT = IDLE;
                            done_o = 1'b1;

                            data_NXT = data_i;
                        end
                    end 
                end
            endcase 
        end

    assign smii_mdio_io = enable ? smii_mdio : 1'bZ;

    assign smii_mdc_o = pulse_i;

endmodule : ethernet_mac2phy

`endif 