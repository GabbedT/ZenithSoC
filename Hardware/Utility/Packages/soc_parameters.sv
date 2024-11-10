`ifndef SOC_PARAMETERS_SV
    `define SOC_PARAMETERS_SV

`include "../../CPU/ApogeoRV/Hardware/Include/Headers/apogeo_memory_map.svh"

package soc_parameters;
    
//====================================================================================
//      CPU PARAMETERS
//====================================================================================

    /* Predictor table size */ 
    localparam PREDICTOR_SIZE = 512;

    /* Branch target buffer cache size */
    localparam BTB_SIZE = 512;

    /* Store buffer entries number */
    localparam STORE_BUFFER_SIZE = 4;

    /* Maximum number of instruction held by the buffer */
    localparam INSTRUCTION_BUFFER_SIZE = 8;

    /* Reorder Buffer entries */
    localparam ROB_DEPTH = 32;

//====================================================================================
//      CACHE PARAMETERS
//====================================================================================

    /* Data cache parameters */
    localparam DCACHE_SIZE = 2**12;
    localparam DBLOCK_SIZE_BYTE = 16;

    /* Instruction cache parameters */
    localparam ICACHE_SIZE = 2**12;
    localparam IBLOCK_SIZE_BYTE = 16;


//====================================================================================
//      UART DEVICE PARAMETERS
//====================================================================================

    /* Size in bytes */
    localparam UART_RX_BUFFER_SIZE = 2**11;
    localparam UART_TX_BUFFER_SIZE = 2**11;

    /* Number of UARTs */
    localparam UART_DEVICE_NUMBER = 1;

    /* Memory mapped registers */
    localparam UART_DEVICE_SPACE = 4;

    /* UART MMIO address */
    localparam UART_BASE_ADDRESS = `IO_START;
    localparam UART_END_ADDRESS = UART_BASE_ADDRESS + ((UART_DEVICE_SPACE - 1) * 4 * UART_DEVICE_NUMBER);


//====================================================================================
//      TIMER DEVICE PARAMETERS
//====================================================================================

    /* Number of UARTs */
    localparam TIMER_DEVICE_NUMBER = 1;

    /* Memory mapped registers */
    localparam TIMER_DEVICE_SPACE = 5;

    /* Timer MMIO address */
    localparam TIMER_BASE_ADDRESS = UART_BASE_ADDRESS + 2**13;
    localparam TIMER_END_ADDRESS = TIMER_BASE_ADDRESS + ((TIMER_DEVICE_SPACE - 1) * 4 * TIMER_DEVICE_NUMBER);


//====================================================================================
//      GPIO DEVICE PARAMETERS
//====================================================================================

    /* Number of UARTs */
    localparam GPIO_DEVICE_NUMBER = 1;

    /* Memory mapped registers */
    localparam GPIO_DEVICE_SPACE = 4;

    /* GPIO MMIO address */
    localparam GPIO_BASE_ADDRESS = TIMER_BASE_ADDRESS + 2**13;
    localparam GPIO_END_ADDRESS = GPIO_BASE_ADDRESS + ((GPIO_DEVICE_SPACE - 1) * 4 * GPIO_DEVICE_NUMBER);


//====================================================================================
//      SPI DEVICE PARAMETERS
//====================================================================================

    /* Size in bytes */
    localparam SPI_RX_BUFFER_SIZE = 2**9;
    localparam SPI_TX_BUFFER_SIZE = 2**9;

    /* Number of SPIs */
    localparam SPI_DEVICE_NUMBER = 1;

    /* Number of possible slaves (each device might have different slaves) (max 32) */
    localparam SPI_SLAVES = 1;

    /* Memory mapped registers */
    localparam SPI_DEVICE_SPACE = 5;

    /* SPI MMIO address */
    localparam SPI_BASE_ADDRESS = GPIO_BASE_ADDRESS + 2**13;
    localparam SPI_END_ADDRESS = SPI_BASE_ADDRESS + ((SPI_DEVICE_SPACE - 1) * 4 * SPI_DEVICE_NUMBER);


//====================================================================================
//      ETHERNET DEVICE PARAMETERS
//====================================================================================

    /* Physical chip address in the board */
    localparam ETH_PHY_ADDRESS = 5'b00001;

    /* SoC MAC address */
    localparam ETH_MAC_ADDRESS = 48'hDE_AD_BE_EF_00_00;

    /* Size in bytes */
    localparam ETH_RX_BUFFER_SIZE = 2**12;
    localparam ETH_TX_BUFFER_SIZE = 2**12;

    /* Number of Ethernet MAC (max 1) */
    localparam ETH_DEVICE_NUMBER = 1;

    /* Number of packets */
    localparam ETH_RX_PACKETS = 16;
    localparam ETH_TX_PACKETS = 16;

    /* Memory mapped registers */
    localparam ETH_DEVICE_SPACE = 40;

    /* Ethernet MAC MMIO address */
    localparam ETH_BASE_ADDRESS = SPI_BASE_ADDRESS + 2**13;
    localparam ETH_END_ADDRESS = ETH_BASE_ADDRESS + ((ETH_DEVICE_SPACE - 1) * 4);


//====================================================================================
//      BOOT MEMORY PARAMETERS
//====================================================================================

    localparam BOOT_SIZE = 2 ** 13;
    
    /* Number of boot memories (max 1) */
    localparam BOOT_MEMORY_NUMBER = 1;


//====================================================================================
//      PSEUDO RANDOM NUMBER GENERATOR
//====================================================================================

    /* Number of PRNGs */
    localparam PRNG_NUMBER = 1;

    /* Ethernet MAC MMIO address */
    localparam PRNG_BASE_ADDRESS = ETH_BASE_ADDRESS + 2**13;
    localparam PRNG_END_ADDRESS = PRNG_BASE_ADDRESS + 4;


//====================================================================================
//      NON CACHABLE MEMORY PARAMETERS
//====================================================================================

    localparam NC_MEMORY_SIZE = 2 ** 11;
    
    /* Number of boot memories (max 1) */
    localparam NC_MEMORY_NUMBER = 1;

    /* Non cachable memory MMIO address */
    localparam NCMEM_BASE_ADDRESS = PRNG_BASE_ADDRESS + 2**13;
    localparam NCMEM_END_ADDRESS = NCMEM_BASE_ADDRESS + NC_MEMORY_SIZE;


//====================================================================================
//      MMIO PARAMETERS
//====================================================================================

    /* Memory bytes between one device address and another */
    localparam BYTES_INTERLEAVE = 'd128;

    /* CPU interrupts */
    localparam INTERRUPT_SOURCES = 6;

    /* Devices connected to AXI network */
    localparam NETWORK_DEVICES = UART_DEVICE_NUMBER + 
                                 TIMER_DEVICE_NUMBER + 
                                 GPIO_DEVICE_NUMBER + 
                                 SPI_DEVICE_NUMBER + 
                                 ETH_DEVICE_NUMBER + 
                                 BOOT_MEMORY_NUMBER + 
                                 PRNG_NUMBER +
                                 NC_MEMORY_NUMBER;


    /* To add more than 1 device, increase the parameter and add the device address into 
     * the array. For example with 3 UARTs: UART_BASE_ADDRESS, UART_BASE_ADDRESS + BYTES_INTERLEAVE
     * UART_BASE_ADDRESS + 2 * BYTES_INTERLEAVE */

    localparam int LOW_SLAVE_ADDRESS [NETWORK_DEVICES] = '{
        `BOOT_START, 

        UART_BASE_ADDRESS, 

        TIMER_BASE_ADDRESS,

        GPIO_BASE_ADDRESS,

        SPI_BASE_ADDRESS,

        ETH_BASE_ADDRESS,

        PRNG_BASE_ADDRESS,

        NCMEM_BASE_ADDRESS
    };

    localparam int HIGH_SLAVE_ADDRESS [NETWORK_DEVICES] = '{
        `BOOT_END, 

        UART_END_ADDRESS, 

        TIMER_END_ADDRESS,

        GPIO_END_ADDRESS,

        SPI_END_ADDRESS,

        ETH_END_ADDRESS,

        PRNG_END_ADDRESS,

        NCMEM_END_ADDRESS
    };

endpackage : soc_parameters

import soc_parameters::*;

`endif