`ifndef APU_PKG_SV
    `define APU_PKG_SV

package apu_pkg;

//====================================================================================
//      AUDIO CAPTURE UNIT
//====================================================================================

    localparam LEFT = 0;
    localparam RIGHT = 1;
    
    /* Event type during conversion operation */
    typedef enum logic [5:0] {
        CAPTURE_UNIT_BUFFER_FULL          = 6'b000001,
        CAPTURE_UNIT_LEFT_THRESHOLD_HIGH  = 6'b000010,
        CAPTURE_UNIT_LEFT_THRESHOLD_LOW   = 6'b000100,
        CAPTURE_UNIT_RIGHT_THRESHOLD_HIGH = 6'b001000,
        CAPTURE_UNIT_RIGHT_THRESHOLD_LOW  = 6'b010000,
        CAPTURE_UNIT_ERROR                = 6'b100000
    } capture_unit_event_t;

    /* Status structure */
    typedef struct packed {
        /* FIFO Status */
        logic buffer_empty;
        logic buffer_full;
    } capture_unit_status_t;

    /* SPI Register offsets */
    typedef enum logic [2:0] {
        /* Status Register */
        CAPTURE_UNIT_STATUS,

        /* Control Register */
        CAPTURE_UNIT_CONTROL,

        /* Gain Register */
        CAPTURE_UNIT_GAIN,

        /* Decimation Factor Register */
        CAPTURE_UNIT_DECIMATION_FACTOR,

        /* Normalization Factor Register */
        CAPTURE_UNIT_NORMALIZER,

        /* FIFO that holds the samples */
        CAPTURE_UNIT_SAMPLE_BUFFER,

        /* FIFO that holds the samples */
        CAPTURE_UNIT_THRESHOLD,

        /* Event Register */
        CAPTURE_UNIT_EVENT
    } capture_unit_registers_t;

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
    } capture_unit_control_t;


//====================================================================================
//      WAVEFORM GENERATOR UNIT
//====================================================================================

    typedef enum logic [2:0] {
        /* Contains various bit for controlling the wave */ 
        WAVE_CONTROL,

        /* Contains the increment step for the phase register */
        WAVE_INCREMENT,

        /* Contains the gain increment during attack phase */
        WAVE_ATTACK_STEP,

        /* Contains the gain increment during decay phase */
        WAVE_DECAY_STEP,

        /* Contains the gain increment during release phase */
        WAVE_RELEASE_STEP,

        /* Duration in cycles for sustain time */
        WAVE_SUSTAIN_TIME,

        /* Maximum gain level during attack */
        WAVE_ATTACK_LEVEL,

        /* Stable sustain gain level */
        WAVE_SUSTAIN_LEVEL
    } wave_synth_registers_t;


    typedef struct packed {
        /* Independent wave gain, it's selected when ADSR modulation is off */
        logic [15:0] wave_gain;

        /* ADSR FSM is in IDLE state */
        logic adsr_idle; 

        /* Enable ADSR modulation */
        logic adsr_enable;

        /* Start the ADSR modulation (used also to reset) */
        logic adsr_start;

        /* Enable wave output for mixer */
        logic wave_enable;

        /* Start the wave (used also to reset) */
        logic wave_start;
    } wave_synth_control_t;

endpackage : apu_pkg

import apu_pkg::*;

`endif
