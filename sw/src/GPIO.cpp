#ifndef GPIO_CPP
#define GPIO_CPP

#include "../lib/driver/GPIO.h"
#include "../lib/mmio.h"
#include "../lib/platform.h"

#include <stdint.h>

/**
 * The GPIO constructor initializes the base address and register addresses for a GPIO device, disables
 * interrupts, sets the direction to output, sets the initial value to 0, and sets the trigger level
 * for interrupts.
 * 
 * @param gpioNumber gpioNumber is the number of the GPIO device that we want to initialize. It is used
 * to calculate the base address of the GPIO device.
 */
GPIO::GPIO(uint32_t gpioNumber) :
    /* Initialize the base addres considering that each device has 4 registers of 1 byte */
    gpioBaseAddress ( (uint32_t * volatile) (GPIO_BASE + gpioNumber) ),

    /* Initialize register addresses based on the base address */
    value            ( (uint8_t * volatile)   gpioBaseAddress       ),
    direction        ( (uint8_t * volatile)  (gpioBaseAddress + 1)  ),
    interruptEnable  ( (uint8_t * volatile)  (gpioBaseAddress + 2)  ),
    triggerLevelLow  ( (uint8_t * volatile)  (gpioBaseAddress + 3)  ),
    triggerLevelHigh ( (uint8_t * volatile)  (gpioBaseAddress + 4)  ),
    interruptPending ( (uint8_t * volatile)  (gpioBaseAddress + 5)  ) {

    /* Disable interrupts */
    *interruptEnable = false;

    /* Output mode set to 0 */
    *direction = GPIO::OUTPUT;
    *value = 0;

    *triggerLevelLow = 0;
    *triggerLevelHigh = 0;
};


/**
 * @brief The GPIO destructor disables the GPIO interrupts, set all the pins to OUTPUT, their values to 0 and set a trigger level
 * for a transition from LOW to HIGH.
 */
GPIO::~GPIO() {
    /* Disable interrupts */
    *interruptEnable = false;

    /* Output mode set to 0 */
    *direction = GPIO::OUTPUT;
    *value = 0;

    /* Interrupt on 0 to 1 transition */
    *triggerLevelLow = 0;
    *triggerLevelHigh = 0;
}



/*****************************************************************/
/*                         CONFIGURATION                         */
/*****************************************************************/

/**
 * @brief Initializes all the 8 GPIOs of a group with the parameters passes to the function.
 * 
 * @return The GPIO object itself to chain the function call.
 */
GPIO& GPIO::init(uint8_t value, uint8_t direction, uint8_t interruptEnable, triggerLevel_e level) {
    /* Disable interrupts */
    *this->interruptEnable = false;
    
    /* Output mode set to 0 */
    *this->direction = direction;
    *this->value = value;

    /* Disable interrupts */
    *this->interruptEnable = interruptEnable;

    for (int i = 0; i < 8; ++i) {
        setTriggerLevel(i, level);
    }

    return *this;
};


/**
 * @brief Set the pin value of a specific pin in the GPIO group 
 * 
 * @param index Index of the pin inside the GPIO group (0 - 7).
 * @param pinLevel Logic level of the pin.
 * @return The GPIO object itself to chain the function call.
 */
GPIO& GPIO::setPinValue(uint32_t index, bool pinLevel) {
    /* Create a mask to clear the bit at the i-th position */
    uint8_t mask = 1 << index; 

    /* Set the bit to the parameter's value */
    *value = (*value & ~mask) | (pinLevel << index);

    return *this;
};


/**
 * @brief Toggle the value of a specific pin in the GPIO group.
 * 
 * @param index Index of the pin inside the GPIO group (0 - 7).
 * @return The GPIO object itself to chain the function call.
 */
GPIO& GPIO::togglePin(uint32_t index) {
    /* Create a mask to clear the bit at the i-th position */
    uint8_t mask = 1 << index; 

    /* Set the bit to the parameter's value */
    *value ^= mask;

    return *this;
};


/**
 * @brief Read the value register of the i-th pin of the GPIO group.
 * 
 * @param index Index of the pin inside the GPIO group (0 - 7).
 * @return The register value.
 */
bool GPIO::getPinValue(uint32_t index) {
    /* Create a mask to clear the bit at the i-th position */
    uint8_t mask = 1 << index; 

    return (*value & mask) >> index;
};


/**
 * @brief Set the pin direction of a specific pin in the GPIO group 
 * 
 * @param index Index of the pin inside the GPIO group (0 - 7).
 * @param value Direction of the pin (INPUT / OUTPUT).
 * @return The GPIO object itself to chain the function call.
 */
GPIO& GPIO::setPinDirection(uint32_t index, pinDirection_e value) {
    /* Create a mask to clear the bit at the i-th position */
    uint8_t mask = 1 << index; 

    /* Set the bit to the parameter's value */
    *direction = (*direction & ~mask) | (value << index);

    return *this;
};


/**
 * @brief Read the direction register of the i-th pin of the GPIO group.
 * 
 * @param index Index of the pin inside the GPIO group (0 - 7).
 * @return The register value.
 */
GPIO::pinDirection_e GPIO::getPinDirection(uint32_t index) {
    /* Create a mask to clear the bit at the i-th position */
    uint8_t mask = 1 << index; 

    return (pinDirection_e) ((*direction & mask) >> index);
};


/**
 * @brief Enable interrupt on a specific pin, works only if the pin is in INPUT mode.
 * 
 * @param index Index of the pin inside the GPIO group (0 - 7).
 * @param enable Enable / Disable interrupting.
 * @return The GPIO object itself to chain the function call.
 */
GPIO& GPIO::setInterruptEnable(uint32_t index, bool enable) {
    /* Create a mask to clear the bit at the i-th position */
    uint8_t mask = 1 << index; 

    /* Set the bit to the parameter's value */
    *interruptEnable = (*interruptEnable & ~mask) | (enable << index);

    return *this;
};


/**
 * @brief Read the interrupt enable register of the i-th pin of the GPIO group.
 * 
 * @param index Index of the pin inside the GPIO group (0 - 7).
 * @return The register value.
 */
bool GPIO::getInterruptEnable(uint32_t index) {
    /* Create a mask to clear the bit at the i-th position */
    uint8_t mask = 1 << index; 

    return (*interruptEnable & mask) >> index;
};


/**
 * @brief Set interrupt pending register.
 * 
 * @param index Index of the pin inside the GPIO group (0 - 7).
 * @param enable Enable / Disable interrupting.
 * @return The GPIO object itself to chain the function call.
 */
GPIO& GPIO::setInterruptPending(uint32_t index, bool enable) {
    /* Create a mask to clear the bit at the i-th position */
    uint8_t mask = 1 << index; 

    /* Set the bit to the parameter's value */
    *interruptPending = (*interruptPending & ~mask) | (enable << index);

    return *this;
};


/**
 * @brief Read the interrupt pending register of the i-th pin of the GPIO group.
 * 
 * @param index Index of the pin inside the GPIO group (0 - 7).
 * @return The register value.
 */
bool GPIO::getInterruptPending(uint32_t index) {
    /* Create a mask to clear the bit at the i-th position */
    uint8_t mask = 1 << index; 

    return (*interruptPending & mask) >> index;
};


/**
 * @brief Set logic transition interrupt trigger for input pins.
 * 
 * @param index Index of the pin inside the GPIO group (0 - 7).
 * @param enable Enable / Disable interrupting.
 * @return The GPIO object itself to chain the function call.
 */
GPIO& GPIO::setTriggerLevel(uint32_t index, triggerLevel_e level) {
    /* Create a mask to clear the bit at the i-th position */
    uint8_t mask = 1 << index; 

    /* Set the bit to the parameter's value */
    *triggerLevelLow = (*triggerLevelLow & ~mask) | ((level & 0x1) << index);
    *triggerLevelHigh = (*triggerLevelHigh & ~mask) | (((level & 0x2) >> 1) << index);

    return *this;
};


/**
 * @brief Read the trigger level register of the i-th pin of the GPIO group.
 * 
 * @param index Index of the pin inside the GPIO group (0 - 7).
 * @return The register value.
 */
GPIO::triggerLevel_e GPIO::getTriggerLevel(uint32_t index) {
    /* Create a mask to clear the bit at the i-th position */
    uint8_t mask = 1 << index; 

    return (triggerLevel_e) ((((*triggerLevelHigh & mask) >> index) << 1) | ((*triggerLevelLow & mask) >> index));
};

#endif 