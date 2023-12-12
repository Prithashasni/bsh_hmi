/**
 * wifi_screen.c
 *
 * Created on: 07-Dec-2023
 * Author: PSI2COB
 */

#include "../../lvgl/lvgl.h"
#include "health_check.h"
#include "styles.h"
#include "launch_screen.h"
#include "progress_screen.h"
#include "colors.h"
#include "allpage_icons.h"
#include "allpage_signals.h"
#include "device_detect.h"
#include "admin_page.h"
#include "admin_access.h"
#include "wifi_screen.h"

lv_obj_t *back_icon;
lv_obj_t *wifi_text;
lv_obj_t *network_bg;
lv_obj_t *no_wifi_text;
lv_obj_t *no_wifi_icon;

void scr_wifi_page()
{
  display_allpage_icons();
  lv_obj_add_flag(img_charger, LV_OBJ_FLAG_HIDDEN);

  back_icon = lv_img_create(scr_home);
  lv_img_set_src(back_icon, &cancel_icon);
  lv_img_set_zoom(back_icon, 180);
  lv_obj_align(back_icon, LV_ALIGN_TOP_LEFT, 25, 88);
  lv_obj_add_flag(back_icon, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_add_event_cb(back_icon, prev_screen_cb, LV_EVENT_CLICKED, NULL);

  wifi_text = lv_label_create(scr_home);
  lv_label_set_text(wifi_text, "Wi-Fi");
  lv_obj_align(wifi_text, LV_ALIGN_TOP_LEFT, 90, 95);
  lv_obj_set_style_text_color(wifi_text, LV_COLOR_WHITE, LV_STATE_DEFAULT);
  lv_obj_set_style_text_font(wifi_text, &lv_font_montserrat_34, LV_STATE_DEFAULT);

  if (Wifisignal < 5)
  {
    network_bg = lv_obj_create(scr_home);
    lv_obj_set_size(network_bg, 800, 500);
    lv_obj_align(network_bg, LV_ALIGN_TOP_LEFT, 30, 150);
    lv_obj_set_style_bg_color(network_bg, LV_COLOR_DARK_GREY, LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(network_bg, LV_COLOR_DARK_GREY, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(network_bg, LV_OPA_90, LV_STATE_DEFAULT);
  }
  else
  {
    no_wifi_icon = lv_img_create(scr_home);
    lv_img_set_src(no_wifi_icon, &no_wifi);
    lv_img_set_zoom(no_wifi_icon, 230);
    // lv_obj_set_style_bg_img_opa(no_wifi_icon, LV_OPA_80, LV_STATE_DEFAULT);
    lv_obj_align(no_wifi_icon, LV_ALIGN_CENTER, 0, 70);

    no_wifi_text = lv_label_create(scr_home);
    lv_label_set_text(no_wifi_text, "No Wi-Fi networks");
    lv_obj_align(no_wifi_text, LV_ALIGN_CENTER, 0, 130);
    lv_obj_set_style_text_color(no_wifi_text, lv_color_hex(0x6c6c6c), LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(no_wifi_text, &lv_font_montserrat_28, LV_STATE_DEFAULT);
  }
}