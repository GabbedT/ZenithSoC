`ifndef AXI_SLAVE_MEMORY
    `define AXI_SLAVE_MEMORY

module axi_slave_memory #(parameter MEMORY_SIZE = 32) (
    input clk_i, rst_n_i,
    
    input logic [31:0] load_address_i,
    input logic load_i,
    output logic [31:0] load_data_o,
    output logic load_done_o,
    
    input logic [31:0] store_address_i,
    input logic [3:0][7:0] store_data_i,
    input logic [3:0] store_strobe_i,
    input logic store_i,
    output logic store_done_o
); 

    logic [3:0][7:0] memory [MEMORY_SIZE];
    
    initial begin
       for (int i = 0; i < MEMORY_SIZE; ++i) begin 
           memory[i] = i;
       end 
    end

        always_ff @(posedge clk_i) begin
            if (!rst_n_i) begin 
                load_done_o <= 1'b0;
                store_done_o <= 1'b0;
            end else begin 
                load_done_o <= load_i;
                store_done_o <= store_i;
            end 
        end
        
        
        always_ff @(posedge clk_i) begin
            if (load_i) begin 
                load_data_o <= memory[load_address_i[$clog2(MEMORY_SIZE) + 1:2]];
            end 
            
            if (store_i) begin 
                for (int i = 0; i < 4; ++i) begin 
                    if (store_strobe_i[i]) begin 
                        memory[store_address_i[$clog2(MEMORY_SIZE) + 1:2]][i] <= store_data_i[i];
                    end 
                end 
            end 
        end
        
endmodule 


`endif 