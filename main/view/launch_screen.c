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
lv_obj_t *img_launch_bg;
lv_obj_t *img_coffee_logo;
lv_obj_t *img_gradient;
lv_obj_t *text1;
lv_obj_t *text2;
lv_obj_t *text3;
lv_obj_t *loader;
lv_obj_t *heading;

LV_IMG_DECLARE(Launch_bg)
LV_IMG_DECLARE(coffee_launch)
LV_IMG_DECLARE(gradient)
LV_IMG_DECLARE(haloeffect)
LV_IMAGE_DECLARE(loader_gif);

// void create_loading_dots(lv_obj_t * parent);

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

void create_loading_dots(lv_obj_t *parent)
{
    // Container for dots
    lv_obj_t *cont = lv_obj_create(parent);
    // lv_obj_set_size(cont, 3 * DOT_RADIUS * 2 + 2 * DOT_GAP, 2 * DOT_RADIUS + FLOAT_HEIGHT);
    lv_obj_set_size(cont, 100, 50);
    lv_obj_center(cont);
    lv_obj_set_style_bg_opa(cont, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_opa(cont, LV_OPA_TRANSP, 0);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW);
    lv_obj_set_style_pad_row(cont, DOT_GAP, 0);

    // Create 3 dots
    lv_obj_t *dots[3];
    for(int i = 0; i < 3; i++) {
        dots[i] = lv_obj_create(cont);
        lv_obj_set_size(dots[i], DOT_RADIUS*2, DOT_RADIUS*2);
        lv_obj_set_style_radius(dots[i], LV_RADIUS_CIRCLE, 0);
        lv_obj_set_style_bg_color(dots[i], lv_color_hex(0x654321), 0);
        lv_obj_set_style_border_opa(dots[i], LV_OPA_TRANSP, 0);

        // Animate Y (float up/down)
        lv_anim_t a;
        lv_anim_init(&a);
        lv_anim_set_var(&a, dots[i]);
        lv_anim_set_exec_cb(&a, anim_y_cb);
        lv_anim_set_values(&a, 0, -FLOAT_HEIGHT);
        lv_anim_set_time(&a, ANIM_TIME);
        lv_anim_set_playback_time(&a, ANIM_TIME);
        lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
        lv_anim_set_delay(&a, i * ANIM_DELAY); // stagger each dot
        lv_anim_start(&a);
    }
}

void create_launch_screen()
{   
  // init_style();
    scr_home = lv_obj_create(lv_scr_act());
    lv_obj_set_size(scr_home, 1280, 800);
    lv_obj_set_scrollbar_mode(scr_home, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_border_opa(scr_home, LV_OPA_TRANSP, 0);
    // lv_obj_add_style(scr_home, &style_scr_backdrop, LV_PART_MAIN);

    img_launch_bg = lv_img_create(scr_home);
    lv_img_set_src(img_launch_bg, &haloeffect);
    lv_img_set_zoom(img_launch_bg, 256); 
    lv_obj_align(img_launch_bg, LV_ALIGN_CENTER, 0, 0);

    img_coffee_logo = lv_img_create(scr_home);
    lv_img_set_src(img_coffee_logo, &coffee_launch);  
    lv_img_set_zoom(img_coffee_logo, 70); 
    lv_obj_align(img_coffee_logo, LV_ALIGN_CENTER, 0, -160);

    text1 = lv_label_create(scr_home);
    lv_label_set_text(text1, "Premium Coffee Experience");
    lv_obj_set_style_text_color(text1, lv_color_hex(0x737d8b), LV_PART_MAIN);
    lv_obj_set_style_text_font(text1, &lv_font_montserrat_20, LV_PART_MAIN);
    lv_obj_align(text1, LV_ALIGN_CENTER, 0, 170);

    lv_obj_t * lottie = lv_rlottie_create_from_file(scr_home, 200, 200,
                                                    "../main/assets/gifs/loader.json");
    lv_obj_align(lottie, LV_ALIGN_CENTER, 0, 300);


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

// static void select_event_cb(lv_event_t * e)
// {
//     // LV_UNUSED(e);

//     // // if(scr_select == NULL) {
//         create_select_screen();  // create it only once
//     // }

//     lv_screen_load(scr_select);  // load the existing screen
// }
