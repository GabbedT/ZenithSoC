DRIVERS := UART Timer GPIO SPI AudioCapture TraceUnit Ethernet SD
include tests/io_direct_interrupt/cases.mk

TRACE ?= 0
MAX_CYCLES ?= 5000000
