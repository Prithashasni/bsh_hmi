/**
 * launch_screen.c
 *
 * Created on: 28-Aug-2023
 * Author: PSI2COB
 */

#include "../../lvgl/lvgl.h"
#include "launch_screen.h"
#include "health_check.h"
#include "styles.h"
#include "device_detect.h"
#include "allpage_icons.h"
#include "admin_page.h"

lv_obj_t *scr_home;
lv_obj_t *img_bosch_logo;

void create_launch_screen()
{   
    scr_home = lv_obj_create(lv_scr_act());
    lv_obj_set_size(scr_home, 1280, 800);
    lv_obj_set_scrollbar_mode(scr_home, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_style(scr_home, &style_scr_backdrop, LV_STATE_DEFAULT);
    // lv_obj_add_event_cb(scr_home, scr_plugged, LV_EVENT_CLICKED, NULL);

    img_bosch_logo = lv_img_create(scr_home);
    lv_img_set_src(img_bosch_logo, &bosch_logo);
    lv_obj_align(img_bosch_logo, LV_ALIGN_CENTER, 0, 0);

    img_animation(img_bosch_logo);
}

void img_animation(lv_obj_t *img_bosch_logo)
{
  lv_anim_t img;
  lv_anim_init(&img);
  lv_anim_set_exec_cb(&img, (lv_anim_exec_xcb_t) lv_img_set_zoom);
  lv_anim_set_var(&img, img_bosch_logo);
  lv_anim_set_time(&img, 250);
  lv_anim_set_delay(&img, 100);
  lv_anim_set_values(&img, 0, 250);
  lv_anim_start(&img);
}

