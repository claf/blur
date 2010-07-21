CC=gcc

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
	cp libppm.dylib /usr/local/lib;
	cp ppm.h /usr/local/include;
else
install:
	cp libppm.so /usr/local/lib;
	cp ppm.h /usr/local/include;
endif

.SILENT: clean
