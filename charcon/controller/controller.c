/* The file that controls the UI State */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "pthread.h"

#include "MQTTClient.h"
#include "controller.h"
#include "../view/launch_screen.h"
#include "../view/health_check.h"
#include "../view/device_detect.h"
#include "../view/allpage_icons.h"
#include "../view/admin_page.h"
#include "../view/admin_access.h"
#include "../view/styles.h"
#include "../../sub.h"
#include "../../lvgl/lvgl.h"

lv_timer_t *page_task;
lv_timer_t *admin_task;
int prev_page;
int header_page;
int prev_header_page;

void ui_start()
{
    init_style();

    prev_page = Page;

    pthread_t thread1;
    pthread_create(&thread1, NULL, mqtt_init, NULL);

    create_launch_screen();

    page_task = lv_timer_create(page_change, NULL, NULL);
    lv_timer_ready(page_task);
    lv_timer_handler();
    lv_timer_set_repeat_count(page_task, -1);
}

void page_change()
{    
    if(Page != CONST_DeviceFailure && header_page != 0)
    {
        if(prev_header_page != header_page)
        {
            if(header_page == CONST_AdminLoginPage)
            {
                admin_login_pages();
                prev_header_page = CONST_AdminLoginPage;
                prev_page = -1;
            }
            if(header_page == CONST_SoftwareUpdate)
            {
                scr_software_update();
                prev_header_page = CONST_SoftwareUpdate;
                prev_page = -1;
            }
            if(header_page == CONST_VseccSettings)
            {
                scr_vsecc_settings();
                prev_header_page = CONST_VseccSettings;
                prev_page = -1;
            }
            if(header_page == CONST_CharconSettings)
            {
                scr_charcon_settings();
                prev_header_page = CONST_CharconSettings;
                prev_page = -1;
            }
            if(header_page == CONST_MasterControl)
            {
                scr_master_control();
                prev_header_page = CONST_MasterControl;
                prev_page = -1;
            }
            if(header_page == CONST_AdminLogsPage)
            {
                scr_logs_page();
                prev_header_page = CONST_AdminLogsPage;
                prev_page = -1;
            }
        }
    }
    else if(prev_page != Page)
    {
        wallbox_page();
    }
}

void wallbox_page()
{
    if(Page == CONST_DeviceFailure)
        {      
            wallbox_failure_status();
            prev_page = CONST_DeviceFailure;
        }
        else if(Page == CONST_HealthCheck)
        {
            screen_dc_check();
            prev_page = CONST_HealthCheck;
        }
        else if(Page == CONST_ChargingInitPage)
        {
            initialize_device();
            prev_page = CONST_ChargingInitPage;
        }
        else if(Page == CONST_ChargingProgressPage)
        {
            create_progress_screen();
            prev_page = CONST_ChargingProgressPage;
        }
        else if(Page == CONST_MissingConfig)
        {
            wallbox_not_configured();
            prev_page = CONST_MissingConfig;
        }
        else if(Page == CONST_ReadyToCharge)
        {
            home_screen();
            prev_page = CONST_ReadyToCharge;
        }
}



