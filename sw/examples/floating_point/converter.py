import struct
import sys

def write_output_to_file(output, file_path):
    with open(file_path, 'w') as file:
        file.write(output)

# Legge i valori esadecimali da un file
def read_hex_values_from_file(file_path):
    with open(file_path, 'r') as file:
        hex_values = file.read().split()
    return hex_values

# Convert HEX to extended floating point format
def convert_and_format_hex_values(hex_values):
    float_values = [
        struct.unpack('!f', bytes.fromhex(value))[0] for value in hex_values
    ]
    float_values_extended = [
        f"{value:.6f}" if not (value != value or value == float('inf') or value == float('-inf')) else 
        "NAN" if value != value else "INFINITY" if value == float('inf') else "-INFINITY"
        for value in float_values
    ]
    return "\n".join(float_values_extended)


def main():
    if len(sys.argv) != 3:
        sys.exit(1)

    input_file_path = sys.argv[1]
    output_file_path = sys.argv[2]

    hex_values = read_hex_values_from_file(input_file_path)
    formatted_output = convert_and_format_hex_values(hex_values)
    write_output_to_file(formatted_output, output_file_path)

if __name__ == "__main__":
    main()
