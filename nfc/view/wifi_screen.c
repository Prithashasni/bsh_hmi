#include "wifi_screen.h"
#include "lvgl/lvgl.h"
#include <stdio.h>
#include "colors.h"
#include "select_screen.h"


static lv_obj_t * label_title;
static lv_obj_t * label_ssid;
static lv_obj_t * label_pw;
static lv_obj_t * scr_wifi;

void create_wifi_screen()
{
    // lv_obj_t * scr = lv_screen_active();

    scr_wifi = lv_obj_create(lv_scr_act());
    lv_obj_set_size(scr_wifi, 900, 450);
    lv_obj_set_scrollbar_mode(scr_wifi, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_border_opa(scr_wifi, LV_OPA_TRANSP, 0);
    lv_obj_set_style_bg_color(scr_wifi, LV_COLOR_LIGHT_GREY, 0);
    lv_obj_set_style_bg_opa(scr_wifi, LV_OPA_50, 0);
    lv_obj_set_align(scr_wifi, LV_ALIGN_CENTER);

    lv_obj_t *connect = lv_label_create(scr_wifi);
    lv_label_set_text(connect, "Connecting to WiFi...");
    lv_obj_set_style_text_font(connect, &lv_font_montserrat_28, 0);
    lv_obj_align(connect, LV_ALIGN_CENTER, 0, 0);

    // lv_obj_t * lottie = lv_rlottie_create_from_file(scr_wifi, 350, 350,
    //                                                 "../nfc/assets/gifs/wifi_connecting.json");
    // lv_obj_align(lottie, LV_ALIGN_CENTER, 0, 0);

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
