# Register machines Simulator

## Overview
Note: Part of a uni coursework

Simulates the execution of a RM program. 

RM Instructions:
- INC(i), _increase_: add 1 to register i
- DECJZ(i, j), _decrease or jump on zero_: subtract 1 from register i or to jump to instruction I<sub>j</sub> if register i is 0

## Compilation 
The file rmsim.cpp can be compiled using either
- c++ -std=c++11 -stdlib=libc++ rmsim.cpp -o rmsim
- g++ rmsim.cpp -o rmsim (for older version of GCC)

Command-line flag -t prints a trace of the execution of the RM.

## RM Programs
- **square.r** is a RM program that computes the square of a number 
    
    &nbsp;_Note_: It expects to find its input in register 0 and will leave the result in register 0.

## To do's
- add a macro facility (similar to C)
