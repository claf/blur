#include <stdio.h>
#include "blur.h"
#include <ppm.h>

char *_fileout_name;
int _xsize;
int _ysize;
int _maxrgb;
int *_array;
int _nb_block;

void signal ()
{
#ifdef BLUR_DEBUG
  printf ("DEBUG : signal method from component Finalizer\n");
#endif

  static int count = 0;

  count = count + 1;

  if (count == _nb_block) {
#ifdef BLUR_DEBUG    
    printf ("DEBUG : Writing data to file outa.ppm\n");
#endif
    ppmb_write (_fileout_name, _xsize, _ysize, _maxrgb, _array);
  }
}

void set_info (char *fileout_name, int xsize, int ysize, int maxrgb, void *array, int nb_block)
{
#ifdef BLUR_DEBUG
  printf ("DEBUG : set_info method from component Finalizer\n");
  printf ("DEBUG : Info for image %s :\n", fileout_name);
  printf ("\twidth :%d\n", xsize);
  printf ("\theight :%d\n", ysize);
  printf ("\tmax color :%d\n", maxrgb);
  printf ("\tnb_block : %d\n", nb_block);
#endif

  _fileout_name = fileout_name;
  _xsize        = xsize;
  _ysize        = ysize;
  _maxrgb       = maxrgb;
  _array        = array;
  _nb_block     = nb_block;
}

