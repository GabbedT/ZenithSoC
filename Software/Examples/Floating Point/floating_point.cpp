#include "../../Library/SerialOut.h"
#include "../../Library/mmio.h"
#include "../../Library/platform.h"

#include "operand_1.h"
#include "operand_2.h"

#define OPERANDS_NUMBER 200

typedef enum {FADD, FSUB, FMUL, FMIN, FMAX, FLE, FLT, FEQ, FCLASS, FLOAT2INT, INT2FLOAT} riscv_float_operation_t;

extern "C" void floating_point() {
    SerialOut::init();

    riscv_float_operation_t operation = FADD;

    volatile float *nc_buffer = (float *) NC_MEMORY_BASE;
    volatile uint32_t *nc_buffer_uint = (uint32_t *) NC_MEMORY_BASE;

    float cpuResult; 


    union {
        float    f;
        uint32_t x;
    } converterCpu, converterOp1, converterOp2; 

    for (operation = FADD; operation <= riscv_float_operation_t::INT2FLOAT; operation = (riscv_float_operation_t) ((int) operation + 1)) {
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

                case FCLASS: {
                    int classif;

                    __asm__ volatile ("fclass.s %0, %1" : "=r" (classif) : "f" (operand_1[i])); 

                    nc_buffer[i] = classif;

                    break;
                }

                case FLOAT2INT: {

                    if ((i % 2) == 0) {
                        uint32_t cvtInt;

                        __asm__ ("fcvt.wu.s %0, %1" : "=r" (cvtInt) : "f" (operand_1[i]));

                        nc_buffer_uint[i] = cvtInt;
                    } else {
                        int32_t cvtInt;

                        __asm__ ("fcvt.w.s %0, %1" : "=r" (cvtInt) : "f" (operand_1[i]));

                        nc_buffer_uint[i] = cvtInt;
                    }

                    break;
                }

                case INT2FLOAT: {
                    /* Extract HEX rapresentation */
                    union floatU {
                        float flt;
                        unsigned int hex;
                    };

                    union floatU tmp; tmp.flt = operand_1[i];
                    
                    if ((i % 2) == 0) {
                        /* Interpret float rapresentation as hexadecimal */
                        unsigned int hexValue = tmp.hex;

                        __asm__ ("fcvt.s.wu %0, %1" : "=f" (nc_buffer[i]) : "r" (hexValue));
                    } else {
                        /* Interpret float rapresentation as hexadecimal */
                        signed int hexValue = (signed int) tmp.hex;

                        __asm__ ("fcvt.s.w %0, %1" : "=f" (nc_buffer[i]) : "r" (hexValue));
                    }

                    break;
                }
                
                default:
                break;
            }
        }
    }
}