module dut_wrapper (
    input  logic        clk,
    input  logic        rst_n,

    input  logic        write_i,
    input  logic [31:0] write_address_i,
    input  logic [31:0] write_data_i,
    input  logic [3:0]  write_strobe_i,
    output logic        write_done_o,
    output logic        write_error_o,

    input  logic        read_i,
    input  logic [31:0] read_address_i,
    output logic [31:0] read_data_o,
    output logic        read_done_o,
    output logic        read_error_o,

    output logic        interrupt_o
);

    logic [7:0] external_pins;
    wire  [7:0] pins;
    logic [7:0] gpio_read_data;
    logic [7:0] gpio_interrupt;

    assign pins = external_pins;
    assign interrupt_o = |gpio_interrupt;

    assign write_done_o = write_i;
    assign read_done_o = read_i;
    assign write_error_o = 1'b0;
    assign read_error_o = 1'b0;

    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            external_pins <= '0;
        end else if (write_i && write_address_i[2:0] == 6) begin
            external_pins <= write_data_i[7:0];
        end
    end


    for (genvar i = 0; i < 8; ++i) begin : gpio_gen
        gpio u_gpio (
            .clk_i       ( clk   ),
            .rst_n_i     ( rst_n ),
            .pin_io      ( pins[i] ),
            .interrupt_o ( gpio_interrupt[i] ),

            .write_i         ( write_i              ),
            .write_address_i ( write_address_i[2:0] ),
            .write_data_i    ( write_data_i[i]      ),
            .write_error_o   (                      ),

            .read_i         ( read_i                ),
            .read_address_i ( read_address_i[2:0]   ),
            .read_data_o    ( gpio_read_data[i]     ),
            .read_error_o   (                       )
        );
    end

    assign read_data_o = {24'b0, gpio_read_data};

endmodule
