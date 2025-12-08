/**
 * menu_screen.h
 *
 * Created on: 22-Oct-2025
 * Author: PSI2COB
 */


#include "../../lvgl/lvgl.h"
#include "launch_screen.h"
#include "menu_screen.h"
#include "styles.h"
#include "colors.h"
#include <math.h>
#include "wifi_screen.h"
// #include "lv_canvas.h"

lv_obj_t *scr_menu;
lv_obj_t *scroll_cont;
lv_obj_t *menu_bar;
lv_obj_t *img_launch_bg;

static const uint32_t btn_count = 4;   
static lv_obj_t *btns[4];            

static lv_style_t style_btn1, style_btn2, style_btn3, style_btn4;

void init_button_styles(void) {
    lv_style_init(&style_btn1);
    lv_style_set_bg_color(&style_btn1, LV_COLOR_GREY);
    lv_style_set_border_color(&style_btn1, lv_color_hex(0x474746));
    lv_style_set_radius(&style_btn1, 10);

    lv_style_init(&style_btn2);
    lv_style_set_bg_color(&style_btn2, lv_color_hex(0x402722));
    lv_style_set_border_color(&style_btn2, lv_color_hex(0x64504a));
    lv_style_set_border_width(&style_btn2, 2);
    lv_style_set_radius(&style_btn2, 10);

    lv_style_init(&style_btn3);
    lv_style_set_bg_color(&style_btn3, LV_COLOR_GREY);
    lv_style_set_border_color(&style_btn3, lv_color_hex(0x474746));
    lv_style_set_radius(&style_btn3, 10);

    lv_style_init(&style_btn4);
    lv_style_set_bg_color(&style_btn4, LV_COLOR_GREY);
    lv_style_set_border_color(&style_btn4, lv_color_hex(0x474746));
    lv_style_set_radius(&style_btn4, 10);
}

static void scroll_loop_event_cb(lv_event_t * e)
{
    lv_coord_t scroll_x = lv_obj_get_scroll_x(scroll_cont);

    // width of one set of buttons (including gaps)
    lv_coord_t btn_w = lv_obj_get_width(btns[0]);
    lv_coord_t gap   = 20;
    lv_coord_t single_set_w = (btn_w + gap) * btn_count - gap;

    // loop scroll
    if(scroll_x >= single_set_w) {
        lv_obj_scroll_to_x(scroll_cont, scroll_x - single_set_w, LV_ANIM_OFF);
    } 
    else if(scroll_x < 0) {
        lv_obj_scroll_to_x(scroll_cont, scroll_x + single_set_w, LV_ANIM_OFF);
    }
}

void scr_menu_scroll()
{
    init_button_styles();

    lv_style_t *btn_styles[4] = { &style_btn1, &style_btn2, &style_btn3, &style_btn4 };
    // const lv_img_dsc_t *imgs[4] = { &img_chat, &img_wifi , &img_feature, &img_solution };
     
    const char *imgs[4] = {
    "A:../nfc/assets/pngs/img_chat.png",
    "A:../nfc/assets/pngs/img_wifi.png",
    "A:../nfc/assets/pngs/img_feature.png",
    "A:../nfc/assets/pngs/img_solution.png"
};

    scr_menu = lv_obj_create(lv_scr_act());
    lv_obj_set_size(scr_menu, 1024, 600);
    lv_obj_set_scrollbar_mode(scr_menu, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_border_opa(scr_menu, LV_OPA_TRANSP, 0);
    lv_obj_clear_flag(scr_menu, LV_OBJ_FLAG_CLICKABLE);
    // lv_obj_add_style(scr_home, &style_scr_backdrop, LV_PART_MAIN);

    img_launch_bg = lv_img_create(scr_menu);
    lv_img_set_src(img_launch_bg, "A:../nfc/assets/pngs/haloeffect.png");
    lv_img_set_zoom(img_launch_bg, 256); 
    lv_obj_clear_flag(img_launch_bg, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_bg_opa(img_launch_bg, LV_OPA_20, LV_PART_MAIN);
    lv_obj_align(img_launch_bg, LV_ALIGN_CENTER, 0, 0);

    scroll_cont = lv_obj_create(scr_menu);
    lv_obj_set_size(scroll_cont, 1024, 500);
    lv_obj_center(scroll_cont);
    lv_obj_set_style_bg_opa(scroll_cont, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_opa(scroll_cont, LV_OPA_TRANSP, 0);
    lv_obj_set_scroll_dir(scroll_cont, LV_DIR_HOR);
    lv_obj_set_scrollbar_mode(scroll_cont, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_flag(scroll_cont, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t *inner_cont = lv_obj_create(scroll_cont);
    lv_coord_t btn_w = 400;
    lv_coord_t btn_h = 400;
    lv_coord_t gap   = 20;
    lv_coord_t inner_w = (btn_w + gap) * btn_count * 2 - gap; 
    lv_obj_set_size(inner_cont, inner_w, 600);
    lv_obj_set_style_bg_opa(inner_cont, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_opa(inner_cont, LV_OPA_TRANSP, 0);
    lv_obj_set_flex_flow(inner_cont, LV_FLEX_FLOW_ROW);
    lv_obj_set_style_pad_gap(inner_cont, gap, LV_PART_MAIN);
    lv_obj_clear_flag(inner_cont, LV_OBJ_FLAG_SCROLLABLE);

    const char *labels[8] = {
        "Chat to CS", "Seamless WiFi Pairing", "Feature Highlights", 
        "Solution Assist"
    };

    const char *sub_labels[8] = {
        "Chat to CS", "Seamless WiFi Pairing", "Feature Highlights", 
        "Solution Assist"
    };

    int img_zoom[4] = { 80, 30, 50, 45};

    for(uint32_t set = 0; set < 2; set++) {
        for(uint32_t i = 0; i < btn_count; i++) {

            lv_obj_t *btn = lv_btn_create(inner_cont);
            lv_obj_set_size(btn, btn_w, btn_h);
            lv_obj_add_style(btn, btn_styles[i], LV_PART_MAIN);

            // img icons
            lv_obj_t *img = lv_img_create(btn);
            lv_img_set_src(img, imgs[i]);
            lv_obj_align(img, LV_ALIGN_CENTER, 0, -100); 
            lv_img_set_zoom(img, img_zoom[i]);

            lv_obj_t *label = lv_label_create(btn);
            lv_label_set_text(label, labels[i]);
            lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
            lv_obj_set_style_text_font(label, &lv_font_montserrat_28, 0);
            // lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), 0);   
            // lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);

            lv_obj_t *sub_label = lv_label_create(btn);
            lv_label_set_text(sub_label, sub_labels[i]);
            lv_obj_align(sub_label, LV_ALIGN_CENTER, 0, 60);
            lv_obj_set_style_text_font(sub_label, &lv_font_montserrat_20, 0);
            // lv_obj_set_style_text_color(sub_label, lv_color_hex(0xFFFFFF), 0);

             if (i == 1) {
                lv_obj_set_style_text_color(sub_label, lv_color_hex(0xFFFFFF), 0); 
                lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), 0);  
            } else {
                lv_obj_set_style_text_color(sub_label, lv_color_hex(0xAAAAAA), 0);  
                lv_obj_set_style_text_color(label, lv_color_hex(0xAAAAAA), 0); 
            }


            if(i == 1 && set == 0) {
                lv_obj_add_event_cb(btn, create_launch_screen, LV_EVENT_CLICKED, NULL);
            }

            if(set == 0) btns[i] = btn; 
        }
    }

    lv_obj_scroll_to_x(scroll_cont, 0, LV_ANIM_OFF);
    lv_obj_add_event_cb(scroll_cont, scroll_loop_event_cb, LV_EVENT_SCROLL, NULL);
}
