#define MMIO_START 0x00004000

#define DEVICE_INTERLEAVE (1 << 13)

/* UART base address */
#define UART_BASE (MMIO_START)

/* Timer base address */
#define TIMER_BASE (UART_BASE + DEVICE_INTERLEAVE)

/* GPIO base address */
#define GPIO_BASE (TIMER_BASE + DEVICE_INTERLEAVE)

/* SPI base address */
#define SPI_BASE (GPIO_BASE + DEVICE_INTERLEAVE)

/* Ethernet base address */
#define ETHERNET_BASE (SPI_BASE + DEVICE_INTERLEAVE)

/* PRNG base address */
#define PRNG_BASE (ETHERNET_BASE + DEVICE_INTERLEAVE)

/* Audio Processing Unit base address */
#define APU_BASE (PRNG_BASE + DEVICE_INTERLEAVE)

/* Non cachable memory base address */
#define NC_MEMORY_BASE (ACU_BASE + DEVICE_INTERLEAVE)
