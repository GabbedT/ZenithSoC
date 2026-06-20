import math
import random
import struct

# MAX and MIN values
FLOATMAX =  3.4028235e+38
FLOATMIN = -3.4028235e+38

# Number of float to generate
NUMBER = 200

def generate_random_float():
    # Random probability
    r = random.random()
    
    # Return floats with random uniform distribution, special values have less probability
    if r < 0.05:
        return float('inf') if random.random() < 0.5 else float('-inf')
    elif r < 0.1:
        return float('nan')
    elif r < 0.2:
        return 0.0
    else:
        exponent = random.randint(-30, 30)
        mantissa = random.uniform(1, 10)
        sign = -1 if random.random() < 0.5 else 1

        # Create the final float
        result = sign * mantissa * (10 ** exponent)

        if result > FLOATMAX:
            result = FLOATMAX
        elif result < FLOATMIN:
            result = FLOATMIN
        
        return result
            


def generate_header_file(filename, array_size):
    with open(filename, "w") as f:
        f.write(f"#ifndef {filename[:-2].upper()}_H\n")
        f.write(f"#define {filename[:-2].upper()}_H\n\n")
        f.write("#include <math.h>\n\n")
        
        f.write(f"float {filename[:-2]}[{array_size}] = {{\n")
        
        # Write the random numbers
        for i in range(array_size):
            random_float = generate_random_float()

            if (random_float == float('inf')):
                random_float = "INFINITY"
            elif (random_float == -float('inf')):
                random_float = "-INFINITY"
            elif (math.isnan(random_float)):
                random_float = "NAN"
                

            # Hex format
            f.write(f"    {random_float}")
            if i < array_size - 1:
                f.write(",\n")
            else:
                f.write("\n")
        
        f.write("};\n\n")
        f.write("#endif // FLOAT_ARRAY_H\n")

generate_header_file("operand_1.h", NUMBER)
generate_header_file("operand_2.h", NUMBER)
