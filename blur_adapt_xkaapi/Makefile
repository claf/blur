CC=gcc
CFLAGS=-std=c99 -Wall -g
LDFLAGS=-lxkaapi -lpthread -lppm

SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)
OUT=blur

all: $(OUT)

main: main.c
	$(CC) $(CFLAGS) -gdwarf-2 -g3 -o $@ $^ $(LDFLAGS)

toto: toto.c
	$(CC) $(CFLAGS) -gdwarf-2 -g3 -o $@ $^ $(LDFLAGS)

test_queue: test_queue.o kaapi_workqueue.o
	$(CC) $(LDFLAGS) -o $@ $<

blur: $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^ 

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<


clean:
	rm -rf blur *.o *.ppm toto

.SILENT: clean
