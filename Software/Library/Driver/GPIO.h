#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

class GPIO {

public:

    /* Pin direction value */
    enum pinDirection_e { OUTPUT, INPUT };

    /* Interrupt level */
    enum triggerLevel_e { HIGH2LOW, LOW2HIGH };


private: 

    /* Base memory address */
    uint32_t* const gpioBaseAddress;

    /* Pin value */
    volatile uint8_t* const value;

    /* Pin direction */
    volatile uint8_t* const direction;

    /* Interrupt enable */
    volatile uint8_t* const interruptEnable;

    /* Interrupt level */
    volatile uint8_t* const triggerLevel;


public:

    /**
     * The GPIO constructor initializes the base address and register addresses for a GPIO device, disables
     * interrupts, sets the direction to output, sets the initial value to 0, and sets the trigger level
     * for interrupts.
     * 
     * @param gpioNumber gpioNumber is the number of the GPIO device that we want to initialize. It is used
     * to calculate the base address of the GPIO device.
     */
    GPIO(uint32_t gpioNumber = 0);

    /**
     * @brief The GPIO destructor disables the GPIO interrupts, set all the pins to OUTPUT, their values to 0 and set a trigger level
     * for a transition from LOW to HIGH.
     */
    ~GPIO();


/*****************************************************************/
/*                         CONFIGURATION                         */
/*****************************************************************/

    /**
     * @brief Initializes all the 8 GPIOs of a group with the parameters passes to the function.
     * 
     * @return The GPIO object itself to chain the function call.
     */
    GPIO& init(uint8_t value, uint8_t direction, uint8_t interruptEnable, uint8_t interruptLevel);

    /**
     * @brief Set the pin value of a specific pin in the GPIO group 
     * 
     * @param index Index of the pin inside the GPIO group (0 - 7).
     * @param pinLevel Logic level of the pin.
     * @return The GPIO object itself to chain the function call.
     */
    GPIO& setPinValue(uint32_t index, bool pinLevel);

    /**
     * @brief Read the value register of the i-th pin of the GPIO group.
     * 
     * @param index Index of the pin inside the GPIO group (0 - 7).
     * @return The register value.
     */
    bool getPinValue(uint32_t index);


    /**
     * @brief Set the pin direction of a specific pin in the GPIO group 
     * 
     * @param index Index of the pin inside the GPIO group (0 - 7).
     * @param value Direction of the pin (INPUT / OUTPUT).
     * @return The GPIO object itself to chain the function call.
     */
    GPIO& setPinDirection(uint32_t index, pinDirection_e value);

    /**
     * @brief Read the direction register of the i-th pin of the GPIO group.
     * 
     * @param index Index of the pin inside the GPIO group (0 - 7).
     * @return The register value.
     */
    pinDirection_e getPinDirection(uint32_t index);


    /**
     * @brief Enable interrupt on a specific pin, works only if the pin is in INPUT mode.
     * 
     * @param index Index of the pin inside the GPIO group (0 - 7).
     * @param enable Enable / Disable interrupting.
     * @return The GPIO object itself to chain the function call.
     */
    GPIO& setInterruptEnable(uint32_t index, bool enable);

    /**
     * @brief Read the interrupt enable register of the i-th pin of the GPIO group.
     * 
     * @param index Index of the pin inside the GPIO group (0 - 7).
     * @return The register value.
     */
    bool getInterruptEnable(uint32_t index);


    /**
     * @brief Set logic transition interrupt trigger for input pins.
     * 
     * @param index Index of the pin inside the GPIO group (0 - 7).
     * @param enable Enable / Disable interrupting.
     * @return The GPIO object itself to chain the function call.
     */
    GPIO& setTriggerLevel(uint32_t index, triggerLevel_e triggerLevel);

    /**
     * @brief Read the trigger level register of the i-th pin of the GPIO group.
     * 
     * @param index Index of the pin inside the GPIO group (0 - 7).
     * @return The register value.
     */
    triggerLevel_e getTriggerLevel(uint32_t index);

};

#endif 