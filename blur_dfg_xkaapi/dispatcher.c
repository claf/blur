/*
 *  blur.h
 *  xkaapi
 *
 *  Created by CL on 05/07/10.
 *  Copyright 2009 INRIA. All rights reserved.
 *
 */

#include "blur.h"
#include "ppm.h"
#define BLUR_DEBUG

int  *array;
int xsize, ysize;

int dispatch_blur (int block_size);

void app_main_body (void* taskarg, kaapi_thread_t* thread)
{
  app_main_arg_t* arg0 = (app_main_arg_t*) taskarg;

  int argc = arg0->argc;
  char** argv = arg0->argv;

  




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

  filein_name = "/home/claferri/img/Lena.512.ppm";
  fileout_name = "blured.ppm";
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
  //CALL(REQUIRED.infos, set_info, fileout_name, xsize, ysize, maxrgb, array, nb_block);
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
  /* REPLACEMENT END */

#ifdef BLUR_DEBUG
  printf ("DEBUG : Dispatch and apply blur to data\n");
#endif
  result = dispatch_blur (block_size);

  if ( result != 0 )
    printf ("ERROR : dispatch_blur error!\n" );

}

int dispatch_blur (int block_size) 
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
    //CALL (REQUIRED.work, blur, array, ysize, xstart, ystart, xsize, ysize);
    return 0;
  }
  
  do {
    do {
#ifdef BLUR_DEBUG
	printf ("DEBUG : call to worker\n");
#endif
	//CALL (REQUIRED.work, blur, array, ysize, xstart, ystart, min (xleft, block_size), min (yleft, block_size));

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
