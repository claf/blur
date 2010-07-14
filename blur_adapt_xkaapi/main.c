#include <kaapi.h>
#include <stdio.h>

typedef struct task_work
{
  int argc;
  char** argv;
} task_work_t;

static void common_entry (void* arg, kaapi_thread_t* thread)
{
  /* Open file, place ranges in stack, declare adaptive task, launch work. */
  task_work_t* task_arg = (task_work_t*) arg;
  printf ("I have %d arguments, second one is string %s\n", task_arg->argc, task_arg->argv[1]);
}

int main (int argc, char** argv)
{
  task_work_t* work;
  kaapi_thread_t* thread;
  kaapi_task_t* task;
  kaapi_frame_t frame;

  thread = kaapi_self_thread ();
  kaapi_thread_save_frame (thread, &frame);

  task = kaapi_thread_toptask (thread);
  kaapi_task_init (task, common_entry, NULL);

  work = (task_work_t*) malloc (sizeof (task_work_t));

  work->argc = argc;
  work->argv = argv;

  kaapi_task_setargs(task, (void*)work);
  kaapi_thread_pushtask(thread);

  kaapi_sched_sync();
  kaapi_thread_restore_frame(thread, &frame);

  return 1;
}
