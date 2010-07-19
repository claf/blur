#include "blur.h"
#include <ppm.h>

void blur_body (void* taskarg, kaapi_thread_t* thread)
{
  blur_arg_t* arg0 = (blur_arg_t*) taskarg;
  kaapi_task_t* task;
  signal_arg_t* args;

#ifdef BLUR_DEBUG
  printf ("DEBUG : blur method from component Worker\n");
#endif

  ppmb_blur (arg0->array, arg0->out, arg0->ysize, arg0->xstart, arg0->ystart, arg0->xblock_size, arg0->yblock_size);

  task = kaapi_thread_toptask(thread);
  kaapi_task_initdfg( task, signal_body, kaapi_thread_pushdata(thread, sizeof(signal_arg_t)) );
  args = kaapi_task_getargst( task, signal_arg_t );

  kaapi_thread_pushtask(thread);
}
