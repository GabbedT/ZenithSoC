`ifndef MEMORY_BANK_INCLUDE_SV
    `define MEMORY_BANK_INCLUDE_SV

module memory_bank #(
    parameter string FILE_PATH = "",
    parameter MEMORY_SIZE = 2 ** 12,
    parameter BANKS_COUNT = 4,
    parameter BANK_NUMBER = 0
) (
    input logic clk_A_i, 
    input logic clk_B_i,
    
    /* Enable port operations */
    input logic enable_A_i,
    input logic enable_B_i,
    
    input logic write_A_i,
    input logic [$clog2(MEMORY_SIZE) - 1:0] address_A_i,
    input logic [7:0] data_A_i,
    output logic [7:0] data_A_o,
    
    input logic write_B_i,
    input logic [$clog2(MEMORY_SIZE) - 1:0] address_B_i,
    input logic [7:0] data_B_i,
    output logic [7:0] data_B_o
);

    logic [7:0] memory [MEMORY_SIZE]; 

    initial begin 
        automatic logic [3:0][7:0] init_memory [MEMORY_SIZE];

        $readmemh(FILE_PATH, init_memory);

        for (int i = 0; i < MEMORY_SIZE; ++i) begin 
            memory[i] = '0;
        end 

        for (int i = 0; i < MEMORY_SIZE; ++i) begin 
            memory[i] = init_memory[i][BANK_NUMBER];
        end 
    end 


    always_ff @(posedge clk_A_i) begin
        if (enable_A_i) begin
            if (write_A_i)
                memory[address_A_i] <= data_A_i;
                
            data_A_o <= memory[address_A_i];
        end
    end
    
    always_ff @(posedge clk_B_i) begin
        if (enable_B_i) begin
            if (write_B_i)
                memory[address_B_i] <= data_B_i;
                
            data_B_o <= memory[address_B_i];
        end
    end

endmodule : memory_bank

`endif 