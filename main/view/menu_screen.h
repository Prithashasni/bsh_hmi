/**
 * menu_screen.h
 *
 * Created on: 22-Oct-2025
 * Author: PSI2COB
 */


#ifndef _MENU_SCREEN_H_
#define _MENU_SCREEN_H_

/* System Includes*/
#include <stdio.h>
#include <time.h>
#include "../../lvgl/lvgl.h"

extern lv_obj_t *scr_menu;
// extern lv_obj_t *img_bosch_logo;
// extern lv_obj_t *img_initializing;

// LV_IMG_DECLARE(bosch_logo)

/* Project Includes */
extern void scr_menu_scroll();
// extern void img_animation(lv_obj_t *img_bosch_logo);
// static void scr_plugged(lv_event_t *event);

#endif