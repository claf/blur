CC=gcc
CFLAGS=-std=c99 -Wall -I/home/claferri/opt/include/ $(shell pkg-config --cflags xkaapi)
LDFLAGS=-lppm -L/home/claferri/opt/lib/ $(shell pkg-config --libs xkaapi)

SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)
OUT=blur

all: $(OUT)

blur: $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^ 

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<


clean:
	rm -rf blur *.o *.ppm *~

.SILENT: clean
