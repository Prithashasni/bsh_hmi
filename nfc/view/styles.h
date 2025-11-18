/**
 * styles.h
 *
 * Created on: 28-Aug-2023
 * Author: PSI2COB
 */

#include "../../lvgl/lvgl.h"

#ifndef _STYLES_H_
#define _STYLES_H_

/** Typedef **/
typedef struct
{
  int elem_x_pos;
  int elem_y_pos;
  int elem_width;
  int elem_height;
} element_attribs;


extern lv_style_t style_scr_backdrop;
extern lv_style_t style_scr_menu;

extern void init_style();
extern void init_style_backdrop();

#endif