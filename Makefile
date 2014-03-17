CC = gcc
CFLAGS = -g -pthread


sudoku: resolve.cpp sudoku.cpp
     gcc -o sudoku resolve.cpp sudoku.cpp -I.
