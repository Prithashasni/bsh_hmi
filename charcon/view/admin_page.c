/**
 * admin_page.c
 *
 * Created on: 21-Sep-2023
 * Author: PSI2COB
 */

#include "../../lvgl/lvgl.h"
#include "health_check.h"
#include "styles.h"
#include "launch_screen.h"
#include "progress_screen.h"
#include "colors.h"
#include "allpage_icons.h"
#include "device_detect.h"
#include "admin_page.h"
#include "admin_access.h"
#include "../../sub.h"
#include "../controller/controller.h"

lv_obj_t *scr_admin;
lv_obj_t *login_text;
lv_obj_t *pwd_ta;
lv_obj_t *text_ta;
lv_obj_t *keyboard_area;
lv_obj_t *user_text;
lv_obj_t *pwd_text;
lv_obj_t *cancel_button;
lv_obj_t *cancel_text;
lv_obj_t *car_plugged;
const char *text;
lv_obj_t *text1;
lv_obj_t *prev;
lv_obj_t *prev_screen;
lv_obj_t *scr_default;


/////////// User Login Page //////////
void admin_login_pages()
{
    scr_admin = lv_obj_create(lv_scr_act());
    lv_obj_set_size(scr_admin, 1280, 800);
    lv_obj_set_scrollbar_mode(scr_admin, LV_SCROLLBAR_MODE_OFF);
    lv_obj_clear_flag(scr_admin, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(scr_admin, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_style(scr_admin, &style_scr_backdrop, LV_STATE_DEFAULT);

    lv_obj_add_flag(img_charger, LV_OBJ_FLAG_HIDDEN);
    allpage_status(scr_admin);

    login_text = lv_label_create(scr_admin);
    lv_label_set_text(login_text, "Login to View/Change System Settings");
    lv_obj_align(login_text, LV_ALIGN_TOP_LEFT, 40, 120);
    lv_obj_add_style(login_text, &style_vehicle_text, LV_STATE_DEFAULT);

    user_text = lv_label_create(scr_admin);
    lv_label_set_text(user_text, "User ID:");
    lv_obj_align(user_text, LV_ALIGN_TOP_LEFT, 40, 260);
    lv_obj_add_style(user_text, &style_check_text, LV_STATE_DEFAULT);

    pwd_text = lv_label_create(scr_admin);
    lv_label_set_text(pwd_text, "Password:");
    lv_obj_align(pwd_text, LV_ALIGN_TOP_LEFT, 40, 350);
    lv_obj_add_style(pwd_text, &style_check_text, LV_STATE_DEFAULT);

    //// Text box
    text_ta = lv_textarea_create(scr_admin);
    lv_obj_set_size(text_ta, 350, 150);
    lv_obj_align(text_ta, LV_ALIGN_TOP_LEFT, 220, 250);
    lv_textarea_set_one_line(text_ta, true);
    lv_textarea_set_password_mode(text_ta, false);
    lv_textarea_set_max_length(text_ta, 10);
    lv_obj_add_style(text_ta, &style_textbox, LV_PART_MAIN);
    lv_obj_add_event_cb(text_ta, ta_event_cb, LV_EVENT_ALL, NULL);

    //// Password box
    pwd_ta = lv_textarea_create(scr_admin);
    lv_obj_set_size(pwd_ta, 350, 150);
    lv_obj_align(pwd_ta, LV_ALIGN_TOP_LEFT, 220, 340);
    lv_textarea_set_text(pwd_ta, "");
    lv_textarea_set_password_mode(pwd_ta, true);
    lv_textarea_set_one_line(pwd_ta, true);
    lv_textarea_set_max_length(pwd_ta, 10);
    lv_obj_add_style(pwd_ta, &style_textbox, LV_PART_MAIN);
    lv_obj_add_event_cb(pwd_ta, ta_event_cb, LV_EVENT_ALL, NULL);

    keyboard_area = lv_keyboard_create(scr_admin);
    lv_obj_clear_flag(keyboard_area, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(keyboard_area, 600, 300);
    lv_obj_align(keyboard_area, LV_ALIGN_RIGHT_MID, -30, 10);
    lv_keyboard_set_textarea(keyboard_area, text_ta); /*Focus it on one of the text areas to start*/
    lv_obj_add_style(keyboard_area, &style_keyboard, LV_PART_MAIN);
    lv_obj_add_style(keyboard_area, &style_keyboard_btn, LV_PART_ITEMS);

    ///// Cancel Button
    cancel_button = lv_btn_create(scr_admin);
    lv_obj_set_size(cancel_button, 230, 80);
    lv_obj_align(cancel_button, LV_ALIGN_LEFT_MID, 50, 200);
    lv_obj_add_style(cancel_button, &style_red_button, LV_STATE_DEFAULT);
    lv_obj_add_flag(cancel_button, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_clear_flag(cancel_button, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_event_cb(cancel_button, prev_screen_cb, LV_EVENT_CLICKED, NULL);

    cancel_text = lv_label_create(cancel_button);
    lv_label_set_text(cancel_text, "Cancel");
    lv_obj_align(cancel_text, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_style(cancel_text, &style_stop_text, LV_STATE_DEFAULT);

}

void save_username(const char *text)
{
    FILE *file = fopen("/home/config.json", "w");
    if (file) {
        fprintf(file, "{\"User ID\": \"%s\"}", text);
        fclose(file);
        printf("Text saved to config.json\n");
    } else {
        printf("Error opening config.json for writing\n");
    }
}

void save_password(const char *text)
{
    FILE *file = fopen("/home/config.json", "a");
    if (file) {
        fprintf(file, ",\n{\"Password\": \"%s\"}", text);
        fclose(file);
        printf("Text saved to config.json\n");
    } else {
        printf("Error opening config.json for writing\n");
    }
}

static void ta_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * ta = lv_event_get_target(e);
    if(code == LV_EVENT_CLICKED || code == LV_EVENT_FOCUSED) {
        /*Focus on the clicked text area*/
        if(keyboard_area != NULL) lv_keyboard_set_textarea(keyboard_area, ta);
    }

    if(code == LV_EVENT_READY) {
        LV_LOG_USER("Ready, current text: %s", lv_textarea_get_text(ta));
        const char *text =  lv_textarea_get_text(text_ta);
        const char *ta =  lv_textarea_get_text(pwd_ta);
        // save_username(text);
        // save_password(ta);
        if(strcmp(text, "admin") == 0 && strcmp(ta,"admin") == 0)
        {
            // LV_LOG_USER("Admin text");
            lv_label_set_text(label_guest, "Admin");
            strcpy(admin_text, "Admin");
            if(text1 != NULL){
                lv_obj_add_flag(text1, LV_OBJ_FLAG_HIDDEN);
            }  
        }
        else 
        {
            text1 = lv_label_create(scr_admin);
            lv_label_set_text(text1, "Incorrect username or password");
            lv_obj_align_to(text1, pwd_ta, LV_ALIGN_OUT_BOTTOM_MID, -140, 20);
            lv_obj_add_style(text1, &style_red_text, LV_STATE_DEFAULT);
        }
    }
}


////////// A call back for previous screen ////////
void prev_screen_cb()
{
    header_page = 0;
    prev_header_page = -1;
    wallbox_page();
}

///////////////// Deafault screen ///////////////////
void home_screen()
{
    scr_default = lv_obj_create(lv_scr_act());
    lv_obj_set_size(scr_default, 1280, 800);
    lv_obj_set_scrollbar_mode(scr_default, LV_SCROLLBAR_MODE_OFF);
    lv_obj_clear_flag(scr_default, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(scr_default, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_style(scr_default, &style_scr_backdrop, LV_STATE_DEFAULT);

    allpage_status(scr_default);

    ready_text = lv_label_create(scr_default);
    lv_label_set_text(ready_text, "Charger Ready! Please Plug in to start Charging");
    lv_obj_align(ready_text, LV_ALIGN_CENTER, 0, -260);
    lv_obj_add_style(ready_text, &style_dc_ready, LV_STATE_DEFAULT);

}

