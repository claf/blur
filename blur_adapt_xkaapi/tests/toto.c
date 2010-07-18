#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "kaapi.h"

typedef struct kaapikaapi_processor_t kaapi_processor_t;
kaapi_processor_t* kaapi_request_kproc(kaapi_request_t*);

static int split_work (kaapi_stealcontext_t* sc, int reqcount, kaapi_request_t* reqs, void* arg);

typedef struct range
{
  kaapi_access_t /* unsigned int* */ base;
  unsigned int i;
  unsigned int j;
} range_t;

typedef struct task_work
{
  volatile long lock;
  range_t range;
  kaapi_taskadaptive_result_t* ktr;
} task_work_t;

static unsigned int get_range_size(const range_t* range)
{
  return range->j - range->i;
}

static void create_range
(range_t* range, unsigned int* base, unsigned int i, unsigned j)
{
  *KAAPI_DATA(unsigned int*, range->base) = base;

  range->i = i;
  range->j = j;
}

static int steal_range
(range_t* sub, range_t* range, unsigned int size)
{
  if (get_range_size(range) < size)
    return -1;

  sub->j = range->j;
  sub->i = range->j - size;
  range->j = sub->i;

  return 0;
}

static void lock_work(task_work_t* work)
{
  while (1)
  {
    if (__sync_bool_compare_and_swap(&work->lock, 0, 1))
      break;
  }
}


static void unlock_work(task_work_t* work)
{
  work->lock = 0;
  __sync_synchronize();
}

static task_work_t* alloc_work(kaapi_thread_t* thread)
{
  task_work_t* const work = kaapi_thread_pushdata_align
    (thread, sizeof(task_work_t), 8);

  kaapi_thread_allocateshareddata
    (&work->range.base, thread, sizeof(unsigned int*));

  work->range.i = 0;
  work->range.j = 0;

  work->lock = 0;
  work->ktr = NULL;

  return work;
}

static int reduce_work
(kaapi_stealcontext_t* sc, void* targ, void* tptr, size_t tsize, void* vptr)
{
  task_work_t* const vwork = (task_work_t*)vptr;
  task_work_t* const twork = (task_work_t*)tptr;

  /* twork is a copy, dont lock */
  lock_work(vwork);
  vwork->range.i = twork->range.i;
  vwork->range.j = twork->range.j;
  unlock_work(vwork);

  return 0;
}

static int split_range
(range_t* sub, range_t* range, unsigned int size)
{
  if (get_range_size(range) == 0)
  {
    return -1;
  }
  else if (get_range_size(range) < size)
  {
    /* succeed even if size too large */
    size = get_range_size(range);
  }

  sub->i = range->i;
  sub->j = range->i + size;
  range->i += size;

  return 0;
}

static void do_work(kaapi_stealcontext_t* sc, task_work_t* work)
{
  kaapi_taskadaptive_result_t* ktr;
  unsigned int i;
  int stealres;
  range_t subrange;

  printf ("range : %d to %d\n", work->range.i, work->range.j);

 compute_work:
  while (1)
  {
    lock_work(work);
    stealres = split_range(&subrange, &work->range, 512);

    subrange.base.data = work->range.base.data;
    //*KAAPI_DATA(unsigned int*, subrange.base) = *KAAPI_DATA(unsigned int*, work->range.base);
    unlock_work(work);

    if (stealres == -1)
      break ;

    for (i = subrange.i; i < subrange.j; ++i)
      (*KAAPI_DATA(unsigned int*, subrange.base))[i] += 1;

    if (work->ktr != NULL)
    {
      if (kaapi_preemptpoint(work->ktr, sc, NULL, NULL, NULL, 0, NULL))
	return ;
    }
  }

  ktr = kaapi_get_thief_head(sc);
  if (ktr == NULL)
    return ;

  kaapi_preempt_thief(sc, ktr, NULL, reduce_work, work);
  goto compute_work;
}

static void common_entry(void* arg, kaapi_thread_t* thread)
{
  kaapi_stealcontext_t* const sc = kaapi_thread_pushstealcontext
    (thread, KAAPI_STEALCONTEXT_DEFAULT, split_work, arg, NULL);

  do_work(sc, arg);

  kaapi_steal_finalize(sc);
}

static int check_sequence
(const unsigned int* base, unsigned int nelem)
{
  const unsigned int saved_nelem = nelem;

  for (; nelem; ++base, --nelem)
  {
    if (*base != 1)
    {
      printf("[!] check_sequence: %u\n", saved_nelem - nelem);
      return -1;
    }
  }

  printf ("Ended \n");

  return 0;
}

static int split_work
(kaapi_stealcontext_t* sc, int reqcount, kaapi_request_t* reqs, void* arg)
{
  task_work_t* const vwork = (task_work_t*)arg;
  task_work_t* twork;
  kaapi_thread_t* tthread;
  kaapi_task_t* ttask;
  kaapi_processor_t* tproc;
  unsigned int rangesize;
  unsigned int unitsize;
  range_t subrange;
  int stealres = -1;
  int repcount = 0;

  lock_work(vwork);

  rangesize = get_range_size(&vwork->range);

#if 0 /* fixme */
  if ((int)rangesize < 0)
  {
    unlock_work(vwork);
    return 0;
  }
#endif /* fixme */

  if (rangesize > 512)
  {
    unitsize = rangesize / (reqcount + 1);
    if (unitsize == 0)
    {
      unitsize = 512;
      reqcount = rangesize / 512;
    }

    stealres = steal_range
      (&subrange, &vwork->range, unitsize * reqcount);
  }
  unlock_work(vwork);

  if (stealres == -1)
    return 0;

  for (; reqcount > 0; ++reqs)
  {
    if (!kaapi_request_ok(reqs))
      continue ;

    tthread = kaapi_request_getthread(reqs);
    ttask = kaapi_thread_toptask(tthread);

    tproc = kaapi_request_kproc(reqs);

    kaapi_task_init(ttask, common_entry, NULL);

    twork = alloc_work(tthread);
    *KAAPI_DATA(unsigned int*, twork->range.base) =
      *KAAPI_DATA(unsigned int*, vwork->range.base);
    twork->ktr = kaapi_allocate_thief_result(sc, sizeof(task_work_t), NULL);

    split_range(&twork->range, &subrange, unitsize);

    kaapi_task_setargs(ttask, (void*)twork);
    kaapi_thread_pushtask(tthread);
    kaapi_request_reply_head(sc, reqs, twork->ktr);

    --reqcount;
    ++repcount;
  }

  return repcount;
}

int main(int argc, char** argv)//(unsigned int* base, unsigned int nelem)
{
#define ELEM_COUNT 100000
  static unsigned int base[ELEM_COUNT];

  int nelem = ELEM_COUNT;

  for (int i=0; i < ELEM_COUNT; i++)
    base[i] = 0;

  task_work_t* work;
  kaapi_thread_t* thread;
  kaapi_task_t* task;
  kaapi_frame_t frame;

  thread = kaapi_self_thread();
  kaapi_thread_save_frame(thread, &frame);

  /* processing task */
  task = kaapi_thread_toptask(thread);
  kaapi_task_init(task, common_entry, NULL);

  work = alloc_work(thread);
  create_range(&work->range, base, 0, nelem);

  kaapi_task_setargs(task, (void*)work);
  kaapi_thread_pushtask(thread);

#if 0
  /* checking task */
  task = kaapi_thread_toptask(thread);
  kaapi_task_init(task, print_entry, (void*)work);
  kaapi_thread_pushtask(thread);
#endif

  kaapi_sched_sync();
  kaapi_thread_restore_frame(thread, &frame);


  check_sequence(base, ELEM_COUNT);
}
