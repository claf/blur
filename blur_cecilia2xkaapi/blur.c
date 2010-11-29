/*
 * kaapi_main.c
 *
 * Created by Christophe Laferriere on 22/11/10.
 * Copyright 2010 INRIA. All rights reserved.
 *
 */
#include <kaapi.h>
#include "blur.h"

int main (int argc, char** argv)
{
  kaapi_frame_t frame;
  kaapi_task_t* task;
  kaapi_thread_t* thread;

  main_arg_t* argm;

  int result;

  kaapi_init ();
  thread = kaapi_self_thread ();
  kaapi_thread_save_frame(thread, &frame);

  task = kaapi_thread_toptask(thread);

  kaapi_task_init( task, main_body, kaapi_thread_pushdata(thread, sizeof(main_arg_t)) );

  argm = kaapi_task_getargst( task, main_arg_t );
  argm->argc = argc;
  argm->argv = argv;

  kaapi_thread_pushtask(thread);

  result = kaapi_sched_sync ();

  if (result != 0)
    printf ("kaapi_sched_sync() probleme : %d\n", result);

  return 0;
}
