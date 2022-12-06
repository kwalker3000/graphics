#!/bin/sh

cmake -DGLFW_BUILD_DOCS=OFF -S . -B out/build -D CMAKE_BUILD_TYPE=Debug
