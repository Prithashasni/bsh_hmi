/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023 PHYTEC Messtechnik GmbH
 */

#include "lvgl/lvgl.h"
#include "lvgl/demos/lv_demos.h"
#include "lv_drivers/wayland/wayland.h"
#include <unistd.h>


// #include "charcon/controller/controller.h"
#include "nfc/view/menu_screen.h"
#include "nfc/view/styles.h"

int main(void)
{
    printf("[INFO] Starting LVGL Wayland application...\n");
    /* LVGL init */
    lv_init();

    /* Wayland display init */
    lv_wayland_init();
    printf("[INFO] Display init...\n");
    lv_disp_t * disp = lv_wayland_create_window(1024, 600, "window", NULL);
    lv_wayland_window_set_fullscreen(disp, true);

    /* Set a cursor for the touchscreen */
    lv_indev_t * touchscreen = lv_wayland_get_touchscreen(disp);

    printf("[INFO] Touchscreen...\n");

    /* Search connected keyboard and attach it to the driver */
    lv_group_t * g = lv_group_create();
    lv_group_set_default(g);

    lv_indev_t * cur_drv = NULL;
    for (;;) {
        cur_drv = lv_indev_get_next(cur_drv);
        if (!cur_drv) {
            break;
        }

        if (cur_drv->driver->type == LV_INDEV_TYPE_KEYPAD) {
            lv_indev_set_group(cur_drv, g);
        }

        if (cur_drv->driver->type == LV_INDEV_TYPE_ENCODER) {
            lv_indev_set_group(cur_drv, g);
        }
    }

    init_style();
    scr_menu_scroll();

     printf("[OK] Menu screen loaded.\n");

    printf("[INFO] Entering main LVGL loop...\n");


  while(1) {
      lv_tick_inc(1);
      lv_timer_handler();
      usleep(500);

        static int counter = 0;
        if (++counter % 2000 == 0) {
            printf("[DEBUG] LVGL running (tick=%d)\n", counter);
        }
  }

    return 0;
}
