`ifndef CLOCK_SOURCE_SV
    `define CLOCK_SOURCE_SV

module clock_source (
    /* External clock source */
    input logic ext_clk_i,

    /* Generated clocks */
    output logic sys_clk_o,
    output logic mem_clk_o,

    /* PLL is locked */
    output logic locked_o
);

    `ifdef _VIVADO_ 

    /* Vivado IP */
    system_clocking pll (
        .sys_clk_o ( sys_clk_o ),
        .mem_clk_o ( mem_clk_o ), 
        .ext_clk_i ( ext_clk_i ),
        .locked    ( locked_o  )
    );

    `else 

    /* Used only for simulation */
    initial begin
        sys_clk_o <= 1'b0;
        mem_clk_o <= 1'b0;
        locked_o <= 1'b0;

        #2us;
        locked_o <= 1'b1;
    end

    always #5ns sys_clk_o <= !sys_clk_o;

    always #2.5ns mem_clk_o <= !mem_clk_o;

    `endif 

endmodule : clock_source

`endif 