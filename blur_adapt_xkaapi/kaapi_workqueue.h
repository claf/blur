#include <stdlib.h>

typedef struct lfs_element {
  void* value;
  struct lfs_element* next;
} lfs_element_t;

typedef struct lfs {
  lfs_element_t* tail;
  lfs_element_t* dummy;
  int counter;
} lfs_t;

typedef lfs_t kaapi_stack_t;

void stack_init (lfs_t* mst) {
  mst->counter = 0;

  mst->dummy = malloc (sizeof (lfs_element_t));
  mst->dummy->value = NULL;
  mst->dummy->next = NULL;

  mst->tail = mst->dummy;
}

int stack_pop (lfs_t* mst, void* element) {
  lfs_element_t* back_tail;

  while (1) {
    if (!mst->counter) return 0;

    back_tail = mst->tail;
    element = back_tail->value;

    if( __sync_bool_compare_and_swap( &mst->tail, back_tail, mst->tail->next ) ) break;
  }

  free (back_tail);
  __sync_sub_and_fetch(&mst->counter,1);

  return (element==NULL)?0:1;
}

void stack_destroy (lfs_t* mst) {
  while (mst->counter)
    stack_pop (mst, NULL);
}

int stack_push (lfs_t* mst, void* element) {
  lfs_element_t* back_tail;
  lfs_element_t* new_element = malloc (sizeof (lfs_element_t));

  new_element->value = element;
  new_element->next = NULL;

  while (1) {
    back_tail = mst->tail;
    new_element->next = back_tail;
    if ( __sync_bool_compare_and_swap( &mst->tail, back_tail, new_element ) ) break;
  }

  __sync_add_and_fetch(&mst->counter,1);
  return 1;
}

int stack_isempty (lfs_t* mst) { return !mst->counter; }
int stack_size (lfs_t* mst) { return mst->counter; }
