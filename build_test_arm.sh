#!/bin/sh
####################################################################################################
# This script will build the test of  EGL for the arm platform.
# To run it, remember to give execution permission
# to the script, and issue the command:
#
# $ ./build_test_arm.sh

####################################################################################################
# Use these variables to configure your build. You probably don't need to change them, but you
# might want to look at them if anything goes wrong.
if [ -z "$NDK_VERSION" ]; then
    NDK_VERSION=22.0.7026061
fi

export ANDROID_NDK_HOME="/home/$USER/Android/Sdk/ndk/$NDK_VERSION"


####################################################################################################
# For convenience, we change into the build directory before making. This will ensure that built
# files will reside in the correct folders without having to mingle with the Cmake files.
mkdir -p build_test_arm
cd build_test_arm
cmake   -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK_HOME/build/cmake/android.toolchain.cmake -DANDROID_ABI="arm64-v8a" -S ../test_egl
make -j $(nproc)
