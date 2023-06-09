#!/bin/bash

echo "Compiling ..."

mkdir -p bin

find . -iname '*.cpp' | sort | xargs \
  g++ -fdiagnostics-color=always -std=c++11 \
  -I/usr/local/include/python3.7m \
  -I/usr/local/lib/python3.7/site-packages/numpy/core/include \
  -I/usr/include/eigen3/ \
  -lpython3.7m \
  -lpthread -lutil -ldl \
  -Xlinker -export-dynamic \
  -o bin/a.out
  
echo "Compilation successful"