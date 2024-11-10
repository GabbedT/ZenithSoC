#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>

#include "operand_1.h"
#include "operand_2.h"

#define OPERANDS_NUMBER 200

typedef enum {FADD, FSUB, FMUL, FMIN, FMAX, FLE, FLT, FEQ, FCLASS} riscv_float_operation_t;

FILE* file_result[9];
FILE* file_op1;
FILE* file_op2;

char* files[] = {"Golden Model Data/1_gm.txt", "Golden Model Data/2_gm.txt", 
                 "Golden Model Data/3_gm.txt", "Golden Model Data/4_gm.txt", 
                 "Golden Model Data/5_gm.txt", "Golden Model Data/6_gm.txt", 
                 "Golden Model Data/7_gm.txt", "Golden Model Data/8_gm.txt", 
                 "Golden Model Data/9_gm.txt"};

int main() {
    float results[OPERANDS_NUMBER];

    riscv_float_operation_t operation = FADD; 

    for (operation = FADD; operation <= FCLASS; operation = (riscv_float_operation_t) ((int) operation + 1)) {
        file_result[operation] = fopen(files[operation], "w");

        for (int i = 0; i < OPERANDS_NUMBER; ++i) {
            switch (operation) {
                case FADD:
                    results[i] = operand_1[i] + operand_2[i];
                break;

                case FSUB:
                    results[i] = operand_1[i] - operand_2[i];
                break;

                case FMUL:
                    results[i] = operand_1[i] * operand_2[i];
                break;

                case FMIN:
                    if (isnan(operand_1[i]) ^ isnan(operand_2[i])) {
                        results[i] = isnan(operand_1[i]) ? operand_2[i] : operand_1[i];
                    } else if (isnan(operand_1[i]) & isnan(operand_2[i])) {
                        results[i] = NAN;
                    } else  {
                        results[i] = (operand_1[i] > operand_2[i]) ? operand_2[i] : operand_1[i];
                    }
                break;

                case FMAX:
                if (isnan(operand_1[i]) ^ isnan(operand_2[i])) {
                        results[i] = isnan(operand_1[i]) ? operand_2[i] : operand_1[i];
                    } else if (isnan(operand_1[i]) & isnan(operand_2[i])) {
                        results[i] = NAN;
                    } else  {
                        results[i] = (operand_1[i] > operand_2[i]) ? operand_1[i] : operand_2[i];
                    }
                break;

                case FLE:
                    results[i] = (operand_1[i] <= operand_2[i]) ? 1.0F : 0.0F;
                break;

                case FLT:
                    results[i] = (operand_1[i] < operand_2[i]) ? 1.0F : 0.0F;
                break;

                case FEQ:
                    results[i] = (operand_1[i] == operand_2[i]) ? 1.0F : 0.0F;
                break;

                case FCLASS: 
                    if (isinf(operand_1[i]) && signbit(operand_1[i])) {
                        results[i] = 0.0F;
                    } else if (operand_1[i] < 0) {
                        results[i] = 1.0F;
                    } else if ((fpclassify(operand_1[i]) == FP_SUBNORMAL) && (operand_1[i] < 0)) {
                        results[i] = 2.0F;
                    } else if ((operand_1[i] == 0.0F) && signbit(operand_1[i])) {
                        results[i] = 3.0F;
                    } else if ((operand_1[i] == +0.0F) && !signbit(operand_1[i])) {
                        results[i] = 4.0F;
                    }  else if ((fpclassify(operand_1[i]) == FP_SUBNORMAL) && (operand_1[i] > 0)) {
                        results[i] = 5.0F;
                    } else if (isinf(operand_1[i]) && !signbit(operand_1[i])) {
                        results[i] = 7.0F;
                    } else if (operand_1[i] > 0) {
                        results[i] = 6.0F;
                    } else if (isnan(operand_1[i])) {
                        unsigned int tmp = (((unsigned int) operand_1[i]) >> 23) & 0x1;

                        if (tmp == 1) {
                            /* Signaling NaN */
                            results[i] = 8.0F;
                        } else {
                            /* Quiet NaN */
                            results[i] = 9.0F;
                        }
                    }

                    fprintf(file_result[operation], "%f\n", results[i]);
                break;
                
                default:
                    break;
            }
        }

        if (operation != FCLASS) {
            for (int i = 0; i < OPERANDS_NUMBER; ++i) {
                if (isinf(results[i])) {
                    fprintf(file_result[operation], "%s", results[i] > 0.0F ? "INFINITY" : "-INFINITY");
                } else if (isnan(results[i])) {
                    fprintf(file_result[operation], "NAN");
                } else {
                    fprintf(file_result[operation], "%f", results[i]);
                }
                
                if (i != OPERANDS_NUMBER - 1) {
                    fprintf(file_result[operation], "\n");
                }
            }
        }

        fclose(file_result[operation]);
    }
}