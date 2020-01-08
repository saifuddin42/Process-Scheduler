all: scheduler

CFLAGS=-g -Og -Wall -std=c99
CC=gcc

scheduler: context.c schedule.c sys.o clock.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f scheduler
