#compiler variable
CC=gcc
#flag compile variable
CFLAGS=-c -Wall -Wno-unused-value

LIBFLAG=-lpthread

all : caltrain

caltrain: caltrain-runner.o caltrain.o
	$(CC) caltrain-runner.o caltrain.o -o caltrain $(LIBFLAG)

caltrain-runner.o : caltrain-runner.c caltrain.h
	$(CC) $(CFLAGS) caltrain-runner.c

caltrain.o : caltrain.c caltrain.h
	$(CC) $(CFLAGS) caltrain.c

clean : 
	rm -rf *o caltrain