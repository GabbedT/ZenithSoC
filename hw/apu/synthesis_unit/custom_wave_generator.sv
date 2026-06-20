`ifndef CUSTOM_WAVE_GENERATOR_SV
    `define CUSTOM_WAVE_GENERATOR_SV

module custom_wave_generator (
    input logic clk_i,
    input logic rst_n_i,

    /* Table control */
    input logic write_table_i,
    input logic [10:0] table_addr_i,
    input logic [15:0] pcm_i,
    output logic active_table_o,

    /* Generator control */
    input logic enable_i,
    input logic start_i,

    /* Clock divisor to reach custom sampling frequencies */
    input logic [31:0] inc_step_i,

    /* PCM output */
    output logic [15:0] wave_o
);

//====================================================================================
//      PHASE REGISTER
//====================================================================================

   logic [31:0] phase_register, phase_next; logic [9:0] table_index_prev;

    assign phase_next = phase_register + inc_step_i;

        always_ff @(posedge clk_i) begin
            if (!rst_n_i | start_i) begin
                phase_register <= '0;
                table_index_prev <= '0;
            end else if (enable_i) begin
                phase_register <= phase_next;
                table_index_prev <= phase_register[31:22];
            end
        end


//====================================================================================
//      TABLE SWAP CONTROL
//====================================================================================

    logic active_table;

        always_ff @(posedge clk_i) begin
            if (!rst_n_i | start_i) begin
                active_table <= 1'b0;
            end else if (phase_register[31:22] < table_index_prev) begin
                /* Check overflow */
                active_table <= !active_table;
            end
        end

    assign active_table_o = active_table;


//====================================================================================
//      CUSTOM TABLES
//====================================================================================

    /* Custom audio PCM samples, double buffer */
    logic [15:0] audio_table_1 [1023:0];
    logic [15:0] audio_table_2 [1023:0];

        always_ff @(posedge clk_i) begin
            if (write_table_i & !table_addr_i[10]) begin
                audio_table_1[table_addr_i[9:0]] <= pcm_i;
            end
        end

        always_ff @(posedge clk_i) begin
            if (write_table_i & table_addr_i[10]) begin
                audio_table_2[table_addr_i[9:0]] <= pcm_i;
            end
        end


//====================================================================================
//      OUTPUT WAVE
//====================================================================================

    // (* keep = "true" *) logic [15:0] wave;

    /* Index of both tables */
    logic [9:0] table_index; assign table_index = phase_register[31:22];

        always_ff @(posedge clk_i) begin
            if (enable_i) begin
                wave_o <= active_table ? audio_table_2[table_index] : audio_table_1[table_index];
            end else begin
                wave_o <= '0;
            end
        end
    
    // assign wave_o = wave;

    // (* keep = "true" *) logic read_latch, full, empty;
    // (* keep = "true" *) logic write_data;
    // (* keep = "true" *) logic [9:0] table_write_address;
    // (* keep = "true" *) logic [15:0] wave_produced;

    // assign write_data = write_table_i;
    // assign table_write_address = table_addr_i;
    // assign wave_produced = wave_o;

    // always_ff @(posedge clk_i) begin
    //     if (!rst_n_i) begin
    //         read_latch <= '0;
    //     end else begin
    //         if (full) begin
    //             read_latch <= '1;
    //         end else if (empty) begin
    //             read_latch <= '0;
    //         end
    //     end
    // end

    // (* keep = "true" *) logic [15:0] pcm_fifo;
    // (* keep = "true" *) logic [10:0] indexes;
    // // Instantiate the synchronous buffer
    // (* keep_hierarchy = "yes" *)
    // synchronous_buffer #(
    //     .BUFFER_DEPTH(2**15),
    //     .DATA_WIDTH(16 + 11),
    //     .FIRST_WORD_FALL_TROUGH(0)
    // ) audio_buffer (
    //     .clk_i(clk_i),
    //     .rst_n_i(rst_n_i),

    //     // Control signals
    //     .write_i(table_index_prev != table_index), // Write when the phase register changes
    //     .read_i(read_latch), // Read when the buffer is full

    //     // Status signals
    //     .empty_o(empty),
    //     .full_o(full),

    //     // Data
    //     .write_data_i({wave, table_index}),
    //     .read_data_o({pcm_fifo, indexes})
    // );

endmodule : custom_wave_generator

`endif