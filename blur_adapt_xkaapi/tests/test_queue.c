#include <stdio.h>
#include <stdlib.h>
#include "kaapi_workqueue.h"

typedef struct margs {
  int num;
  char* name;
  void* buffer;
} margs_t;

int main (int argc, char** argv)
{
  kaapi_stack_t mst;
  
  stack_init (&mst);
  
  printf ("Stack size : %d so the stack is empty right? %s\n", stack_size (&mst),
          stack_is_empty(&mst)?"right!":"wrong!");
  
  margs_t* arg1 = (margs_t*) malloc (sizeof (margs_t));
  
  stack_push (&mst, arg1);
  
  printf ("Stack size : %d so the stack is empty right? %s\n", stack_size (&mst),
          stack_is_empty(&mst)?"right!":"wrong!");
  
  margs_t* arg2;

  stack_pop (&mst, &arg2);

  printf ("Pop works ? %s\n", arg1==arg2?"yes!":"no!");

  printf ("Stack size : %d so the stack is empty right? %s\n", stack_size (&mst),
          stack_is_empty(&mst)?"right!":"wrong!");
  
  stack_push (&mst, arg1);
  stack_push (&mst, arg1);
  stack_push (&mst, arg1);
  stack_push (&mst, arg1);

  printf ("Stack size : %d so the stack is empty right? %s\n", stack_size (&mst),
          stack_is_empty(&mst)?"right!":"wrong!");

  stack_steal (&mst, &arg2);

  printf ("Steal works ? %s\n", arg1==arg2?"yes!":"no!");

  return 0;
}
