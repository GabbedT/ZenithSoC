#include "platform.h"

#include <stdint.h>
#include <stdbool.h>

#include "GPIO.h"
#include "mmio.h"

#define GPIO_EXTERNAL_INPUT (GPIO_BASE + (6 * 4))
#define GPIO_EXTERNAL_VALUE (*(volatile uint32_t *) GPIO_EXTERNAL_INPUT)

extern "C" int main(void) {
    GPIO gpio(0);

    /* Exit flag */
    bool error = false;

    gpio.init(0x00, 0x00, 0x00, GPIO::HIGH);
    gpio.setPortValue(0xA5);

    /* Read the whole GPIO output registers */
    if (gpio.getPortValue() != 0xA5) {
        vp_println("[ERROR] GPIO port value error");
        TEST_FAIL();
    }

    for (int i = 0; i < 8; ++i) {           /* GPIO pin */
        bool value = i & 1;

        gpio.setPinDirection(i, GPIO::OUTPUT);
        gpio.setPinValue(i, value);
        error = gpio.getPinValue(i) != value;
        error |= gpio.getPinDirection(i) != GPIO::OUTPUT;

        gpio.togglePin(i);
        error |= gpio.getPinValue(i) == value;

        if (error) {
            vp_println("[ERROR] GPIO output error");
            TEST_FAIL();
        }
    }

    /* Initialize as input */
    gpio.init(0x00, 0xFF, 0x00, GPIO::HIGH);

    /* Write to external register in wrapper */
    GPIO_EXTERNAL_VALUE = 0xA5;

    for (int i = 0; i < 8; ++i) {           /* GPIO input */
        /* Take the bit at i-th index */
        bool value = (0xA5 >> i) & 1;

        /* Let the testbench run for a few cycles */
        for (int j = 0; j < 4; ++j) { gpio.getPortValue(); }

        error = gpio.getPinValue(i) != value;
        error |= gpio.getPinDirection(i) != GPIO::INPUT;

        if (error) {
            vp_println("[ERROR] GPIO input error");
            TEST_FAIL();
        }
    }

    for (int i = 0; i < 4; ++i) {           /* Interrupt trigger */
        GPIO::triggerLevel_e trigger;

        switch (i) {
            case 0: trigger = GPIO::HIGH;
                break;

            case 1: trigger = GPIO::POSEDGE;
                break;

            case 2: trigger = GPIO::NEGEDGE;
                break;

            case 3: trigger = GPIO::BOTH;
                break;

            default: trigger = GPIO::HIGH;
                break;
        }

        gpio.setInterruptEnable(0, false).setInterruptPending(0, false);
        gpio.setTriggerLevel(0, trigger);

        if (gpio.getTriggerLevel(0) != trigger) {
            vp_println("[ERROR] GPIO trigger configuration error");
            TEST_FAIL();
        }

        /* Clear */
        GPIO_EXTERNAL_VALUE = 0x00;
        for (int j = 0; j < 4; ++j) { gpio.getPortValue(); }

        
        /* Interrupt test */
        gpio.setInterruptEnable(0, true);


        if (trigger == GPIO::HIGH || trigger == GPIO::POSEDGE || trigger == GPIO::BOTH) {
            /* Positive edge */
            GPIO_EXTERNAL_VALUE = 0x01;
        } else {
            /* Negative edge */
            GPIO_EXTERNAL_VALUE = 0x01;
            for (int j = 0; j < 4; ++j) {
                gpio.getPortValue();
            }

            GPIO_EXTERNAL_VALUE = 0x00;
        }

        for (int j = 0; j < 4; ++j) { gpio.getPortValue(); }
        
        /* Interrupt must be triggered */
        if (!gpio.getInterruptPending(0)) {
            vp_println("[ERROR] GPIO interrupt error");
            TEST_FAIL();
        }

        /* Clear interrupt */
        gpio.setInterruptEnable(0, false).setInterruptPending(0, false);

        if (gpio.getInterruptPending(0)) {
            vp_println("[ERROR] GPIO interrupt clear error");
            TEST_FAIL();
        }
    }

    if (gpio.getInterruptEnable(0)) {
        vp_println("[ERROR] GPIO interrupt disable error");
        TEST_FAIL();
    }

    TEST_PASS();
}
