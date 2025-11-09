#!/bin/bash

# Script to compare output files from scatter_gather and standard_comm

SCATTER_DIR="output_files/scatter_gather"
STANDARD_DIR="output_files/standard_comm"

echo "=================================="
echo "Comparing output files"
echo "=================================="
echo ""

# Check if directories exist
if [ ! -d "$SCATTER_DIR" ]; then
    echo "Error: Directory $SCATTER_DIR does not exist"
    exit 1
fi

if [ ! -d "$STANDARD_DIR" ]; then
    echo "Error: Directory $STANDARD_DIR does not exist"
    exit 1
fi

# Track statistics
total_files=0
identical_files=0
different_files=0
missing_files=0

# Iterate through all files in scatter_gather/
for file1 in "$SCATTER_DIR"/*.txt; do
    # Get just the filename (e.g., "1.txt")
    filename=$(basename "$file1")
    file2="$STANDARD_DIR/$filename"

    total_files=$((total_files + 1))

    echo "Comparing: $filename"
    echo "  Scatter/Gather: $file1"
    echo "  Standard Comm:  $file2"

    # Check if corresponding file exists in standard_comm/
    if [ ! -f "$file2" ]; then
        echo "  Status: MISSING in $STANDARD_DIR"
        missing_files=$((missing_files + 1))
        echo ""
        continue
    fi

    # Get file sizes
    size1=$(wc -c < "$file1")
    size2=$(wc -c < "$file2")
    echo "  Size (scatter_gather):  $size1 bytes"
    echo "  Size (standard_comm):   $size2 bytes"

    # Compare files
    if cmp -s "$file1" "$file2"; then
        echo "  Status: ✓ IDENTICAL - Results match!"
        identical_files=$((identical_files + 1))
    else
        echo "  Status: ✗ DIFFERENT - Results don't match!"
        different_files=$((different_files + 1))

        # Show first few characters of each file for context
        echo "  Preview scatter_gather/$filename: $(head -c 100 "$file1")..."
        echo "  Preview standard_comm/$filename:  $(head -c 100 "$file2")..."
    fi

    echo ""
done

# Check for files in standard_comm/ that don't exist in scatter_gather/
echo "Checking for files only in $STANDARD_DIR..."
for file2 in "$STANDARD_DIR"/*.txt; do
    filename=$(basename "$file2")
    file1="$SCATTER_DIR/$filename"

    if [ ! -f "$file1" ]; then
        echo "  $filename: EXISTS only in $STANDARD_DIR"
        missing_files=$((missing_files + 1))
    fi
done

echo ""
echo "=================================="
echo "Summary"
echo "=================================="
echo "Total files checked: $total_files"
echo "Identical files:     $identical_files"
echo "Different files:     $different_files"
echo "Missing files:       $missing_files"
echo "=================================="
