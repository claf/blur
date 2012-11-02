/*
 *  blur.h
 *  xkaapi
 *
 *  Created by CL on 05/07/10.
 *  Copyright 2009 INRIA. All rights reserved.
 *
 */

#include <kaapi.h>
#include <stddef.h> // offsetof().
#include "../include/blur_const.h"

/**********************
 * COMPONENTS METHODS *
 **********************/

// the application main entrypoint.
void app_main_body (void* taskarg, kaapi_thread_t* thread);
// blur method from Work interface.
void blur_body     (void* taskarg, kaapi_thread_t* thread);
// signal meth from Collect interf.
void signal_body   (void* taskarg, kaapi_thread_t* thread);
// set_info m. from Collect interf.
void set_info_body (void* taskarg, kaapi_thread_t* thread);

/************************
 * ARGUMENTS STRUCTURES *
 ************************/

/* app_main argument structure : */

typedef struct app_main_arg_t {
  //void *_this;
  int argc;
  char** argv;
} app_main_arg_t;

/* blur argument structure : */

typedef struct blur_arg_t {
  //void *_this;
  void *array;
  void *out;
  int  ysize;
  int  xstart;
  int  ystart;
  int  xblock_size;
  int  yblock_size;
} blur_arg_t;

/* signal argument structure : */

typedef struct signal_arg_t {
  //void* _this;
} signal_arg_t;

/* set_info argument structure : */

typedef struct set_info_arg_t {
  //void *_this;
  char *fileout_name;
  int  xsize;
  int  ysize;
  int  maxrgb;
  void *array;
  int nb_block;
} set_info_arg_t;

/*********************
 * TASKS DECLARATION *
 *********************/

/* app_main task decl. : */

KAAPI_REGISTER_TASKFORMAT ( app_main_format,
			    "app_main",
			    app_main_body,
			    sizeof (app_main_arg_t),
			    2,
			    (kaapi_access_mode_t[]) {KAAPI_ACCESS_MODE_V, KAAPI_ACCESS_MODE_V},
			    (kaapi_offset_t[]) {offsetof(app_main_arg_t, argc.data), offsetof(app_main_arg_t, argv.data)},
			    (kaapi_offset_t[]) {offsetof(app_main_arg_t, argc.version), offsetof(app_main_arg_t, argv.version)},
			    (const struct kaapi_format_t*[]) {kaapi_int_format, kaapi_ulong_format},
          0)

/* blur task decl. : */

KAAPI_REGISTER_TASKFORMAT ( blur_format,
			    "blur",
			    blur_body,
			    sizeof (blur_arg_t),
			    7,
			    (kaapi_access_mode_t[]) {KAAPI_ACCESS_MODE_V, KAAPI_ACCESS_MODE_V, KAAPI_ACCESS_MODE_V, KAAPI_ACCESS_MODE_V, KAAPI_ACCESS_MODE_V, KAAPI_ACCESS_MODE_V, KAAPI_ACCESS_MODE_V},
			    (kaapi_offset_t[]) {offsetof(blur_arg_t, array.data), offsetof(blur_arg_t, out.data), offsetof(blur_arg_t, ysize.data), offsetof(blur_arg_t, xstart.data), offsetof(blur_arg_t, ystart.data), offsetof(blur_arg_t, xblock_size.data), offsetof(blur_arg_t,yblock_size.data)},
			    (kaapi_offset_t[]) {offsetof(blur_arg_t, array.version), offsetof(blur_arg_t, out.version), offsetof(blur_arg_t, ysize.version), offsetof(blur_arg_t, xstart.version), offsetof(blur_arg_t, ystart.version), offsetof(blur_arg_t, xblock_size.version), offsetof(blur_arg_t,yblock_size.version)},
			    (const struct kaapi_format_t*[]) {kaapi_ulong_format, kaapi_ulong_format, kaapi_int_format, kaapi_int_format, kaapi_int_format, kaapi_int_format, kaapi_int_format},
          0)

/* signal task decl. : */

KAAPI_REGISTER_TASKFORMAT ( signal_format,
			    "signal",
			    signal_body,
			    sizeof (signal_arg_t),
			    0,
			    (kaapi_access_mode_t[]) {},
			    (kaapi_offset_t[]) {},
			    (const struct kaapi_format_t*[]) {},
          0)

/* set_info task decl. : */

KAAPI_REGISTER_TASKFORMAT ( set_info_format,
			    "set_info",
			    set_info_body,
			    sizeof (set_info_arg_t),
			    6,
			    (kaapi_access_mode_t[]) {KAAPI_ACCESS_MODE_V, KAAPI_ACCESS_MODE_V, KAAPI_ACCESS_MODE_V, KAAPI_ACCESS_MODE_V, KAAPI_ACCESS_MODE_V, KAAPI_ACCESS_MODE_V},
			    (kaapi_offset_t[]) {offsetof(set_info_arg_t, fileout_name.data), offsetof(set_info_arg_t, xsize.data), offsetof(set_info_arg_t, ysize.data), offsetof(set_info_arg_t, maxrgb.data), offsetof(set_info_arg_t, array.data), offsetof(set_info_arg_t, nb_block.data)},
			    (kaapi_offset_t[]) {offsetof(set_info_arg_t, fileout_name.version), offsetof(set_info_arg_t, xsize.version), offsetof(set_info_arg_t, ysize.version), offsetof(set_info_arg_t, maxrgb.version), offsetof(set_info_arg_t, array.version), offsetof(set_info_arg_t, nb_block.version)},
			    (const struct kaapi_format_t*[]) {kaapi_ulong_format, kaapi_int_format, kaapi_int_format, kaapi_int_format, kaapi_ulong_format, kaapi_int_format},
          0)
