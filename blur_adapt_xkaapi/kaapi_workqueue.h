#include <pthread.h>

#ifndef _KAAPI_WORKQUEUE_H_
#define _KAAPI_WORKQUEUE_H_

#define STACK_MAX_ELEMENT 5000

extern pthread_mutex_t _internal_mutex;

typedef struct stack
{
  void** _stack;
  volatile int    _beg;
  volatile int    _end;
  volatile int    _lock;
} kaapi_stack_t;

void stack_init (kaapi_stack_t* my_stack);

int stack_size (kaapi_stack_t* my_stack);

int stack_is_empty (kaapi_stack_t* my_stack);

// return 1 if succeed and 0 if fail.
int stack_push (kaapi_stack_t* my_stack, void* element);

//void stack_push_back (kaapi_stack_t* my_stack, void* element);

// return 1 if succeed and 0 if fail.
int stack_pop (kaapi_stack_t* my_stack, void** element);

//void stack_pop_safe (kaapi_stack_t* my_stack, void* element);

// return 1 if succeed and 0 if fail.
int stack_steal (kaapi_stack_t* my_stack, void** element);

//void stack_steal_unsafe (kaapi_stack_t* my_stack, void* element);

//void lock_pop();

//void lock_steal();

//void unlock();

#endif //_KAAPI_WORKQUEUE_H_
