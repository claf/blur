#include <kaapi.h>
#include "kaapi_workqueue.h"

void stack_init (stack_t* my_stack)
{
  my_stack->_beg = 0;
  my_stack->_end = 0;
  my_stack->_lock = 0;

  my_stack->_stack = malloc (STACK_MAX_ELEMENT * sizeof (void*));
}

int stack_size (stack_t* my_stack)
{
  int e = my_stack->_end;
  kaapi_readmem_barrier();
  int b = my_stack->_beg;
  return b < e ? e-b : 0;
}

int stack_is_empty (stack_t* my_stack)
{
  int b = my_stack->_beg;
  kaapi_readmem_barrier();
  int e = my_stack->_end;
  return e <= b;
}

void stack_push (stack_t* my_stack, void* element)
{
  kaapi_assert_debug (my_stack->_end < my_stack->_beg + STACK_MAX_ELEMENT);

  my_stack->_end++;
  my_stack->_stack[my_stack->_end % STACK_MAX_ELEMENT] = element;
}

void stack_pop (stack_t* my_stack, void* element);

void stack_pop_safe (stack_t* my_stack, void* element);

void stack_steal (stack_t* my_stack, void* element);

void stack_steal_unsafe (stack_t* my_stack, void* element);

void lock_pop();

void lock_steal();

void unlock();
