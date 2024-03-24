/*
 *  Program to test the SerialOut debug class. It sends data through
 *  the UART device, the receiver end then have to print the received 
 *  data into a terminal (use PUTTY for example).
 */

#include "../../Library/SerialOut.h"
#include "../../Library/Driver/Timer.h"

#include <inttypes.h>

#define repeat(n) for (int i = 0; i < n; ++i)

extern "C" void debugPrint() {
    /* Initialize UART */
    SerialOut::init();

    SerialOut::write('C');
    SerialOut::write('\n');
    SerialOut::write('\0');

    SerialOut::write("Example\n\n");

    char buffer[] = "Example2\n";

    SerialOut::write(buffer, sizeof(buffer));


    SerialOut::write((uint8_t) UINT32_MAX, SerialOut::HEX); SerialOut::write('\n');
    SerialOut::write((uint16_t) UINT32_MAX, SerialOut::HEX); SerialOut::write('\n');
    SerialOut::write((uint32_t) UINT32_MAX, SerialOut::HEX); SerialOut::write('\n');
    SerialOut::write((uint64_t) UINT64_MAX, SerialOut::HEX); SerialOut::write('\n');

    SerialOut::write(12457, SerialOut::DEC); SerialOut::write('\n');
    SerialOut::write(-2141, SerialOut::DEC); SerialOut::write('\n');
    SerialOut::write(0, SerialOut::DEC); SerialOut::write('\n');

    SerialOut::write(UINT64_MAX, SerialOut::DEC); SerialOut::write('\n');
    SerialOut::write(UINT32_MAX, SerialOut::DEC); SerialOut::write('\n');
    SerialOut::write(UINT16_MAX, SerialOut::DEC); SerialOut::write('\n');
    SerialOut::write(UINT8_MAX, SerialOut::DEC); SerialOut::write('\n');

    SerialOut::write(INT64_MIN, SerialOut::DEC); SerialOut::write('\n');
    SerialOut::write(INT32_MIN, SerialOut::DEC); SerialOut::write('\n');
    SerialOut::write(INT16_MIN, SerialOut::DEC); SerialOut::write('\n');
    SerialOut::write(INT8_MIN, SerialOut::DEC); SerialOut::write('\n');


    SerialOut::writeD(sizeof(uint64_t), false); SerialOut::write('\n');

    SerialOut::writeB((uint64_t) 1325L); SerialOut::write('\n');
    SerialOut::writeH((uint64_t) 1325L); SerialOut::write('\n');
    SerialOut::writeD((uint64_t) 1325L, false); SerialOut::write('\n');

    SerialOut::writeB((int64_t) -1L); SerialOut::write('\n');
    SerialOut::writeH((int64_t) -1L); SerialOut::write('\n');
    SerialOut::writeD((uint64_t) -1L, false); SerialOut::write('\n');

    SerialOut::write((uint8_t) 15, SerialOut::DEC); SerialOut::write('\n');
    SerialOut::write((uint8_t) 15, SerialOut::HEX); SerialOut::write('\n');
    SerialOut::write((uint8_t) 15, SerialOut::BIN); SerialOut::write('\n');

    SerialOut::write((uint16_t) 15, SerialOut::DEC); SerialOut::write('\n');
    SerialOut::write((uint16_t) 15, SerialOut::HEX); SerialOut::write('\n');
    SerialOut::write((uint16_t) 15, SerialOut::BIN); SerialOut::write('\n');

    SerialOut::write((uint32_t) 15, SerialOut::DEC); SerialOut::write('\n');
    SerialOut::write((uint32_t) 15, SerialOut::HEX); SerialOut::write('\n');
    SerialOut::write((uint32_t) 15, SerialOut::BIN); SerialOut::write('\n');

    SerialOut::write((uint64_t) 15, SerialOut::DEC); SerialOut::write('\n');
    SerialOut::write((uint64_t) 15, SerialOut::HEX); SerialOut::write('\n');
    SerialOut::write((uint64_t) 15, SerialOut::BIN); SerialOut::write('\n');

    SerialOut::write(0, SerialOut::DEC); SerialOut::write('\n');
    SerialOut::write(0, SerialOut::HEX); SerialOut::write('\n');
    SerialOut::write(0, SerialOut::BIN); SerialOut::write('\n');

    SerialOut::write((uint8_t) INT32_MAX, SerialOut::DEC); SerialOut::write('\n');
    SerialOut::write((uint16_t) INT32_MAX, SerialOut::DEC); SerialOut::write('\n');
    SerialOut::write((uint32_t) INT32_MAX, SerialOut::DEC); SerialOut::write('\n');
    SerialOut::write((uint64_t) INT32_MAX, SerialOut::DEC); SerialOut::write('\n');

    SerialOut::write((uint8_t) INT32_MAX, SerialOut::HEX); SerialOut::write('\n');
    SerialOut::write((uint16_t) INT32_MAX, SerialOut::HEX); SerialOut::write('\n');
    SerialOut::write((uint32_t) INT32_MAX, SerialOut::HEX); SerialOut::write('\n');
    SerialOut::write((uint64_t) INT64_MAX, SerialOut::HEX); SerialOut::write('\n');

    SerialOut::write((uint8_t) INT32_MAX, SerialOut::BIN); SerialOut::write('\n');
    SerialOut::write((uint16_t) INT32_MAX, SerialOut::BIN); SerialOut::write('\n');
    SerialOut::write((uint32_t) INT32_MAX, SerialOut::BIN); SerialOut::write('\n');
    SerialOut::write((uint64_t) INT64_MAX, SerialOut::BIN); SerialOut::write('\n');

    repeat(10) { SerialOut::write('\n'); }

    SerialOut::printf("PRINTF\n");
    SerialOut::println("PRINTLN");
    SerialOut::write('\n'); SerialOut::write('\n');

    SerialOut::printf(": %d", 15); SerialOut::write('\n');
    SerialOut::printf(": %d", -15); SerialOut::write('\n');
    SerialOut::printf(": %l", (uint64_t) 15L); SerialOut::write('\n');
    SerialOut::printf(": %u", -1); SerialOut::write('\n');
    SerialOut::printf(": %x", -1); SerialOut::write('\n');
    SerialOut::printf(": %xb", -1); SerialOut::write('\n');
    SerialOut::printf(": %xh", -1); SerialOut::write('\n');
    SerialOut::printf(": %xd", (uint64_t) -1L); SerialOut::write('\n');
    SerialOut::printf(": %b", -1); SerialOut::write('\n');
    SerialOut::printf(": %bb", -1); SerialOut::write('\n');
    SerialOut::printf(": %bh", -1); SerialOut::write('\n');
    SerialOut::printf(": %bd", (uint64_t) -1L); SerialOut::write('\n');

    SerialOut::println("PRINTLN");

    SerialOut::println(": %d", 15); SerialOut::write('\n');
    SerialOut::println(": %d", -15); SerialOut::write('\n');
    SerialOut::println(": %l", (uint64_t) 15); SerialOut::write('\n');
    SerialOut::println(": %u", -1); SerialOut::write('\n');
    SerialOut::println(": %x", -1); SerialOut::write('\n');
    SerialOut::println(": %xb", -1); SerialOut::write('\n');
    SerialOut::println(": %xh", -1); SerialOut::write('\n');
    SerialOut::println(": %xd", (uint64_t) -1); SerialOut::write('\n');
    SerialOut::println(": %b", -1); SerialOut::write('\n');
    SerialOut::println(": %bb", -1); SerialOut::write('\n');
    SerialOut::println(": %bh", -1); SerialOut::write('\n');
    SerialOut::println(": %bd", (uint64_t) -1); SerialOut::write('\n');

    Timer timer(0);

    timer.init(-1, Timer::FREE_RUNNING)
         .delay(100000); 

};