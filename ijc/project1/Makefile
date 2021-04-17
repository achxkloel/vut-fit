##
# File:        Makefile
# Project:     IJC-DU1
# Author:      Evgeny Torbin
# Date:        25.03.2021
# Compiled:    gcc (Ubuntu 7.5.0-3ubuntu1~18.04) 7.5.0
##

#
# Variables
#

CC = gcc
CFLAGS = -g -O2 -std=c11 -pedantic -Wall -Wextra
BIN = primes primes-i steg-decode

#
# Targets will be compiled after make command
#

all: $(BIN)

#
# Binary files
#

# with macros
primes: eratosthenes.o error.o primes.o
	$(CC) eratosthenes.o error.o primes.o -o primes $(CFLAGS)

# with inline functions
primes-i: eratosthenes-i.o error.o primes-i.o bitset-i.o
	$(CC) eratosthenes-i.o error.o primes-i.o bitset-i.o -o primes-i $(CFLAGS) -DUSE_INLINE

steg-decode: steg-decode.o ppm.o error.o eratosthenes.o
	$(CC) steg-decode.o ppm.o error.o eratosthenes.o -o steg-decode $(CFLAGS)

#
# Object files
#

# with macros
eratosthenes.o: eratosthenes.c bitset.h
	$(CC) -c eratosthenes.c -o eratosthenes.o $(CFLAGS)
error.o: error.c
	$(CC) -c error.c -o error.o $(CFLAGS)
primes.o: primes.c eratosthenes.h bitset.h
	$(CC) -c primes.c -o primes.o $(CFLAGS)

# with inline functions
eratosthenes-i.o: eratosthenes.c bitset.h
	$(CC) -c eratosthenes.c -o eratosthenes-i.o $(CFLAGS) -DUSE_INLINE
primes-i.o: primes.c eratosthenes.h bitset.h
	$(CC) -c primes.c -o primes-i.o $(CFLAGS) -DUSE_INLINE
bitset-i.o: bitset.c bitset.h
	$(CC) -c bitset.c -o bitset-i.o $(CFLAGS) -DUSE_INLINE

steg-decode.o: steg-decode.c ppm.h error.h bitset.h eratosthenes.h
	$(CC) -c steg-decode.c -o steg-decode.o $(CFLAGS)
ppm.o: ppm.c ppm.h error.h
	$(CC) -c ppm.c -o ppm.o $(CFLAGS)

#
# Other commands
#

# run primes, primes-i
run: primes primes-i
	./primes
	./primes-i

# delete all object, binary and .zip files
clean:
	rm -f *.o *.zip $(BIN)

# compress all source, header files and Makefile
zip:
	zip xtorbi00.zip *.c *.h Makefile