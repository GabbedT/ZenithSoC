#ifndef GPIO_CPP
#define GPIO_CPP

#include "../Library/Driver/GPIO.h"
#include "../Library/mmio.h"
#include "../Library/platform.h"

#include <stdint.h>

GPIO::GPIO(uint32_t gpioNumber) :
    /* Initialize the base addres considering that each device has 4 registers of 1 byte */
    gpioBaseAddress ( (uint32_t * volatile) (GPIO_BASE + gpioNumber) ),

    /* Initialize register addresses based on the base address */
    value           ( (uint8_t * volatile) gpioBaseAddress     ),
    direction       ( (uint8_t * volatile) gpioBaseAddress + 1 ),
    interruptEnable ( (uint8_t * volatile) gpioBaseAddress + 2 ),
    triggerLevel  ( (uint8_t * volatile) gpioBaseAddress + 3 ) {

    /* Output mode set to 0 */
    *direction = 0;
    *value = 0;

    /* Disable interrupts */
    *interruptEnable = 0;

    /* Interrupt on 0 to 1 transition */
    *triggerLevel = 1;
};


GPIO::GPIO(uint32_t gpioNumber, uint8_t value, uint8_t direction, uint8_t interruptEnable, uint8_t triggerLevel) :
    /* Initialize the base addres considering that each device has 4 registers of 1 byte */
    gpioBaseAddress ( (uint32_t * volatile) (GPIO_BASE + gpioNumber) ),

    /* Initialize register addresses based on the base address */
    value           ( (uint8_t * volatile) gpioBaseAddress     ),
    direction       ( (uint8_t * volatile) gpioBaseAddress + 1 ),
    interruptEnable ( (uint8_t * volatile) gpioBaseAddress + 2 ),
    triggerLevel  ( (uint8_t * volatile) gpioBaseAddress + 3 ) {

    /* Output mode set to 0 */
    *this->direction = direction;
    *this->value = value;

    /* Disable interrupts */
    *this->interruptEnable = interruptEnable;

    /* Interrupt on 0 to 1 transition */
    *this->triggerLevel = triggerLevel;
};



/*****************************************************************/
/*                         CONFIGURATION                         */
/*****************************************************************/

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
 * @brief Read the value register of the i-th pin of the GPIO group.
 * 
 * @param index Index of the pin inside the GPIO group (0 - 7).
 * @return The register value.
 */
bool GPIO::getPinValue(uint32_t index) {
    /* Create a mask to clear the bit at the i-th position */
    uint8_t mask = 1 << index; 

    return (*value & ~mask) >> index;
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

    return (pinDirection_e) ((*direction & ~mask) >> index);
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

    return (*interruptEnable & ~mask) >> index;
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
    *triggerLevel = (*triggerLevel & ~mask) | (level << index);

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

    return (triggerLevel_e) ((*triggerLevel & ~mask) >> index);
};

#endif 