#!/bin/bash

set -e

cmake -S "$(pwd)" -B "$(pwd)/build" "$@"
cmake --build "$(pwd)/build" -j8
