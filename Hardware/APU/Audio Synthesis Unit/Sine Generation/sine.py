import math

# Parameters
N = 512                   # Number of samples
mid = 2**15 - 1           # Center value: 32767
amp = 2**15               # Amplitude: 32768

# Generate the quarter-sine samples
sine_quarter = []
for i in range(N):
    # Calculate the corresponding angle (ranging from 0 to π/2)
    theta = (i / (N - 1)) * (math.pi / 2)
    # Calculate the sample: shift and scale the sin(theta) value
    sample = int(round(mid + math.sin(theta) * amp))
    sine_quarter.append(sample)

# Write the values to a text file in hexadecimal format
# Each value is formatted as a 4-digit hexadecimal number (padded with zeros) 
# in lower-case, one per line.
with open("sine_quarter.hex", "w") as f:
    for sample in sine_quarter:
        f.write(f"{sample:04x}\n")

print("The hexadecimal values have been written to 'sine_quarter.hex'")
