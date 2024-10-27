CC=gcc
CFLAGS=-I.

parallel_min_max : utils.o find_min_max.o utils.h find_min_max.h
	$(CC) -o parallel_min_max utils.o find_min_max.o parallel_min_max.c $(CFLAGS)

# Правило для программы process_memory
process_memory: process_memory.o
	$(CC) -o process_memory process_memory.c $(CFLAGS)

parallel_sum: parallel_sum.o utils.o utils.h
	$(CC) -o parallel_sum utils.o parallel_sum.c -lpthread $(CFLAGS)

utils.o : utils.h
	$(CC) -o utils.o -c utils.c $(CFLAGS)

find_min_max.o : utils.h find_min_max.h
	$(CC) -o find_min_max.o -c find_min_max.c $(CFLAGS)

clean:
	rm -f *.o parallel_min_max process_memory parallel_sum