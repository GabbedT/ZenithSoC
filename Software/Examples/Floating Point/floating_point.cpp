#include "../../Library/SerialOut.h"

#include "operand_1.h"
#include "operand_2.h"
#include "result.h"

#define OPERANDS_NUMBER 200

typedef enum {FADD, FSUB, FMUL, FMIN, FMAX, FLE, FLT, FEQ, FCLASS} riscv_float_operation_t;

extern "C" void floating_point() {
    SerialOut::init();

    riscv_float_operation_t operation = FADD;

    float cpuResult; 

    for (int i = 0; i < OPERANDS_NUMBER; ++i) {
        switch (operation) {
            case FADD:
                cpuResult = operand_1[i] + operand_2[i];
            break;

            case FSUB:
                cpuResult = operand_1[i] - operand_2[i];
            break;

            case FMUL:
                cpuResult = operand_1[i] * operand_2[i];
            break;

            case FMIN:
                cpuResult = (operand_1[i] > operand_2[i]) ? operand_2[i] : operand_1[i];
            break;

            case FMAX:
                cpuResult = (operand_1[i] > operand_2[i]) ? operand_1[i] : operand_2[i];
            break;

            case FLE:
                cpuResult = (operand_1[i] <= operand_2[i]) ? 1.0F : 0.0F;
            break;

            case FLT:
                cpuResult = (operand_1[i] < operand_2[i]) ? 1.0F : 0.0F;
            break;

            case FEQ:
                cpuResult = (operand_1[i] == operand_2[i]) ? 1.0F : 0.0F;
            break;
            
            default:
                break;
        }

        if (cpuResult == float_result[i]) {
            SerialOut::printf("Test %d: OK\n", i);
        } else {
            SerialOut::printf("Test %d: KO => 0x%x vs 0x%x", i, cpuResult, float_result[i]);
        }
    }
}