#!/bin/bash

# Quick test script - runs each configuration once to verify setup

set -e

echo "Running quick test (1 iteration each)..."
echo ""

# Create output directories
mkdir -p output_files/sequential
mkdir -p output_files/standard_comm
mkdir -p output_files/scatter_gather

# Test sequential with file 1
echo "Testing sequential with file 1..."
./sequential 1

# Test standard_comm with file 1 and 5 processes
echo "Testing standard_comm with file 1 and 5 processes..."
mpirun -np 5 --map-by :OVERSUBSCRIBE ./standard_comm 1

# Test scatter_gather with file 1 and 5 processes
echo "Testing scatter_gather with file 1 and 5 processes..."
mpirun -np 5 --map-by :OVERSUBSCRIBE ./scatter_gather 1

echo ""
echo "Quick test complete! Checking if outputs match..."
echo ""

if ./compare_outputs.sh; then
    echo "✓ All outputs match! The setup is working correctly."
    echo ""
    echo "You can now run the full benchmark with:"
    echo "  ./run_full_benchmark.sh"
else
    echo "✗ Output mismatch detected. Please check your implementations."
fi
