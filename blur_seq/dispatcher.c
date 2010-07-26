#include <stdio.h>
#include <ppm.h>
#include "blur.h"

int  *array;
int *out;
int xsize, ysize;

double get_elapsedtime ()
{
  struct timeval tv;
  int err = gettimeofday( &tv, 0);
  if (err  !=0) return 0;
  return (double)tv.tv_sec + 1e-6*(double)tv.tv_usec;
}


int dispatch_blur (int block_size);

int main (int argc, char* argv[])
{
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

#ifdef BLUR_TIMING
  double t0, t1, t2;

  /* Timing : */
  t0 = get_elapsedtime();
#endif

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

#ifdef BLUR_DEBUG
  printf ("DEBUG : reading data for file %s\n", filein_name);
#endif
  
  printf ("# Blur Sequentiel : %s -> %s :: %d\n", filein_name, fileout_name, block_size);

  /* Read the data. */
  result = ppmb_read (filein_name, &xsize, &ysize, &maxrgb, &array);

  numbytes = 3 * ( xsize ) * ( ysize ) * sizeof ( int );
  out = ( int * ) malloc ( numbytes );

  nb_block = (block_size + (xsize - (2*NB_NEIGHBOURS)) - 1) / block_size;
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

  set_info (fileout_name, xsize, ysize, maxrgb, out, nb_block);

#ifdef BLUR_DEBUG
  printf ("DEBUG : Dispatch and apply blur to data\n");
#endif

#ifdef BLUR_TIMING
  /* Timing : */
  t1 = get_elapsedtime();
  printf("fopen %f\n", t1-t0);
#endif

  result = dispatch_blur (block_size);

  if ( result != 0 )
    printf ("ERROR : dispatch_blur error!\n" );

#ifdef BLUR_TIMING
  /* Timing : */
  t2 = get_elapsedtime();
  printf("total %f\n", t2-t0);
#endif
}


int dispatch_blur (int block_size) 
{
  int xleft;
  int yleft;
  int xstart;
  int ystart;

  xleft = xsize - (2 * NB_NEIGHBOURS);
  yleft = ysize - (2 * NB_NEIGHBOURS);

  xstart = NB_NEIGHBOURS;
  ystart = NB_NEIGHBOURS;
  
  /* TODO : if the image isn't a square. */
  if (block_size >= xsize) {
    printf ("Choose a smaller block size please\n");
    return 0;
  }
  
#ifdef BLUR_TIMING
  double t0, t1, tTot = 0;
#endif

  do {
    do {
#ifdef BLUR_DEBUG
      printf ("DEBUG : call to worker\n");
#endif

#ifdef BLUR_TIMING
  t0 = get_elapsedtime();
#endif
      blur (array, out, ysize, xstart, ystart, min (xleft, block_size), min (yleft, block_size));
#ifdef BLUR_TIMING
  t1 = get_elapsedtime();
  tTot += t1-t0;
#endif
      signal ();


      xstart += block_size;
      xleft  -= block_size;
      
    } while (xleft > 0);
    
    xleft = xsize - (2 * NB_NEIGHBOURS);
    xstart = NB_NEIGHBOURS;

    ystart += block_size;
    yleft  -= block_size;

  } while (yleft > 0);

#ifdef BLUR_TIMING
  printf ("blur %f\n", tTot);
#endif
    
  return 0;
}
