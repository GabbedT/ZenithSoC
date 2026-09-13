`ifndef VGA_REGISTERS_SV
    `define VGA_REGISTERS_SV

module vga_registers (
    input logic clk_i,
    input logic rst_n_i,
    output logic interrupt_o,

    /* Configuration */
    output logic enable_video_o,
    output resolution_t resolution_o,

    /* Sprite interface */
    output logic write_ctable_o,
    output logic write_ptable_o,
    output logic [11:0] sprite_data_o,
    output logic [6:0] sprite_address_o,
    output logic [9:0] sprite_x_o,
    output logic [9:0] sprite_y_o,
    output logic sprite_enable_o,

    /* Status */
    input logic [8:0] vsync_counter_i,
    input logic video_on_i,
    input logic buffer_empty_i,
    input logic ddr_req_error_i,
    input logic frame_done_i,

    /* Write interface */
    input logic write_i,
    input logic [31:0] write_address_i,
    input logic [3:0][7:0] write_data_i,
    output lo,

    /* Read interface */
    input logic read_i,
    input logic [31:0] read_address_i,
    output logic [31:0] read_data_o,
    output logic read_error_o
);

    registers_t write_address, read_address;

    assign write_address = registers_t'(write_address_i[1:0]);
    assign read_address = registers_t'(read_address_i[1:0]);

//====================================================================================
//      ERROR CHECK
//====================================================================================

    assign write_error_o = write_address_i >= 32'd64;

    assign read_error_o = read_address_i >= 32'd64;


//====================================================================================
//      STATUS REGISTER
//====================================================================================

    /* Status register holds the status and control of VGA device */
    control_status_register_t status_register;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                status_register.enable_interrupt <= '0;
                status_register.enable_video <= 1'b0;
                status_register.auto_increment <= 1'b0;

                status_register.resolution <= _640x480_;
            end else if (write_i & (write_address == VGA_CTLR_STATUS)) begin
                // FIX SEE VGA_PKG.sv
                status_register.enable_interrupt <= {write_data_i[0][7:6]}; 
                status_register.enable_video <= write_data_i[0][5];

                status_register.resolution <= resolution_t'(write_data_i[0][4]);
            end 
        end 


    assign status_register.video_on = video_on_i;
    assign status_register.buffer_empty = buffer_empty_i;
    assign status_register.frame_done = frame_done_i;

    assign status_register.vsync_counter = vsync_counter_i;

    assign enable_video_o = status_register.enable_video;
    assign resolution_o = status_register.resolution;


//====================================================================================
//      FRAME BUFFER BASE ADDRESS
//====================================================================================

    logic [26:0] frame_buffer_base;

        always_ff @(posedge clk_i) begin
            if (write_i & (write_address == VGA_FRM_BUF_BASE)) begin
                if (write_strobe_i[0]) begin 
                    frame_buffer_base[7:0] <= write_data_i[7:0];
                end

                if (write_strobe_i[1]) begin 
                    frame_buffer_base[15:8] <= write_data_i[15:8];
                end

                if (write_strobe_i[2]) begin 
                    frame_buffer_base[23:16] <= write_data_i[23:16];
                end

                if (write_strobe_i[3]) begin 
                    frame_buffer_base[26:24] <= write_data_i[26:24];
                end
            end 
        end 

    assign frame_buffer_base_o = frame_buffer_base;


//====================================================================================
//      FRAME BUFFER SIZE
//====================================================================================

    /* To contain a full 640x480 buffer */
    logic [18:0] frame_buffer_size;

        always_ff @(posedge clk_i) begin
            if (write_i & (write_address == VGA_FRM_BUF_SIZE)) begin
                if (write_strobe_i[0]) begin 
                    frame_buffer_size[7:0] <= write_data_i[7:0];
                end

                if (write_strobe_i[1]) begin 
                    frame_buffer_size[15:8] <= write_data_i[15:8];
                end

                if (write_strobe_i[2]) begin 
                    frame_buffer_size[23:16] <= write_data_i[23:16];
                end

                if (write_strobe_i[3]) begin 
                    frame_buffer_size[26:24] <= write_data_i[26:24];
                end
            end 
        end 

    assign frame_buffer_size_o = frame_buffer_size;


//====================================================================================
//      INTERRUPT
//====================================================================================

    logic [2:0] event_edge;

    // TODO: BUFFER EMPTY MUST BE: BUFFER_EMPTY & ENABLE VIDEO REGISTRED
    edge_detector #(1, 0) buffer_empty_detector (
        .clk_i   ( clk_i   ),
        .rst_n_i ( rst_n_i ),

        .signal_i ( buffer_empty_i ),
        .edge_o   ( event_edge[0]  )
    );

    edge_detector #(1, 0) frame_done_detector (
        .clk_i   ( clk_i   ),
        .rst_n_i ( rst_n_i ),

        .signal_i ( frame_done_i  ),
        .edge_o   ( event_edge[1] )
    );

    edge_detector #(1, 0) ddr_error_detector (
        .clk_i   ( clk_i   ),
        .rst_n_i ( rst_n_i ),

        .signal_i ( ddr_req_error_i  ),
        .edge_o   ( event_edge[1]    )
    );

    assign interrupt_o = ((event_edge & status_register.enable_interrupt) != '0);


//====================================================================================
//      EVENT REGISTER
//====================================================================================

    event_register_t event_register;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                event_register <= '0;
            end else if (write_i & (write_address == VGA_EVENT)) begin
                event_register <= write_data_i[0][2:0];
            end else begin 
                if (event_edge[0]) begin
                    event_register.buffer_empty <= 1'b1;
                end

                if (event_edge[1]) begin
                    event_register.frame_done <= 1'b1;
                end

                if (event_edge[2]) begin
                    event_register.ddr_error <= 1'b1;
                end
            end 
        end 


//====================================================================================
//      SPRITE REGISTER
//====================================================================================

    sprite_register_t sprite_register;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                sprite_register.enable <= 1'b1;
            end else if (write_i & (write_address == VGA_SPRITE)) begin 
                sprite_register.enable <= write_data_i[0][0];
            end 
        end 

        always_ff @(posedge clk_i) begin
            if (write_i & (write_address == VGA_SPRITE)) begin 
                sprite_register.x_position <= {write_data_i[1][2:0], write_data_i[0][7:1]};
                sprite_register.y_position <= {write_data_i[2][4:0], write_data_i[1][7:3]};
            end 
        end 

    assign sprite_enable_o = sprite_register.enable;

    assign sprite_x_o = sprite_register.x_position;
    assign sprite_y_o = sprite_register.y_position;


    // TODO: OPTIMIZE THE COMPARISONS BITS
    /* Pattern table array memory */
    assign write_ptable_o = write_i & write_address_i >= 32'd20 & write_address_i < 32'd84;

    /* Color table array memory */
    assign write_ctable_o = write_i & write_address_i >= 32'd4 & write_address_i < 32'd20;

    assign sprite_data_o = {write_data_i[1][2:0], write_data_i[0][7:0]};

    assign sprite_address_o = write_address_i[6:0];


//====================================================================================
//      DATA READ
//====================================================================================

        always_comb begin
            /* Default value */
            read_data_o = '0;

            case (read_address)
                VGA_CTLR_STATUS: read_data_o = status_register;

                VGA_FRM_BUF_BASE: read_data_o = {'0, increment};

                VGA_FRM_BUF_SIZE: read_data_o = {'0, increment};

                VGA_EVENT: read_data_o = event_register;

                VGA_SPRITE: read_data_o = sprite_register;
            endcase 
        end

endmodule : vga_registers

`endif 