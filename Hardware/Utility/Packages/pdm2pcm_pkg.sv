`ifndef PDM2PCM_PKG_SV
    `define PDM2PCM_PKG_SV

package pdm2pcm_pkg;

    localparam LEFT = 0;
    localparam RIGHT = 1;
    
    /* Event type during conversion operation */
    typedef enum logic [5:0] {
        PDM2PCM_BUFFER_FULL          = 6'b000001,
        PDM2PCM_LEFT_THRESHOLD_HIGH  = 6'b000010,
        PDM2PCM_LEFT_THRESHOLD_LOW   = 6'b000100,
        PDM2PCM_RIGHT_THRESHOLD_HIGH = 6'b001000,
        PDM2PCM_RIGHT_THRESHOLD_LOW  = 6'b010000,
        PDM2PCM_ERROR                = 6'b100000
    } pdm2pcm_event_t;

    /* Status structure */
    typedef struct packed {
        /* FIFO Status */
        logic buffer_empty;
        logic buffer_full;
    } pdm2pcm_status_t;

    /* SPI Register offsets */
    typedef enum logic [2:0] {
        /* Status Register */
        PDM2PCM_STATUS,

        /* Control Register */
        PDM2PCM_CONTROL,

        /* Gain Register */
        PDM2PCM_GAIN,

        /* Decimation Factor Register */
        PDM2PCM_DECIMATION_FACTOR,

        /* Normalization Factor Register */
        PDM2PCM_NORMALIZER,

        /* FIFO that holds the samples */
        PDM2PCM_SAMPLE_BUFFER,

        /* FIFO that holds the samples */
        PDM2PCM_THRESHOLD,

        /* Event Register */
        PDM2PCM_EVENT
    } pdm2pcm_registers_t;

    /* Control structure */
    typedef struct packed {
        /* Enable PDM interface */
        logic interface_enable;

        /* Enable writing into the sample buffer */
        logic buffer_enable;

        /* Single or dual channel selection */
        logic dual_channel;

        /* Select channel on single mode */
        logic channel;

        /* Enable interrupt generation */
        logic [5:0] interrupt_enable;
        
        /* Divide the clock */
        logic [6:0] clock_divisor;
    } pdm2pcm_control_t;

endpackage : pdm2pcm_pkg

import pdm2pcm_pkg::*;

`endif
