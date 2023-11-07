`ifndef AXI_MASTER_DRIVER
    `define AXI_MASTER_DRIVER

module axi_master_driver (
    input logic clk_i, 
    input logic rst_n_i, 

    input logic [31:0] write_address_i,
    input logic [31:0] write_data_i,
    input logic [3:0] write_strobe_i,
    input logic write_load_i,
    input logic write_stop_i,

    output logic [31:0] write_address_o,
    output logic [31:0] write_data_o,
    output logic [3:0] write_strobe_o,
    output logic write_start_o,
    input logic write_cts_i,


    input logic [31:0] read_address_i,
    input logic read_load_i,
    input logic read_stop_i,

    output logic [31:0] read_address_o,
    output logic read_start_o,  
    input logic read_cts_i    
);

    localparam BUFFER_DEPTH = 128;

    /* Write and read pointers */
    logic [$clog2(BUFFER_DEPTH) - 1:0] write_push_ptr, write_inc_push_ptr, write_pull_ptr, write_inc_pull_ptr;

    assign write_inc_push_ptr = write_push_ptr + 1'b1;
    assign write_inc_pull_ptr = write_pull_ptr + 1'b1;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin 
            if (!rst_n_i) begin
                write_pull_ptr <= '0;
                write_push_ptr <= '0; 
            end else begin 
                /* Increment pointer */
                if (write_load_i) begin
                    write_push_ptr <= write_inc_push_ptr;
                end

                if (write_start_o) begin
                    write_pull_ptr <= write_inc_pull_ptr;
                end
            end 
        end


    logic [67:0] write_buffer [BUFFER_DEPTH];

        always_ff @(posedge clk_i) begin
            if (write_load_i) begin 
                write_buffer[write_push_ptr] <= {write_address_i, write_data_i, write_strobe_i};
            end 
        end  

    assign {write_address_o, write_data_o, write_strobe_o} = write_buffer[write_pull_ptr];

    /* FIFO access mode */
    localparam logic [1:0] PULL_OPERATION = 2'b01;
    localparam logic [1:0] PUSH_OPERATION = 2'b10;

    logic write_buffer_full, write_buffer_empty;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin 
            if (!rst_n_i) begin 
                write_buffer_full <= 1'b0;
                write_buffer_empty <= 1'b1;
            end else begin 
                case ({write_load_i, write_start_o})
                    PULL_OPERATION: begin
                        write_buffer_full <= 1'b0;
                        write_buffer_empty <= (write_push_ptr == write_inc_pull_ptr);
                    end

                    PUSH_OPERATION: begin
                        write_buffer_full <= (write_pull_ptr == write_inc_push_ptr);
                        write_buffer_empty <= 1'b0; 
                    end
                endcase 
            end
        end 

    assign write_start_o = write_cts_i & !write_buffer_empty;




    /* Write and read pointers */
    logic [$clog2(BUFFER_DEPTH) - 1:0] read_push_ptr, read_inc_push_ptr, read_pull_ptr, read_inc_pull_ptr;

    assign read_inc_push_ptr = read_push_ptr + 1'b1;
    assign read_inc_pull_ptr = read_pull_ptr + 1'b1;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin 
            if (!rst_n_i) begin
                read_pull_ptr <= '0;
                read_push_ptr <= '0; 
            end else begin 
                /* Increment pointer */
                if (read_load_i) begin
                    read_push_ptr <= read_inc_push_ptr;
                end

                if (read_start_o) begin
                    read_pull_ptr <= read_inc_pull_ptr;
                end
            end 
        end


    logic [31:0] read_buffer [BUFFER_DEPTH];

        always_ff @(posedge clk_i) begin
            if (read_load_i) begin 
                read_buffer[read_push_ptr] <= read_address_i;
            end 
        end  

    assign read_address_o = read_buffer[read_pull_ptr];


    logic read_buffer_full, read_buffer_empty;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin 
            if (!rst_n_i) begin 
                read_buffer_full <= 1'b0;
                read_buffer_empty <= 1'b1;
            end else begin 
                case ({read_load_i, read_start_o})
                    PULL_OPERATION: begin
                        read_buffer_full <= 1'b0;
                        read_buffer_empty <= (read_push_ptr == read_inc_pull_ptr);
                    end

                    PUSH_OPERATION: begin
                        read_buffer_full <= (read_pull_ptr == read_inc_push_ptr);
                        read_buffer_empty <= 1'b0; 
                    end
                endcase 
            end
        end 

    assign read_start_o = read_cts_i & !read_buffer_empty;

endmodule 

`endif 