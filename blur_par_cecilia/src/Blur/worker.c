#include <stdio.h>
#include <ppm.h>
#include <blur_const.h>

DECLARE_DATA {
  
};

#include <cecilia.h>

void METHOD(work, blur)(void *_this, void *array, void *out, int ysize, int xstart, int ystart, int xblock_size, int yblock_size)
{

#ifdef BLUR_DEBUG
  printf ("DEBUG : blur method from component Worker\n");
#endif

  ppmb_blur (array, out, ysize, xstart, ystart, xblock_size, yblock_size, NB_NEIGHBOURS);

  CALL (REQUIRED.signal, signal);
}
