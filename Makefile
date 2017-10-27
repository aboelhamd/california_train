CC=gcc
CFLAGS=-Wall -Wno-unused-value
LIBFLAG=-lpthread

caltrain: caltrain-runner.c caltrain.c caltrain.h
	$(CC) $(CFLAGS) -o caltrain caltrain-runner.c -lpthread

