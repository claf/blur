#include <kaapi.h>
#include <stdlib.h>
#include "kaapi_workqueue.h"

pthread_mutex_t _internal_mutex = PTHREAD_MUTEX_INITIALIZER;

void stack_init (kaapi_stack_t* my_stack)
{
  my_stack->_beg = 0;
  my_stack->_end = 0;
  my_stack->_lock = 0;

  my_stack->_stack = malloc (STACK_MAX_ELEMENT * sizeof (void*));
  //pthread_mutex_init (&_internal_mutex, NULL);
}

int stack_size (kaapi_stack_t* my_stack)
{
  pthread_mutex_lock (&_internal_mutex);
  int e = my_stack->_end;
  kaapi_readmem_barrier();
  int b = my_stack->_beg;
  pthread_mutex_unlock (&_internal_mutex);
  return b < e ? e-b : 0;
}

int stack_is_empty (kaapi_stack_t* my_stack)
{
  pthread_mutex_lock (&_internal_mutex);
  int b = my_stack->_beg;
  kaapi_readmem_barrier();
  int e = my_stack->_end;
  pthread_mutex_unlock (&_internal_mutex);
  return e <= b;
}

int stack_push (kaapi_stack_t* my_stack, void* element)
{
  pthread_mutex_lock (&_internal_mutex);
  if (my_stack->_end >= (my_stack->_beg + STACK_MAX_ELEMENT))
    {
      pthread_mutex_unlock (&_internal_mutex);
      return 0;
    }
  my_stack->_stack[my_stack->_end % STACK_MAX_ELEMENT] = element;
  my_stack->_end++;
  pthread_mutex_unlock (&_internal_mutex);
  return 1;
}

int stack_pop (kaapi_stack_t* my_stack, void** element)
{
  pthread_mutex_lock (&_internal_mutex);
  if (my_stack->_beg >= my_stack->_end)
    {
      pthread_mutex_unlock (&_internal_mutex);
      return 0;
    }
  
  *element = my_stack->_stack[my_stack->_beg];
  my_stack->_beg++;
  pthread_mutex_unlock (&_internal_mutex);
  return 1;
}

//void stack_pop_safe (kaapi_stack_t* my_stack, void* element);

int stack_steal (kaapi_stack_t* my_stack, void** element)
{
  pthread_mutex_lock (&_internal_mutex);
  if (my_stack->_beg >= my_stack->_end)
    {
      pthread_mutex_unlock (&_internal_mutex);
      return 0;
    }
  
  my_stack->_end--;
  *element = (my_stack->_stack[my_stack->_end]);
  pthread_mutex_unlock (&_internal_mutex);
  return 1;
}

//void stack_steal_unsafe (kaapi_stack_t* my_stack, void* element);

//void lock_pop();

//void lock_steal();

//void unlock();
