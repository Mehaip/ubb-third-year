#!/bin/bash

SCATTER_DIR="output_files/scatter_gather"
STANDARD_DIR="output_files/standard_comm"
SEQUENTIAL_DIR="output_files/sequential"

all_good=true

for file_scatter in "$SCATTER_DIR"/*.txt; do
    filename=$(basename "$file_scatter")
    file_standard="$STANDARD_DIR/$filename"
    file_sequential="$SEQUENTIAL_DIR/$filename"

    if [ ! -f "$file_standard" ] || [ ! -f "$file_sequential" ]; then
        echo "ERROR: Missing file $filename"
        all_good=false
        continue
    fi

    if ! cmp -s "$file_scatter" "$file_standard"; then
        echo "ERROR: $filename differs between scatter_gather and standard_comm"
        all_good=false
    fi

    if ! cmp -s "$file_scatter" "$file_sequential"; then
        echo "ERROR: $filename differs between scatter_gather and sequential"
        all_good=false
    fi
done

if [ "$all_good" = true ]; then
    echo "OKAY"
else
    exit 1
fi
