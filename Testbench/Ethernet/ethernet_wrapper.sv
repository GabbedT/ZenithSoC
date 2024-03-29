`ifndef ETH_WRAPPER
    `define ETH_WRAPPER

`include "../../Hardware/System/clock_source.sv"

module eth_wrapper (
    input logic rst_n_i,
    input logic clk_i,
    output logic done_o,
    output logic [15:0] read_data_o,

    /* RMII Interface */
    input logic phy_interrupt_i,
    inout logic [1:0] rmii_rxd_io,
    inout logic rmii_crsdv_io,
    input logic rmii_rxer_i,
    output logic [1:0] rmii_txd_o,
    output logic rmii_txen_o,
    output logic rmii_refclk_o,
    output logic rmii_rstn_o,

    /* SMII interface */
    output logic smii_mdc_o,
    inout logic smii_mdio_io
);

    localparam _TIMES_ = 20;

    localparam S10 = 39'd10_000_000;
    localparam N10 = 39'd30;


    logic [39:0] cnt; logic [6:0] times; logic block, unlock, increment;

    always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
        if (!rst_n_i) begin 
            cnt <= '0;
            times <= '0;
            block <= 1'b0;
        end else begin 
            if (!block) begin
                cnt <= cnt + 1;
            end else begin
                if (unlock) begin
                    cnt <= '0;
                end
            end

            if (increment) begin
                times <= times + 1;
            end

            if (!unlock) begin
                block <= cnt > N10; 
            end else begin
                block <= 1'b0;
            end
        end 
    end 

    assign done_o = times == _TIMES_;


    typedef struct packed {
        logic write;
        logic [5:0] write_address;
        logic [31:0] write_data;

        logic read;
        logic [5:0] read_address;
    } eth_pkt_t;

    logic [6:0] rd_ptr; logic read_command;

    function automatic eth_pkt_t write_mac(input logic [5:0] write_addr, input logic [31:0] write_data);
        automatic eth_pkt_t temp;

        temp.read = 1'b0;
        temp.read_address = '0;

        temp.write = 1'b1;
        temp.write_address = write_addr;
        temp.write_data = write_data;

        return temp;
    endfunction

    function automatic eth_pkt_t read_mac(input logic [5:0] read_addr);
        automatic eth_pkt_t temp;

        temp.write = 1'b0;
        temp.write_address = '0;
        temp.write_data = '0;

        temp.read = 1'b1;
        temp.read_address = read_addr;

        return temp;
    endfunction 

    
    logic write, read, write_done, read_done; logic [5:0] write_address, read_address; logic [31:0] write_data, read_data;
    eth_pkt_t temporary;
    
    always_ff @(posedge clk_i) begin
        if (read_command) begin
            if (rd_ptr > 'd4 & rd_ptr < 'd8) begin
                case (rd_ptr)
                    'd5: temporary <= write_mac(0, 16'b0000000100000000);
                    // 'd6: temporary <= write_mac(33, {16'hAA_16, 16'd4});
                    'd6: temporary <= write_mac(33, {16'hFF_FF, 16'd4});
                    // 'd7: temporary <= write_mac(34, {32'hD8_BB_C1_57});
                    'd7: temporary <= write_mac(34, '1);

                    default: temporary <= '1;
                endcase
            end else if (rd_ptr < 'd5) begin
                case (rd_ptr) 
                    'd0: temporary <= write_mac(32, {'0, 1'b1, 4'b0, 1'b1, 2'b11, 2'b0, 4'b0, 4'b0});
                    'd1: temporary <= write_mac(35, 8'hDE);
                    'd2: temporary <= write_mac(35, 8'hAD);
                    'd3: temporary <= write_mac(35, 8'hBE);
                    'd4: temporary <= write_mac(35, 8'hEF);

                    default: temporary <= '1;
                endcase 
            end else begin
                temporary <= read_mac(38);
            end 
        end else begin
            temporary <= 46'b0;
        end
    end 

    assign write = temporary.write;
    assign write_address = temporary.write_address;
    assign write_data = temporary.write_data;

    assign read = temporary.read;

    always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
        if (!rst_n_i) begin 
            rd_ptr <= '0;
        end else begin 
            if (read_command) begin
                rd_ptr <= rd_ptr + 1;
            end
        end 
    end 



    typedef enum logic [1:0] { IDLE, COMMAND, WAIT } state_t;

    state_t state_CRT, state_NXT; logic [5:0] address_CRT, address_NXT;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                state_CRT <= IDLE;
                address_CRT <= '0;
            end else begin 
                state_CRT <= state_NXT;
                address_CRT <= address_NXT;
            end 
        end 

        always_comb begin
            state_NXT = state_CRT;
            address_NXT = address_CRT;
            read_address = '0;
            read_command = 1'b0;
            increment = 1'b0;
            unlock = 1'b0;

            case (state_CRT)
                IDLE: begin
                    if (block & (times != _TIMES_) & rmii_rstn_o) begin
                        state_NXT = COMMAND;

                        read_command = 1'b1;
                    end
                end

                COMMAND: begin
                    address_NXT = temporary.read_address;
                    read_address = temporary.read_address;

                    if (write_done | read_done) begin
                        state_NXT = IDLE;
                        unlock = 1'b1;
                        increment = 1'b1;
                    end else begin
                        state_NXT = WAIT;
                    end
                end

                WAIT: begin
                    read_address = address_CRT;

                    if (write_done | read_done) begin
                        state_NXT = IDLE;
                        unlock = 1'b1;
                        increment = 1'b1;
                    end
                end
            endcase 
        end


    ethernet #(5'b00001, 48'hDE_AD_BE_EF_00_00, 512, 512, 32, 32) mac (
        /* Global signals */
        .clk_i       ( clk_i ),
        .rst_n_i     ( rst_n_i ),
        .interrupt_o (  ),

        /* Write interface */
        .write_i         ( write         ),
        .write_address_i ( write_address ),
        .write_data_i    ( write_data    ),
        .write_error_o   (  ),
        .write_done_o    ( write_done    ),

        /* Read interface */
        .read_i         ( read         ),
        .read_address_i ( read_address ),
        .read_data_o    ( read_data    ),
        .read_error_o   (  ),
        .read_done_o    ( read_done    ),

        /* RMII Interface */
        .phy_interrupt_i ( phy_interrupt_i ),
        .rmii_rxd_io     ( rmii_rxd_io     ),
        .rmii_crsdv_io   ( rmii_crsdv_io   ),
        .rmii_rxer_i     ( rmii_rxer_i     ),
        .rmii_txd_o      ( rmii_txd_o      ),
        .rmii_txen_o     ( rmii_txen_o     ),
        .rmii_refclk_o   ( rmii_refclk_o   ),
        .rmii_rstn_o     ( rmii_rstn_o     ),

        /* SMII interface */
        .smii_mdc_o   ( smii_mdc_o   ),
        .smii_mdio_io ( smii_mdio_io )
    );

    always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
        if (!rst_n_i) begin 
            read_data_o <= '0;
        end else begin 
            if (read_done) begin
                read_data_o <= read_data;
            end
        end 
    end 

endmodule 

`endif 