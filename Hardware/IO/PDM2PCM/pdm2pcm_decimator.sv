`ifndef PDM2PCM_DECIMATOR_SV
    `define PDM2PCM_DECIMATOR_SV

module pdm2pcm_decimator #(
    /* Output width */
    parameter WIDTH = 16,

    /* Int - Comb stages */
    parameter FILTER_ORDER = 2,

    /* M delay */
    parameter COMB_DELAY = 1
) (
    input logic clk_i,
    input logic rst_n_i,
    
    input logic reset_filter_i,
    input logic [7:0] decimator_factor_i,

    /* PDM input */
    input logic pdm_i,
    input logic valid_i,

    /* PCM output */
    output logic [WIDTH - 1:0] pcm_o,
    output logic valid_o
);

    genvar i;

//==========================================================
//      DECIMATOR COUNTER
//==========================================================

    logic [7:0] decimator; logic valid_sample;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin
                decimator <= '0;
            end else if (reset_filter_i | valid_sample) begin 
                decimator <= '0;
            end else begin
                /* Increment only if a valid sample is going to the 
                 * comb stages, however if the counter is not equal
                 * to the decimator factor, the sample is simply discarded */
                decimator <= decimator + valid_i;
            end
        end

    assign valid_sample = (decimator == decimator_factor_i);


//==========================================================
//      INTEGRATOR STAGES
//==========================================================

    logic [FILTER_ORDER - 1:0][WIDTH - 1:0] integrator, integrator_ff;

    generate
        /* Integration stages (sum the previous with the current) */
        for (i = 0; i < FILTER_ORDER; ++i) begin
            always_comb begin
                if (i == 0) begin
                    /* First integrator takes PDM input and saved value */
                    integrator[0] = (pdm_i & valid_i) + integrator_ff[0];
                end else begin
                    integrator[i] = integrator_ff[i - 1] + integrator_ff[i];
                end
            end
        end

        /* Pipeline stage */
        for (i = 0; i < FILTER_ORDER; ++i) begin
            always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
                if (!rst_n_i) begin
                    integrator_ff[i] <= '0;
                end else if (reset_filter_i) begin 
                    integrator_ff[i] <= '0;
                end else if (valid_i) begin
                    integrator_ff[i] <= integrator[i];
                end
            end
        end
    endgenerate


    logic [COMB_DELAY - 1:0][WIDTH - 1:0] integrator_delay;
    
    generate 
        if (COMB_DELAY > 1) begin 
            always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
                if (!rst_n_i) begin
                    integrator_delay <= '0;
                end else if (reset_filter_i) begin 
                    integrator_delay <= '0;
                end else if (valid_i) begin
                    integrator_delay <= {integrator_delay[COMB_DELAY - 2:0], integrator_ff[FILTER_ORDER - 1]};
                end
            end
        end else begin 
            always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
                if (!rst_n_i) begin
                    integrator_delay[0] <= '0;
                end else if (reset_filter_i) begin 
                    integrator_delay[0] <= '0;
                end else if (valid_i) begin
                    integrator_delay[0] <= integrator_ff[FILTER_ORDER - 1];
                end
            end
        end
    endgenerate

//==========================================================
//      COMB STAGES
//==========================================================

    logic [FILTER_ORDER - 1:0][WIDTH - 1:0] comb, comb_ff;
    logic [FILTER_ORDER - 1:0][COMB_DELAY - 1:0][WIDTH - 1:0] comb_delay;

    generate
        /* Comb stages */
        for (i = 0; i < FILTER_ORDER; ++i) begin
            always_comb begin
                if (i == 0) begin
                    /* First integrator takes PDM input and saved value */
                    comb[0] = integrator_ff[FILTER_ORDER - 1] - integrator_delay[COMB_DELAY - 1];
                end else begin
                    comb[i] = comb_ff[i - 1] - comb_delay[i - 1][COMB_DELAY - 1];
                end
            end
        end

        /* Pipeline stage */
        for (i = 0; i < FILTER_ORDER; ++i) begin
            always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
                if (!rst_n_i) begin
                    comb_ff[i] <= '0;
                end else if (reset_filter_i) begin 
                    comb_ff[i] <= '0;
                end else if (valid_sample) begin
                    comb_ff[i] <= comb[i];
                end
            end
        end

        /* Delay stage */
        if (COMB_DELAY > 1) begin
            for (i = 0; i < FILTER_ORDER - 1; ++i) begin
                always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
                    if (!rst_n_i) begin
                        for (int j = 0; j < COMB_DELAY; ++j) begin
                            comb_delay[i][j] <= '0;
                        end
                    end else if (reset_filter_i) begin 
                        for (int j = 0; j < COMB_DELAY; ++j) begin
                            comb_delay[i][j] <= '0;
                        end
                    end else if (valid_sample) begin
                        comb_delay[i] <= {comb_delay[i][COMB_DELAY - 2:0], comb_ff[i]};
                    end
                end
            end
        end else begin
            for (i = 0; i < FILTER_ORDER - 1; ++i) begin
                always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
                    if (!rst_n_i) begin
                        comb_delay[i][0] <= '0;
                    end else if (reset_filter_i) begin 
                        comb_delay[i][0] <= '0;
                    end else if (valid_sample) begin
                        comb_delay[i][0] <= comb_ff[i];
                    end
                end
            end
        end
    endgenerate


//==========================================================
//      OUTPUT STAGE
//==========================================================

    /* Wait until the pipeline is filled */
    logic [FILTER_ORDER - 1:0] valid_out;

        always_ff @(posedge clk_i `ifdef ASYNC or negedge rst_n_i `endif) begin
            if (!rst_n_i) begin
                valid_out <= '0;
            end else if (reset_filter_i) begin 
                valid_out <= '0;
            end else if (valid_sample) begin
                valid_out <= {valid_out[FILTER_ORDER - 2:0], valid_sample};
            end
        end

    assign valid_o = valid_out[FILTER_ORDER - 1] & valid_sample & !reset_filter_i;

    assign pcm_o = comb_ff[FILTER_ORDER - 1];

endmodule : pdm2pcm_decimator

`endif 