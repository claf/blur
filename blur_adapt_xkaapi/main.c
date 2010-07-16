#include <kaapi.h>
#include <stdio.h>
#include <ppm.h>
#include "blur.h"

int  *array;
int xsize, ysize;

typedef struct task_work
{
  int argc;
  char** argv;
} task_work_t;

void do_work ()
{
  /* Pop the first range, apply blur, check if thiefs,
     answer thiefs, signal finaliser component ... */

}

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
  
  /* TODO : if the image isn't a square. */
  if (block_size >= xsize) {
    printf ("Choose a smaller block size please\n");
    return 0;
  }
  
  do {
    do {
#ifdef BLUR_DEBUG
      printf ("DEBUG : call to worker\n");
#endif

      /* TODO : Insert own task range in a stack that adaptive main task while schedule */
      

      /* kaapi_task_t* task; */
      /* blur_arg_t* argb; */

      /* task = kaapi_thread_toptask(thread); */
      /* kaapi_task_initdfg( task, blur_body, kaapi_thread_pushdata(thread, sizeof(blur_arg_t)) ); */
      /* argb = kaapi_task_getargst( task, blur_arg_t ); */
      /* argb->array = array; */
      /* argb->ysize = ysize; */
      /* argb->xstart = xstart; */
      /* argb->ystart = ystart; */
      /* argb->xblock_size = min (xleft, block_size); */
      /* argb->yblock_size = min (yleft, block_size); */
	
      /* kaapi_thread_pushtask(thread); */
      

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

  do_work();
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
