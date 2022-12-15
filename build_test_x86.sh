#!/bin/sh
####################################################################################################
# This script will build the test of EGL for the x86 platform.
# To run it, remember to give execution permission
# to the script, and issue the command:
#
# $ ./build_test_x86.sh


####################################################################################################
# For convenience, we change into the build directory before making. This will ensure that built
# files will reside in the correct folders without having to mingle with the Cmake files.
mkdir -p build_test_x86
cd build_test_x86
cmake  -DCMAKE_BUILD_TYPE=Release -S ../test_egl
make -j $(nproc)
