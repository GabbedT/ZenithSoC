`ifndef ON_CHIP_MEMORY_SV
    `define ON_CHIP_MEMORY_SV

`include "ApogeoRV/Hardware/Include/Interfaces/bus_interface.sv"

`include "../Utility/Interfaces/axi_read_interface.sv"
`include "../Utility/Interfaces/axi_write_interface.sv"

`include "memory_bank.sv"

module on_chip_memory #(
    /* In 32 bit words */
    parameter MEMORY_SIZE = 1024
) (
    input logic clk_i, 
    input logic rst_n_i, 

    /* Store data channel */ 
    input logic store_i,
    input logic [$clog2(MEMORY_SIZE / 4) + 1:0] store_address_i,
    input logic [31:0] store_data_i,
    input logic [3:0] store_width_i,
    output logic store_done_o,

    /* Load data channel */ 
    input logic load_i,
    input logic [$clog2(MEMORY_SIZE / 4) + 1:0] load_address_i,
    output logic [31:0] load_data_o,
    output logic load_done_o,

    /* Instruction channel */
    fetch_interface.slave fetch_channel  
);

//==========================================================
//      VALID SIGNAL LOGIC
//==========================================================

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin
                load_done_o <= 1'b0;
            end else begin 
                load_done_o <= load_i;
            end 
        end

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin
                store_done_o <= 1'b0;
            end else begin 
                store_done_o <= store_i;
            end 
        end


    logic buffered_valid;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin
                buffered_valid <= 1'b0;
            end else if (fetch_channel.fetch) begin 
                buffered_valid <= 1'b1; 
            end else begin
                buffered_valid <= 1'b0; 
            end
        end


    logic fetch_valid; 

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin
                fetch_valid <= 1'b0;
            end else begin
                fetch_valid <= buffered_valid & !fetch_channel.invalidate; 
            end
        end

    assign fetch_channel.valid = fetch_valid & !fetch_channel.invalidate;


//==========================================================
//      MEMORY 
//==========================================================

    logic [3:0] data_port_write_enable;

    /* Load have more priority */
    assign data_port_write_enable = (store_i & !load_i) ? store_width_i : '0;


    logic [$clog2(MEMORY_SIZE / 4) - 1:0] data_port_address; 

    /* Select address based on operation (default on load channel address) */ 
    assign data_port_address = store_i ? store_address_i[$clog2(MEMORY_SIZE / 4) + 1:2] : load_address_i[$clog2(MEMORY_SIZE / 4) + 1:2]; 


    logic [$clog2(MEMORY_SIZE / 4) - 1:0] instr_port_address; logic [3:0][7:0] instruction_read;

    assign instr_port_address = fetch_channel.address[$clog2(MEMORY_SIZE / 4) + 1:2]; 


    genvar i; 

    generate
        for (i = 0; i < 4; ++i) begin
            memory_bank #("", MEMORY_SIZE / 4, 4, i) bank (
                .clk_A_i ( clk_i ),
                .clk_B_i ( clk_i ), 

                .enable_A_i  ( store_i | load_i          ),
                .address_A_i ( data_port_address         ),
                .data_A_i    ( store_data_i[(i * 8)+:8]  ),
                .write_A_i   ( data_port_write_enable[i] ),
                .data_A_o    ( load_data_o[(i * 8)+:8]   ),

                .enable_B_i  ( fetch_channel.fetch ),
                .address_B_i ( instr_port_address  ),
                .data_B_i    ( '0                  ),
                .write_B_i   ( '0                  ),
                .data_B_o    ( instruction_read[i] )
            );
        end
    endgenerate


        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin
                fetch_channel.instruction <= '0;
            end else begin 
                fetch_channel.instruction <= instruction_read;
            end 
        end

endmodule : on_chip_memory 

`endif 