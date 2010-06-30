CC=gcc

all: libppm.so

libppm.so: ppm.o
	gcc -shared -Wl,-soname,libppm.so -o libppm.so ppm.o

ppm.o: ppm.c ppm.h
	gcc -fPIC -c ppm.c

clean:
	rm -fr *.so *.o libppm.so.1.0 

install:
	cp libppm.so /usr/local/lib;
	cp ppm.h /usr/local/include;

.SILENT: clean
