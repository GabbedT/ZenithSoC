`ifndef VGA_REGISTERS_SV
    `define VGA_REGISTERS_SV

`include "../../System/edge_detector.sv"

`include "../../Utility/Packages/vga_pkg.sv"

module vga_registers #(
    parameter BUFFER_SIZE = 2**16
) (
    input logic clk_i,
    input logic rst_n_i,
    output logic interrupt_o,

    /* Configuration */
    output logic enable_video_o,
    output resolution_t resolution_o,

    /* Line buffer interface */
    output logic [$clog2(BUFFER_SIZE) - 1:0] address_o, 
    output logic write_buffer_o,
    output pixel_t pixel_o,

    /* Status */
    input logic [$clog2(BUFFER_SIZE) - 1:0] buffer_size_i,
    input logic video_on_i,
    input logic buffer_empty_i,
    input logic frame_done_i,

    /* Write interface */
    input logic write_i,
    input logic [$clog2(BUFFER_SIZE):0] write_address_i,
    input logic [3:0][7:0] write_data_i,
    output logic write_error_o,

    /* Read interface */
    input logic read_i,
    input logic [$clog2(BUFFER_SIZE):0] read_address_i,
    output logic [31:0] read_data_o,
    output logic read_error_o
);

    registers_t write_address, read_address; logic write_register;

    assign write_address = registers_t'(write_address_i[1:0]);
    assign read_address = registers_t'(read_address_i[1:0]);

    assign write_register = write_address_i[$clog2(BUFFER_SIZE)] & write_i;

    assign pixel_o = {write_data_i[1][3:0], write_data_i[0]};


//====================================================================================
//      ERROR CHECK
//====================================================================================

    assign write_error_o = (write_address_i >= (BUFFER_SIZE + 2)) & write_i;

    assign read_error_o = (read_address_i >= (BUFFER_SIZE + 2)) | (read_address_i <= (BUFFER_SIZE - 1)) & read_i;


//====================================================================================
//      STATUS REGISTER
//====================================================================================

    /* Status register holds the status and control of VGA device */
    status_register_t status_register;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                status_register.enable_interrupt <= '0;
                status_register.enable_video <= 1'b0;
                status_register.auto_increment <= 1'b0;

                status_register.resolution <= _640x480_;
            end else if (write_register & (write_address == VGA_STATUS) & !write_error_o) begin
                status_register.enable_interrupt <= {write_data_i[1][0], write_data_i[0][7:6]}; 
                status_register.enable_video <= write_data_i[0][5];
                status_register.auto_increment <= write_data_i[0][0];

                status_register.resolution <= resolution_t'(write_data_i[0][4]);
            end 
        end 


    assign status_register.video_on = video_on_i;
    assign status_register.buffer_empty = buffer_empty_i;
    assign status_register.frame_done = frame_done_i;

    assign enable_video_o = status_register.enable_video;
    assign resolution_o = status_register.resolution;


//====================================================================================
//      INCREMENT REGISTER
//====================================================================================

    /* Adds the value to the base address of the VGA line buffer */
    logic [$clog2(BUFFER_SIZE) - 1:0] increment;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                increment <= '0;
            end else if (!status_register.auto_increment) begin 
                increment <= '0;
            end else if (write_register & (write_address == VGA_INCREMENT)) begin 
                increment <= write_data_i;
            end else if (write_i & !write_address_i[$clog2(BUFFER_SIZE)]) begin
                /* Increment each time the buffer is written */
                increment <= increment + 1'b1;
            end
        end 

    assign address_o = write_address_i[$clog2(BUFFER_SIZE) - 1:0] + increment;

    assign write_buffer_o = write_i & (write_address_i <= (BUFFER_SIZE - 1));


//====================================================================================
//      INTERRUPT
//====================================================================================

    logic [2:0] event_edge;

    edge_detector #(1, 0) buffer_empty_detector (
        .clk_i   ( clk_i   ),
        .rst_n_i ( rst_n_i ),

        .signal_i ( buffer_empty_i & status_register.enable_interrupt[0] ),
        .edge_o   ( event_edge[0]                                        )
    );

    edge_detector #(1, 0) video_on_detector (
        .clk_i   ( clk_i   ),
        .rst_n_i ( rst_n_i ),

        .signal_i ( !video_on_i & status_register.enable_interrupt[1] ),
        .edge_o   ( event_edge[1]                                     )
    );

    edge_detector #(1, 0) frame_done_detector (
        .clk_i   ( clk_i   ),
        .rst_n_i ( rst_n_i ),

        .signal_i ( frame_done_i & status_register.enable_interrupt[2] ),
        .edge_o   ( event_edge[2]                                      )
    );

    assign interrupt_o = (event_edge != '0);


//====================================================================================
//      EVENT REGISTER
//====================================================================================

    event_register_t event_register;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                event_register <= '0;
            end else if (write_register & (write_address == VGA_EVENT)) begin
                event_register <= write_data_i[0][2:0];
            end else begin 
                if (event_edge[0]) begin
                    event_register.buffer_empty <= 1'b1;
                end

                if (event_edge[1]) begin
                    event_register.video_on <= 1'b1;
                end

                if (event_edge[2]) begin
                    event_register.frame_done <= 1'b1;
                end
            end 
        end 


//====================================================================================
//      DATA READ
//====================================================================================

        always_comb begin
            /* Default value */
            read_data_o = '0;

            case (read_address)
                VGA_STATUS: read_data_o = status_register;

                VGA_INCREMENT: read_data_o = {'0, increment};

                VGA_BUFFER_SIZE: read_data_o = buffer_size_i;

                VGA_EVENT: read_data_o = event_register;
            endcase 
        end

endmodule : vga_registers

`endif 