`ifndef COSIM_IO_SV
`define COSIM_IO_SV

module cosim_io (
    input  logic clk_i,
    input  logic rst_n_i,

    load_interface.slave  io_load_channel,
    store_interface.slave io_store_channel

);

    logic load_valid_q, store_done_q;

    always_ff @(posedge clk_i or negedge rst_n_i) begin
        if (!rst_n_i) begin
            load_valid_q <= 1'b0;
            store_done_q <= 1'b0;
        end
        else begin
            load_valid_q <= io_load_channel.request & ~io_load_channel.invalidate;
            store_done_q <= io_store_channel.request;
        end
    end

    assign io_load_channel.valid = load_valid_q;
    assign io_load_channel.data  = '0;            // IO reads always return 0
    assign io_store_channel.done = store_done_q;  // IO writes always accepted

endmodule : cosim_io

`endif