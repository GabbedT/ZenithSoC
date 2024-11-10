#include "../../Library/SerialOut.h"
#include "../../Library/mmio.h"
#include "../../Library/platform.h"

#include "operand_1.h"
#include "operand_2.h"

#define OPERANDS_NUMBER 200

typedef enum {FADD, FSUB, FMUL, FMIN, FMAX, FLE, FLT, FEQ, FCLASS} riscv_float_operation_t;

extern "C" void floating_point() {
    SerialOut::init();

    riscv_float_operation_t operation = FADD;

    volatile float *nc_buffer = (float *) NC_MEMORY_BASE;

    float cpuResult; 


    union {
        float    f;
        uint32_t x;
    } converterCpu, converterOp1, converterOp2; 

    for (operation = FADD; operation <= riscv_float_operation_t::FCLASS; operation = (riscv_float_operation_t) ((int) operation + 1)) {
        for (int i = 0; i < OPERANDS_NUMBER; ++i) {

            switch (operation) {
                case FADD:
                    nc_buffer[i] = operand_1[i] + operand_2[i];
                break;

                case FSUB:
                    nc_buffer[i] = operand_1[i] - operand_2[i];
                break;

                case FMUL:
                    nc_buffer[i] = operand_1[i] * operand_2[i];
                break;

                case FMIN:
                    __asm__ volatile ("fmin.s %0, %1, %2" : "=f"(nc_buffer[i]) : "f"(operand_1[i]), "f"(operand_2[i]));
                break;

                case FMAX:
                    __asm__ volatile ("fmax.s %0, %1, %2" : "=f"(nc_buffer[i]) : "f"(operand_1[i]), "f"(operand_2[i]));
                break;

                case FLE:
                    nc_buffer[i] = (operand_1[i] <= operand_2[i]) ? 1.0F : 0.0F;
                break;

                case FLT:
                    nc_buffer[i] = (operand_1[i] < operand_2[i]) ? 1.0F : 0.0F;
                break;

                case FEQ:
                    nc_buffer[i] = (operand_1[i] == operand_2[i]) ? 1.0F : 0.0F;
                break;

                case FCLASS: 
                    int classif;

                    __asm__ volatile ("fclass.s %0, %1" : "=r" (classif) : "f" (operand_1[i])); 

                    nc_buffer[i] = (float) classif;
                break;
                
                default:
                break;
            }
        }
    }

    // for (int i = 0; i < OPERANDS_NUMBER; ++i) {
    //     converterCpu.f = nc_buffer[i];

    //     SerialOut::writeH(converterCpu.x);
    //     SerialOut::write('\n');
    // }

    // SerialOut::write("\n\n\n");
}