/*
 * How this test works:
 * Trace a fixed branch and jump sequence, decode the UART packet stream, and
 * compare every packet with the expected path. A second phase raises an
 * illegal instruction and checks both mcause and Trace Unit event number 2.
 */
#include "interrupt.h"

#include "Serial_IO.h"
#include "driver/TraceUnit.h"
#include "driver/UART.h"

#include <stdint.h>

extern "C" void trace_program(volatile TraceUnit::traceControl_s* control,
                              uint32_t enabledControl,
                              uint32_t drainControl);
extern "C" void trace_illegal_program(
    volatile TraceUnit::traceControl_s* control,
    uint32_t enabledControl,
    uint32_t drainControl);

extern "C" void trace_illegal_entry();

extern "C" {
    volatile uint32_t trace_illegal_cause = 0xffffffff;
}

/* Assembly branch tags */
extern "C" uint8_t trace_start;
extern "C" uint8_t trace_loop;
extern "C" uint8_t trace_loop_branch;
extern "C" uint8_t trace_taken_branch;
extern "C" uint8_t trace_taken;
extern "C" uint8_t trace_call;
extern "C" uint8_t trace_forward;
extern "C" uint8_t trace_finish_jump;
extern "C" uint8_t trace_finish;

namespace {

    enum packetType_e {
        EVENT_PACKET = 0,
        DIVERGENCE_PACKET = 1,
        OVERFLOW_PACKET = 2,
        START_PACKET = 3
    };


    enum eventNumber_e {
        INSTR_ILLEGAL = 2,
        BRANCH_OPERATION = 20,
        JUMP_OPERATION = 21
    };


    struct packet_s {
        packetType_e type;
        uint8_t event;
        int32_t deltaPC;
        uint32_t timestamp;
    };


    void report(const char* name, bool passed) {
        Serial_IO::write(passed ? "[PASS] " : "[FAIL] ");
        Serial_IO::write(name);
        Serial_IO::write('\n');
    }


    void writePacket(uint32_t index, const packet_s& packet) {
        Serial_IO::write("Packet 0x");
        Serial_IO::write(index, Serial_IO::HEX);
        Serial_IO::write(": type=0x");
        Serial_IO::write(static_cast<uint32_t>(packet.type), Serial_IO::HEX);

        if (packet.type == EVENT_PACKET) {
            Serial_IO::write(" event=0x");
            Serial_IO::write(packet.event, Serial_IO::HEX);
        } else if (packet.type == DIVERGENCE_PACKET) {
            Serial_IO::write(" delta=0x");
            Serial_IO::write(static_cast<uint32_t>(packet.deltaPC), Serial_IO::HEX);
        }

        Serial_IO::write('\n');
    }


    int32_t addressDelta(const uint8_t* target, const uint8_t* source) {
        return static_cast<int32_t>(
            reinterpret_cast<uintptr_t>(target) -
            reinterpret_cast<uintptr_t>(source));
    }


    void setPacket(packet_s& packet, packetType_e type, uint8_t event,
                   int32_t deltaPC) {
        packet.type = type;
        packet.event = event;
        packet.deltaPC = deltaPC;
        packet.timestamp = 0;
    }


    bool waitForTrace(UART& uart, TraceUnit& trace) {
        uint32_t timeout = 500'000;

        while (timeout--) {
            if (trace.isBufferEmpty() && uart.getCtrlStatus()->emptyTX) {
                waitCycles(512);

                return true;
            }
        }

        return false;
    }


    uint32_t readTrace(UART& uart, uint8_t* data, uint32_t capacity) {
        uint32_t size = 0;

        while (!uart.getCtrlStatus()->emptyRX && (size < capacity)) {
            data[size++] = uart.receiveByte();
        }

        return size;
    }


    bool parseTrace(const uint8_t* data, uint32_t size, packet_s* packets,
                    uint32_t& packetCount) {
        uint32_t offset = 0;
        packetCount = 0;

        /* Trace bytes use 0x1e framing and 0x1d escaping. */
        auto readByte = [&](uint8_t& byte) {
            if (offset >= size || data[offset] == 0x1e) {
                return false;
            }

            byte = data[offset++];

            if (byte == 0x1d) {
                bool validEscape = offset < size &&
                                   (data[offset] == 0x3d ||
                                    data[offset] == 0x3e);

                if (!validEscape) {
                    return false;
                }

                byte = data[offset++] ^ 0x20;
            }

            return true;
        };

        while ((offset < size) && (packetCount < 16)) {
            if (data[offset++] != 0x1e) {
                return false;
            }

            uint8_t header;

            if (!readByte(header)) {
                return false;
            }

            packet_s& packet = packets[packetCount++];
            packet.type = static_cast<packetType_e>(header >> 6);
            packet.event = header & 0x3f;
            packet.deltaPC = 0;
            packet.timestamp = 0;

            /* Address packets carry one big-endian 32-bit value. */
            if (packet.type == DIVERGENCE_PACKET ||
                packet.type == START_PACKET) {
                uint32_t address = 0;

                for (uint32_t i = 0; i < 4; ++i) {
                    uint8_t byte;

                    if (!readByte(byte)) {
                        return false;
                    }

                    address = (address << 8) | byte;
                }

                packet.deltaPC = static_cast<int32_t>(address);
            }

            /* Event packets carry one big-endian 24-bit timestamp. */
            if (packet.type == EVENT_PACKET ||
                packet.type == DIVERGENCE_PACKET) {
                for (uint32_t i = 0; i < 3; ++i) {
                    uint8_t byte;

                    if (!readByte(byte)) {
                        return false;
                    }

                    packet.timestamp = (packet.timestamp << 8) | byte;
                }
            }
        }

        return offset == size;
    }


    bool comparePackets(const packet_s* actual, uint32_t actualCount) {
        packet_s expected[12];

        setPacket(expected[0], START_PACKET, 0,
                  static_cast<int32_t>(
                      reinterpret_cast<uintptr_t>(&trace_start)));

        setPacket(expected[1],  EVENT_PACKET,      BRANCH_OPERATION, 0);
        setPacket(expected[2],  DIVERGENCE_PACKET, 0, addressDelta(&trace_loop, &trace_start));
        setPacket(expected[3],  EVENT_PACKET,      BRANCH_OPERATION, 0);
        setPacket(expected[4],  DIVERGENCE_PACKET, 0, addressDelta(&trace_loop, &trace_loop));
        setPacket(expected[5],  EVENT_PACKET,      BRANCH_OPERATION, 0);
        setPacket(expected[6],  EVENT_PACKET,      BRANCH_OPERATION, 0);
        setPacket(expected[7],  DIVERGENCE_PACKET, 0, addressDelta(&trace_taken, &trace_loop));
        setPacket(expected[8],  EVENT_PACKET,      JUMP_OPERATION, 0);
        setPacket(expected[9],  DIVERGENCE_PACKET, 0, addressDelta(&trace_forward, &trace_taken));
        setPacket(expected[10],  EVENT_PACKET,      JUMP_OPERATION, 0);
        setPacket(expected[11], DIVERGENCE_PACKET, 0, addressDelta(&trace_finish, &trace_forward));

        const uint32_t expectedCount = 12;

        if (actualCount != expectedCount) {
            Serial_IO::write("Expected/actual packet count: 0x");
            Serial_IO::write(expectedCount, Serial_IO::HEX);
            Serial_IO::write("/0x");
            Serial_IO::write(actualCount, Serial_IO::HEX);
            Serial_IO::write('\n');

            for (uint32_t i = 0; i < actualCount; ++i) {
                writePacket(i, actual[i]);
            }

            return false;
        }

        for (uint32_t i = 0; i < expectedCount; ++i) {
            bool equal = actual[i].type == expected[i].type;

            if (expected[i].type == EVENT_PACKET) {
                equal &= actual[i].event == expected[i].event;
            } else if (expected[i].type == DIVERGENCE_PACKET ||
                       expected[i].type == START_PACKET) {
                equal &= actual[i].deltaPC == expected[i].deltaPC;
            }

            if (!equal) {
                Serial_IO::write("Packet mismatch at index 0x");
                Serial_IO::write(i, Serial_IO::HEX);
                Serial_IO::write('\n');
                writePacket(i, expected[i]);
                writePacket(i, actual[i]);

                return false;
            }
        }

        return true;
    }


    bool compareIllegalPacket(const packet_s* packets, uint32_t packetCount) {
        if (packetCount != 1) {
            return false;
        }

        return packets[0].type == EVENT_PACKET && packets[0].event == INSTR_ILLEGAL;
    }


    void writeRawTrace(const uint8_t* data, uint32_t size) {
        Serial_IO::write("Raw trace bytes: 0x");
        Serial_IO::write(size, Serial_IO::HEX);
        Serial_IO::write('\n');

        for (uint32_t i = 0; i < size; ++i) {
            Serial_IO::write("0x");
            Serial_IO::write(data[i], Serial_IO::HEX);
            Serial_IO::write((i & 0x7) == 0x7 ? '\n' : ' ');
        }

        if ((size & 0x7) != 0) {
            Serial_IO::write('\n');
        }
    }

}


extern "C" int main() {
    UART uart;
    TraceUnit trace;

    Serial_IO::init(6'250'000, false, UART::EVEN, UART::STOP1, UART::BIT8);

    TraceUnit::eventEnable_s events = {};
    events.branch = true;
    events.jump = true;

    trace.setEventEnable(events)
         .setTriggerPC(reinterpret_cast<uintptr_t>(&trace_start))
         .enableTriggerPC(true);

    /* Enable event/branch tracing, both timestamp formats, and the PC trigger. */
    trace_program(trace.control,
                  (1 << 7) | (1 << 6) | (1 << 5) | (1 << 4) | (1 << 3),
                  (1 << 5) | (1 << 4));

    bool completed = waitForTrace(uart, trace);

    uint8_t rawTrace[256];
    uint32_t rawSize = completed ? readTrace(uart, rawTrace, sizeof(rawTrace)) : 0;

    packet_s packets[16];
    uint32_t packetCount = 0;
    bool parsed = completed && parseTrace(rawTrace, rawSize, packets, packetCount);

    TraceUnit::eventEnable_s illegalEvents = {};
    illegalEvents.instrIllegal = true;

    trace.setEventEnable(illegalEvents)
         .enableTriggerPC(false);

    uintptr_t savedTrapVector;
    asm volatile ("csrr %0, mtvec" : "=r"(savedTrapVector));
    asm volatile ("csrw mtvec, %0" :: "r"(trace_illegal_entry) : "memory");

    trace_illegal_cause = 0xffffffff;

    /* Trace one illegal instruction without branch packets from trap entry. */
    trace_illegal_program(trace.control, (1 << 7) | (1 << 5), (1 << 5));

    bool illegalCompleted = waitForTrace(uart, trace);

    uint8_t illegalRawTrace[16];
    uint32_t illegalRawSize = illegalCompleted
        ? readTrace(uart, illegalRawTrace, sizeof(illegalRawTrace))
        : 0;

    packet_s illegalPackets[16];
    uint32_t illegalPacketCount = 0;
    bool illegalParsed = illegalCompleted &&
                         parseTrace(illegalRawTrace, illegalRawSize,
                                    illegalPackets, illegalPacketCount);

    asm volatile ("csrw mtvec, %0" :: "r"(savedTrapVector) : "memory");

    /* Diagnostic UART text must not be looped into the data being checked. */
    uart.setModeRX(false);

    if (!parsed) {
        writeRawTrace(rawTrace, rawSize);
    }

    if (!illegalParsed ||
        !compareIllegalPacket(illegalPackets, illegalPacketCount)) {
        writeRawTrace(illegalRawTrace, illegalRawSize);
    }

    bool packetsPassed = parsed && comparePackets(packets, packetCount);

    bool illegalPacketPassed = illegalParsed &&
                               compareIllegalPacket(illegalPackets, illegalPacketCount);
    bool illegalCausePassed = trace_illegal_cause == INSTR_ILLEGAL;

    bool noOverflow = true;
    bool timestampsPassed = parsed && (packetCount != 0);

    uint32_t previousTimestamp = 0;

    for (uint32_t i = 0; i < packetCount; ++i) {
        noOverflow &= packets[i].type != OVERFLOW_PACKET;

        if (packets[i].type == START_PACKET) {
            continue;
        }

        if (previousTimestamp != 0) {
            timestampsPassed &= packets[i].timestamp > previousTimestamp;
        }

        previousTimestamp = packets[i].timestamp;
    }

    report("Trace stream completed", completed);
    report("Trace packet framing", parsed);
    report("Branch and jump packets", packetsPassed);
    report("Monotonic packet timestamps", timestampsPassed);
    report("No trace buffer overflow", noOverflow);
    report("Illegal instruction trap cause", illegalCausePassed);
    report("Illegal instruction trace packet", illegalPacketPassed);

    uint32_t failures = !completed + !parsed + !packetsPassed;
    failures += !timestampsPassed + !noOverflow;
    failures += !illegalCompleted + !illegalParsed;
    failures += !illegalCausePassed + !illegalPacketPassed;
    failures += unexpectedInterrupts != 0;

    Serial_IO::write(failures ? "Trace Unit IO test FAILED\n"
                              : "Trace Unit IO test PASSED\n");

    return failures;
}
