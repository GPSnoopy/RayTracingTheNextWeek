#!/bin/sh
g++ -o book2 -std=c++1z -Wall -Wextra -O3 -ffast-math -march=skylake -Isrc src/main.cpp -lpthread
