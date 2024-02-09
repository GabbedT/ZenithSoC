`ifndef ON_CHIP_MEMORY_SV
    `define ON_CHIP_MEMORY_SV

`include "memory_bank.sv"

module on_chip_memory #(
    /* In 32 bytes */
    parameter MEMORY_SIZE = 1024,

    /* Memory init file path */
    parameter string INIT_FILE_PATH = ""
) (
    input logic clk_i, 
    input logic rst_n_i, 

    /* Store data channel */ 
    input logic store_i,
    input logic [$clog2(MEMORY_SIZE) - 1:0] store_address_i,
    input logic [31:0] store_data_i,
    input logic [3:0] store_width_i,
    output logic store_done_o,

    /* Load data channel */ 
    input logic load_i,
    input logic [$clog2(MEMORY_SIZE) - 3:0] load_address_i,
    input logic load_invalid_i,
    output logic [31:0] load_data_o,
    output logic load_done_o,

    /* Instruction channel */
    input logic fetch_i,
    input logic invalidate_i,
    input logic [$clog2(MEMORY_SIZE) - 3:0] fetch_address_i,
    output logic [31:0] instruction_o,
    output logic fetch_done_o
);

//==========================================================
//      VALID SIGNAL LOGIC
//==========================================================

    logic load; assign load = load_i & !load_invalid_i;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin
                load_done_o <= 1'b0;
            end else begin 
                load_done_o <= load;
            end 
        end


    logic store_pending, store_done; 

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin
                store_done_o <= 1'b0;
                store_pending <= 1'b0;
            end else begin 
                store_done_o <= (store_i & !load) | store_done;

                if (store_i & load) begin
                    store_pending <= 1'b1;
                end else if (store_done) begin
                    store_pending <= 1'b0;
                end
            end 
        end


    logic fetch_valid; 

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin
                fetch_valid <= 1'b0;
            end else begin
                fetch_valid <= fetch_i; 
            end
        end

    assign fetch_done_o = fetch_valid & !invalidate_i;


//==========================================================
//      MEMORY 
//==========================================================

    logic [3:0] data_port_write_enable;
    logic [$clog2(MEMORY_SIZE) - 3:0] data_port_address; 
    logic enable_port; 

        always_comb begin
            data_port_address = '0;
            store_done = 1'b0;

            if (!store_pending) begin
                case ({store_i, load})
                    2'b10: data_port_address = store_address_i[$clog2(MEMORY_SIZE) - 1:2];

                    2'b01: data_port_address = load_address_i;

                    2'b11: data_port_address = load_address_i;
                endcase 
                
                data_port_write_enable = (store_i & !load) ? store_width_i : '0;

                enable_port = store_i | load;
            end else begin
                data_port_address = store_address_i[$clog2(MEMORY_SIZE) - 1:2];

                data_port_write_enable = store_width_i;

                store_done = 1'b1;
                enable_port = 1'b1;
            end
        end


    logic [$clog2(MEMORY_SIZE) - 3:0] instr_port_address; logic [3:0][7:0] instruction_read;

    assign instr_port_address = fetch_address_i; 


    genvar i; 

    generate
        for (i = 0; i < 4; ++i) begin : mem_bank
            memory_bank #(INIT_FILE_PATH, MEMORY_SIZE / 4, 4, i) bank (
                .clk_A_i ( clk_i ),
                .clk_B_i ( clk_i ), 

                .enable_A_i  ( enable_port               ),
                .address_A_i ( data_port_address         ),
                .data_A_i    ( store_data_i[(i * 8)+:8]  ),
                .write_A_i   ( data_port_write_enable[i] ),
                .data_A_o    ( load_data_o[(i * 8)+:8]   ),

                .enable_B_i  ( fetch_i ),
                .address_B_i ( instr_port_address  ),
                .data_B_i    ( '0                  ),
                .write_B_i   ( '0                  ),
                .data_B_o    ( instruction_read[i] )
            );
        end : mem_bank
    endgenerate

    assign instruction_o = instruction_read;

endmodule : on_chip_memory 

`endif 