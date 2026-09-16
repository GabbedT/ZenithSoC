# The VGA VP device covers the register space, framebuffer and display memory.
# Address decoding remains inside dut_wrapper, just like the hardware bus.
IO_BASE = 0x00010000
IO_SIZE = 0x08086000
SIM_ARGS += +io_absolute

FW_SRCS += ../sw/src/VGA.cpp
