/**
 * select_screen.c
 *
 * Created on: 22-Oct-2025
 * Author: PSI2COB
 */

#include "../../lvgl/lvgl.h"
#include "launch_screen.h"
#include "menu_screen.h"
#include "styles.h"
#include "../../lvgl/src/core/lv_obj.h" 

LV_IMG_DECLARE(icon_wifi)
LV_IMG_DECLARE(icon_home)
LV_IMG_DECLARE(icon_cup)
LV_IMG_DECLARE(img_cappuccino)
LV_IMG_DECLARE(img_espresso)
LV_IMG_DECLARE(img_macchiato)
LV_IMG_DECLARE(img_icedcoffee)
LV_IMG_DECLARE(img_latte)
LV_IMG_DECLARE(img_americano)
LV_IMG_DECLARE(img_flatwhite)
LV_IMG_DECLARE(img_milk)
LV_IMG_DECLARE(haloeffect)

static void scroll_event_cb(lv_event_t * e);
static void home_event_cb(lv_event_t * e);
static void item_select_event_cb(lv_event_t * e);
void anim_rotate_cb(void * var, int32_t v);

lv_obj_t *scr_select;
lv_obj_t *img_wifi_icon;
lv_obj_t *img_home_icon;    
lv_obj_t *img_cup_icon;
lv_obj_t *carousel_cont;
lv_obj_t *home_cont;
lv_obj_t *img_select_bg;
lv_obj_t *select_label;

static const char *bev_names[] = {
    "Cappuccino",
    "Espresso",
    "Macchiato",
    "Iced Coffee",
    "Cafe Latte",
    "Americano",
    "Flat White",
    "Milk"
};

static const lv_img_dsc_t *bev_imgs[] = {
    &img_cappuccino,
    &img_espresso,
    &img_macchiato,
    &img_icedcoffee,
    &img_latte,
    &img_americano,
    &img_flatwhite,
    &img_milk
};

/* Scroll + Zoom & Opacity Event Callback */
static void scroll_event_cb(lv_event_t * e)
{
    lv_obj_t * cont = lv_event_get_target(e);
    lv_coord_t scroll_x = lv_obj_get_scroll_x(cont);

    uint32_t btn_count = lv_obj_get_child_count(cont) / 3; // original set count
    lv_coord_t gap = 20; // same gap as used when adding items
    lv_coord_t btn_w = lv_obj_get_width(lv_obj_get_child(cont, 0));
    lv_coord_t single_set_w = (btn_w + gap) * btn_count - gap;

    // === Looping logic ===
    if(scroll_x >= 2 * single_set_w) {
        lv_obj_scroll_to_x(cont, scroll_x - single_set_w, LV_ANIM_OFF);
        scroll_x -= single_set_w;
    } 
    else if(scroll_x <= 0) {
        lv_obj_scroll_to_x(cont, scroll_x + single_set_w, LV_ANIM_OFF);
        scroll_x += single_set_w;
    }

    // === Zoom & opacity logic ===
    lv_coord_t cont_w = lv_obj_get_width(cont);
    lv_coord_t cont_center_x = cont_w / 2;

    for(uint32_t i = 0; i < btn_count * 3; i++) {
        lv_obj_t * item = lv_obj_get_child(cont, i);
        lv_coord_t item_x = lv_obj_get_x(item); 
        lv_coord_t item_center = item_x + btn_w / 2 - scroll_x;

        int32_t diff_x = LV_ABS(item_center - cont_center_x);

        // Zoom effect
        int32_t zoom = 256 - lv_map(diff_x, 0, 200, 0, 128);
        if(zoom < 128) zoom = 128;
        if(zoom > 256) zoom = 256;

        lv_obj_t * img = lv_obj_get_child(item, 0);
        lv_img_set_zoom(img, zoom);

        // Opacity effect
        lv_opa_t opa = LV_OPA_COVER - lv_map(diff_x, 0, 200, 0, LV_OPA_COVER - LV_OPA_50);
        lv_obj_set_style_opa(item, opa, 0);
    }
}

/* Create beverage carousel */
void create_select_screen()
{
    scr_select = lv_obj_create(lv_scr_act());
    lv_obj_set_size(scr_select, 1280, 800);
    lv_obj_set_scrollbar_mode(scr_select, LV_SCROLLBAR_MODE_OFF);
    lv_obj_clear_flag(scr_select, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_border_opa(scr_select, LV_OPA_TRANSP, 0);

    img_select_bg = lv_img_create(scr_select);
    lv_img_set_src(img_select_bg, &haloeffect);
    lv_img_set_zoom(img_select_bg, 256); 
    lv_obj_align(img_select_bg, LV_ALIGN_CENTER, 0, 0);

    img_wifi_icon = lv_img_create(scr_select);
    lv_img_set_src(img_wifi_icon, &icon_wifi);
    lv_img_set_zoom(img_wifi_icon, 150);
    lv_obj_align(img_wifi_icon, LV_ALIGN_TOP_RIGHT, -80, -15);

    home_cont = lv_obj_create(scr_select);
    lv_obj_set_size(home_cont, 50, 50);
    lv_obj_set_style_bg_opa(home_cont, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_opa(home_cont, LV_OPA_TRANSP, 0);
    lv_obj_set_scrollbar_mode(home_cont, LV_SCROLLBAR_MODE_OFF);
    lv_obj_align(home_cont, LV_ALIGN_TOP_RIGHT, -10, -5);
    lv_obj_add_event_cb(home_cont, home_event_cb, LV_EVENT_PRESSED, NULL);

    img_home_icon = lv_img_create(home_cont);
    lv_img_set_src(img_home_icon, &icon_home);
    lv_img_set_zoom(img_home_icon, 400);
    lv_obj_align(img_home_icon, LV_ALIGN_CENTER, 0, 0);

    img_cup_icon = lv_img_create(scr_select);
    lv_img_set_src(img_cup_icon, &icon_cup);
    lv_img_set_zoom(img_cup_icon, 256);
    lv_obj_align(img_cup_icon, LV_ALIGN_TOP_LEFT, 10, 0);

    select_label = lv_label_create(scr_select);
    lv_label_set_text(select_label, "Select Your Beverage");
    lv_obj_set_style_text_font(select_label, &lv_font_montserrat_32, 0);
    lv_obj_set_style_text_color(select_label, lv_color_hex(0xFFFFFF), 0);
    lv_obj_align(select_label, LV_ALIGN_TOP_MID, 0, 10);

    /* Create carousel container */
    carousel_cont = lv_obj_create(scr_select);
    lv_obj_set_size(carousel_cont, 1280, 400);
    lv_obj_center(carousel_cont);
    lv_obj_set_style_bg_opa(carousel_cont, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_opa(carousel_cont, LV_OPA_TRANSP, 0);
    lv_obj_set_flex_flow(carousel_cont, LV_FLEX_FLOW_ROW);
    lv_obj_set_scroll_dir(carousel_cont, LV_DIR_HOR);
    lv_obj_set_scroll_snap_x(carousel_cont, LV_SCROLL_SNAP_START);
    lv_obj_set_scrollbar_mode(carousel_cont, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_event_cb(carousel_cont, scroll_event_cb, LV_EVENT_SCROLL, NULL);

    uint32_t original_count = 8;
    lv_coord_t gap = 20;
    lv_coord_t btn_w = 250; // same as item width

    for(int set = 0; set < 3; set++) {
        for(int i = 0; i < original_count; i++) {
            lv_obj_t * item = lv_obj_create(carousel_cont);
            lv_obj_clear_flag(item, LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_size(item, btn_w, 340); 
            lv_obj_set_style_bg_opa(item, LV_OPA_TRANSP, 0);
            lv_obj_set_style_border_opa(item, LV_OPA_TRANSP, 0);
            lv_obj_set_scrollbar_mode(item, LV_SCROLLBAR_MODE_OFF);

            lv_obj_add_event_cb(item, item_select_event_cb, LV_EVENT_CLICKED, NULL);
            lv_obj_add_flag(item, LV_OBJ_FLAG_CLICKABLE);

            lv_obj_t * img = lv_img_create(item);
            lv_img_set_src(img, bev_imgs[i]);
            lv_img_set_zoom(img, 250);
            lv_obj_align(img, LV_ALIGN_CENTER, 0, -30);  

            // lv_obj_add_event_cb(img, item_select_event_cb, LV_EVENT_CLICKED, NULL);
            // lv_obj_add_flag(img, LV_OBJ_FLAG_CLICKABLE);

            lv_obj_t * label = lv_label_create(item);
            lv_label_set_text(label, bev_names[i]);
            lv_obj_set_style_text_font(label, &lv_font_montserrat_22, 0);
            lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), 0);
            // lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
            lv_obj_set_width(label, btn_w);
            lv_obj_align(label, LV_ALIGN_CENTER, 60, 110); 
            // lv_obj_align_to(label, item, LV_ALIGN_OUT_BOTTOM_MID, 0, 100); 
        }
    }

    /* Initialize carousel position to middle set */
    lv_obj_scroll_to_x(carousel_cont, (btn_w + gap) * original_count, LV_ANIM_OFF);
    lv_obj_send_event(carousel_cont, LV_EVENT_SCROLL, NULL);
}

/* Zoom and rotate animation when image is selected */
static void item_select_event_cb(lv_event_t * e)
{
    lv_obj_t * item = lv_event_get_target(e);

    // Get image child (assumes first child is the image)
    lv_obj_t * img = lv_obj_get_child(item, 0);

    // --- 1. Zoom animation ---
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, img);
    lv_anim_set_values(&a, lv_img_get_zoom(img), lv_img_get_zoom(img) + 50);
    lv_anim_set_time(&a, 150);
    lv_anim_set_playback_time(&a, 150);
    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t) lv_img_set_zoom);
    lv_anim_start(&a);

    //  lv_img_set_pivot(img,
    //     lv_obj_get_width(img) / 2,
    //     lv_obj_get_height(img) / 2);

    // --- Smooth rotation animation ---
    lv_anim_t rot;
    lv_anim_init(&rot);
    lv_anim_set_var(&rot, img);
    lv_anim_set_time(&rot, 1000);
    lv_anim_set_values(&rot, 0, 3600);  
    lv_anim_set_repeat_count(&rot, 1);
    lv_anim_set_path_cb(&rot, lv_anim_path_ease_in_out);
    lv_anim_set_exec_cb(&rot, anim_rotate_cb);
    lv_anim_start(&rot);

}

void anim_rotate_cb(void * var, int32_t v)
{
    lv_img_set_angle(var, v);
}

/* Home button event */
static void home_event_cb(lv_event_t * e)
{
    create_launch_screen();
    lv_screen_load(scr_home); 
}
