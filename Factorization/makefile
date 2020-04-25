# the compiler we will be using is g++
CC = g++

# compiler flags:
# -g		adds debug info
# -Wall		Turns on optional warnings
CFLAGS = -Ofast -g -Wall -march=native -mtune=native -funroll-loops -funswitch-loops \
 -Wunsafe-loop-optimizations -ftree-vectorizer-verbose=7 -fopt-info-vec-optimized -floop-interchange \
 -ftree-vectorize
# CFLAGS = -Og -g -Wall

# the output binary file
# TARGET = parse-csv.cc

# object files
OBJS = parse-csv.o factorize.o

# Working linking compilation
# Include the /boost library in your home folder

factorize : parse-csv.o factorize.o
	$(CC) $(CFLAGS) -o factorize factorize.o parse-csv.o ~/boost/boost_1_72_0/stage/lib/libboost_serialization.a

parse-csv.o : parse-csv.cc parse-csv.h globals.h
	$(CC) $(CFLAGS) -c parse-csv.cc

factorize.o : factorize.cc factorize.h globals.h
	$(CC) $(CFLAGS) -I ~/boost/boost_1_72_0 -c factorize.cc

.PHONY : clean

# removes non-source files
# -f		ignores nonexistent files and arguments

clean : 
	rm -f factorize $(OBJS)
