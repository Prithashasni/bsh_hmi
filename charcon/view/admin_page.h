/**
 * admin_page.h
 *
 * Created on: 21-Sep-2023
 * Author: PSI2COB
 */

#ifndef _ADMIN_PAGE_H_
#define _ADMIN_PAGE_H_

/* System Includes*/
#include <stdio.h>
#include <time.h>
#include "../../lvgl/lvgl.h"

extern lv_obj_t *prev;
extern lv_obj_t *scr_admin;
extern lv_obj_t *scr_default;
extern lv_obj_t *text_ta;
extern lv_obj_t *keyboard_area;
extern lv_obj_t *cancel_button;
extern lv_obj_t *cancel_text;

extern void admin_login_pages();
static void ta_event_cb(lv_event_t * e);
extern void prev_screen_cb();
extern void home_screen();

#endif