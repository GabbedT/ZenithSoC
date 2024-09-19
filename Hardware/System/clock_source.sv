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

    system_clocking pll (
        .sys_clk_o ( sys_clk_o ),
        .mem_clk_o ( mem_clk_o ), 
        .ext_clk_i ( ext_clk_i ),
        .locked    ( locked_o  )
    );

endmodule : clock_source

`endif 