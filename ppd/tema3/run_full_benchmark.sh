#!/bin/bash

# Master script to run full benchmark suite and analyze results

set -e

echo "=========================================="
echo "MPI Big Number Addition - Full Benchmark"
echo "=========================================="
echo ""
echo "This will run all tests with 10 iterations each."
echo "This may take several minutes depending on input sizes."
echo ""
read -p "Press Enter to continue or Ctrl+C to cancel..."
echo ""

# Run the benchmark
./benchmark_all.sh

# Find the most recent CSV file
LATEST_CSV=$(ls -t benchmark_results/benchmark_*.csv 2>/dev/null | head -n1)

if [ -z "$LATEST_CSV" ]; then
    echo "Error: No benchmark results found!"
    exit 1
fi

echo ""
echo "=========================================="
echo "Analyzing results..."
echo "=========================================="
echo ""

# Analyze results
./analyze_results.py "$LATEST_CSV"

echo ""
echo "Full results saved in: ${LATEST_CSV%.csv}.txt"
echo "CSV data available in: $LATEST_CSV"
echo ""
