static void common_entry (void* arg, kaapi_thread_t* thread)
{
  /* Open file, place ranges in stack, declare adaptive task, launch work. */
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
}