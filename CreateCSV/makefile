# the compiler we will be using is g++
CC = g++

# compiler flags:
# -g		adds debug info
# -Wall		Turns on optional warnings
# CFLAGS = -Ofast -g -Wall -march=native -mtune=native -funroll-loops -funswitch-loops \
 -Wunsafe-loop-optimizations -ftree-vectorizer-verbose=7 -fopt-info-vec-optimized -floop-interchange \
 -ftree-vectorize
CFLAGS = -Og -g -Wall

# the output binary file
# TARGET = parse-csv.cc

# object files
OBJS = create-csv.o

# Working linking compilation
# Include the /boost library in your home folder

create-csv : create-csv.o 
	$(CC) $(CFLAGS) -o create-csv create-csv.o ~/boost/boost_1_72_0/stage/lib/libboost_serialization.a

create-csv.o : create-csv.cc create-csv.h ../Factorization/factorize.h ../Factorization/globals.h
	$(CC) $(CFLAGS) -I ../Factorization/ -I ~/boost/boost_1_72_0 -c create-csv.cc

.PHONY : clean

# removes non-source files
# -f		ignores nonexistent files and arguments

clean : 
	rm -f factorize $(OBJS)
