`ifndef AS_UNIT_REGISTERS_SV
    `define AS_UNIT_REGISTERS_SV

`include "waveform_registers.sv"

module as_unit_registers (
    input logic clk_i,
    input logic rst_n_i,

    /* Write interface */
    input logic write_i,
    input logic [10:0] write_address_i,
    input logic [3:0][7:0] write_data_i,
    input logic [3:0] write_strobe_i,

    /* Read interface */
    input logic read_i,
    input logic [10:0] read_address_i,
    output logic [31:0] read_data_o,
    output logic read_error_o,

    /* Start ADSR cycle */
    output logic [3:0] adsr_start_o,
    output logic [3:0] adsr_enable_o,
    
    /* Increment of each state */
    output logic [3:0][31:0] attack_step_o,
    output logic [3:0][31:0] decay_step_o,
    output logic [3:0][31:0] release_step_o,

    /* Time duration */
    output logic [3:0][31:0] sustain_duration_o,

    /* Gain level  */
    output logic [3:0][31:0] attack_level_o,
    output logic [3:0][31:0] sustain_level_o,

    /* Status */
    input logic [3:0] adsr_idle_i,

    /* Custom table control */
    output logic write_table_o,
    output logic [9:0] table_addr_o,
    output logic [15:0] pcm_o,

    /* Control */
    output logic [3:0] wave_enable_o,
    output logic [3:0] wave_start_o,

    /* Frequency control */
    output logic [3:0][31:0] wave_frequency_o,

    /* Duty cycle control */
    output logic [31:0] square_wave_duty_cycle_o,

    /* Gains Q1.15 */
    output logic [3:0][15:0] wave_gain_o
);

//====================================================================================
//      ERROR LOGIC
//====================================================================================

    assign read_error_o = read_i & (read_address_i < 1024);


//====================================================================================
//      CUSTOM TABLE LOGIC
//====================================================================================

    assign pcm_o = write_data_i[1:0];

    assign write_table_o = (write_address_i < 1024) & write_i;

    assign table_addr_o = write_address_i[9:0];


//====================================================================================
//      WAVEFORM SYNTHESISERS REGISTERS
//====================================================================================

    logic [3:0] write_register, read_register; logic [3:0][31:0] read_data_register;

    genvar i;

    generate
        for (i = 0; i < 4; ++i) begin
            /* Each waveform has a common 8 register interface */
            assign write_register[i] = (write_address_i >= 1024 + (i * 8)) & (write_address_i < 1024 + (i * 8) + 8) * write_i;
            assign read_register[i] = (read_address_i >= 1024 + (i * 8)) & (read_address_i < 1024 + (i * 8) + 8) * read_i;

            waveform_registers registers (
                .clk_i   ( clk_i   ),
                .rst_n_i ( rst_n_i ),

                /* Address interface */
                .write_i         ( write_register[i]    ),
                .write_strobe_i  ( write_strobe_i       ),
                .write_address_i ( write_address_i[2:0] ),
                .read_address_i  ( read_address_i[2:0]  ),
                .read_i          ( read_register[i]     ),

                /* Data interface */
                .write_data_i ( write_data_i          ),
                .read_data_o  ( read_data_register[i] ),

                /* Control interface */
                .wave_start_o  ( wave_start_o[i]  ),
                .wave_enable_o ( wave_enable_o[i] ),
                .adsr_start_o  ( adsr_start_o[i]  ),
                .adsr_enable_o ( adsr_enable_o[i] ),
                .adsr_idle_i   ( adsr_idle_i[i]   ),

                /* Gain interface */
                .gain_o ( wave_gain_o[i] ),

                /* Frequency interface */
                .phase_increment_o ( wave_frequency_o[i] ),

                /* ADSR interface */
                .attack_step_o      ( attack_step_o[i]      ),
                .decay_step_o       ( decay_step_o[i]       ),
                .release_step_o     ( release_step_o[i]     ),
                .sustain_duration_o ( sustain_duration_o[i] ),
                .attack_level_o     ( attack_level_o[i]     ),
                .sustain_level_o    ( sustain_level_o[i]    )
            );
        end    
    endgenerate


//====================================================================================
//      SQUARE WAVE DUTY CYCLE REGISTER
//====================================================================================

    logic [3:0][7:0] mask;

        always_comb begin
            for (int i = 0; i < 4; ++i) begin
                mask[i] = write_strobe_i[i] ? '1 : '0;
            end
        end


    logic [31:0] square_wave_duty_cycle;

        always_ff @(posedge clk_i) begin
            if (!rst_n_i) begin
                square_wave_duty_cycle <= '0;
            end else if (write_i & (write_address_i == 1055)) begin
                square_wave_duty_cycle <= write_data_i | (square_wave_duty_cycle & mask);
            end
        end

    assign square_wave_duty_cycle_o = square_wave_duty_cycle;


//====================================================================================
//      OUTPUT LOGIC
//====================================================================================

    always_comb begin
        if (read_i) begin
            read_data_o = ((read_address_i == 1055) ? square_wave_duty_cycle : '0) | read_data_register[0] | 
                           read_data_register[1] | read_data_register[2] | read_data_register[3];
        end else begin
            read_data_o = '0;
        end
    end

endmodule : as_unit_registers

`endif 