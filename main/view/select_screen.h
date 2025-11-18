/**
 * launch_screen.h
 *
 * Created on: 22-Oct-2025
 * Author: PSI2COB
 */


#ifndef _SELECT_SCREEN_H_
#define _SELECT_SCREEN_H_

/* System Includes*/
#include <stdio.h>
#include <time.h>
#include "../../lvgl/lvgl.h"

extern lv_obj_t *scr_select;
extern lv_obj_t *img_wifi_icon;
extern lv_obj_t *img_home_icon;    
extern lv_obj_t *img_cup_icon;
extern lv_obj_t *carousel_cont;
extern lv_obj_t *home_cont;
extern lv_obj_t *img_select_bg;

/* Project Includes */
extern void create_select_screen();

#endif