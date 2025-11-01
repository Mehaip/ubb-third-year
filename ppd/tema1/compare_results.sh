#!/bin/bash

# Compare results for different matrix sizes
for size in 10 1000 10000; do
    echo "========== Comparing output${size}.txt =========="

    # Compare C++ parallel vs sequential
    diff -w results/cpp/parallel/output${size}.txt results/cpp/sequential/output${size}.txt && \
        echo "cpp/parallel vs cpp/sequential - IDENTICAL" || \
        echo "cpp/parallel vs cpp/sequential - DIFFERENT"

    # Compare Java parallel vs sequential
    diff -w results/java/parallel/output${size}.txt results/java/sequential/output${size}.txt && \
        echo "java/parallel vs java/sequential - IDENTICAL" || \
        echo "java/parallel vs java/sequential - DIFFERENT"

    # Compare C++ parallel vs Java parallel
    diff -w results/cpp/parallel/output${size}.txt results/java/parallel/output${size}.txt && \
        echo "cpp/parallel vs java/parallel - IDENTICAL" || \
        echo "cpp/parallel vs java/parallel - DIFFERENT"

    # Compare C++ sequential vs Java sequential
    diff -w results/cpp/sequential/output${size}.txt results/java/sequential/output${size}.txt && \
        echo "cpp/sequential vs java/sequential - IDENTICAL" || \
        echo "cpp/sequential vs java/sequential - DIFFERENT"

    echo ""
done