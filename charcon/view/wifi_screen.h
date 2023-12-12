/**
 * wifi_screen.h
 *
 * Created on: 07-Dec-2023
 * Author: PSI2COB
 */

#ifndef _WIFI_SCREEN_H_
#define _WIFI_SCREEN_H_


#include "../../lvgl/lvgl.h"

extern lv_obj_t *back_icon;
extern lv_obj_t *wifi_text;
extern lv_obj_t *network_bg;
extern lv_obj_t *no_wifi_text;
extern lv_obj_t *no_wifi_icon;

LV_IMG_DECLARE(cancel_icon)
LV_IMG_DECLARE(no_wifi)

extern void scr_wifi_page();

#endif