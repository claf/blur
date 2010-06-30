#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

//#define BLUR_DEBUG

#define min(a,b) (a < b ? a : b)

int ppmb_read (char *filein_name, int *xsize, int *ysize, int *maxrgb, int **array);

int ppmb_write (char *fileout_name, int xsize, int ysize, int maxrgb, int *array);

int *ppm_getcell (int *array, int ysize, int x, int y, int k );

int ppmb_blur (int *array, int ysize, int xstart, int ystart, int xblock_size, int yblock_size);
