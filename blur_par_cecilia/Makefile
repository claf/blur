# START SNIPPET: Content
# If the CECILIA_ROOT variable is defined, look for 'ceciliac' in this directory
# otherwise, expect that 'ceciliac' is in the PATH.
ifdef CECILIA_ROOT
  # If CECILIA_ROOT contains '\', replace them by '/' (for Windows)
  CECILIAC = $(subst \,/, $(CECILIA_ROOT)/bin/ceciliac)
else 
  CECILIAC = ceciliac
endif

all: blur 

# START SNIPPET: blur-rule
blur: build/blur
	$(CECILIAC) -I="../include/" -l="-lppm" -src-path=src -o=build/blur -t=unix \
	  Blur.Blur:blur
# END SNIPPET: blur-rule

build/blur:
	mkdir -p build/blur

clean:
	rm -Rf build target out.ppm

run: blur
	./build/blur/obj/blur

.PHONY: all blur
	
.SILENT: clean
