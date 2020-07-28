#!/bin/sh

if [ -z {BRD_ROOT:+x} ]; then
    BRD_ROOT=$(git rev-parse --show-toplevel); [ $? -eq 0 ] || exit 1
fi
BRD_NPROC=$(nproc); [ $? -eq 0 ] || exit 1

rm -rf build && mkdir build
cd build

# conan setup
if ! { conan profile list | grep -q '^brodilka$'; }; then
    conan profile new brodilka --detect || exit 1
    if ! { $CXX --version | grep -i 'apple'; }; then
        conan profile update settings.compiler.libcxx=libstdc++11 brodilka
    fi
    conan profile update settings.cppstd=17 brodilka
fi
if ! { conan remote list | grep -q '^bincrafters:'; }; then
    conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan || exit 1
fi

conan install -pr brodilka -if . -bmissing .. || exit 1

cmake -DCMAKE_BUILD_TYPE=${BRD_BUILD_TYPE:-Debug} -DBRD_WANT_ASAN=${BRD_WANT_ASAN:-0} .. || exit 1
cmake --build . --config Debug --target game -- -j $BRD_NPROC || exit 1
