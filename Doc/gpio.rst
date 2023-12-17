GPIO
====

Overview
--------

The General-Purpose Input/Output (GPIO) is a common digital interface found in many digital systems. Its primary feature is to connect the main CPU to the outside word through 
a basic interface without any protocols and timing rules. The CPU can configure the device to its liking to act as an input or an output, then perform operations on it like reading
the external value or setting the pin to a particular logic level.

Finally, the way the GPIOs are grouped and associated with individual modules plays a crucial role in reducing interconnection congestion. In complex systems like SoCs, managing the 
myriad connections without causing signal interference or latency is a challenge. The ZenithSoC addresses this through its thoughtful GPIO grouping, which not only enhances overall 
performance but also ensures smoother and more reliable operations. By minimizing interconnection congestion, the system operates more efficiently but also reduces the 
potential for errors, making it a robust solution for a wide range of applications.

**Features**:

* Configurable direction
* Configurable interrupt trigger 
* Enable / Disable interrupt generation

Register Description
--------------------

The register map is the interface between the GPIO circuitry and the CPU, the user can interact with the register through simple MMIO load / store instructions. There are 4 registers:

* **Value Register** (0x0)
* **Direction Register** (0x4)
* **Interrupt Enable Register** (0x8)
* **Trigger Level Register** (0xC)

Those are 8 bit registers aligned to a 4 byte boundary. Each bit of the register correspond to a single GPIO pin.

Value Register
~~~~~~~~~~~~~~

The value registers store the current logic state of the GPIO pins. Importantly, these value registers are accessible in both input and output modes, serving different functions in each mode.

In output mode, the value registers are particularly significant. Here, writing to these registers is permissible and is a primary method for controlling the state of the GPIO pins. When a 
value is written to these registers in output mode, it sets the pins to a specific logic level - either high or low, depending on the requirement of the application. This ability to write 
to the value registers enables precise control over the GPIO pins, allowing them to interface with other hardware components, drive LEDs, control motors, or communicate with other digital systems.

Conversely, in input mode, the role of the value registers changes. Rather than being used to set the pin state, they instead reflect the current state of the GPIO pins. In this mode, 
the register takes on the value corresponding to the level of the externally applied voltage to the pin, which has been synchronized with the system clock to ensure data integrity. 
This means that if an external device is driving the pin high, the value register will read a high value, and similarly for a low value. Reading these registers in input mode is essential 
for understanding the state of external devices connected to the GPIO pins, making it a critical feature for applications that require the system to respond to external signals.


Direction Register
~~~~~~~~~~~~~~~~~~

The direction register determines the operational mode of each GPIO pin. This register is crucial because it dictates whether a pin functions as an input or an output, 
which is essential for the correct interfacing of the system with external devices or circuits.

In the context of GPIO, writing a '0' or a '1' to the direction register has specific and significant effects:

1. **Writing '0' - Output Mode:** When a '0' is written to a specific bit in the direction register, the corresponding GPIO pin is configured as an output. In this mode, the pin is actively driven by the system. The state (high or low) of the output pin is controlled by what is written to the value register. The pin directly reflects the logic level set in the value register, enabling the system to send signals out to other devices, such as turning on an LED, driving a motor, or sending a digital signal to another digital system.

2. **Writing '1' - Input Mode:** Conversely, writing a '1' to the direction register sets the corresponding GPIO pin as an input. In this mode, the pin is not driven by the system. Instead, it is in a high-impedance state, effectively making it sensitive to external voltages. This high-impedance state is achieved through the activation of a tristate buffer. A tristate buffer allows the pin to be disconnected from the internal circuitry of the system, preventing it from affecting the external circuit connected to the pin. In this mode, the system can read the level of an external signal (high or low) through the value register, enabling it to sense and respond to inputs from the outside world, such as switches, sensors, or communication from other digital devices.



Interrupt Enable Register
~~~~~~~~~~~~~~~~~~~~~~~~~

The functionality of the interrupt enable register is especially pertinent when a GPIO pin is set to input mode. 
In this mode, the GPIO pins are typically used to detect and respond to external events, such as a button press, 
a sensor trigger, or a signal from another device. Enabling an interrupt on a pin allows the system to respond 
immediately to these events without the need for constant polling, which can be inefficient.

Trigger Register
~~~~~~~~~~~~~~~~

The trigger register in a GPIO (General Purpose Input/Output) system is a crucial component for managing interrupts, specifically in terms of defining the conditions under which an interrupt should be generated. 
This register is particularly relevant when GPIO pins are configured in input mode, as it determines the type of logic level transition that must occur on a pin to trigger an interrupt.

* Low to High Transition (0 -> 1): To configure a GPIO pin to generate an interrupt on a low-to-high transition (i.e., when the signal changes from 0 to 1), a '1' is written to the corresponding bit in the trigger register. This setting is often used for detecting rising edges, such as a button being pressed (assuming the button is configured to go from a low state when unpressed to a high state when pressed).
* High to Low Transition (1 -> 0): Conversely, to configure a GPIO pin to generate an interrupt on a high-to-low transition (i.e., when the signal changes from 1 to 0), a '0' is written to the corresponding bit in the trigger register. This setting is commonly used for detecting falling edges, such as a button being released.