#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

class GPIO {

public:

    /* Pin direction value */
    enum pinDirection_e { OUTPUT, INPUT };

    /* Interrupt level */
    enum triggerLevel_e { LOW, HIGH };


private: 

    /* Base memory address */
    uint32_t* const gpioBaseAddress;

    /* Pin value */
    uint8_t* volatile const value;

    /* Pin direction */
    uint8_t* volatile const direction;

    /* Interrupt enable */
    uint8_t* volatile const interruptEnable;

    /* Interrupt level */
    uint8_t* volatile const triggerLevel;


public:

    /* Constructors: `gpioNumber` specifies the id of the UART in the system, if there are more than 1 */
    GPIO(uint32_t gpioNumber);

    /* Deconstructor */
    ~GPIO();


/*****************************************************************/
/*                         CONFIGURATION                         */
/*****************************************************************/
    
    GPIO& init(uint8_t value, uint8_t direction, uint8_t interruptEnable, uint8_t interruptLevel);

    /* Single pin value set */
    GPIO& setPinValue(uint32_t index, bool pinLevel);

    /* Single pin value get */
    bool getPinValue(uint32_t index);


    /* Single pin direction set */
    GPIO& setPinDirection(uint32_t index, pinDirection_e value);

    /* Single pin direction get */
    pinDirection_e getPinDirection(uint32_t index);


    /* Single pin direction set */
    GPIO& setInterruptEnable(uint32_t index, bool enable);

    /* Single pin direction get */
    bool getInterruptEnable(uint32_t index);


    /* Single pin direction set */
    GPIO& setTriggerLevel(uint32_t index, triggerLevel_e triggerLevel);

    /* Single pin direction get */
    triggerLevel_e getTriggerLevel(uint32_t index);

};

#endif 