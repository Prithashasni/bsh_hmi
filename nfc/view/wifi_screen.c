#include "wifi_screen.h"
#include "lvgl/lvgl.h"
#include <stdio.h>


static lv_obj_t * label_title;
static lv_obj_t * label_ssid;
static lv_obj_t * label_pw;

void create_wifi_screen()
{
    lv_obj_t * scr = lv_scr_act();

    // Title
    // label_title = lv_label_create(scr);
    // lv_label_set_text(label_title, "WiFi Credentials");
    // lv_obj_set_style_text_font(label_title, &lv_font_montserrat_28, 0);
    // lv_obj_align(label_title, LV_ALIGN_TOP_MID, 0, 20);

    // // SSID
    // label_ssid = lv_label_create(scr);
    // lv_label_set_text(label_ssid, "SSID: -");
    // lv_obj_set_style_text_font(label_ssid, &lv_font_montserrat_22, 0);
    // lv_obj_align(label_ssid, LV_ALIGN_CENTER, 0, -30);

    // // PASSWORD
    // label_pw = lv_label_create(scr);
    // lv_label_set_text(label_pw, "Password: -");
    // lv_obj_set_style_text_font(label_pw, &lv_font_montserrat_22, 0);
    // lv_obj_align(label_pw, LV_ALIGN_CENTER, 0, 20);
}

void update_wifi_screen(const char *ssid, const char *pw)
{
    static char ssid_buf[128];
    static char pw_buf[128];

    snprintf(ssid_buf, sizeof(ssid_buf), "SSID: %s", ssid);
    snprintf(pw_buf, sizeof(pw_buf), "Password: %s", pw);

    lv_label_set_text(label_ssid, ssid_buf);
    lv_label_set_text(label_pw, pw_buf);
}
