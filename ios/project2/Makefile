##
# File:        Makefile
# Project:     IOS-2 (synchronization)
# Author:      Evgeny Torbin
# Date:        02.05.2021
# Compiled:    gcc (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0
##

#
# Variables
#

CC = gcc
CFLAGS = -g -std=gnu99 -Wall -Wextra -Werror -pedantic -pthread -lrt
BIN = proj2

#
# Targets will be compiled after make command
#

all: $(BIN)

#
# Binary file
#

proj2: proj2.o base.o resource.o process.o
	$(CC) $(CFLAGS) $^ -o $@

#
# Object files
#

proj2.o: proj2.c proj2.h base.h resource.h process.h
	$(CC) $(CFLAGS) -c $< -o $@

base.o: base.c proj2.h
	$(CC) $(CFLAGS) -c $< -o $@

resource.o: resource.c proj2.h
	$(CC) $(CFLAGS) -c $< -o $@

process.o: process.c proj2.h
	$(CC) $(CFLAGS) -c $< -o $@

#
# Other commands
#

# delete all object, binary and output files
clean:
	rm -f *.o proj2.out $(BIN)

# compress all source, header files and Makefile
zip:
	zip proj2.zip *.c *.h Makefile