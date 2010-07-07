#include "blur.h"
#include "ppm.h"

char *_fileout_name;
int _xsize;
int _ysize;
int _maxrgb;
int *_array;
int _nb_block;

void signal_body (void* taskarg, kaapi_thread_t* thread)
{
#ifdef BLUR_DEBUG
  printf ("DEBUG : signal method from component Finalizer\n");
#endif

  static int count = 0;

  count = count + 1;

  if (count == _nb_block) {
#ifdef BLUR_DEBUG
    printf ("DEBUG : Writing data to file %s\n", _fileout_name);
#endif
    ppmb_write (_fileout_name, _xsize, _ysize, _maxrgb, _array);
  }
}

void set_info_body (void* taskarg, kaapi_thread_t* thread)
{
  set_info_arg_t* arg0 = (set_info_arg_t*) taskarg;

#ifdef BLUR_DEBUG
  printf ("DEBUG : set_info method from component Finalizer\n");
  printf ("DEBUG : Info for image %s :\n", arg0->fileout_name);
  printf ("\twidth :%d\n", arg0->xsize);
  printf ("\theight :%d\n", arg0->ysize);
  printf ("\tmax color :%d\n", arg0->maxrgb);
  printf ("\tnb_block : %d\n", arg0->nb_block);
#endif

  _fileout_name = arg0->fileout_name;
  _xsize        = arg0->xsize;
  _ysize        = arg0->ysize;
  _maxrgb       = arg0->maxrgb;
  _array        = arg0->array;
  _nb_block     = arg0->nb_block;
}
