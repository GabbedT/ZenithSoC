#define MMIO_START 0x00002000

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

/* PRNG */
#define PRNG_BASE (ETHERNET_BASE + DEVICE_INTERLEAVE)
