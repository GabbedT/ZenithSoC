/* System clock frequency */
#define CLK_FREQUENCY 100000000

#define BIT8  3
#define STOP1 0
#define EVEN  0

#define BAUDRATE 115200

#define UART_BASE 0x00004000


struct uartCtrlStatus_s {
    /* Buffer status */
    unsigned int emptyRX : 1;
    unsigned int fullRX : 1;
    unsigned int emptyTX : 1;
    unsigned int fullTX : 1;

    /* Enable interrupt for each event */
    unsigned int interruptEnable : 5;
    
    /* Operation enable */
    unsigned int enableRX : 1;
    unsigned int enableTX : 1;

    /* Communication config */
    unsigned int parityEnable : 1;
    unsigned int parityMode : 1;
    unsigned int stopBits : 1;
    unsigned int dataBits : 2;

    /* Enable CTS - RTS flow control */
    unsigned int flowControl : 1;

    /* Clock divider to generate the right baud rate */
    unsigned int clockDivider : 15;
};