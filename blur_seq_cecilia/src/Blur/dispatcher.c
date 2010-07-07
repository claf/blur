#include <stdio.h>
#include <ppm.h>

DECLARE_DATA {
  //int xsize, ysize; /* Image dimensions. */  
};

#define BLUR_DEBUG

#include <cecilia.h>

int  *array;
int xsize, ysize;

int METHOD(entry, main)(void *_this, int argc, char* argv[])
{
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

  filein_name = "in.ppm";
  fileout_name = "out.ppm";
  block_size = 512;

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

  CALL(REQUIRED.infos, set_info, fileout_name, xsize, ysize, maxrgb, array);

#ifdef BLUR_DEBUG
  printf ("DEBUG : Dispatch and apply blur to data\n");
#endif
  CALL (REQUIRED.work, blur, array, ysize, 0, 0, xsize, ysize);

  if ( result != 0 )
    printf ("ERROR : dispatch_blur error!\n" );
}
