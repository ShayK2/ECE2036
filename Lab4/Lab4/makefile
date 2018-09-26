CC = g++

all:calc

calc: complex-calc.o complex.o string-parse.o
	$(CC) complex-calc.o complex.o string-parse.o -o calc

complex-calc.o: complex-calc.cpp
	$(CC) -c complex-calc.cpp

complex.o: complex.cpp complex.h
	$(CC) -c complex.cpp

string-parse.o: string-parse.cpp string-parse.h
	$(CC) -c string-parse.cpp

clean:
	rm *.o calc
