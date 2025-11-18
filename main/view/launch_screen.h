/**
 * launch_screen.h
 *
 * Created on: 22-Oct-2025
 * Author: PSI2COB
 */


#ifndef _LAUNCH_SCREEN_H_
#define _LAUNCH_SCREEN_H_

/* System Includes*/
#include <stdio.h>
#include <time.h>
#include "../../lvgl/lvgl.h"

extern lv_obj_t *scr_home;
extern lv_obj_t *scr_home;
extern lv_obj_t *img_launch_bg;
extern lv_obj_t *img_coffee_logo;
extern lv_obj_t *img_gradient;
extern lv_obj_t *text1;
extern lv_obj_t *text2;
extern lv_obj_t *text3;
extern lv_obj_t *heading;

/* Project Includes */
extern void create_launch_screen();

#endif