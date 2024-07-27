#!/bin/bash
for filename in /home/omer/Code/C++/proj/src/*.cpp; do

    clang-tidy \
    -p /home/omer/Code/C++/proj/build/ \
    --fix \
    --checks='-*,readability-identifier-naming' \
    --config="{
        CheckOptions: [
            {key: readability-identifier-naming.FunctionCase, value: lower_case},
        ],
    }" \
    $filename
done
