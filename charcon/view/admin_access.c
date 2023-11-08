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

// Define a file path for your value (change this to your file path)
#define LOGS_FILE_PATH "/home/mqtt_log.txt"


lv_obj_t *scr_cloud;
lv_obj_t *scr_vsecc;
lv_obj_t *scr_charcon;
lv_obj_t *scr_master;
lv_obj_t *vsecc_settings;
lv_obj_t *charcon_settings;
lv_obj_t *vehicle_label;
lv_obj_t *vehicle_num;
lv_obj_t *state_label;
lv_obj_t *state_value;
lv_obj_t *mode_label;
lv_obj_t *mode_value;
lv_obj_t *underline;
lv_obj_t *current_limit;
lv_obj_t *current_label;
lv_obj_t *voltage_label;
lv_obj_t *current_value;
lv_obj_t *voltage_value;
lv_obj_t *text_ta1;
lv_obj_t *img_init;
lv_obj_t *img_back;
lv_obj_t *back_text;
lv_obj_t *scr_logs;
lv_obj_t *logs_page;
lv_obj_t *rect_ta;
lv_obj_t *rect_ta1;
lv_timer_t *log_timer;
lv_timer_t *rect_timer;
lv_timer_t *rect1_timer;

char *next_log_message[100];
int current_line = 0;
char **lines = NULL;
int line_count = 0;

static const char * btnm_map[] = {"1", "2", "3", "\n",
                                      "4", "5", "6", "\n",
                                      "7", "8", "9", "\n",
                                      LV_SYMBOL_BACKSPACE, "0", ".", LV_SYMBOL_OK, ""
                                    };



///////////////// Software Update Screen ///////////////
void scr_software_update()
{
    scr_cloud = lv_obj_create(lv_scr_act());
    lv_obj_set_size(scr_cloud, 1280, 800);
    lv_obj_set_scrollbar_mode(scr_cloud, LV_SCROLLBAR_MODE_OFF);
    lv_obj_clear_flag(scr_cloud, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(scr_cloud, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_style(scr_cloud, &style_scr_backdrop, LV_STATE_DEFAULT);

    allpage_status(scr_cloud);

    lv_obj_set_style_img_recolor(img_cloud, lv_color_hex(0xfffc04), LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor_opa(img_cloud, LV_OPA_COVER, LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(sw_text, lv_color_hex(0xfffc04), LV_STATE_DEFAULT);

    img_init = lv_img_create(img_charger);
    lv_img_set_src(img_init, &img_loader);
    lv_obj_align(img_init, LV_ALIGN_CENTER, 0, 0);

    initialize_text = lv_label_create(scr_cloud);
    lv_label_set_text(initialize_text, "Checking for Software Updates...");
    lv_obj_align(initialize_text, LV_ALIGN_CENTER, 0, 220);
    lv_obj_add_style(initialize_text, &style_confg_text, LV_STATE_DEFAULT);

    lv_anim_t anim_loader;
    lv_anim_init(&anim_loader);
    lv_anim_set_var(&anim_loader, img_init);
    lv_anim_set_exec_cb(&anim_loader, anim_rotate_cb);
    lv_anim_set_time(&anim_loader, 2000);  // Animation duration in milliseconds
    lv_anim_set_values(&anim_loader, 0, 3600);  
    lv_anim_set_repeat_count(&anim_loader, LV_ANIM_REPEAT_INFINITE);  
    lv_anim_start(&anim_loader);


    img_back =  lv_img_create(scr_cloud);
    lv_img_set_src(img_back, &back_button);
    lv_img_set_zoom(img_back, 150);
    lv_obj_align(img_back, LV_ALIGN_TOP_LEFT, 20, 80);
    lv_obj_add_flag(img_back, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(img_back, prev_screen_cb, LV_EVENT_CLICKED, NULL);

    back_text = lv_label_create(scr_cloud);
    lv_label_set_text(back_text, "Back");
    lv_obj_align(back_text, LV_ALIGN_TOP_LEFT, 90, 95);
    lv_obj_add_flag(back_text, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_text_color(back_text, LV_COLOR_WHITE , LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(back_text, &lv_font_montserrat_34 , LV_STATE_DEFAULT);
    lv_obj_add_event_cb(back_text, prev_screen_cb, LV_EVENT_CLICKED, NULL);
 
}

///////////////// v.SECC Settings Screen ///////////////
void scr_vsecc_settings()
{
    scr_vsecc = lv_obj_create(lv_scr_act());
    lv_obj_set_size(scr_vsecc, 1280, 800);
    lv_obj_set_scrollbar_mode(scr_vsecc, LV_SCROLLBAR_MODE_OFF);
    lv_obj_clear_flag(scr_vsecc, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(scr_vsecc, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_style(scr_vsecc, &style_scr_backdrop, LV_STATE_DEFAULT);

    allpage_status(scr_vsecc);

    img_vsecc = lv_img_create(scr_vsecc);
    lv_img_set_src(img_vsecc, &vsecc_icon);
    lv_img_set_zoom(img_vsecc, 350);
    lv_obj_set_style_img_recolor(img_vsecc, LV_COLOR_YELLOW, LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor_opa(img_vsecc, LV_OPA_COVER, LV_STATE_DEFAULT);
    lv_obj_align(img_vsecc, LV_ALIGN_TOP_LEFT, 60, 130);

    vsecc_settings = lv_label_create(scr_vsecc);
    lv_label_set_text(vsecc_settings, "v.SECC Settings");
    lv_obj_align(vsecc_settings, LV_ALIGN_TOP_LEFT, 130, 120);
    lv_obj_set_style_text_font(vsecc_settings, &lv_font_montserrat_36, LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(vsecc_settings, LV_COLOR_YELLOW, LV_STATE_DEFAULT);


    underline = lv_obj_create(scr_vsecc);
    lv_obj_set_size(underline, 285, 5);
    lv_obj_align(underline, LV_ALIGN_TOP_LEFT, 130, 165);
    lv_obj_set_style_radius(underline, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(underline, LV_COLOR_YELLOW, LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(underline, LV_COLOR_YELLOW, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(underline, LV_OPA_COVER, LV_STATE_DEFAULT);

    vehicle_label = lv_label_create(scr_vsecc);
    lv_label_set_text(vehicle_label, "Vehicle ID \t\t\t\t\t\t\t\t\t\t\t:");
    lv_obj_align(vehicle_label, LV_ALIGN_TOP_LEFT, 50, 250);
    lv_obj_add_style(vehicle_label, &style_white_text, LV_STATE_DEFAULT);

    vehicle_num = lv_label_create(scr_vsecc);
    lv_label_set_text(vehicle_num, "123456 NM");
    lv_obj_align(vehicle_num, LV_ALIGN_TOP_LEFT, 500, 250);
    lv_obj_add_style(vehicle_num, &style_white_text, LV_STATE_DEFAULT);

    state_label = lv_label_create(scr_vsecc);
    lv_label_set_text(state_label, "Charging State  \t\t\t\t\t :");
    lv_obj_align(state_label, LV_ALIGN_TOP_LEFT, 50, 330);
    lv_obj_add_style(state_label, &style_white_text, LV_STATE_DEFAULT);

    state_value = lv_label_create(scr_vsecc);
    lv_label_set_text(state_value, "STANDBY");
    lv_obj_align(state_value, LV_ALIGN_TOP_LEFT, 500, 330);
    lv_obj_add_style(state_value, &style_white_text, LV_STATE_DEFAULT);

    mode_label = lv_label_create(scr_vsecc);
    lv_label_set_text(mode_label, "Charging Mode  \t\t\t\t\t:");
    lv_obj_align(mode_label, LV_ALIGN_TOP_LEFT, 50, 410);
    lv_obj_add_style(mode_label, &style_white_text, LV_STATE_DEFAULT);

    mode_value = lv_label_create(scr_vsecc);
    lv_label_set_text(mode_value, "UNKNOWN");
    lv_obj_align(mode_value, LV_ALIGN_TOP_LEFT, 500, 410);
    lv_obj_add_style(mode_value, &style_white_text, LV_STATE_DEFAULT);

    ///// Cancel Button
    cancel_button = lv_btn_create(scr_vsecc);
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

///////////////// Charcon Settings Screen ///////////////
void scr_charcon_settings()
{
    scr_charcon = lv_obj_create(lv_scr_act());
    lv_obj_set_size(scr_charcon, 1280, 800);
    lv_obj_set_scrollbar_mode(scr_charcon, LV_SCROLLBAR_MODE_OFF);
    lv_obj_clear_flag(scr_charcon, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(scr_charcon, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_style(scr_charcon, &style_scr_backdrop, LV_STATE_DEFAULT);

    allpage_status(scr_charcon);

    img_charcon = lv_img_create(scr_charcon);
    lv_img_set_src(img_charcon, &charcon_icon);
    lv_img_set_zoom(img_charcon, 350);
    lv_obj_set_style_img_recolor(img_charcon, LV_COLOR_YELLOW, LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor_opa(img_charcon, LV_OPA_COVER, LV_STATE_DEFAULT);
    lv_obj_align(img_charcon, LV_ALIGN_TOP_LEFT, 60, 130);

    charcon_settings = lv_label_create(scr_charcon);
    lv_label_set_text(charcon_settings, "Charcon Settings");
    lv_obj_align(charcon_settings, LV_ALIGN_TOP_LEFT, 130, 120);
    lv_obj_set_style_text_font(charcon_settings, &lv_font_montserrat_36, LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(charcon_settings, LV_COLOR_YELLOW, LV_STATE_DEFAULT);

    underline = lv_obj_create(scr_charcon);
    lv_obj_set_size(underline, 315, 5);
    lv_obj_align(underline, LV_ALIGN_TOP_LEFT, 130, 165);
    lv_obj_set_style_radius(underline, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(underline, LV_COLOR_YELLOW, LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(underline, LV_COLOR_YELLOW, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(underline, LV_OPA_COVER, LV_STATE_DEFAULT);

    current_limit = lv_label_create(scr_charcon);
    lv_label_set_text(current_limit, "Maximum Grid Current Limit \t\t\t:               A");
    lv_obj_align(current_limit, LV_ALIGN_TOP_LEFT, 50, 250);
    lv_obj_add_style(current_limit, &style_white_text, LV_STATE_DEFAULT);

    mode_label = lv_label_create(scr_charcon);
    lv_label_set_text(mode_label, "Present Mode  \t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t:");
    lv_obj_align(mode_label, LV_ALIGN_TOP_LEFT, 50, 330);
    lv_obj_add_style(mode_label, &style_white_text, LV_STATE_DEFAULT);

    mode_value = lv_label_create(scr_charcon);
    lv_label_set_text(mode_value, "SLEEP");
    lv_obj_align(mode_value, LV_ALIGN_TOP_LEFT, 620, 330);
    lv_obj_add_style(mode_value, &style_white_text, LV_STATE_DEFAULT);

    current_label = lv_label_create(scr_charcon);
    lv_label_set_text(current_label, "Present Current value \t\t\t\t\t\t\t\t\t\t:");
    lv_obj_align(current_label, LV_ALIGN_TOP_LEFT, 50, 410);
    lv_obj_add_style(current_label, &style_white_text, LV_STATE_DEFAULT);

    current_value = lv_label_create(scr_charcon);
    lv_label_set_text(current_value, "40.10");
    lv_obj_align(current_value, LV_ALIGN_TOP_LEFT, 620, 410);
    lv_obj_add_style(current_value, &style_white_text, LV_STATE_DEFAULT);

    voltage_label = lv_label_create(scr_charcon);
    lv_label_set_text(voltage_label, "Present Voltage value \t\t\t\t\t\t\t\t\t\t:");
    lv_obj_align(voltage_label, LV_ALIGN_TOP_LEFT, 50, 490);
    lv_obj_add_style(voltage_label, &style_white_text, LV_STATE_DEFAULT);

    voltage_value = lv_label_create(scr_charcon);
    lv_label_set_text(voltage_value, "40.10");
    lv_obj_align(voltage_value, LV_ALIGN_TOP_LEFT, 620, 490);
    lv_obj_add_style(voltage_value, &style_white_text, LV_STATE_DEFAULT);

    ///////// User enter Textare ////////

    text_ta = lv_textarea_create(scr_charcon);
    lv_obj_set_size(text_ta, 90, 50);
    lv_obj_align(text_ta, LV_ALIGN_TOP_LEFT, 620, 240);
    lv_textarea_set_one_line(text_ta, true);
    lv_textarea_set_password_mode(text_ta, false);
    lv_textarea_set_max_length(text_ta, 2);
    lv_obj_set_scrollbar_mode(text_ta, LV_SCROLLBAR_MODE_OFF);   
    lv_obj_add_style(text_ta, &style_numbox, LV_PART_MAIN);
    lv_obj_add_event_cb(text_ta, ta_handler, LV_EVENT_ALL, NULL);
    lv_obj_clear_state(text_ta, LV_STATE_FOCUSED);
    lv_obj_clear_state(text_ta, LV_STATE_DISABLED);

    keyboard_area = lv_btnmatrix_create(scr_charcon);
    lv_obj_set_size(keyboard_area, 400, 300);
    lv_obj_align_to(keyboard_area, text_ta, LV_ALIGN_OUT_RIGHT_MID, 70, 100);
    lv_obj_clear_flag(keyboard_area, LV_OBJ_FLAG_CLICK_FOCUSABLE); /*To keep the text area focused on button clicks*/
    lv_btnmatrix_set_map(keyboard_area, btnm_map);
    lv_obj_add_flag(keyboard_area, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_style(keyboard_area, &style_keyboard, LV_PART_MAIN);
    lv_obj_add_style(keyboard_area, &style_num_key, LV_PART_ITEMS);
    lv_obj_add_event_cb(keyboard_area, btnm_event_handler, LV_EVENT_VALUE_CHANGED, text_ta);

    rect_ta = lv_obj_create(scr_charcon);
    lv_obj_add_flag(rect_ta, LV_OBJ_FLAG_HIDDEN);
    lv_obj_set_size(rect_ta, 90, 60);
    lv_obj_clear_flag(rect_ta, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_scrollbar_mode(rect_ta, LV_SCROLLBAR_MODE_OFF);
    lv_obj_align(rect_ta, LV_ALIGN_TOP_LEFT, 620, 240);
    lv_obj_add_style(rect_ta, &style_ta_rect, LV_PART_MAIN);
    
    ///// Cancel Button
    cancel_button = lv_btn_create(scr_charcon);
    lv_obj_set_size(cancel_button, 230, 80);
    lv_obj_align(cancel_button, LV_ALIGN_LEFT_MID, 50, 235);
    lv_obj_add_style(cancel_button, &style_red_button, LV_STATE_DEFAULT);
    lv_obj_add_flag(cancel_button, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_clear_flag(cancel_button, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_event_cb(cancel_button, prev_screen_cb, LV_EVENT_CLICKED, NULL);

    cancel_text = lv_label_create(cancel_button);
    lv_label_set_text(cancel_text, "Cancel");
    lv_obj_align(cancel_text, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_style(cancel_text, &style_stop_text, LV_STATE_DEFAULT);

    rect_timer = lv_timer_create(ta_rect_cb, 1000, NULL);
}

void ta_rect_cb()
{
        /////// Disable textarea when charging in progress ///////
        if(PlugStatus == 2)
        {
            lv_obj_clear_flag(rect_ta, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(rect_ta, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_state(rect_ta, LV_STATE_FOCUSED);
            lv_obj_add_state(text_ta, LV_STATE_DISABLED);
            lv_obj_clear_state(text_ta, LV_STATE_FOCUSED);
          
        }
        else{
            lv_obj_add_flag(rect_ta, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(rect_ta, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_state(rect_ta, LV_STATE_FOCUSED);
            lv_obj_clear_state(text_ta, LV_STATE_DISABLED);
            lv_obj_add_state(text_ta, LV_STATE_FOCUSED);  
        }
}

void save_max_current(const char *text)
{
    FILE *file = fopen("/home/config.json", "a");
    if (file) {
        fprintf(file, ",\n{\"Maximum Grid Current Limit\": \"%s\"}", text);
        fclose(file);
        printf("Text saved to config.json\n");
    } else {
        printf("Error opening config.json for writing\n");
    }
}

static void ta_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * ta = lv_event_get_target(e);
    if(code == LV_EVENT_CLICKED || code == LV_EVENT_FOCUSED) {
        /*Focus on the clicked text area*/
        if(keyboard_area != NULL) {
            lv_keyboard_set_textarea(keyboard_area, ta);
            lv_obj_clear_flag(keyboard_area, LV_OBJ_FLAG_HIDDEN);
        }
    }
    if(code == LV_EVENT_READY) {
        LV_LOG_USER("Ready, current text: %s", lv_textarea_get_text(ta));
        const char *text =  lv_textarea_get_text(text_ta);
        save_max_current(text);
        if(text != NULL);
        {
            // lv_obj_clear_state(text_ta, LV_STATE_FOCUSED);
            lv_obj_add_flag(keyboard_area, LV_OBJ_FLAG_HIDDEN);          
        }
        // lv_obj_clear_state(text_ta, LV_STATE_FOCUSED);
    }
}

static void btnm_event_handler(lv_event_t * e)
{
    lv_obj_t * obj = lv_event_get_target(e);
    lv_obj_t * ta = lv_event_get_user_data(e);
    const char * txt = lv_btnmatrix_get_btn_text(obj, lv_btnmatrix_get_selected_btn(obj));

    if(strcmp(txt, LV_SYMBOL_BACKSPACE) == 0){
        lv_textarea_del_char(text_ta);
    }
    else if(strcmp(txt, LV_SYMBOL_OK) == 0){
        lv_event_send(text_ta, LV_EVENT_READY, NULL);
    }
    else{
        lv_textarea_add_text(text_ta, txt);
    }

}

void save_cost_grid(const char *text)
{
    FILE *file = fopen("/home/config.json", "a");
    if (file) {
        fprintf(file, ",\n{\"Cost per kWh on Grid\": \"%s\"}", text);
        fclose(file);
        printf("Text saved to config.json\n");
    } else {
        printf("Error opening config.json for writing\n");
    }
}

///////////////// Master Control Screen ///////////////
void scr_master_control()
{
    scr_master = lv_obj_create(lv_scr_act());
    lv_obj_set_size(scr_master, 1280, 800);
    lv_obj_set_scrollbar_mode(scr_master, LV_SCROLLBAR_MODE_OFF);
    lv_obj_clear_flag(scr_master, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(scr_master, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_style(scr_master, &style_scr_backdrop, LV_STATE_DEFAULT);

    allpage_status(scr_master);

    img_mc = lv_img_create(scr_master);
    lv_img_set_src(img_mc, &charcon_icon);
    lv_img_set_zoom(img_mc, 350);
    lv_obj_set_style_img_recolor(img_mc, LV_COLOR_YELLOW, LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor_opa(img_mc, LV_OPA_COVER, LV_STATE_DEFAULT);
    lv_obj_align(img_mc, LV_ALIGN_TOP_LEFT, 60, 130);


    charcon_settings = lv_label_create(scr_master);
    lv_label_set_text(charcon_settings, "Master Control");
    lv_obj_align(charcon_settings, LV_ALIGN_TOP_LEFT, 130, 120);
    lv_obj_set_style_text_font(charcon_settings, &lv_font_montserrat_36, LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(charcon_settings, LV_COLOR_YELLOW, LV_STATE_DEFAULT);

    underline = lv_obj_create(scr_master);
    lv_obj_set_size(underline, 270, 5);
    lv_obj_align(underline, LV_ALIGN_TOP_LEFT, 130, 165);
    lv_obj_set_style_radius(underline, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(underline, LV_COLOR_YELLOW, LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(underline, LV_COLOR_YELLOW, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(underline, LV_OPA_COVER, LV_STATE_DEFAULT);

    current_limit = lv_label_create(scr_master);
    lv_label_set_text(current_limit, "Cost per kWh on Grid \t\t:               Euros");
    lv_obj_align(current_limit, LV_ALIGN_TOP_LEFT, 50, 250);
    lv_obj_add_style(current_limit, &style_white_text, LV_STATE_DEFAULT);

    mode_label = lv_label_create(scr_master);
    lv_label_set_text(mode_label, "Contactor Status  \t\t\t\t\t\t:");
    lv_obj_align(mode_label, LV_ALIGN_TOP_LEFT, 50, 330);
    lv_obj_add_style(mode_label, &style_white_text, LV_STATE_DEFAULT);

    mode_value = lv_label_create(scr_master);
    lv_label_set_text(mode_value, "OPEN");
    lv_obj_align(mode_value, LV_ALIGN_TOP_LEFT, 480, 330);
    lv_obj_add_style(mode_value, &style_white_text, LV_STATE_DEFAULT);

    ///////// User enter Textare ////////

    text_ta1 = lv_textarea_create(scr_master);
    lv_obj_set_size(text_ta1, 90, 50);
    lv_obj_align(text_ta1, LV_ALIGN_TOP_LEFT, 480, 240);
    lv_textarea_set_one_line(text_ta1, true);
    lv_textarea_set_password_mode(text_ta1, false);
    lv_textarea_set_max_length(text_ta1, 4);
    lv_obj_set_scrollbar_mode(text_ta1, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_style(text_ta1, &style_numbox, LV_PART_MAIN);
    lv_obj_add_event_cb(text_ta1, ta_vol_handler, LV_EVENT_ALL, NULL);
    lv_obj_clear_state(text_ta1, LV_STATE_FOCUSED);

    keyboard_area = lv_btnmatrix_create(scr_master);
    lv_obj_set_size(keyboard_area, 400, 300);
    lv_obj_align_to(keyboard_area, text_ta1, LV_ALIGN_OUT_RIGHT_MID, 150, 100);
    lv_obj_clear_flag(keyboard_area, LV_OBJ_FLAG_CLICK_FOCUSABLE); /*To keep the text area focused on button clicks*/
    lv_btnmatrix_set_map(keyboard_area, btnm_map);
    lv_obj_add_flag(keyboard_area, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_style(keyboard_area, &style_keyboard, LV_PART_MAIN);
    lv_obj_add_style(keyboard_area, &style_num_key, LV_PART_ITEMS);
    lv_obj_add_event_cb(keyboard_area, btnm_event1_handler, LV_EVENT_VALUE_CHANGED, text_ta1);  

    rect_ta1 = lv_obj_create(scr_master);
    lv_obj_add_flag(rect_ta1, LV_OBJ_FLAG_HIDDEN);
    lv_obj_set_size(rect_ta1, 90, 60);
    lv_obj_clear_flag(rect_ta1, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_scrollbar_mode(rect_ta1, LV_SCROLLBAR_MODE_OFF);
    lv_obj_align(rect_ta1, LV_ALIGN_TOP_LEFT, 480, 240);
    lv_obj_add_style(rect_ta1, &style_ta_rect, LV_PART_MAIN);
    
    ///// Cancel Button
    cancel_button = lv_btn_create(scr_master);
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

    rect1_timer = lv_timer_create(ta1_rect_cb, 1000, NULL);
}

void ta1_rect_cb()
{
        /////// Disable textarea when charging in progress ///////
        if(PlugStatus == 2)
        {
            lv_obj_clear_flag(rect_ta1, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(rect_ta1, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_state(rect_ta1, LV_STATE_FOCUSED);
            lv_obj_add_state(text_ta1, LV_STATE_DISABLED);
            lv_obj_clear_state(text_ta1, LV_STATE_FOCUSED);            
        }
        else{
            lv_obj_add_flag(rect_ta1, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(rect_ta1, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_state(rect_ta1, LV_STATE_FOCUSED);
            lv_obj_clear_state(text_ta1, LV_STATE_DISABLED);
            lv_obj_add_state(text_ta1, LV_STATE_FOCUSED);   
        }
}

static void ta_vol_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * ta1 = lv_event_get_target(e);
    if(code == LV_EVENT_CLICKED || code == LV_EVENT_FOCUSED) {
        /*Focus on the clicked text area*/
        if(keyboard_area != NULL) {
            lv_keyboard_set_textarea(keyboard_area, ta1);
            lv_obj_clear_flag(keyboard_area, LV_OBJ_FLAG_HIDDEN);
        }
    }
    if(code == LV_EVENT_READY) {
        LV_LOG_USER("Ready, current text: %s", lv_textarea_get_text(ta1));
        const char *text1 =  lv_textarea_get_text(text_ta1);
        if (text1 != NULL && strlen(text1) > 0) {
            strcpy(cost_on_grid, text1);
            lv_obj_add_flag(keyboard_area, LV_OBJ_FLAG_HIDDEN);   
            LV_LOG_USER("text: %s", cost_on_grid); 
            save_cost_grid(text1);
        }
    }
}

static void btnm_event1_handler(lv_event_t * e)
{
    lv_obj_t * obj1 = lv_event_get_target(e);
    lv_obj_t * ta1 = lv_event_get_user_data(e);
    const char * txt1 = lv_btnmatrix_get_btn_text(obj1, lv_btnmatrix_get_selected_btn(obj1));

    if(strcmp(txt1, LV_SYMBOL_BACKSPACE) == 0){
        lv_textarea_del_char(text_ta1);
    }
    else if(strcmp(txt1, LV_SYMBOL_OK) == 0){
        lv_event_send(text_ta1, LV_EVENT_READY, NULL);
    }
    else{
        lv_textarea_add_text(text_ta1, txt1);
    }

}

////////////////// Logs screen //////////////////////////

void scr_logs_page()
{
    scr_logs = lv_obj_create(lv_scr_act());
    lv_obj_set_size(scr_logs, 1280, 800);
    lv_obj_set_scrollbar_mode(scr_logs, LV_SCROLLBAR_MODE_OFF);
    lv_obj_clear_flag(scr_logs, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(scr_logs, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_style(scr_logs, &style_scr_backdrop, LV_STATE_DEFAULT);

    allpage_status(scr_logs);

    img_log = lv_img_create(scr_logs);
    lv_img_set_src(img_log, &charcon_icon);
    lv_img_set_zoom(img_log, 350);
    lv_obj_set_style_img_recolor(img_log, LV_COLOR_YELLOW, LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor_opa(img_log, LV_OPA_COVER, LV_STATE_DEFAULT);
    lv_obj_align(img_log, LV_ALIGN_TOP_LEFT, 70, 90);

    log_text = lv_label_create(scr_logs);
    lv_label_set_text(log_text, "Logs");
    lv_obj_align(log_text, LV_ALIGN_TOP_LEFT, 140, 80);
    lv_obj_set_style_text_font(log_text, &lv_font_montserrat_36, LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(log_text, LV_COLOR_YELLOW, LV_STATE_DEFAULT);

    underline = lv_obj_create(scr_logs);
    lv_obj_set_size(underline, 85, 5);
    lv_obj_align(underline, LV_ALIGN_TOP_LEFT, 140, 125);
    lv_obj_set_style_radius(underline, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(underline, LV_COLOR_YELLOW, LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(underline, LV_COLOR_YELLOW, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(underline, LV_OPA_COVER, LV_STATE_DEFAULT);

    logs_page = lv_textarea_create(scr_logs);
    lv_obj_set_size(logs_page, 1100, 400);
    lv_obj_align(logs_page, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_style(logs_page, &style_page_bg, LV_PART_MAIN);
    lv_textarea_set_one_line(logs_page, false);
    lv_textarea_set_password_mode(logs_page, false);
    lv_textarea_add_text(logs_page, "");
    lv_obj_add_style(logs_page, &style_log_text, LV_PART_MAIN);

    ///// Cancel Button
    cancel_button = lv_btn_create(scr_logs);
    lv_obj_set_size(cancel_button, 200, 80);
    lv_obj_align(cancel_button, LV_ALIGN_LEFT_MID, 65, 270);
    lv_obj_add_style(cancel_button, &style_red_button, LV_STATE_DEFAULT);
    lv_obj_add_flag(cancel_button, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_clear_flag(cancel_button, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_event_cb(cancel_button, prev_screen_cb, LV_EVENT_CLICKED, NULL);

    cancel_text = lv_label_create(cancel_button);
    lv_label_set_text(cancel_text, "Cancel");
    lv_obj_align(cancel_text, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_text_font(cancel_text, &lv_font_montserrat_36, LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(cancel_text, LV_COLOR_WHITE, LV_STATE_DEFAULT); 


    add_log();
}

void add_log() 
{
    FILE *file = fopen(LOGS_FILE_PATH, "r");
    if (file == NULL) {
        perror("Error opening the file");
        return;
    }

    int max_lines = 100;
    lines = (char **)malloc(max_lines * sizeof(char *));
    if (lines == NULL) {
        perror("Memory allocation error");
        fclose(file);
        return;
    }

    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        if (line_count >= max_lines) {
            max_lines *= 2;
            char **new_lines = (char **)realloc(lines, max_lines * sizeof(char *));
            if (new_lines == NULL) {
                perror("Memory reallocation error");
                free(lines);
                fclose(file);
                return;
            }
            lines = new_lines;
        }

        lines[line_count] = strdup(buffer);

        if (lines[line_count] == NULL) {
            perror("Memory allocation error");
            free(lines);
            fclose(file);
            return;
        }

        line_count++;
    }

    fclose(file);

    log_timer = lv_timer_create(print_next_line, 1000, NULL);
}

void print_next_line(lv_timer_t *timer) {
    if (current_line < line_count) {
        lv_textarea_add_text(logs_page, lines[current_line]);
        // lv_textarea_add_text(logs_page, "\n");
        current_line++;
    } else {
        // All lines have been printed, clean up
        lv_timer_pause(log_timer);

        // Free allocated memory
        for (int i = 0; i < line_count; i++) {
            free(lines[i]);
        }
        free(lines);
    }
}

