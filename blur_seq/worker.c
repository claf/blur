#include <stdio.h>
#include "blur.h"
#include <ppm.h>

void blur (void *array, int ysize, int xstart, int ystart, int xblock_size, int yblock_size)
{
#ifdef BLUR_DEBUG
  printf ("DEBUG : blur method from component Worker\n");
#endif
  ppmb_blur (array, ysize, xstart, ystart, xblock_size, yblock_size);
  signal ();
}
