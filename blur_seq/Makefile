CC=gcc
#CFLAGS=-std=c99 -Wall -g -I/home/claferri/opt/include/
CFLAGS=-std=c99 -Wall -O3 -I/home/claferri/opt/include/
LDFLAGS=-lppm -L/home/claferri/opt/lib/

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
