#include <stdio.h>
#include <ppm.h>
#include <blur_const.h>

DECLARE_DATA {
  //int xsize, ysize; /* Image dimensions. */  
};

#define BLUR_DEBUG
#include <cecilia.h>

int *array;
int *out;
int xsize, ysize;

//int dispatch_blur (void *_this, int block_size);

int METHOD(entry, main)(void *_this, int argc, char** argv)
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

  array = NULL;

  //filein_name = "/home/claferri/comete_pthread/blur/src/Blur/in.ppm";
  //fileout_name = "out.ppm";
  //block_size = 128;

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
  
  printf ("# Blur DFG : %s -> %s :: %d\n", filein_name, fileout_name, block_size);

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

  CALL(REQUIRED.infos, set_info, fileout_name, xsize, ysize, maxrgb, out, nb_block);

#ifdef BLUR_DEBUG
  printf ("DEBUG : Dispatch and apply blur to data\n");
#endif

  // DISPATCHER'S CODE :


  int xleft;
  int yleft;
  int xstart;
  int ystart;

  xleft = xsize - (2*NB_NEIGHBOURS);
  yleft = ysize - (2*NB_NEIGHBOURS);

  xstart = NB_NEIGHBOURS;
  ystart = NB_NEIGHBOURS;
  
  
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
	int minx = min (xleft, block_size);
	int miny = min (yleft, block_size);
	CALL (REQUIRED.work, blur, array, out, ysize, xstart, ystart, minx, miny);

	xstart += block_size;
	xleft  -= block_size;
      
    } while (xleft > 0);
    
    xleft = xsize - (2*NB_NEIGHBOURS);
    xstart = NB_NEIGHBOURS;

    ystart += block_size;
    yleft  -= block_size;

  } while (yleft > 0);
    
  return 0;


}


/* int dispatch_blur (void *_this, int block_size)  */
/* { */
/*   int xleft; */
/*   int yleft; */
/*   int xstart; */
/*   int ystart; */

/*   xleft = xsize - (2*NB_NEIGHBOURS); */
/*   yleft = ysize - (2*NB_NEIGHBOURS); */

/*   xstart = NB_NEIGHBOURS; */
/*   ystart = NB_NEIGHBOURS; */
  
  
/*   /\* TODO : if the image isn't a square. *\/ */
/*   if (block_size >= xsize) { */
/*     printf ("Choose a smaller block size please\n"); */
/*     return 0; */
/*   } */
  
/*   do { */
/*     do { */
/* #ifdef BLUR_DEBUG */
/* 	printf ("DEBUG : call to worker\n"); */
/* #endif */
/* 	int minx = min (xleft, block_size); */
/* 	int miny = min (yleft, block_size); */
/* 	CALL (REQUIRED.work, blur, array, out, ysize, xstart, ystart, minx, miny); */

/* 	xstart += block_size; */
/* 	xleft  -= block_size; */
      
/*     } while (xleft > 0); */
    
/*     xleft = xsize - (2*NB_NEIGHBOURS); */
/*     xstart = NB_NEIGHBOURS; */

/*     ystart += block_size; */
/*     yleft  -= block_size; */

/*   } while (yleft > 0); */
    
/*   return 0; */
/* } */
