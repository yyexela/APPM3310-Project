# the compiler we will be using is g++
CC = g++

# compiler flags:
# -g		adds debug info
# -Wall		Turns on optional warnings
CFLAGS = -Og -g -Wall

# the output binary file
# TARGET = parse-csv.cc

# object files
OBJS = parse-csv.o factorize.o

factorize : parse-csv.o factorize.o
	$(CC) $(CFLAGS) -o factorize factorize.o parse-csv.o

parse-csv.o : parse-csv.cc parse-csv.h globals.h
	$(CC) $(CFLAGS) -c parse-csv.cc

factorize.o : factorize.cc factorize.h globals.h
	$(CC) $(CFLAGS) -c factorize.cc

.PHONY : clean

# removes non-source files
# -f		ignores nonexistent files and arguments

clean : 
	rm -f factorize $(OBJS)
