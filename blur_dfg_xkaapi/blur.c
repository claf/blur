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

  double t0, t1;

  app_main_arg_t* argm;

  /* Timing : */
  t0 = kaapi_get_elapsedtime();
  
  thread = kaapi_self_thread();
  kaapi_thread_save_frame(thread, &frame);
  
  task = kaapi_thread_toptask(thread);

  kaapi_task_init( task, app_main_body, kaapi_thread_pushdata(thread, sizeof(app_main_arg_t)) );

  argm = kaapi_task_getargst( task, app_main_arg_t );
  argm->argc = argc;
  argm->argv = argv;

  kaapi_thread_pushtask(thread);

  kaapi_sched_sync ();

  /* Timing : */
  t1 = kaapi_get_elapsedtime();

  printf("Time Task: %f\n", t1-t0);

  return 0;
}

