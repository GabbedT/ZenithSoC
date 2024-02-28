`ifndef SPI_WRAPPER
    `define SPI_WRAPPER

`include "../../Hardware/Utility/Packages/spi_pkg.sv"

module spi_wrapper (
    /* Global signals */
    input logic clk_i,
    input logic rst_n_i,

    /* SPI interface */
    output logic sclk_o,
    output logic cs_n_o,
    output logic mosi_o,
    input logic miso_i,

    input logic [1:0] interrupt_i,
    output logic interrupt_o,
    output logic data_o
);

    localparam _TIMES_ = 11;

    localparam S10 = 39'd100_000_000;
    localparam N10 = 39'd10;

    logic [39:0] cnt; logic [3:0] times; logic block, unlock, increment;

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

    logic [4:0] rd_ptr; logic read_command;

    function automatic eth_pkt_t write_reg(input logic [5:0] write_addr, input logic [31:0] write_data);
        automatic eth_pkt_t temp;

        temp.read = 1'b0;
        temp.read_address = '0;

        temp.write = 1'b1;
        temp.write_address = write_addr;
        temp.write_data = write_data;

        return temp;
    endfunction

    function automatic eth_pkt_t read_reg(input logic [5:0] read_addr);
        automatic eth_pkt_t temp;

        temp.write = 1'b0;
        temp.write_address = '0;
        temp.write_data = '0;

        temp.read = 1'b1;
        temp.read_address = read_addr;

        return temp;
    endfunction 

    
    /* Write interface */
    logic [2:0] write_address; logic [3:0][7:0] write_data;
    logic write_error, write_done, write;

    /* Read interface */
    logic [2:0] read_address; logic [31:0] read_data;
    logic read_error, read, read_done;

    eth_pkt_t temporary; 
    spi_status_t setup;

    assign setup.clock_divisor = 'd49;
    assign setup.cpol = 1'b0;
    assign setup.cpha = 1'b0;
    assign setup.bit_order = 1'b0;
    assign setup.interrupt_enable = 1'b1;

    assign setup.TX_empty = 1'b0;
    assign setup.TX_full = 1'b0;
    assign setup.RX_full = 1'b0;
    assign setup.RX_empty = 1'b0;
    assign setup.idle = 1'b0;

    localparam WRITE_REG = 8'h0A;
    localparam READ_REG  = 8'h0B;
    localparam READ_FIFO = 8'h0D;
    
    always_ff @(posedge clk_i) begin
        if (read_command) begin
            case (rd_ptr)
                'd0: temporary <= write_reg(0, {'0, setup});
                'd1: temporary <= write_reg(4, 1);
                'd2: temporary <= write_reg(1, {'0, READ_REG});
                'd3: temporary <= write_reg(1, 0);
                'd4: temporary <= write_reg(1, 1);
                'd5: temporary <= write_reg(1, 2);
                'd6: temporary <= write_reg(1, 0);
                'd7: temporary <= read_reg(2);
                'd8: temporary <= read_reg(2);
                'd9: temporary <= read_reg(2);
                'd10: temporary <= read_reg(2);

                default: temporary <= 46'b0;
            endcase 
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
            read_command = 1'b0;
            read_address = '0;
            increment = 1'b0;
            unlock = 1'b0;

            case (state_CRT)
                IDLE: begin
                    if (block & (times != _TIMES_)) begin
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

    logic interrupt;

    spi #(32, 32, 1) spi_device (
        .clk_i       ( clk_i     ),
        .rst_n_i     ( rst_n_i   ),
        .interrupt_o ( interrupt ),

        .sclk_o ( sclk_o ),
        .cs_n_o ( cs_n_o ),
        .mosi_o ( mosi_o ),
        .miso_i ( miso_i ),  

        .write_i         ( write         ),
        .write_address_i ( write_address ),
        .write_data_i    ( write_data    ),
        .write_strobe_i  ( '1            ),
        .write_error_o   (               ),
        .write_done_o    ( write_done    ),

        .read_i         ( read         ),
        .read_address_i ( read_address ),
        .read_data_o    ( read_data    ),
        .read_error_o   (              ),
        .read_done_o    ( read_done    )
    );


    logic [7:0] read_data_ff;

    always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
        if (!rst_n_i) begin 
            read_data_ff <= '0;
        end else begin 
            if (read_done) begin
                read_data_ff <= read_data;
            end
        end 
    end 

    assign data_o = read_data_ff != '0;


    logic [2:0] interrupt_ff; 

    always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
        if (!rst_n_i) begin 
            interrupt_ff <= 3'b0;
        end else begin 
            interrupt_ff[2] <= interrupt;
            interrupt_ff[1:0] <= interrupt_i;
        end 
    end 

    assign interrupt_o = interrupt_ff != 0;

endmodule 

`endif 