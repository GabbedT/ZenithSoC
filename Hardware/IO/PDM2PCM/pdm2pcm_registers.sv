`ifndef PDM2PCM_REGISTERS_SV
    `define PDM2PCM_REGISTERS_SV

`include "../../Utility/Packages/pdm2pcm_pkg.sv"

module pdm2pcm_registers #(
    /* Buffer sizes */
    parameter BUFFER_SIZE = 512
) (
    /* Global signals */
    input logic clk_i,
    input logic rst_n_i,
    output logic interrupt_o,

    /* Buffer interface */
    input logic [15:0] pcm_sample_i,
    input logic valid_i,
    input logic channel_i,
    input logic invalid_i,

    /* Configuration */
    output logic [6:0] divisor_o,
    output logic [7:0] decimation_rate_o,
    output logic [15:0] gain_o,
    output logic enable_interface_o,
    
    /* Channel configuration */
    output logic dual_channel_o,
    output logic channel_o,

    /* Write interface */
    input logic write_i,
    input pdm2pcm_registers_t write_address_i,
    input logic [3:0][7:0] write_data_i,
    input logic [3:0] write_strobe_i,
    output logic write_error_o,

    /* Read interface */
    input logic read_i,
    input pdm2pcm_registers_t read_address_i,
    output logic [31:0] read_data_o,
    output logic read_error_o
);


//====================================================================================
//      ERROR CHECK
//====================================================================================

    /* Error checking */
    assign write_error_o = ( (write_address_i == PDM2PCM_SAMPLE_BUFFER) 
                           | (write_address_i == PDM2PCM_STATUS)) & write_i;

    assign read_error_o = 1'b0;


//====================================================================================
//      CONTROL REGISTER
//====================================================================================  

    pdm2pcm_control_t control_register;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                control_register.clock_divisor <= '0;

                /* Interface disabled */
                control_register.interface_enable <= 1'b0;
                control_register.dual_channel <= 1'b0;
                control_register.channel <= 1'b0;

                /* FIFOs disabled */
                control_register.buffer_enable <= 1'b0;

                /* Operation disabled on reset */
                control_register.interrupt_enable <= 5'b0;
            end else begin 
                if ((write_address_i == PDM2PCM_CONTROL) & write_i) begin 
                    if (write_strobe_i[0]) begin
                        control_register.clock_divisor <= write_data_i[0][6:0];
                        control_register.interrupt_enable[0] <= write_data_i[0][7];
                    end

                    if (write_strobe_i[1]) begin
                        control_register.interrupt_enable[4:1] <= write_data_i[1][3:0];
                        control_register.channel <= write_data_i[1][4];
                        control_register.dual_channel <= write_data_i[1][5];
                        control_register.buffer_enable <= write_data_i[1][6];
                        control_register.interface_enable <= write_data_i[1][7];
                    end
                end
            end 
        end 

    assign divisor_o = control_register.clock_divisor;

    assign channel_o = control_register.channel;
    assign dual_channel_o = control_register.dual_channel;

    assign enable_interface_o = control_register.interface_enable;


//====================================================================================
//      GAIN REGISTER
//====================================================================================  

    logic [15:0] gain_register;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                /* Gain = 1.0 */
                gain_register <= 16'h8000;
            end else if ((write_address_i == PDM2PCM_GAIN) & write_i) begin 
                if (write_strobe_i[0]) begin
                    gain_register[7:0] <= write_data_i[0];
                end

                if (write_strobe_i[1]) begin
                    gain_register[15:8] <= write_data_i[1];
                end
            end 
        end 

    assign gain_o = gain_register;


//====================================================================================
//      DECIMATION REGISTER
//====================================================================================  

    logic [7:0] decimation_register;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                decimation_register <= 8'h00;
            end else if ((write_address_i == PDM2PCM_DECIMATION_FACTOR) & write_i) begin 
                if (write_strobe_i[0]) begin
                    decimation_register <= write_data_i[0];
                end
            end 
        end 

    assign decimation_rate_o = decimation_register;


//====================================================================================
//      LEFT BUFFER REGISTER
//====================================================================================  

    logic empty, full, write_buffer, read_buffer; logic [15:0] sample;

    /* Write when left sample is full */
    assign write_buffer = valid_i & control_register.buffer_enable;

    assign read_buffer = read_i & (read_address_i == PDM2PCM_SAMPLE_BUFFER);

    /* TX Buffer asyncronous FIFO instantiation */
    synchronous_buffer #(BUFFER_SIZE, 16) sample_buffer (
        .clk_i   ( clk_i   ),
        .rst_n_i ( rst_n_i ),

        .write_i ( write_buffer & !full ),
        .read_i  ( read_buffer & !empty ),

        .empty_o ( empty ),
        .full_o  ( full  ),

        .write_data_i ( pcm_sample_i ),
        .read_data_o  ( sample       )
    );


//====================================================================================
//      STATUS REGISTER
//====================================================================================  

    pdm2pcm_status_t status_register;

    assign status_register.buffer_empty = empty;
    assign status_register.buffer_full = full;


//====================================================================================
//      THRESHOLD REGISTER
//====================================================================================

    logic [1:0][15:0] threshold_register;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                threshold_register <= '0;
            end else if ((write_address_i == PDM2PCM_THRESHOLD) & write_i) begin 
                if (write_strobe_i[0]) begin
                    /* Right low byte */
                    threshold_register[0][7:0] <= write_data_i[0];
                end

                if (write_strobe_i[1]) begin
                    /* Right high byte */
                    threshold_register[0][15:8] <= write_data_i[1];
                end

                if (write_strobe_i[2]) begin
                    /* Left low byte */
                    threshold_register[1][7:0] <= write_data_i[2];
                end

                if (write_strobe_i[3]) begin
                    /* Left high byte */
                    threshold_register[1][15:8] <= write_data_i[3];
                end
            end 
        end 


//====================================================================================
//      EVENT REGISTER
//====================================================================================

    logic [5:0] event_register; 

    always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
        if (!rst_n_i) begin 
            event_register <= '0;
        end else begin 
            if ((write_address_i == PDM2PCM_EVENT) & write_i) begin
                if (write_strobe_i[0]) begin
                    event_register <= write_data_i[0][5:0];
                end
            end else begin
                if (full) begin
                    event_register[0] <= 1'b1;
                end

                if (valid_i & (channel_i == LEFT) & (pcm_sample_i > threshold_register[1])) begin
                    /* Left channel sample surpassed threshold */
                    event_register[1] <= 1'b1;
                end

                if (valid_i & (channel_i == LEFT) & (pcm_sample_i < threshold_register[1])) begin
                    /* Left channel sample fell below threshold */
                    event_register[2] <= 1'b1;
                end

                if (valid_i & (channel_i == RIGHT) & (pcm_sample_i > threshold_register[0])) begin
                    /* Right channel sample surpassed threshold */
                    event_register[3] <= 1'b1;
                end

                if (valid_i & (channel_i == RIGHT) & (pcm_sample_i < threshold_register[0])) begin
                    /* Right channel sample fell below threshold */
                    event_register[4] <= 1'b1;
                end

                if (invalid_i) begin
                    event_register[5] <= 1'b1;
                end
            end
        end 
    end 


    /* Catch the positive edge of a bit set into the register, otherwise the interrupt bit will stay on */
    logic [5:0] event_edge;

    genvar i;

    generate 
        for (i = 0; i < 6; ++i) begin
            edge_detector #(1, 0) event_detector (
                .clk_i   ( clk_i  ),
                .rst_n_i ( rst_n_i ),

                .signal_i ( event_register[i] ),
                .edge_o   ( event_edge[i]     )
            );
        end
    endgenerate

    assign interrupt_o = (event_edge != '0);


//====================================================================================
//      DATA READ
//====================================================================================

        always_comb begin
            /* Default value */
            read_data_o = '0;

            case (read_address_i)
                PDM2PCM_STATUS: read_data_o = status_register;

                PDM2PCM_CONTROL: read_data_o = control_register;

                PDM2PCM_GAIN: read_data_o = gain_register;

                PDM2PCM_DECIMATION_FACTOR: read_data_o = decimation_register;

                PDM2PCM_SAMPLE_BUFFER: read_data_o = sample;

                PDM2PCM_THRESHOLD: read_data_o = threshold_register;

                PDM2PCM_EVENT: read_data_o = event_register;
            endcase 
        end

endmodule : pdm2pcm_registers

`endif