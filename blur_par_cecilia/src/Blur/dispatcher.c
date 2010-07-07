#include <stdio.h>
#include <ppm.h>

DECLARE_DATA {
  //int xsize, ysize; /* Image dimensions. */  
};

#define BLUR_DEBUG
#include <cecilia.h>

int  *array;
int xsize, ysize;

int dispatch_blur (void *_this, int block_size);

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

  filein_name = "/home/claferri/comete_pthread/blur/src/Blur/in.ppm";
  fileout_name = "out.ppm";
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

  CALL(REQUIRED.infos, set_info, fileout_name, xsize, ysize, maxrgb, array, nb_block);

#ifdef BLUR_DEBUG
  printf ("DEBUG : Dispatch and apply blur to data\n");
#endif
  result = dispatch_blur (_this, block_size);

  if ( result != 0 )
    printf ("ERROR : dispatch_blur error!\n" );
}


int dispatch_blur (void *_this, int block_size) 
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
    CALL (REQUIRED.work, blur, array, ysize, xstart, ystart, xsize, ysize);
    return 0;
  }
  
  do {
    do {
#ifdef BLUR_DEBUG
	printf ("DEBUG : call to worker\n");
#endif
	CALL (REQUIRED.work, blur, array, ysize, xstart, ystart, min (xleft, block_size), min (yleft, block_size));

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
