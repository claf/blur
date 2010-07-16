#ifndef _KAAPI_WORKQUEUE_H_
#define _KAAPI_WORKQUEUE_H_

#define STACK_MAX_ELEMENT 512

struct task
{
  
} task_t;

struct stack
{
  void* _stack;
  int   _beg;
  int   _end;
  int   _lock;
} stack_t;

void stack_init (stack_t* my_stack);

int stack_size (stack_t* my_stack);

int stack_is_empty (stack_t* my_stack);

void stack_push_front (stack_t* my_stack, void* element);

void stack_push_back (stack_t* my_stack, void* element);

void stack_pop (stack_t* my_stack, void* element);

void stack_pop_safe (stack_t* my_stack, void* element);

void stack_steal (stack_t* my_stack, void* element);

void stack_steal_unsafe (stack_t* my_stack, void* element);

void lock_pop();

void lock_steal();

void unlock();

#endif //_KAAPI_WORKQUEUE_H_
