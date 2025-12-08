/**
 * launch_screen.c
 *
 * Created on: 22-Oct-2025
 * Author: PSI2COB
 */

#include "../../lvgl/lvgl.h"
#include "launch_screen.h"
#include "menu_screen.h"
#include "select_screen.h"
#include "styles.h"

lv_obj_t *scr_home;
lv_obj_t *img_bg;
lv_obj_t *img_coffee_logo;
lv_obj_t *img_gradient;
lv_obj_t *text1;
lv_obj_t *text2;
lv_obj_t *text3;
lv_obj_t *loader;
lv_obj_t *heading;

void create_floating_dots(lv_obj_t *parent);
static void select_event_cb(lv_event_t * e);

#define DOT_RADIUS      8
#define DOT_GAP         20
#define FLOAT_HEIGHT    10
#define ANIM_TIME       500
#define ANIM_DELAY      150

static void anim_y_cb(void * obj, int32_t v)
{
    lv_obj_set_y(obj, v);
}

void create_launch_screen()
{   
  // init_style();
    scr_home = lv_obj_create(lv_scr_act());
    lv_obj_set_size(scr_home, 1024, 600);
    lv_obj_set_scrollbar_mode(scr_home, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_border_opa(scr_home, LV_OPA_TRANSP, 0);
    // lv_obj_add_style(scr_home, &style_scr_backdrop, LV_PART_MAIN);

    img_bg = lv_img_create(scr_home);
    lv_img_set_src(img_bg, "/usr/share/nfc/assets/pngs/haloeffect.png");
    lv_img_set_zoom(img_bg, 256); 
    lv_obj_align(img_bg, LV_ALIGN_CENTER, 0, 0);

    img_coffee_logo = lv_img_create(scr_home);
    lv_img_set_src(img_coffee_logo, "/usr/share/nfc/assets/pngs/coffee_launch.png");  
    lv_img_set_zoom(img_coffee_logo, 120); 
    lv_obj_align(img_coffee_logo, LV_ALIGN_CENTER, 0, -160);

    text1 = lv_label_create(scr_home);
    lv_label_set_text(text1, "Premium Coffee Experience");
    lv_obj_set_style_text_color(text1, lv_color_hex(0x737d8b), LV_PART_MAIN);
    lv_obj_set_style_text_font(text1, &lv_font_montserrat_20, LV_PART_MAIN);
    lv_obj_align(text1, LV_ALIGN_CENTER, 0, 170);

    // lv_obj_t * lottie = lv_rlottie_create_from_file(scr_home, 200, 200,
    //                                                 "../nfc/assets/gifs/loader.json");
    // lv_obj_align(lottie, LV_ALIGN_CENTER, 0, 250);


    text2 = lv_label_create(scr_home);
    lv_label_set_text(text2, "Touch to continue");
    lv_obj_set_style_text_color(text2, lv_color_hex(0x737d8b), LV_PART_MAIN);
    lv_obj_set_style_text_font(text2, &lv_font_montserrat_18, LV_PART_MAIN);
    lv_obj_align(text2, LV_ALIGN_CENTER, 0, 340);

    heading = lv_label_create(scr_home);
    lv_label_set_text(heading, "COFFEE MACHINE");
    lv_obj_set_style_text_color(heading, lv_color_hex(0xfdfcfb), LV_PART_MAIN);
    lv_obj_set_style_text_font(heading, &lv_font_montserrat_38, LV_PART_MAIN);
    lv_obj_align(heading, LV_ALIGN_CENTER, 0, -30);

    text3 = lv_label_create(scr_home);
    lv_label_set_text(text3, "EQ.900");
    lv_obj_set_style_text_color(text3, lv_color_hex(0xfdfcfb), LV_PART_MAIN);
    lv_obj_set_style_text_font(text3, &lv_font_montserrat_38, LV_PART_MAIN);
    lv_obj_align(text3, LV_ALIGN_CENTER, 0, 80);

    lv_obj_add_event_cb(scr_home, create_select_screen, LV_EVENT_CLICKED, NULL);

}
