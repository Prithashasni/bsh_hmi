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

    /* LVGL init */
    printf("[INFO] Initializing LVGL...\n");
    lv_init();
    printf("[OK] LVGL initialized.\n");

    /* Wayland display init */
    printf("[INFO] Initializing Wayland backend...\n");
    lv_wayland_init();
    printf("[OK] Wayland backend initialized.\n");

    printf("[INFO] Creating Wayland window 1280x800...\n");
    lv_disp_t *disp = lv_wayland_create_window(1280, 800, "window", NULL);
    if (!disp) {
        printf("[ERROR] Failed to create Wayland display window\n");
        return -1;
    }
    printf("[OK] Wayland window created.\n");

    lv_wayland_window_set_fullscreen(disp, true);
    printf("[INFO] Fullscreen enabled.\n");

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

    /* LVGL Screen Setup */
    printf("[INFO] Initializing UI styles...\n");
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
