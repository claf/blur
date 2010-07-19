#include <stdio.h>
#include "blur.h"
#include <ppm.h>

void blur (void *array, void* out, int ysize, int xstart, int ystart, int xblock_size, int yblock_size)
{
#ifdef BLUR_DEBUG
  printf ("DEBUG : blur method from component Worker\n");
#endif
  ppmb_blur (array, out, ysize, xstart, ystart, xblock_size, yblock_size, NB_NEIGHBOURS);
  signal ();
}
