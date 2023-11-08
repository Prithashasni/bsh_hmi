/**
 * allpage_icons.h
 *
 * Created on: 11-Sep-2023
 * Author: PSI2COB
 */

#ifndef _ALLPAGE_ICONS_H_
#define _ALLPAGE_ICONS_H_

/* System Includes*/
#include <stdio.h>
#include <time.h>
#include "../../lvgl/lvgl.h"

extern lv_obj_t *img_charger;
extern lv_obj_t *label_plug;
extern lv_obj_t *img_plug;
extern lv_obj_t *label_guest;
extern lv_obj_t *img_cloud;
extern lv_obj_t *sw_text;
extern lv_obj_t *img_vsecc;
extern lv_obj_t *img_health;
extern lv_obj_t *img_charcon;
extern lv_obj_t *img_mc;
extern lv_obj_t *img_log;
extern lv_obj_t *log_text;

LV_IMG_DECLARE(charger)
LV_IMG_DECLARE(footer_logo)
LV_IMG_DECLARE(icon_not_plug)
LV_IMG_DECLARE(icon_connect)
LV_IMG_DECLARE(icon_charging)
LV_IMG_DECLARE(icon_profile)
LV_IMG_DECLARE(icon_wifi)
LV_IMG_DECLARE(icon_network)
LV_IMG_DECLARE(icon_location)
LV_IMG_DECLARE(icon_good)
LV_IMG_DECLARE(red_failure)
LV_IMG_DECLARE(white_confg)
LV_IMG_DECLARE(settings_icon)
LV_IMG_DECLARE(cloud_icon)
LV_IMG_DECLARE(home_icon)
LV_IMG_DECLARE(log_icon)
LV_IMG_DECLARE(vsecc_icon)
LV_IMG_DECLARE(charcon_icon)
LV_IMG_DECLARE(icon_connect_error)
LV_IMG_DECLARE(cloud_failure)
LV_IMG_DECLARE(cloud_connect)
LV_IMG_DECLARE(arrow_up)
LV_IMG_DECLARE(img_signal)
LV_IMG_DECLARE(img_network_1)
LV_IMG_DECLARE(img_network_2)
LV_IMG_DECLARE(img_network_3)
LV_IMG_DECLARE(img_network_4)
LV_IMG_DECLARE(img_network_5)

extern char admin_text[50];
extern const int CONST_AdminLoginPage;
extern const int CONST_SoftwareUpdate;
extern const int CONST_VseccSettings;
extern const int CONST_CharconSettings;
extern const int CONST_MasterControl;
extern const int CONST_AdminLogsPage;

/* Project Includes */
extern void allpage_status(lv_obj_t *obj);
extern void charger_plug_status(int PlugStatus, lv_obj_t *obj);
extern void health_icon_status(int HealthIcon, lv_obj_t *obj);
extern void cloud_connection_status(int CloudConnection, lv_obj_t *obj);
extern void header_icons_display();
extern void scr_login();
extern void header_icons_close();
extern void scr_sw_update();
extern void scr_vsecc_set();
extern void scr_charcon_set();
extern void scr_mc_set();
extern void scr_log_set();
extern void img_recolor(lv_obj_t *img, lv_obj_t *text);
extern void health_check_status(lv_obj_t *obj);
extern void get_current_datetime(char *datetime_str);
extern int get_position();
extern void get_gsm_signal();
extern void parse_json(const char *json);
#endif