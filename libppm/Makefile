CC=gcc
prefix=/usr/local

all:

ifeq ($(shell uname -s),Darwin)
LIB_EXT=dylib
libppm.dylib: ppm.o
	gcc -dynamiclib -Wl,-dylib_install_name,libppm.dylib -o $@ $^
else
LIB_EXT=so
libppm.so: ppm.o
	gcc -shared -Wl,-soname,libppm.so -o $@ $^
endif

all: libppm.$(LIB_EXT)

ppm.o: ppm.c ppm.h
	gcc -fPIC -c ppm.c

clean:
	rm -fr *.so *.dylib *.o libppm.so.1.0 

ifeq ($(shell uname -s),Darwin)
install:
	cp libppm.dylib $(prefix)/lib;
	cp ppm.h $(prefix)/include;
else
install:
	cp libppm.so $(prefix)/lib;
	cp ppm.h $(prefix)/include;
endif

.SILENT: clean
