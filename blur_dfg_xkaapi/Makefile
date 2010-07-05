CC=gcc
CFLAGS=-std=c99 -Wall
LDFLAGS=-lxkaapi -lpthread -lppm

SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)
OUT=blur

all: $(OUT)

blur: $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^ 

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<


clean:
	rm -rf blur *.o

.SILENT: clean
