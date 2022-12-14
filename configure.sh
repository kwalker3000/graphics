#!/bin/sh

cmake -D GLFW_BUILD_DOCS=OFF -D CMAKE_CXX_COMPILER=/usr/bin/g++ -S . -B out/build -D CMAKE_BUILD_TYPE=Debug
