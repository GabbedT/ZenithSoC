`ifndef VGA_REGISTERS_SV
    `define VGA_REGISTERS_SV

module vga_registers (
    input logic clk_i,
    input logic rst_n_i,
    output logic interrupt_o,

    /* Configuration */
    output logic enable_video_o,
    output resolution_t resolution_o,
    output logic [26:0] frame_buffer_base_o,
    output logic [18:0] frame_buffer_size_o,

    /* Sprite interface */
    output logic write_ctable_o,
    output logic write_ptable_o,
    output logic [11:0] sprite_data_o,
    output logic [6:0] sprite_address_o,
    output logic [9:0] sprite_x_o,
    output logic [9:0] sprite_y_o,
    output logic sprite_enable_o,

    /* Status */
    input logic [9:0] vsync_counter_i,
    input logic video_on_i,
    input logic buffer_empty_i,
    input logic ddr_req_error_i,
    input logic frame_done_i,

    /* Write interface */
    input logic write_i,
    input logic [6:0] write_address_i,
    input logic [3:0][7:0] write_data_i,
    input logic [3:0] write_strobe_i,
    output logic write_error_o,

    /* Read interface */
    input logic read_i,
    input logic [6:0] read_address_i,
    output logic [31:0] read_data_o,
    output logic read_error_o
);

    registers_t write_address, read_address;
    logic write_register;

    assign write_address = registers_t'(write_address_i[2:0]);
    assign read_address = registers_t'(read_address_i[2:0]);
    assign write_register = write_i & (write_address_i < VGA_REGISTER_SPACE);

//====================================================================================
//      ERROR CHECK
//====================================================================================

    assign write_error_o = write_i & (write_address_i >= VGA_DEVICE_SPACE);

    /* The sprite tables are write-only through this interface. */
    assign read_error_o = read_i & (read_address_i >= VGA_REGISTER_SPACE);


//====================================================================================
//      STATUS REGISTER
//====================================================================================

    /* Status register holds the status and control of VGA device */
    control_status_register_t status_register;
    logic [3:0] enable_interrupt;
    logic enable_video;
    resolution_t resolution;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                enable_interrupt <= '0;
                enable_video <= 1'b0;
                resolution <= _640x480_;
            end else if (write_register & (write_address == VGA_CTLR_STATUS)) begin
                if (write_strobe_i[0]) begin
                    enable_interrupt[2:0] <= write_data_i[0][7:5];
                    enable_video <= write_data_i[0][4];
                    resolution <= resolution_t'(write_data_i[0][3:2]);
                end

                if (write_strobe_i[1]) begin
                    enable_interrupt[3] <= write_data_i[1][0];
                end
            end 
        end 


        always_comb begin
            status_register = '0;

            status_register.vsync_counter = vsync_counter_i;
            status_register.enable_interrupt = enable_interrupt;
            status_register.enable_video = enable_video;
            status_register.resolution = resolution;
            status_register.video_on = video_on_i;
            status_register.frame_done = frame_done_i;
        end

    assign enable_video_o = enable_video;
    assign resolution_o = resolution;


//====================================================================================
//      FRAME BUFFER REGISTERS
//====================================================================================

    logic [26:0] frame_buffer_base;
    logic [18:0] frame_buffer_size;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin
                frame_buffer_base <= '0;
                frame_buffer_size <= '0;
            end else begin
                if (write_register & (write_address == VGA_FRM_BUF_BASE)) begin
                    if (write_strobe_i[0]) begin
                        frame_buffer_base[7:0] <= write_data_i[0];
                    end

                    if (write_strobe_i[1]) begin
                        frame_buffer_base[15:8] <= write_data_i[1];
                    end

                    if (write_strobe_i[2]) begin
                        frame_buffer_base[23:16] <= write_data_i[2];
                    end

                    if (write_strobe_i[3]) begin
                        frame_buffer_base[26:24] <= write_data_i[3][2:0];
                    end
                end

                if (write_register & (write_address == VGA_FRM_BUF_SIZE)) begin
                    if (write_strobe_i[0]) begin
                        frame_buffer_size[7:0] <= write_data_i[0];
                    end

                    if (write_strobe_i[1]) begin
                        frame_buffer_size[15:8] <= write_data_i[1];
                    end

                    if (write_strobe_i[2]) begin
                        frame_buffer_size[18:16] <= write_data_i[2][2:0];
                    end
                end
            end
        end

    assign frame_buffer_base_o = frame_buffer_base;
    assign frame_buffer_size_o = frame_buffer_size;


//====================================================================================
//      INTERRUPT
//====================================================================================

    logic [3:0] event_edge;

        edge_detector #(1, 0) buffer_empty_detector (
            .clk_i   ( clk_i   ),
            .rst_n_i ( rst_n_i ),

            .signal_i ( buffer_empty_i & enable_video_o ),
            .edge_o   ( event_edge[0]                       )
        );

        edge_detector #(1, 0) video_on_detector (
            .clk_i   ( clk_i   ),
            .rst_n_i ( rst_n_i ),

            .signal_i ( video_on_i & enable_video_o ),
            .edge_o   ( event_edge[1]                   )
        );

        edge_detector #(1, 0) frame_done_detector (
            .clk_i   ( clk_i   ),
            .rst_n_i ( rst_n_i ),

            .signal_i ( frame_done_i & enable_video_o ),
            .edge_o   ( event_edge[2]                         )
        );

        edge_detector #(1, 0) ddr_error_detector (
            .clk_i   ( clk_i   ),
            .rst_n_i ( rst_n_i ),

            .signal_i ( ddr_req_error_i ),
            .edge_o   ( event_edge[3]    )
        );

    assign interrupt_o = ((event_edge & enable_interrupt) != '0);


//====================================================================================
//      EVENT REGISTER
//====================================================================================

    event_register_t event_register;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin 
                event_register <= '0;
            end else if (write_register & (write_address == VGA_EVENT)) begin
                if (write_strobe_i[0]) begin
                    if (write_data_i[0][0]) begin
                        event_register.buffer_empty <= 1'b0;
                    end

                    if (write_data_i[0][1]) begin
                        event_register.video_on <= 1'b0;
                    end

                    if (write_data_i[0][2]) begin
                        event_register.frame_done <= 1'b0;
                    end

                    if (write_data_i[0][3]) begin
                        event_register.ddr_error <= 1'b0;
                    end
                end
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

                if (event_edge[3]) begin
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
                sprite_register <= '0;
                sprite_register.enable <= 1'b1;
            end else if (write_register & (write_address == VGA_SPRITE)) begin
                if (write_strobe_i[0]) begin
                    sprite_register.enable <= write_data_i[0][0];
                    sprite_register.x_position[6:0] <= write_data_i[0][7:1];
                end

                if (write_strobe_i[1]) begin
                    sprite_register.x_position[9:7] <= write_data_i[1][2:0];
                    sprite_register.y_position[4:0] <= write_data_i[1][7:3];
                end

                if (write_strobe_i[2]) begin
                    sprite_register.y_position[9:5] <= write_data_i[2][4:0];
                end
            end 
        end 

    assign sprite_enable_o = sprite_register.enable;

    assign sprite_x_o = sprite_register.x_position;
    assign sprite_y_o = sprite_register.y_position;


//====================================================================================
//      SPRITE TABLES
//====================================================================================

    assign write_ctable_o = write_i & (write_address_i >= VGA_CTABLE_BASE) &
                                      (write_address_i < VGA_CTABLE_BASE + VGA_CTABLE_SIZE);

    assign write_ptable_o = write_i & (write_address_i >= VGA_PTABLE_BASE) &
                                      (write_address_i < VGA_PTABLE_BASE + VGA_PTABLE_SIZE);

    assign sprite_data_o = {write_data_i[1][3:0], write_data_i[0]};

    logic [6:0] sprite_table_offset;

        always_comb begin
            sprite_address_o = '0;
            sprite_table_offset = '0;

            if (write_ctable_o) begin
                sprite_address_o[6] = 1'b1;
                sprite_table_offset = write_address_i - VGA_CTABLE_BASE;
                sprite_address_o[5:0] = sprite_table_offset[5:0];
            end else if (write_ptable_o) begin
                sprite_table_offset = write_address_i - VGA_PTABLE_BASE;
                sprite_address_o[5:0] = sprite_table_offset[5:0];
            end
        end


//====================================================================================
//      DATA READ
//====================================================================================

        always_comb begin
            /* Default value */
            read_data_o = '0;

            case (read_address)
                VGA_CTLR_STATUS: read_data_o = {{13{1'b0}}, status_register};

                VGA_FRM_BUF_BASE: read_data_o = {{5{1'b0}}, frame_buffer_base};

                VGA_FRM_BUF_SIZE: read_data_o = {{13{1'b0}}, frame_buffer_size};

                VGA_EVENT: read_data_o = {{28{1'b0}}, event_register};

                VGA_SPRITE: read_data_o = {{11{1'b0}}, sprite_register};

                default: read_data_o = '0;
            endcase
        end


//====================================================================================
//      ASSERTIONS
//====================================================================================

`ifndef SYNTHESIS

    initial begin
        assert (VGA_CTABLE_BASE == 7'd5)
            else $error("VGA color table overlaps the register space");

        assert (VGA_PTABLE_BASE == VGA_CTABLE_BASE + VGA_CTABLE_SIZE)
            else $error("VGA pattern table does not follow the color table");

        assert (VGA_DEVICE_SPACE == VGA_PTABLE_BASE + VGA_PTABLE_SIZE)
            else $error("VGA device space does not contain the sprite tables");
    end

    assert property (@(posedge clk_i)
        disable iff (!rst_n_i)
        !(write_ctable_o && write_ptable_o))
        else $error("VGA color and pattern table writes overlap");

    assert property (@(posedge clk_i)
        disable iff (!rst_n_i)
        (write_ctable_o || write_ptable_o) |-> !write_register)
        else $error("VGA sprite table write aliases a register address");

    assert property (@(posedge clk_i)
        disable iff (!rst_n_i)
        write_ctable_o |-> (sprite_address_o ==
                            (7'b1000000 | (write_address_i - VGA_CTABLE_BASE))))
        else $error("VGA color table address translation failed");

    assert property (@(posedge clk_i)
        disable iff (!rst_n_i)
        write_ptable_o |-> (sprite_address_o ==
                            (write_address_i - VGA_PTABLE_BASE)))
        else $error("VGA pattern table address translation failed");

    assert property (@(posedge clk_i)
        disable iff (!rst_n_i)
        enable_video_o |-> ((frame_buffer_base[3:0] == '0) &&
                            (frame_buffer_size[3:0] == '0) &&
                            (frame_buffer_size != '0)))
        else $error("VGA framebuffer configuration is not valid");

    assert property (@(posedge clk_i)
        disable iff (!rst_n_i)
        (write_register && (write_address == VGA_CTLR_STATUS) &&
         write_strobe_i[0]) |-> (write_data_i[0][3:2] <= 2'd1))
        else $error("VGA status register contains an invalid resolution");

`endif


endmodule : vga_registers

`endif
