#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <inttypes.h>

#include "operand_1.h"
#include "operand_2.h"

#define OPERANDS_NUMBER 200

typedef enum {FADD, FSUB, FMUL, FMIN, FMAX, FLE, FLT, FEQ, FCLASS, FLOAT2INT, INT2FLOAT} riscv_float_operation_t;

FILE* file_result[9];
FILE* file_op1;
FILE* file_op2;

char* files[] = {"Golden Model Data/1_gm.txt", "Golden Model Data/2_gm.txt", 
                 "Golden Model Data/3_gm.txt", "Golden Model Data/4_gm.txt", 
                 "Golden Model Data/5_gm.txt", "Golden Model Data/6_gm.txt", 
                 "Golden Model Data/7_gm.txt", "Golden Model Data/8_gm.txt", 
                 "Golden Model Data/9_gm.txt", "Golden Model Data/10_gm.txt",
                 "Golden Model Data/11_gm.txt"};

int main() {
    float results[OPERANDS_NUMBER];

    riscv_float_operation_t operation = FADD; 

    for (operation = FADD; operation <= INT2FLOAT; operation = (riscv_float_operation_t) ((int) operation + 1)) {
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
                        uint32_t tmp = (((uint32_t) operand_1[i]) >> 23) & 0x1;

                        if (tmp == 1) {
                            /* Signaling NaN */
                            results[i] = 8.0F;
                        } else {
                            /* Quiet NaN */
                            results[i] = 9.0F;
                        }
                    }

                    fprintf(file_result[operation], "%f", results[i]);

                    if (i != OPERANDS_NUMBER - 1) {
                        fprintf(file_result[operation], "\n");
                    }
                break;

                case FLOAT2INT: 
                printf ("%d ", i);
                    if ((i % 2) == 0) {
                        printf("Unsigned, op: %f ==> ", operand_1[i]);

                  
                        if (operand_1[i] >= 4294967295.0F || isnan(operand_1[i])) {
                            /* In HEX is: FFFFFFFF */
                            fprintf(file_result[operation], "%x", UINT32_MAX);

                            printf(" UINT32_MAX\n");
                        } else if (operand_1[i] <= 0.0F) {
                            /* In HEX is: 00000000 */
                            fprintf(file_result[operation], "00000000");

                            printf(" 0.000000\n");
                        } else {
                            fprintf(file_result[operation], "%08x", (uint32_t) operand_1[i]);
                        }
                    } else {
                        
                        printf("Signed, op: %f ==> ", operand_1[i]);

                        if (isnan(operand_1[i]) ) {
                            fprintf(file_result[operation], "%x", UINT32_MAX);

                            printf(" NAN\n");
                        } else if (operand_1[i] >= 2147483647.0F) {
                            /* In HEX is: 7FFFFFFF */
                            fprintf(file_result[operation], "%x", INT32_MAX);

                            printf("1 2147483648.000000\n");
                        } else if (operand_1[i] <= -2147483648.0F) {
                            /* In HEX is: 80000000 */
                            fprintf(file_result[operation], "%x", INT32_MIN);

                            printf("2 -2147483648.00000\n");
                        } else if (operand_1[i] > -1.0F && operand_1[i] < 1.0F) {
                            fprintf(file_result[operation], "00000000");

                            printf("3 0.000000\n");
                        } else {
                            fprintf(file_result[operation], "%08x", (int32_t) operand_1[i]);
                        }
                    }

                    if (i != OPERANDS_NUMBER - 1) {
                        fprintf(file_result[operation], "\n");
                    }
                break;

                case INT2FLOAT: 
                    /* Extract HEX rapresentation */
                    union floatU {
                        float flt;
                        uint32_t hex;
                    };

                    union floatU tmp; tmp.flt = operand_1[i];
                    
                    if ((i % 2) == 0) {
                        /* Interpret float rapresentation as hexadecimal */
                        uint32_t hexValue = tmp.hex;

                        /* Convert the integer into a float */
                        fprintf(file_result[operation], "%f", (float) hexValue);
                    } else {
                        /* Interpret float rapresentation as hexadecimal */
                        int32_t hexValue = (int32_t) tmp.hex;

                        /* Convert the integer into a float */
                        fprintf(file_result[operation], "%f", (float) hexValue);
                    }

                    if (i != OPERANDS_NUMBER - 1) {
                        fprintf(file_result[operation], "\n");
                    }
                break;
                
                default:
                break;
            }
        }

        if (operation < FCLASS) {
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