#include "interrupt.h"

#include "Serial_IO.h"
#include "driver/TraceUnit.h"
#include "driver/UART.h"

#include <stdint.h>

extern "C" void trace_program(volatile TraceUnit::traceControl_s* control, uint32_t enabledControl, uint32_t drainControl);

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
        OVERFLOW_PACKET = 2
    };


    enum eventNumber_e {
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
        return static_cast<int32_t>(reinterpret_cast<uintptr_t>(target) - reinterpret_cast<uintptr_t>(source));
    }


    void setPacket(packet_s& packet, packetType_e type, uint8_t event, int32_t deltaPC) {
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


    bool parseTrace(const uint8_t* data, uint32_t size, packet_s* packets, uint32_t& packetCount) {
        uint32_t offset = 0;
        packetCount = 0;

        while ((offset < size) && (packetCount < 16)) {
            uint8_t header = data[offset++];
            packet_s& packet = packets[packetCount++];

            packet.type = static_cast<packetType_e>(header >> 6);
            packet.event = header & 0x3F;
            packet.deltaPC = 0;
            packet.timestamp = 0;

            if (packet.type == DIVERGENCE_PACKET) {
                if ((size - offset) < 7) {
                    return false;
                }

                packet.deltaPC = static_cast<int32_t>(
                    (static_cast<uint32_t>(data[offset + 0]) << 24) |
                    (static_cast<uint32_t>(data[offset + 1]) << 16) |
                    (static_cast<uint32_t>(data[offset + 2]) << 8) |
                    (static_cast<uint32_t>(data[offset + 3]) << 0));
                offset += 4;
            } else if (packet.type == EVENT_PACKET) {
                if ((size - offset) < 3) {
                    return false;
                }
            } else if (packet.type != EVENT_PACKET && packet.type != OVERFLOW_PACKET) {
                return false;
            }

            if (packet.type != OVERFLOW_PACKET) {
                packet.timestamp =
                    (static_cast<uint32_t>(data[offset + 0]) << 16) |
                    (static_cast<uint32_t>(data[offset + 1]) << 8) |
                    (static_cast<uint32_t>(data[offset + 2]) << 0);
                offset += 3;
            }
        }

        return (offset == size) && (packetCount < 16);
    }


    bool comparePackets(const packet_s* actual, uint32_t actualCount) {
        packet_s expected[11];
        setPacket(expected[0],  EVENT_PACKET,      BRANCH_OPERATION, 0);
        setPacket(expected[1],  DIVERGENCE_PACKET, 0, addressDelta(&trace_loop, &trace_loop_branch));
        setPacket(expected[2],  EVENT_PACKET,      BRANCH_OPERATION, 0);
        setPacket(expected[3],  DIVERGENCE_PACKET, 0, addressDelta(&trace_loop, &trace_loop_branch));
        setPacket(expected[4],  EVENT_PACKET,      BRANCH_OPERATION, 0);
        setPacket(expected[5],  EVENT_PACKET,      BRANCH_OPERATION, 0);
        setPacket(expected[6],  DIVERGENCE_PACKET, 0, addressDelta(&trace_taken, &trace_taken_branch));
        setPacket(expected[7],  EVENT_PACKET,      JUMP_OPERATION, 0);
        setPacket(expected[8],  DIVERGENCE_PACKET, 0, addressDelta(&trace_forward, &trace_call));
        setPacket(expected[9],  EVENT_PACKET,      JUMP_OPERATION, 0);
        setPacket(expected[10], DIVERGENCE_PACKET, 0, addressDelta(&trace_finish, &trace_finish_jump));

        const uint32_t expectedCount = 11;

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
            } else if (expected[i].type == DIVERGENCE_PACKET) {
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
                  (1u << 7) | (1u << 6) | (1u << 5) | (1u << 4) | (1u << 3),
                  (1u << 5) | (1u << 4));

    bool completed = waitForTrace(uart, trace);

    uint8_t rawTrace[96];
    uint32_t rawSize = completed ? readTrace(uart, rawTrace, sizeof(rawTrace)) : 0;

    packet_s packets[16];
    uint32_t packetCount = 0;
    bool parsed = completed && parseTrace(rawTrace, rawSize, packets, packetCount);

    /* Diagnostic UART text must not be looped into the data being checked. */
    uart.setModeRX(false);

    if (!parsed) {
        Serial_IO::write("Raw trace bytes: 0x");
        Serial_IO::write(rawSize, Serial_IO::HEX);
        Serial_IO::write('\n');

        for (uint32_t i = 0; i < rawSize; ++i) {
            Serial_IO::write("0x");
            Serial_IO::write(rawTrace[i], Serial_IO::HEX);
            Serial_IO::write((i & 0x7) == 0x7 ? '\n' : ' ');
        }

        if ((rawSize & 0x7) != 0) {
            Serial_IO::write('\n');
        }
    }

    bool packetsPassed = parsed && comparePackets(packets, packetCount);
    bool noOverflow = true;
    bool timestampsPassed = parsed && (packetCount != 0);
    uint32_t previousTimestamp = 0;

    for (uint32_t i = 0; i < packetCount; ++i) {
        noOverflow &= packets[i].type != OVERFLOW_PACKET;

        if (i != 0) {
            timestampsPassed &= packets[i].timestamp > previousTimestamp;
        }

        previousTimestamp = packets[i].timestamp;
    }

    report("Trace stream completed", completed);
    report("Trace packet framing", parsed);
    report("Branch and jump packets", packetsPassed);
    report("Monotonic packet timestamps", timestampsPassed);
    report("No trace buffer overflow", noOverflow);

    uint32_t failures = !completed + !parsed + !packetsPassed + !timestampsPassed + !noOverflow;
    failures += unexpectedInterrupts != 0;

    Serial_IO::write(failures ? "Trace Unit IO test FAILED\n" : "Trace Unit IO test PASSED\n");

    return failures;
}
