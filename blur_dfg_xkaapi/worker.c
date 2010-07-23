#include "blur.h"
#include <ppm.h>

void blur_body (void* taskarg, kaapi_thread_t* thread)
{
  blur_arg_t* arg0 = (blur_arg_t*) taskarg;

#ifdef BLUR_DEBUG
  printf ("DEBUG : blur method from component Worker\n");
#endif

  ppmb_blur (arg0->array, arg0->out, arg0->ysize, arg0->xstart, arg0->ystart, arg0->xblock_size, arg0->yblock_size, NB_NEIGHBOURS);

  signal_body();
}
