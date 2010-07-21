#include <kaapi.h>
#include <stdio.h>
#include <ppm.h>
#include "blur.h"
#include "kaapi_workqueue.h"

int  *array;
int  *out;
int xsize, ysize;
kaapi_stack_t mst;  

typedef struct kaapikaapi_processor_t kaapi_processor_t;
kaapi_processor_t* kaapi_request_kproc(kaapi_request_t*);

typedef struct task_work
{
  int argc;
  char** argv;  
  kaapi_taskadaptive_result_t* ktr;
} task_work_t;

void do_work (kaapi_stealcontext_t* sc)
{
  /* Pop the first range, apply blur, check if thiefs,
     answer thiefs, signal finaliser component ... */

  blur_arg_t* argb;
  kaapi_task_t* task;
  signal_arg_t* args;
  kaapi_thread_t* thread;
  kaapi_taskadaptive_result_t* ktr;
  int success = 0;  

 compute:
  while (1)
    {
      success = stack_pop (&mst, (void**) &argb);

      if (!success) break;

#ifdef BLUR_DEBUG
      printf ("Start range (%d,  %d) to (%d, %d) in stack\n", argb->xstart, argb->ystart, argb->xstart + argb->xblock_size, argb->yblock_size);
      printf ("Stack size : %d\n", stack_size (&mst));
#endif
  
      ppmb_blur (argb->array, argb->out, argb->ysize, argb->xstart, argb->ystart, argb->xblock_size, argb->yblock_size, NB_NEIGHBOURS);
  
  
      thread = kaapi_self_thread ();
      task = kaapi_thread_toptask(thread);
      kaapi_task_initdfg( task, signal_body, kaapi_thread_pushdata(thread, sizeof(signal_arg_t)) );
      args = kaapi_task_getargst( task, signal_arg_t );
  
      kaapi_thread_pushtask(thread);
    }

  // END :
  ktr = kaapi_get_thief_head(sc);
  if (ktr == NULL)
    return ;

  kaapi_preempt_thief(sc, ktr, NULL, NULL, NULL);
  goto compute;
}

// TODO : ADAPT

static int split_work
(kaapi_stealcontext_t* sc, int reqcount, kaapi_request_t* reqs, void* arg)
{
  blur_arg_t* argb;
  kaapi_thread_t* tthread;
  kaapi_task_t* ttask;
  kaapi_processor_t* tproc;
  int repcount = 0;
  int success = 0;
#ifdef STEAL_HALF
  int nbtask = 0;
  int ssize = stack_size(&mst);
#endif

  for (; reqcount > 0; ++reqs)
  {
    if (!kaapi_request_ok(reqs))
      continue ;

#ifdef STEAL_HALF
    for (nbtask = 0; nbtask <= ssize / (reqcount+1); nbtask++)
      {
#endif
    tthread = kaapi_request_getthread(reqs);
    ttask = kaapi_thread_toptask(tthread);

    tproc = kaapi_request_kproc(reqs);

    kaapi_task_init(ttask, blur_body, NULL);

    //twork = alloc_work(tthread);
    //*KAAPI_DATA(unsigned int*, twork->range.base) =
    //*KAAPI_DATA(unsigned int*, vwork->range.base);
    //twork->ktr = kaapi_allocate_thief_result(sc, sizeof(task_work_t), NULL);

    //split_range(&twork->range, &subrange, unitsize);

    success = stack_steal (&mst, (void**) &argb);
    if (!success) return 0;

    kaapi_task_setargs(ttask, argb);
    kaapi_thread_pushtask(tthread);

#ifdef STEAL_HALF
      }
#endif
    kaapi_request_reply_head(sc, reqs, NULL);

    /* kaapi_task_init(ttask, common_entry, NULL); */

    /* twork = alloc_work(tthread); */
    /* *KAAPI_DATA(unsigned int*, twork->range.base) = */
    /*   *KAAPI_DATA(unsigned int*, vwork->range.base); */
    /* twork->ktr = kaapi_allocate_thief_result(sc, sizeof(task_work_t), NULL); */

    /* split_range(&twork->range, &subrange, unitsize); */

    /* kaapi_task_setargs(ttask, (void*)twork); */
    /* kaapi_thread_pushtask(tthread); */
    /* kaapi_request_reply_head(sc, reqs, twork->ktr); */

    --reqcount;
    ++repcount;
  }


  return repcount;
}

int dispatch_blur (int block_size, kaapi_thread_t* thread) 
{
  int xleft;
  int yleft;
  int xstart;
  int ystart;

  xleft = xsize - 6;
  yleft = ysize - 6;

  xstart = 3;
  ystart = 3;

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
      argb->out = out;
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
    
    xleft = xsize - 6;
    xstart = 3;

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
  int  numbytes;

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
    fileout_name = "img.ppm";

  if (argc > 3)
    block_size = atoi (argv[3]);
  else
    block_size = 128;

  if (argc > 4){
      if (atoi (argv[4]) == 1){
          #define STEAL_HALF
      }
  }


#ifdef BLUR_DEBUG
  printf ("DEBUG : reading data for file %s\n", filein_name);
#endif
  
  /* Read the data. */
  result = ppmb_read (filein_name, &xsize, &ysize, &maxrgb, &array);

  numbytes = 3 * ( xsize ) * ( ysize ) * sizeof ( int );
  out = ( int * ) malloc ( numbytes );

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
  argsi->array = out;
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

#ifdef BLUR_TIMING
  double t0, t1;
  
  /* Timing : */
  t0 = kaapi_get_elapsedtime();
#endif

  do_work(sc);

#ifdef BLUR_TIMING
  /* Timing : */
  t1 = kaapi_get_elapsedtime();
  printf("Blur and fwrite Task: %f\n", t1-t0);
#endif  

  kaapi_steal_finalize(sc);
}

int main (int argc, char** argv)
{
  task_work_t* work;
  kaapi_thread_t* thread;
  kaapi_task_t* task;
  kaapi_frame_t frame;

#ifdef BLUR_TIMING
  double t0, t1;

  /* Timing : */
  t0 = kaapi_get_elapsedtime();
#endif

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

#ifdef BLUR_TIMING
  /* Timing : */
  t1 = kaapi_get_elapsedtime();
  printf("Time Task: %f\n", t1-t0);
#endif


  kaapi_thread_restore_frame(thread, &frame);

  return 1;
}
