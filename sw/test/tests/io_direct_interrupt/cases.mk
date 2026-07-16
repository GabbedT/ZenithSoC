CASES := \
	uart_tx_empty uart_tx_done uart_rx_done uart_rx_full \
	timer gpio_posedge gpio_negedge gpio_both gpio_high \
	trace spi apu

CASE ?= uart_tx_empty

CASE_ID_uart_tx_empty := 1
CASE_ID_uart_tx_done  := 2
CASE_ID_uart_rx_done  := 3
CASE_ID_uart_rx_full  := 4
CASE_ID_timer         := 5
CASE_ID_gpio_posedge  := 6
CASE_ID_gpio_negedge  := 7
CASE_ID_gpio_both     := 8
CASE_ID_gpio_high     := 9
CASE_ID_trace         := 10
CASE_ID_spi           := 11
CASE_ID_apu           := 12

CASE_ID := $(CASE_ID_$(CASE))

ifeq ($(CASE_ID),)
$(error Unknown CASE '$(CASE)')
endif

CASE_CPPFLAGS := -DINTERRUPT_TEST_CASE=$(CASE_ID)
