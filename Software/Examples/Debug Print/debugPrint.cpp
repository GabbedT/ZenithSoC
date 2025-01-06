/*
 *  Program to test the Serial_IO debug class. It sends data through
 *  the UART device, the receiver end then have to print the received 
 *  data into a terminal (use PUTTY for example).
 */

#include "../../Library/Serial_IO.h"
#include "../../Library/Driver/Timer.h"

#include <inttypes.h>

#define repeat(n) for (int i = 0; i < n; ++i)

extern "C" void debugPrint() {
    /* Initialize UART */
    Serial_IO::init();

    Serial_IO::write('C');
    Serial_IO::write('\n');
    Serial_IO::write('\0');

    Serial_IO::write("Example\n\n");

    char buffer[] = "Example2\n";

    Serial_IO::write(buffer, sizeof(buffer));


    Serial_IO::write((uint8_t) UINT32_MAX, Serial_IO::HEX); Serial_IO::write('\n');
    Serial_IO::write((uint16_t) UINT32_MAX, Serial_IO::HEX); Serial_IO::write('\n');
    Serial_IO::write((uint32_t) UINT32_MAX, Serial_IO::HEX); Serial_IO::write('\n');
    Serial_IO::write((uint64_t) UINT64_MAX, Serial_IO::HEX); Serial_IO::write('\n');

    Serial_IO::write(12457, Serial_IO::DEC); Serial_IO::write('\n');
    Serial_IO::write(-2141, Serial_IO::DEC); Serial_IO::write('\n');
    Serial_IO::write(0, Serial_IO::DEC); Serial_IO::write('\n');

    Serial_IO::write(UINT64_MAX, Serial_IO::DEC); Serial_IO::write('\n');
    Serial_IO::write(UINT32_MAX, Serial_IO::DEC); Serial_IO::write('\n');
    Serial_IO::write(UINT16_MAX, Serial_IO::DEC); Serial_IO::write('\n');
    Serial_IO::write(UINT8_MAX, Serial_IO::DEC); Serial_IO::write('\n');

    Serial_IO::write(INT64_MIN, Serial_IO::DEC); Serial_IO::write('\n');
    Serial_IO::write(INT32_MIN, Serial_IO::DEC); Serial_IO::write('\n');
    Serial_IO::write(INT16_MIN, Serial_IO::DEC); Serial_IO::write('\n');
    Serial_IO::write(INT8_MIN, Serial_IO::DEC); Serial_IO::write('\n');


    Serial_IO::writeD(sizeof(uint64_t), false); Serial_IO::write('\n');

    Serial_IO::writeB((uint64_t) 1325L); Serial_IO::write('\n');
    Serial_IO::writeH((uint64_t) 1325L); Serial_IO::write('\n');
    Serial_IO::writeD((uint64_t) 1325L, false); Serial_IO::write('\n');

    Serial_IO::writeB((int64_t) -1L); Serial_IO::write('\n');
    Serial_IO::writeH((int64_t) -1L); Serial_IO::write('\n');
    Serial_IO::writeD((uint64_t) -1L, false); Serial_IO::write('\n');

    Serial_IO::write((uint8_t) 15, Serial_IO::DEC); Serial_IO::write('\n');
    Serial_IO::write((uint8_t) 15, Serial_IO::HEX); Serial_IO::write('\n');
    Serial_IO::write((uint8_t) 15, Serial_IO::BIN); Serial_IO::write('\n');

    Serial_IO::write((uint16_t) 15, Serial_IO::DEC); Serial_IO::write('\n');
    Serial_IO::write((uint16_t) 15, Serial_IO::HEX); Serial_IO::write('\n');
    Serial_IO::write((uint16_t) 15, Serial_IO::BIN); Serial_IO::write('\n');

    Serial_IO::write((uint32_t) 15, Serial_IO::DEC); Serial_IO::write('\n');
    Serial_IO::write((uint32_t) 15, Serial_IO::HEX); Serial_IO::write('\n');
    Serial_IO::write((uint32_t) 15, Serial_IO::BIN); Serial_IO::write('\n');

    Serial_IO::write((uint64_t) 15, Serial_IO::DEC); Serial_IO::write('\n');
    Serial_IO::write((uint64_t) 15, Serial_IO::HEX); Serial_IO::write('\n');
    Serial_IO::write((uint64_t) 15, Serial_IO::BIN); Serial_IO::write('\n');

    Serial_IO::write(0, Serial_IO::DEC); Serial_IO::write('\n');
    Serial_IO::write(0, Serial_IO::HEX); Serial_IO::write('\n');
    Serial_IO::write(0, Serial_IO::BIN); Serial_IO::write('\n');

    Serial_IO::write((uint8_t) INT32_MAX, Serial_IO::DEC); Serial_IO::write('\n');
    Serial_IO::write((uint16_t) INT32_MAX, Serial_IO::DEC); Serial_IO::write('\n');
    Serial_IO::write((uint32_t) INT32_MAX, Serial_IO::DEC); Serial_IO::write('\n');
    Serial_IO::write((uint64_t) INT32_MAX, Serial_IO::DEC); Serial_IO::write('\n');

    Serial_IO::write((uint8_t) INT32_MAX, Serial_IO::HEX); Serial_IO::write('\n');
    Serial_IO::write((uint16_t) INT32_MAX, Serial_IO::HEX); Serial_IO::write('\n');
    Serial_IO::write((uint32_t) INT32_MAX, Serial_IO::HEX); Serial_IO::write('\n');
    Serial_IO::write((uint64_t) INT64_MAX, Serial_IO::HEX); Serial_IO::write('\n');

    Serial_IO::write((uint8_t) INT32_MAX, Serial_IO::BIN); Serial_IO::write('\n');
    Serial_IO::write((uint16_t) INT32_MAX, Serial_IO::BIN); Serial_IO::write('\n');
    Serial_IO::write((uint32_t) INT32_MAX, Serial_IO::BIN); Serial_IO::write('\n');
    Serial_IO::write((uint64_t) INT64_MAX, Serial_IO::BIN); Serial_IO::write('\n');

    repeat(10) { Serial_IO::write('\n'); }

    Serial_IO::printf("PRINTF\n");
    Serial_IO::println("PRINTLN");
    Serial_IO::write('\n'); Serial_IO::write('\n');

    Serial_IO::printf(": %d", 15); Serial_IO::write('\n');
    Serial_IO::printf(": %d", -15); Serial_IO::write('\n');
    Serial_IO::printf(": %l", (uint64_t) 15L); Serial_IO::write('\n');
    Serial_IO::printf(": %u", -1); Serial_IO::write('\n');
    Serial_IO::printf(": %x", -1); Serial_IO::write('\n');
    Serial_IO::printf(": %xb", -1); Serial_IO::write('\n');
    Serial_IO::printf(": %xh", -1); Serial_IO::write('\n');
    Serial_IO::printf(": %xd", (uint64_t) -1L); Serial_IO::write('\n');
    Serial_IO::printf(": %b", -1); Serial_IO::write('\n');
    Serial_IO::printf(": %bb", -1); Serial_IO::write('\n');
    Serial_IO::printf(": %bh", -1); Serial_IO::write('\n');
    Serial_IO::printf(": %bd", (uint64_t) -1L); Serial_IO::write('\n');

    Serial_IO::println("PRINTLN");

    Serial_IO::println(": %d", 15); Serial_IO::write('\n');
    Serial_IO::println(": %d", -15); Serial_IO::write('\n');
    Serial_IO::println(": %l", (uint64_t) 15); Serial_IO::write('\n');
    Serial_IO::println(": %u", -1); Serial_IO::write('\n');
    Serial_IO::println(": %x", -1); Serial_IO::write('\n');
    Serial_IO::println(": %xb", -1); Serial_IO::write('\n');
    Serial_IO::println(": %xh", -1); Serial_IO::write('\n');
    Serial_IO::println(": %xd", (uint64_t) -1); Serial_IO::write('\n');
    Serial_IO::println(": %b", -1); Serial_IO::write('\n');
    Serial_IO::println(": %bb", -1); Serial_IO::write('\n');
    Serial_IO::println(": %bh", -1); Serial_IO::write('\n');
    Serial_IO::println(": %bd", (uint64_t) -1); Serial_IO::write('\n');

    Timer timer(0);

    timer.init(-1, Timer::FREE_RUNNING)
         .delay(100000); 

};