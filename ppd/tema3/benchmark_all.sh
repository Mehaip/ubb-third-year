#!/bin/bash

# Comprehensive benchmark script for MPI big number addition
# Tests sequential, standard_comm, and scatter_gather implementations
# with multiple process counts and averages over 10 runs

set -e

# Configuration
NUM_RUNS=10
RESULTS_DIR="benchmark_results"
TIMESTAMP=$(date +%Y%m%d_%H%M%S)
RESULTS_FILE="${RESULTS_DIR}/benchmark_${TIMESTAMP}.txt"
CSV_FILE="${RESULTS_DIR}/benchmark_${TIMESTAMP}.csv"

# Create results directory
mkdir -p "${RESULTS_DIR}"
mkdir -p output_files/sequential
mkdir -p output_files/standard_comm
mkdir -p output_files/scatter_gather

# Initialize results files
echo "MPI Big Number Addition Benchmark - $(date)" > "${RESULTS_FILE}"
echo "Number of runs per configuration: ${NUM_RUNS}" >> "${RESULTS_FILE}"
echo "========================================" >> "${RESULTS_FILE}"
echo "" >> "${RESULTS_FILE}"

# CSV header
echo "Program,File,Processes,Run,Time(s)" > "${CSV_FILE}"

# Compile all programs
echo "Compiling programs..."
echo "Compiling programs..." >> "${RESULTS_FILE}"

# Compile sequential
g++ sequential.cpp file_manager.cpp -o sequential
echo "  ✓ sequential compiled"

# Compile standard_comm
mpic++ standard_comm.cpp file_manager.cpp file_manager.h -o standard_comm
echo "  ✓ standard_comm compiled"

# Compile scatter_gather
mpic++ scatter_gather.cpp file_manager.cpp file_manager.h -o scatter_gather
echo "  ✓ scatter_gather compiled"

echo ""
echo "Compilation complete" >> "${RESULTS_FILE}"
echo "" >> "${RESULTS_FILE}"

# Function to run a test and measure time
run_test() {
    local program=$1
    local file_num=$2
    local num_procs=$3
    local run_num=$4

    local start_time=$(date +%s.%N)

    if [ "$program" == "sequential" ]; then
        ./sequential "$file_num" > /dev/null 2>&1
    else
        mpirun -np "$num_procs" --map-by :OVERSUBSCRIBE ./"$program" "$file_num" > /dev/null 2>&1
    fi

    local end_time=$(date +%s.%N)
    local elapsed=$(echo "$end_time - $start_time" | bc)

    echo "$elapsed"
}

# Function to calculate average
calculate_average() {
    local sum=0
    local count=$#

    for time in "$@"; do
        sum=$(echo "$sum + $time" | bc)
    done

    echo "scale=6; $sum / $count" | bc
}

# Function to calculate standard deviation
calculate_stddev() {
    local avg=$1
    shift
    local times=("$@")
    local sum_sq_diff=0
    local count=${#times[@]}

    for time in "${times[@]}"; do
        local diff=$(echo "$time - $avg" | bc)
        local sq_diff=$(echo "$diff * $diff" | bc)
        sum_sq_diff=$(echo "$sum_sq_diff + $sq_diff" | bc)
    done

    echo "scale=6; sqrt($sum_sq_diff / $count)" | bc
}

# Test configurations
# Format: "file_number:process_count1,process_count2,..."
# For file 1: only 5 processes
# For file 2 and 3: 5, 9, 17 processes
# For file 4 (10M digits): 5, 9, 17, 33 processes

declare -A test_configs
test_configs["sequential_1"]="1:"
test_configs["sequential_2"]="2:"
test_configs["sequential_3"]="3:"
test_configs["sequential_4"]="4:"

test_configs["standard_comm_1"]="1:5"
test_configs["standard_comm_2"]="2:5,9,17"
test_configs["standard_comm_3"]="3:5,9,17"
test_configs["standard_comm_4"]="4:5,9,17,33"

test_configs["scatter_gather_1"]="1:5"
test_configs["scatter_gather_2"]="2:5,9,17"
test_configs["scatter_gather_3"]="3:5,9,17"
test_configs["scatter_gather_4"]="4:5,9,17,33"

# Run all tests
for config_key in "${!test_configs[@]}"; do
    IFS='_' read -r program file_num <<< "$config_key"
    config_value="${test_configs[$config_key]}"
    IFS=':' read -r test_file proc_counts <<< "$config_value"

    if [ "$program" == "sequential" ]; then
        echo "================================================"
        echo "Testing: $program with file ${test_file}.txt"
        echo "================================================"
        echo "" >> "${RESULTS_FILE}"
        echo "Testing: $program with file ${test_file}.txt" >> "${RESULTS_FILE}"
        echo "------------------------------------------------" >> "${RESULTS_FILE}"

        declare -a times

        for run in $(seq 1 $NUM_RUNS); do
            echo -n "  Run $run/${NUM_RUNS}... "
            time_elapsed=$(run_test "$program" "$test_file" "1" "$run")
            times+=("$time_elapsed")
            echo "${time_elapsed}s"
            echo "$program,$test_file,1,$run,$time_elapsed" >> "${CSV_FILE}"
        done

        avg=$(calculate_average "${times[@]}")
        stddev=$(calculate_stddev "$avg" "${times[@]}")

        echo ""
        echo "  Average: ${avg}s"
        echo "  Std Dev: ${stddev}s"
        echo "Average time: ${avg}s (±${stddev}s)" >> "${RESULTS_FILE}"
        echo "" >> "${RESULTS_FILE}"

        unset times
    else
        # Split proc_counts by comma
        IFS=',' read -ra PROCS <<< "$proc_counts"

        for num_procs in "${PROCS[@]}"; do
            echo "================================================"
            echo "Testing: $program with file ${test_file}.txt and $num_procs processes"
            echo "================================================"
            echo "" >> "${RESULTS_FILE}"
            echo "Testing: $program with file ${test_file}.txt and $num_procs processes" >> "${RESULTS_FILE}"
            echo "------------------------------------------------" >> "${RESULTS_FILE}"

            declare -a times

            for run in $(seq 1 $NUM_RUNS); do
                echo -n "  Run $run/${NUM_RUNS}... "
                time_elapsed=$(run_test "$program" "$test_file" "$num_procs" "$run")
                times+=("$time_elapsed")
                echo "${time_elapsed}s"
                echo "$program,$test_file,$num_procs,$run,$time_elapsed" >> "${CSV_FILE}"
            done

            avg=$(calculate_average "${times[@]}")
            stddev=$(calculate_stddev "$avg" "${times[@]}")

            echo ""
            echo "  Average: ${avg}s"
            echo "  Std Dev: ${stddev}s"
            echo "Average time: ${avg}s (±${stddev}s)" >> "${RESULTS_FILE}"
            echo "" >> "${RESULTS_FILE}"

            unset times
        done
    fi
done

echo ""
echo "================================================"
echo "All benchmarks complete!"
echo "================================================"
echo "Results saved to:"
echo "  - ${RESULTS_FILE}"
echo "  - ${CSV_FILE}"
echo ""
echo "Verifying correctness..."

# Run comparison script to verify all outputs match
if ./compare_outputs.sh; then
    echo "✓ All outputs are correct!"
    echo "" >> "${RESULTS_FILE}"
    echo "Correctness check: PASSED" >> "${RESULTS_FILE}"
else
    echo "✗ Output mismatch detected! Check the outputs."
    echo "" >> "${RESULTS_FILE}"
    echo "Correctness check: FAILED" >> "${RESULTS_FILE}"
fi

echo ""
echo "Done!"
