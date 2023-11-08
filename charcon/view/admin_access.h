/**
 * admin_access.h
 *
 * Created on: 03-Oct-2023
 * Author: PSI2COB
 */

#ifndef _ADMIN_ACCESS_H_
#define _ADMIN_ACCESS_H_

/* System Includes*/
#include <stdio.h>
#include <time.h>
#include "../../lvgl/lvgl.h"


extern lv_obj_t *scr_cloud;
extern lv_obj_t *scr_vsecc;
extern lv_obj_t *scr_charcon;
extern lv_obj_t *scr_master;
extern lv_obj_t *scr_logs;
extern lv_obj_t *img_log;
extern lv_obj_t *log_text;
extern lv_obj_t *logs_page;
extern lv_obj_t *underline;

LV_IMG_DECLARE(back_button)

extern void scr_software_update();
extern void scr_vsecc_settings();
extern void scr_charcon_settings();
extern void scr_master_control();
extern void scr_logs_page();
static void ta_handler(lv_event_t * e);
static void btnm_event_handler(lv_event_t * e);
static void ta_vol_handler(lv_event_t * e);
static void btnm_event1_handler(lv_event_t * e);
extern void add_log();
extern void print_next_line(lv_timer_t *timer);
extern void ta_rect_cb();
extern void ta1_rect_cb();

#endif