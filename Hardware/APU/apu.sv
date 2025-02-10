`ifndef APU_SV
    `define APU_SV

`include "audio_capture_unit.sv"
`include "audio_synthesis_unit.sv"

module apu #(
    parameter BUFFER_SIZE = 512
) (
    input logic clk_i,
    input logic rst_n_i,
    output logic interrupt_o,

    /* Write interface */
    input logic write_i,
    input logic [11:0] write_address_i,
    input logic [3:0][7:0] write_data_i,
    input logic [3:0] write_strobe_i,
    output logic write_error_o,

    /* Read interface */
    input logic read_i,
    input logic [11:0] read_address_i,
    output logic [31:0] read_data_o,
    output logic read_error_o,

    /* External microphone interface */
    input logic pdm_data_i,
    output logic pdm_clk_o,
    output logic pdm_lrsel_o,

    /* PWM interface */
    output logic pwm_o,
    output logic audio_enable_o
);

//====================================================================================
//      AUDIO CAPTURE UNIT
//====================================================================================

    localparam ACU_END_ADDRESS = 12'b000000000111;

    logic [31:0] acu_read_data; logic acu_read_done, acu_read_error, acu_write_done, acu_write_error;
    logic acu_write, acu_read, acu_interrupt;

    assign acu_write = !write_address_i[11] & write_i;
    assign acu_read = !read_address_i[11] & read_i;

    audio_capture_unit #(
        .BUFFER_SIZE( BUFFER_SIZE )
    ) ac_unit (
        .clk_i       ( clk_i       ),
        .rst_n_i     ( rst_n_i     ),
        .interrupt_o ( interrupt_o ),

        .pdm_data_i  ( pdm_data_i  ),
        .pdm_clk_o   ( pdm_clk_o   ),
        .pdm_lrsel_o ( pdm_lrsel_o ),

        .write_i         ( acu_write            ),
        .write_address_i ( write_address_i[2:0] ),
        .write_data_i    ( write_data_i         ),
        .write_strobe_i  ( write_strobe_i       ),
        .write_done_o    ( acu_write_done       ),
        .write_error_o   ( acu_write_error      ),

        .read_i         ( acu_read            ),
        .read_address_i ( read_address_i[2:0] ),
        .read_data_o    ( acu_read_data       ),
        .read_done_o    ( acu_read_done       ),
        .read_error_o   ( acu_read_error      )
    );

    logic acu_read_address_error, acu_write_address_error;

    assign acu_write_address_error = (write_address_i > ACU_END_ADDRESS & acu_write);
    assign acu_read_address_error = (read_address_i > ACU_END_ADDRESS & acu_read);


//====================================================================================
//      AUDIO SYNTHESIS UNIT
//====================================================================================

    localparam ASU_END_ADDRESS = 12'b110000100001;

    logic [31:0] asu_read_data; logic asu_read_done, asu_read_error, asu_write_done, asu_write_error;
    logic asu_write, asu_read;

    assign asu_write = write_address_i[11] & write_i;
    assign asu_read = read_address_i[11] & read_i;

    audio_synthesis_unit ws_unit (
        .clk_i   ( clk_i   ),
        .rst_n_i ( rst_n_i ),

        .write_i         ( asu_write             ),
        .write_address_i ( write_address_i[10:0] ),
        .write_data_i    ( write_data_i          ),
        .write_strobe_i  ( write_strobe_i        ),
        .write_error_o   ( asu_write_error       ),

        .read_i         ( asu_read             ),
        .read_address_i ( read_address_i[10:0] ),
        .read_data_o    ( asu_read_data        ),
        .read_error_o   ( asu_read_error       ),

        .pwm_o          ( pwm_o          ),
        .audio_enable_o ( audio_enable_o )
    );

    assign asu_read_done = asu_read;
    assign asu_write_done = asu_write;


    logic asu_read_address_error, asu_write_address_error;

    assign asu_write_address_error = (write_address_i > ASU_END_ADDRESS & asu_write);
    assign asu_read_address_error = (read_address_i > ASU_END_ADDRESS & asu_read);
    

//====================================================================================
//      OUTPUT LOGIC
//====================================================================================

    assign read_done_o = asu_read_done | acu_read_done;
    assign write_done_o = asu_write_done | acu_write_done;

    assign read_error_o = asu_read_error | acu_read_error | asu_read_address_error | acu_read_address_error;
    assign write_error_o = asu_write_error | acu_write_error | asu_write_address_error | acu_write_address_error;

        always_comb begin
            if (asu_read_done) begin
                read_data_o = asu_read_data;
            end else if (acu_read_done) begin
                read_data_o = acu_read_data;
            end else begin
                read_data_o = '0;
            end
        end

endmodule : apu

`endif 