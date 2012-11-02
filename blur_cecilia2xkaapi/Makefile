CC=gcc
CFLAGS=-std=c99 -Wall -g -I../include/ $(shell pkg-config --cflags kaapi)
#CFLAGS=-std=c99 -O3 -I../include/ $(shell pkg-config --cflags kaapi)
LDFLAGS=$(shell pkg-config --libs kaapi) -lpthread -lppm

SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)
OUT=blur

all: $(OUT)

blur: $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^ 

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

perl:
	c2x ./ ../blur_par_cecilia/src/Blur/

clean:
	rm -rf blur *.o *.ppm

veryclean:
	rm -rf blur *.o *.ppm *.h *.c

.SILENT: clean veryclean
