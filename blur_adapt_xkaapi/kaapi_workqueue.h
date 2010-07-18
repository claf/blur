#ifndef _KAAPI_WORKQUEUE_H_
#define _KAAPI_WORKQUEUE_H_

#define STACK_MAX_ELEMENT 512

typedef struct stack
{
  void** _stack;
  int    _beg;
  int    _end;
  int    _lock;
} kaapi_stack_t;

void stack_init (kaapi_stack_t* my_stack);

int stack_size (kaapi_stack_t* my_stack);

int stack_is_empty (kaapi_stack_t* my_stack);

void stack_push (kaapi_stack_t* my_stack, void* element);

//void stack_push_back (kaapi_stack_t* my_stack, void* element);

void stack_pop (kaapi_stack_t* my_stack, void** element);

//void stack_pop_safe (kaapi_stack_t* my_stack, void* element);

void stack_steal (kaapi_stack_t* my_stack, void** element);

//void stack_steal_unsafe (kaapi_stack_t* my_stack, void* element);

//void lock_pop();

//void lock_steal();

//void unlock();

#endif //_KAAPI_WORKQUEUE_H_