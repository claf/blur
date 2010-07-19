#include "../include/blur_const.h"

void blur (void *array, int ysize, int xstart, int ystart, int xblock_size, int yblock_size);

void signal ();

void set_info (char *fileout_name, int xsize, int ysize, int maxrgb, void *array, int nb_block);
