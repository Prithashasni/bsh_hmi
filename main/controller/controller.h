#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include "../../lvgl/lvgl.h"

extern void ui_start();
extern void page_change();
extern void admin_change();
extern void wallbox_page();

extern lv_timer_t *page_task;
extern lv_timer_t *admin_task;
extern int header_page;
extern int prev_header_page;

#endif //_CONTROLLER_H_