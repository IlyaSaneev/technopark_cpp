#!/usr/bin/env bash

valgrind --tool=memcheck --leak-check=full --leak-resolution=med  --track-origins=yes ./cmake-build-debug/prog

# valgrind --tool=memcheck --leak-check=yes ./cmake-build-debug/prog