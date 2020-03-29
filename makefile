# the compiler we will be using is g++
CC = g++

# compiler flags:
# -g		adds debug info
# -Wall		Turns on optional warnings
CFLAGS = -g -Wall

# the output binary file
# TARGET = parse-csv.cc

parse-csv : parse-csv.o
	$(CC) $(CFLAGS) -o parse-csv parse-csv.o

parse-csv.o : parse-csv.cc
	$(CC) $(CFLAGS) -c parse-csv.cc

clean : 
	rm parse-csv parse-csv.o
