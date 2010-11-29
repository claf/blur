CC=gcc
CFLAGS=-std=c99 -Wall -g $(shell pkg-config --cflags kaapi)
#CFLAGS=-std=c99 -Wall -O3 $(shell pkg-config --cflags kaapi)
LDFLAGS=$(shell pkg-config --libs kaapi) -lpthread -lppm

SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)
OUT=blur

all: $(OUT)

blur: $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^ 

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<


clean:
	rm -rf blur *.o *.ppm

.SILENT: clean
