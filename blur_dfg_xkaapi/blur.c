/*
 *  blur.c
 *  xkaapi
 *
 *  Created by CL on 24/06/10.
 *  Copyright 2009 INRIA. All rights reserved.
 *
 */

#include "blur.h"
#include <stdio.h>


/********************
 * MAIN DECLARATION *
 ********************/
 
int main (int argc, char** argv)
{
  kaapi_frame_t frame;
  kaapi_task_t* task;
  kaapi_thread_t* thread;
  app_main_arg_t* argm;
  int result;

#ifdef BLUR_TIMING
  double t0, t1;

  /* Timing : */
  t0 = kaapi_get_elapsedtime();
#endif
//  kaapi_set_workload(kaapi_get_current_processor(), 1);

  kaapi_init ();
  thread = kaapi_self_thread();
  kaapi_thread_save_frame(thread, &frame);
  
  task = kaapi_thread_toptask(thread);

  kaapi_task_init( task, app_main_body, kaapi_thread_pushdata(thread, sizeof(app_main_arg_t)) );

  argm = kaapi_task_getargst( task, app_main_arg_t );
  argm->argc = argc;
  argm->argv = argv;


  kaapi_thread_pushtask(thread);

  result = kaapi_sched_sync ();

  if (result != 0)
    printf ("kaapi_sched_sync() probleme : %d\n", result);

#ifdef BLUR_TIMING
  /* Timing : */
  t1 = kaapi_get_elapsedtime();
  printf("total %f\n", t1-t0);
#endif
  
  return 0;
}

