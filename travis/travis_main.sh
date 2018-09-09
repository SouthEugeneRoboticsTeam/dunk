#!/bin/bash
set -e   # Make sure any error makes the script to return an error code

cppcheck --enable=all -i build --report-progress .

mkdir build
cd build
cmake ..
make
