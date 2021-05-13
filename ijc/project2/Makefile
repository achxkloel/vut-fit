##
# File:        Makefile
# Project:     IJC-DU2
# Author:      Evgeny Torbin
# Date:        26.04.2021
# Compiled:    gcc (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0
##

#
# Variables
#

CC = gcc
CFLAGS = -g -O2 -std=c99 -pedantic -Wall -Wextra
BIN = tail wordcount wordcount-dynamic
LIB = libhtab.a libhtab.so
OBJ = htab_hash_function.o htab_init.o htab_move.o htab_size.o
OBJ += htab_bucket_count.o htab_find.o htab_lookup_add.o htab_erase.o
OBJ += htab_create_item.o htab_free_item.o htab_debug_print.o htab_print.o
OBJ += htab_for_each.o htab_clear.o htab_free.o

#
# Targets will be compiled after make command
#

all: $(BIN)

#
# Binary files
#

tail: tail.c
	$(CC) $(CFLAGS) $^ -o $@

wordcount: wordcount.o io.o libhtab.a
	$(CC) $(CFLAGS) -static wordcount.o io.o -o $@ -L. -lhtab # -DHASHTEST -DMOVETEST

wordcount-dynamic: wordcount.o io.o libhtab.so
	$(CC) $(CFLAGS) -Wl,-rpath,. wordcount.o io.o -o $@ -L. -lhtab # -DHASHTEST -DMOVETEST

#
# Libraries
#

# static library
libhtab.a: $(OBJ)
	ar rcs $@ $^

# dynamic library
libhtab.so: $(OBJ)
	$(CC) $(CFLAGS) -fPIC -shared $^ -o $@

#
# Object files
#

wordcount.o: wordcount.c io.h
	$(CC) $(CFLAGS) -c $< -o $@ # -DHASHTEST -DMOVETEST

io.o: io.c
	$(CC) $(CFLAGS) -c $< -o $@

htab_hash_function.o: htab_hash_function.c htab.h
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

htab_init.o: htab_init.c htab.h htab_private.h
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

htab_move.o: htab_move.c htab.h htab_private.h
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

htab_size.o: htab_size.c htab.h htab_private.h
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

htab_bucket_count.o: htab_bucket_count.c htab.h htab_private.h
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

htab_find.o: htab_find.c htab.h htab_private.h
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

htab_lookup_add.o: htab_lookup_add.c htab.h htab_private.h
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

htab_erase.o: htab_erase.c htab.h htab_private.h
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

htab_for_each.o: htab_for_each.c htab.h htab_private.h
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

htab_clear.o: htab_clear.c htab.h htab_private.h
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

htab_free.o: htab_free.c htab.h htab_private.h
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

htab_create_item.o: htab_create_item.c htab.h htab_private.h
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

htab_free_item.o: htab_free_item.c htab.h htab_private.h
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

htab_debug_print.o: htab_debug_print.c htab.h htab_private.h
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

htab_print.o: htab_print.c htab.h htab_private.h
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

#
# Other commands
#

# delete all object, binary files and libraries
clean:
	rm -f *.o $(BIN) $(LIB)

# compress all source, header files and Makefile
zip:
	zip xtorbi00.zip *.c *.h Makefile
