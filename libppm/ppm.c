#include "ppm.h"

/******************************************************************************/

int ppmb_read_data ( FILE *filein, int xsize, int ysize, int *array )
{
  int   i;
  int   int_val;
  int  *index;
  int   j;
  int   k;
  int   numval;

  index = array;
  numval = 0;

  for ( j = 0; j < ysize; j++ ) {
    for ( i = 0; i < xsize; i++ ) {

      for ( k = 0; k < 3; k++ ) {

        int_val = fgetc ( filein );

        if ( int_val == EOF ) {
          printf ( "\n" );
          printf ( "PPMB_READ_DATA: Failed reading data byte %d.\n", numval );
          return 1;
        }
        else {
	  *index = int_val;
	  index = index + 1;
        }
        numval = numval + 1;
      }
    }
  }
  return 0;
}


/******************************************************************************/

int ppmb_read_header ( FILE *filein, int *xsize, int *ysize, int *maxrgb )
{
  int   c_val;
  int   count;
  int   flag;
  int   nchar;
  int   state;
  char  string[80];

  state = 0;
  nchar = 0;

  for ( ;; ) {

    c_val = fgetc ( filein );

    if ( c_val == EOF ) {
      return 1;
    }
/*
  If not whitespace, add the character to the current string.
*/
    flag = isspace ( c_val );

    if ( !flag ) {
      string[nchar] = c_val;
      nchar = nchar + 1;
    }
/*
  See if we have finished an old item, or begun a new one.
*/
    if ( state == 0 ) {
      if ( !flag ) {
        state = 1;
      }
      else {
        return 1;
      }
    }
    else if ( state == 1 ) {
      if ( flag ) {
        string[nchar] = 0;
        nchar = nchar + 1;
        if ( strcmp ( string, "P6" ) != 0 && strcmp ( string, "p6" ) != 0 ) {
          printf ( "\n" );
          printf ( "PPMB_READ_HEADER: Fatal error.\n" );
          printf ( "  Bad magic number = %s.\n", string );
          return 1;
        }
        nchar = 0;
        state = 2;
      }
    }
    else if ( state == 2 ) {
      if ( !flag ) {
        state = 3;
      }
    }
    else if ( state == 3 ) {
      if ( flag ) {
        string[nchar] = 0;
        nchar = nchar + 1;
        count = sscanf ( string, "%d", xsize );
        if ( count == EOF ) {
          return 1;
        }
        nchar = 0;
        state = 4;
      }
    }
    else if ( state == 4 ) {
      if ( !flag ) {
        state = 5;
      }
    }
    else if ( state == 5 ) {
      if ( flag ) {
        string[nchar] = 0;
        nchar = nchar + 1;
        count = sscanf ( string, "%d", ysize );
        if ( count == EOF ) {
          return 1;
        }
        nchar = 0;
        state = 6;
      }
    }
    else if ( state == 6 ) {
      if ( !flag ) {
        state = 7;
      }
    }
    else if ( state == 7 ) {
      if ( flag ) {
        string[nchar] = 0;
        nchar = nchar + 1;
        count = sscanf ( string, "%d", maxrgb );
        if ( count == EOF ) {
          return 1;
        }
        nchar = 0;
        return 0;
      }
    }
  }
}

/******************************************************************************/

int ppmb_read ( char *filein_name, int *xsize, int *ysize, int *maxrgb, int **array )
{
  FILE *filein;
  int   numbytes;
  int   result;

  filein = fopen ( filein_name, "rb" );

  if ( filein == NULL ) {
    printf ( "\n" );
    printf ( "PPMB_READ: Fatal error!\n" );
    printf ( "  Cannot open the input file %s.\n", filein_name );
    return 1;
  }
/*
  Read the header.
*/
  result = ppmb_read_header ( filein, xsize, ysize, maxrgb );

  if ( result != 0 ) {
    printf ( "\n" );
    printf ( "PPMB_READ: Fatal error!\n" );
    printf ( "  PPMB_READ_HEADER failed.\n" );
    return 1;
  }
/*
  Allocate storage for the data.
*/
  numbytes = 3 * ( *xsize ) * ( *ysize ) * sizeof ( int );

  *array = ( int * ) malloc ( numbytes );

  if ( *array == NULL ) {
    printf ( "\n" );
    printf ( "PPMB_READ: Fatal error!\n" );
    printf ( "  Unable to allocate memory for data.\n" );
    printf ( "  Seeking %d bytes.\n", numbytes );
    return 1;
  }

/*
  Read the data.
*/
  result = ppmb_read_data (filein, *xsize, *ysize, *array);

  if ( result != 0 ) {
    printf ( "\n" );
    printf ( "PPMB_READ: Fatal error!\n" );
    printf ( "  PPMB_READ_DATA failed.\n" );
    return 1;
  }
/*
  Close the file.
*/
  fclose ( filein );

  return 0;
}
/******************************************************************************/

int ppmb_write_header ( FILE *fileout, int xsize, int ysize, int maxrgb )
{
  fprintf ( fileout, "P6 %d %d %d ", xsize, ysize, maxrgb );

  return 0;
}

/******************************************************************************/

int ppmb_write_data ( FILE *fileout, int xsize, int ysize, int *array )
{
  int  i;
  int *index;
  int  j;
  int  k;
  
  index = array;

  for ( j = 0; j < ysize; j++ ) {
    for ( i = 0; i < xsize; i++ ) {
      for ( k = 0; k < 3; k++ ) {
	fputc ( *index, fileout );
	index = index + 1;
      }
    }
  }
  return 0;
}

/******************************************************************************/

int ppmb_write ( char *fileout_name, int xsize, int ysize, int maxrgb, int *array )
{
  FILE *fileout;
  int   result;
/*
  Open the output file.
*/
  fileout = fopen ( fileout_name, "wb" );

  if ( fileout == NULL ) {
    printf ( "\n" );
    printf ( "PPMB_WRITE: Fatal error!\n" );
    printf ( "  Cannot open the output file %s.\n", fileout_name );
    return 1;
  }
/*
  Write the header.
*/
  result = ppmb_write_header ( fileout, xsize, ysize, maxrgb );

  if ( result != 0 ) {
    printf ( "\n" );
    printf ( "PPMB_WRITE: Fatal error!\n" );
    printf ( "  PPMB_WRITE_HEADER failed.\n" );
    return 1;
  }
/*
  Write the data.
*/
  result = ppmb_write_data (fileout, xsize, ysize, array);

  if ( result != 0 ) {
    printf ( "\n" );
    printf ( "PPMB_WRITE: Fatal error!\n" );
    printf ( "  PPMB_WRITE_DATA failed.\n" );
    return 1;
  }
/*
  Close the file.
*/
  fclose ( fileout );

  return 0;
}

/******************************************************************************/

int *ppm_getcell (int *array, int ysize, int x, int y, int k) {
  return (array + (y * 3 * ysize) + (3 * x) + k);
}

/******************************************************************************/

int ppmb_blur (int *array, int* out, int ysize, int xstart, int ystart, int xblock_size, int yblock_size, int neighbours) 
{
  /* Neighbours : */
  int accumulate;
  
  int i;
  int j;
  int k;
  int x, y;
  int *index;
  int total_neighbours = (neighbours * 2) * (neighbours * 2);

#ifdef PPM_DEBUG  
  printf ("Je traite de (%d,%d) à (%d,%d)\n", xstart, ystart,
	  xstart + xblock_size, ystart + yblock_size);
#endif

  /*************/
  /* a   b   c */
  /* d index e */
  /* f   g   h */
  /*************/

  for ( i = xstart ; i < xstart + xblock_size; i++) {
    for ( j = ystart ; j < ystart + yblock_size; j++) {
      for ( k = 0; k < 3; k++) {
	index = ppm_getcell (out, ysize, i  , j  , k);
	accumulate = 0;
	for ( x = i - neighbours; x < i + neighbours; x++) {
	  for ( y = j - neighbours; y < j + neighbours; y++) {
	    accumulate += *(ppm_getcell (array, ysize, x, y, k));
	  }
	}
	*index = accumulate / total_neighbours;
      }
    }
  }
  
  return 0;
}

/******************************************************************************/

int ppm_writecell (char *fileout_name, int *array, int xsize, int ysize, int maxrgb)
{
  int i;
  int j;
  int k;
  FILE *fileout;
  int *index;

  index = NULL;

  fileout = fopen ( fileout_name, "wb" );

  fprintf ( fileout, "P6 %d %d %d ", xsize, ysize, maxrgb );

  for ( j = 0; j < ysize; j++ ) {
    for ( i = 0; i < xsize; i++ ) {
      for ( k = 0; k < 3; k++ ) {
	index = ppm_getcell (array, ysize, i, j, k);
	fputc ( *index, fileout );
      }
    }
  }
  return 0;
}


