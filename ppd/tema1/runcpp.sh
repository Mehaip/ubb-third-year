#!/bin/bash

g++ -std=c++14 -pthread cpp/benchmark.cpp cpp/convolution_static.cpp cpp/convolution_vector.cpp -o cpp/benchmark
./cpp/benchmark