#!/bin/bash

# Check if file number argument is provided
if [ $# -eq 0 ]; then
    echo "Usage: $0 <file_number>"
    echo "file_number should be 1, 2, or 3"
    exit 1
fi

FILE_NUMBER=$1

mpic++ standard_comm.cpp file_manager.cpp file_manager.h -o standard_comm
mpirun -np 4 --map-by :OVERSUBSCRIBE ./standard_comm $FILE_NUMBER