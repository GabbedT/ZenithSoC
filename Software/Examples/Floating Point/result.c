#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>

#include "operand_1.h"
#include "operand_2.h"

#define OPERANDS_NUMBER 200

typedef enum {FADD, FSUB, FMUL, FMIN, FMAX, FLE, FLT, FEQ, FCLASS} riscv_float_operation_t;

FILE* file_result;
FILE* file_op1;
FILE* file_op2;

int main() {
    float results[OPERANDS_NUMBER];

    file_result = fopen("result.h", "w");

    fprintf(file_result, "#include <math.h>\n");

    riscv_float_operation_t operation = FADD; 


    for (int i = 0; i < OPERANDS_NUMBER; ++i) {
        switch (operation) {
            case FADD:
                results[i] = operand_1[i] + operand_2[i];
                printf("%f = %f + %f\n", results[i], operand_1[i], operand_2[i]);
            break;

            case FSUB:
                results[i] = operand_1[i] - operand_2[i];
            break;

            case FMUL:
                results[i] = operand_1[i] * operand_2[i];
            break;

            case FMIN:
                results[i] = (operand_1[i] > operand_2[i]) ? operand_2[i] : operand_1[i];
            break;

            case FMAX:
                results[i] = (operand_1[i] > operand_2[i]) ? operand_1[i] : operand_2[i];
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
            
            default:
                break;
        }
    }

    fprintf(file_result, "float float_result[%d] = {\n", OPERANDS_NUMBER);
    
    for (int i = 0; i < OPERANDS_NUMBER; ++i) {
        printf("%f\n", results[i]);

        if (isinf(results[i])) {
            fprintf(file_result, "%s", results[i] > 0.0F ? "INFINITY" : "-INFINITY");
        } else if (isnan(results[i])) {
            fprintf(file_result, "NAN");
        } else {
            fprintf(file_result, "%f", results[i]);
        }

        if (i < OPERANDS_NUMBER - 1) {
            fprintf(file_result, ",\n");
        } else {
            fprintf(file_result, "\n");
        }
    }

    fprintf(file_result, "};");

    fclose(file_result);
}