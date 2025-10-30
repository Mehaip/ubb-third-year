#!/bin/bash

javac -d build java/*.java
if [ $? -eq 0 ]; then
    java -classpath build Main
else
    echo "Compilation failed. Not running the program."
    exit 1
fi