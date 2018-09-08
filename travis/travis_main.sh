#!/bin/bash
set -e   # Make sure any error makes the script to return an error code

mkdir build
cd build
cmake .
make
