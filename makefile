# the compiler we will be using is g++
CC = g++

# compiler flags:
# -g		adds debug info
# -Wall		Turns on optional warnings
CFLAGS = -Og -g -Wall

# the output binary file
# TARGET = parse-csv.cc

# object files
OBJS = parse-csv.o

parse-csv : parse-csv.o
	$(CC) $(CFLAGS) -o parse-csv parse-csv.o

parse-csv.o : parse-csv.cc parse-csv.h
	$(CC) $(CFLAGS) -c parse-csv.cc

.PHONY : clean

# removes non-source files
# -f		ignores nonexistent files and arguments

clean : 
	rm -f parse-csv $(OBJS)
