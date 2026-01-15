#!/bin/bash

check() {
    local size=$1
    if diff -q results/cpp/sequential/output${size}.txt results/cpp/parallel/output${size}.txt >/dev/null; then
        echo "${size}: DA"
    else
        echo "${size}: NU"
    fi
}

check 10
check 1000
check 10000
