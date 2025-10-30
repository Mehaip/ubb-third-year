#!/bin/bash

g++ cpp/benchmark.cpp cpp/convolution_static.cpp cpp/convolution_vector.cpp cpp/convolution.h -o cpp/benchmark
./cpp/benchmark