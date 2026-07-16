DRIVERS := UART Timer GPIO SPI AudioCapture TraceUnit Ethernet SD
include tests/io_direct_interrupt/cases.mk

SHARED_CPP := tests/io_direct_interrupt/io_interrupt_tests.cpp
SHARED_INCLUDE := tests/io_direct_interrupt
STARTUP_DEFINES := -DINTERRUPT_VECTORED

TRACE ?= 0
MAX_CYCLES ?= 5000000
