CC=gcc
CFLAGS=-std=c99 -Wall -g $(shell pkg-config --cflags kaapi) #-gdwarf-2 -g3
#CFLAGS=-std=c99 -Wall -O3 $(shell pkg-config --cflags kaapi) 
LDFLAGS=$(shell pkg-config --libs kaapi) -lpthread -lppm

SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)
OUT=blur

all: $(OUT)

main: main.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

toto: toto.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

test_queue: test_queue.o kaapi_workqueue.o
	$(CC) $(LDFLAGS) -o $@ $^

blur: $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^ 

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<


clean:
	rm -rf blur *.o *.ppm toto main test_queue

.SILENT: clean
