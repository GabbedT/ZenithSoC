#!/bin/bash

# Name of the input file
input_file="/home/gabriele/Desktop/Projects/ZenithSoC/Testbench/System/output_trace.txt"

# Maximum number of lines per file
max_lines=200

# Count the number of lines in the file
total_lines=$(wc -l < "$input_file")

# Calculate the number of required files
num_files=$(( (total_lines + max_lines - 1) / max_lines ))

echo "Number of files = $num_files ; Total lines = $total_lines"

# Split the file into parts with a maximum of $max_lines lines each
split -l $max_lines -d --additional-suffix=.txt "$input_file" file_

cd "ZenithSoC Data"
rm *
cd ../

# Rename the files to make them numerical and sequential
a=1
for i in file_*; do
    mv "$i" "ZenithSoC Data/$a.txt"
    ((a++))
done

cd "ZenithSoC Data"


a=1

for i in $(ls -v); do
    echo -n "Analyzing $i ..." 

    if [[ "$i" == "10.txt" ]]; then
        truncate -s -1 $i

        res=$(diff -u "$i" "../Golden Model Data/10_gm.txt")
        ((a++))

        if [ $res -z ]; then
            echo "[OK]"
        else 
            echo "[ERROR]"
        fi 

        continue
    fi

    python3 ../converter.py $i $i

    res=$(diff -u "$i" "../Golden Model Data/${a}_gm.txt")
    ((a++))

    if [ $res -z ]; then
        echo "[OK]"
    else 
        echo "[ERROR]"
    fi 
done

