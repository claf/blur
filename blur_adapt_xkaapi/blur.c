#include <kaapi.h>
#include <stdio.h>
#include <ppm.h>
#include "blur.h"
#include "kaapi_workqueue.h"

int  *array;
int xsize, ysize;
kaapi_stack_t mst;  

typedef struct task_work
{
  int argc;
  char** argv;
} task_work_t;

void do_work (kaapi_stealcontext_t* sc)
{
  /* Pop the first range, apply blur, check if thiefs,
     answer thiefs, signal finaliser component ... */

  blur_arg_t* argb;
  kaapi_task_t* task;
  signal_arg_t* args;
  kaapi_thread_t* thread;

  while (!stack_is_empty(&mst))
    {
      stack_pop (&mst, &argb);

#ifdef BLUR_DEBUG
      printf ("Start range (%d,  %d) to (%d, %d) in stack\n", argb->xstart, argb->ystart, argb->xstart + argb->xblock_size, argb->yblock_size);
      printf ("Stack size : %d\n", stack_size (&mst));
#endif

      ppmb_blur (argb->array, argb->ysize, argb->xstart, argb->ystart, argb->xblock_size, argb->yblock_size);

      
      thread = kaapi_self_thread ();
      task = kaapi_thread_toptask(thread);
      kaapi_task_initdfg( task, signal_body, kaapi_thread_pushdata(thread, sizeof(signal_arg_t)) );
      args = kaapi_task_getargst( task, signal_arg_t );
      
      kaapi_thread_pushtask(thread);
    }

}

// TODO : ADAPT

/* static int split_work */
/* (kaapi_stealcontext_t* sc, int reqcount, kaapi_request_t* reqs, void* arg) */
/* { */
/*   task_work_t* const vwork = (task_work_t*)arg; */
/*   task_work_t* twork; */
/*   kaapi_thread_t* tthread; */
/*   kaapi_task_t* ttask; */
/*   kaapi_processor_t* tproc; */
/*   unsigned int rangesize; */
/*   unsigned int unitsize; */
/*   range_t subrange; */
/*   int stealres = -1; */
/*   int repcount = 0; */

/*   lock_work(vwork); */

/*   rangesize = get_range_size(&vwork->range); */

/* #if 0 /\* fixme *\/ */
/*   if ((int)rangesize < 0) */
/*   { */
/*     unlock_work(vwork); */
/*     return 0; */
/*   } */
/* #endif /\* fixme *\/ */

/*   if (rangesize > 512) */
/*   { */
/*     unitsize = rangesize / (reqcount + 1); */
/*     if (unitsize == 0) */
/*     { */
/*       unitsize = 512; */
/*       reqcount = rangesize / 512; */
/*     } */

/*     stealres = steal_range */
/*       (&subrange, &vwork->range, unitsize * reqcount); */
/*   } */
/*   unlock_work(vwork); */

/*   if (stealres == -1) */
/*     return 0; */

/*   for (; reqcount > 0; ++reqs) */
/*   { */
/*     if (!kaapi_request_ok(reqs)) */
/*       continue ; */

/*     tthread = kaapi_request_getthread(reqs); */
/*     ttask = kaapi_thread_toptask(tthread); */

/*     tproc = kaapi_request_kproc(reqs); */

/*     kaapi_task_init(ttask, common_entry, NULL); */

/*     twork = alloc_work(tthread); */
/*     *KAAPI_DATA(unsigned int*, twork->range.base) = */
/*       *KAAPI_DATA(unsigned int*, vwork->range.base); */
/*     twork->ktr = kaapi_allocate_thief_result(sc, sizeof(task_work_t), NULL); */

/*     split_range(&twork->range, &subrange, unitsize); */

/*     kaapi_task_setargs(ttask, (void*)twork); */
/*     kaapi_thread_pushtask(tthread); */
/*     kaapi_request_reply_head(sc, reqs, twork->ktr); */

/*     --reqcount; */
/*     ++repcount; */
/*   } */

/*   return repcount; */
/* } */

int dispatch_blur (int block_size, kaapi_thread_t* thread) 
{
  int xleft;
  int yleft;
  int xstart;
  int ystart;
  int result;
  int dispatch = 0;

  xleft = xsize;
  yleft = ysize;

  xstart = 0;
  ystart = 0;

  stack_init (&mst);

  
  /* TODO : if the image isn't a square. */
  if (block_size >= xsize) {
    printf ("Choose a smaller block size please\n");
    return 0;
  }
  
  do {
    do {

      blur_arg_t* argb = (blur_arg_t*) malloc (sizeof (blur_arg_t));

      argb->array = array;
      argb->ysize = ysize;
      argb->xstart = xstart;
      argb->ystart = ystart;
      argb->xblock_size = min (xleft, block_size);
      argb->yblock_size = min (yleft, block_size);
	
      stack_push (&mst, argb);

#ifdef BLUR_DEBUG
      printf ("Push range (%d,  %d) to (%d, %d) in stack\n", xstart, ystart, xstart + min (xleft, block_size), ystart + min (yleft, block_size));
      printf ("Stack size : %d\n", stack_size (&mst));
#endif

      xstart += block_size;
      xleft  -= block_size;
      
    } while (xleft > 0);
    
    xleft = xsize;
    xstart = 0;

    ystart += block_size;
    yleft  -= block_size;

  } while (yleft > 0);
  
    
  return 0;
}

static void common_entry (void* arg, kaapi_thread_t* thread)
{
  /* Open file, place ranges in stack, declare adaptive task, launch work. */
  task_work_t* task_arg = (task_work_t*) arg;
  int argc = task_arg->argc;
  char** argv = task_arg->argv;

#ifdef BLUR_DEBUG
  printf ("DEBUG : entry method from component Dispatcher\n");
#endif

  int  result;
  int  block_size;
  int  maxrgb;
  int  nb_block;

  char *filein_name;
  char *fileout_name;

  array = NULL;

  if (argc > 1)
    filein_name= argv[1];
  else
    filein_name = "/home/claferri/img/Lena.512.ppm";

  if (argc > 2)
    fileout_name = argv[2];
  else
    fileout_name = "blured.ppm";

  if (argc > 3)
    block_size = atoi (argv[3]);
  else
    block_size = 128;

#ifdef BLUR_DEBUG
  printf ("DEBUG : reading data for file %s\n", filein_name);
#endif
  
  /* Read the data. */
  result = ppmb_read (filein_name, &xsize, &ysize, &maxrgb, &array);

  nb_block = (block_size + xsize - 1) / block_size;
  nb_block = nb_block * nb_block;

  if ( result != 0 )
    printf ("ERROR : ppmb_read error!\n" );
#ifdef BLUR_DEBUG
  else {
    printf ("DEBUG : Info for image %s :\n", filein_name);
    printf ("\twidth :%d\n", xsize);
    printf ("\theight :%d\n", ysize);
    printf ("\tmax color :%d\n", maxrgb);
    printf ("\tblock_size : %d\n", block_size);
    printf ("\tnb_block : %d\n", nb_block);
  }
#endif
  /* REPLACEMENT BEG */

  kaapi_task_t* task;
  set_info_arg_t* argsi;

  task = kaapi_thread_toptask(thread);
  kaapi_task_initdfg( task, set_info_body, kaapi_thread_pushdata(thread, sizeof(set_info_arg_t)) );
  argsi = kaapi_task_getargst( task, set_info_arg_t );
  argsi->fileout_name = fileout_name;
  argsi->xsize = xsize;
  argsi->ysize = ysize;
  argsi->maxrgb = maxrgb;
  argsi->array = array;
  argsi->nb_block = nb_block;

  kaapi_thread_pushtask(thread);


  kaapi_sched_sync( );

  /* REPLACEMENT END */

#ifdef BLUR_DEBUG
  printf ("DEBUG : Dispatch and apply blur to data\n");
#endif
  result = dispatch_blur (block_size, thread);

  if ( result != 0 )
    printf ("ERROR : dispatch_blur error!\n" );


  kaapi_stealcontext_t* const sc = kaapi_thread_pushstealcontext
    (thread, KAAPI_STEALCONTEXT_DEFAULT, split_work, arg, NULL);

  do_work(sc);

  kaapi_steal_finalize(sc);
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
