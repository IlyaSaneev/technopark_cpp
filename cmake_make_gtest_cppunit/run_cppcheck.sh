#!/usr/bin/env bash

cppcheck -i3rdparty/ -icmake-build-debug/ -ibuild -ival_probs.c \
--inconclusive --enable=all --language=c++ .

# cppcheck -i3rdparty/ -icmake-build-debug/ -ibuild -ival_probs.c  --enable=warning,performance,portability,style --language=c++ .