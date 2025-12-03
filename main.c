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
#include "nfc.h"
#include "interrupt.h"
// #include <pthread.h>
#include "main.h"

int running = 1;
int main(void)
{
    /* LVGL init */
    lv_init();

    /* Wayland display init */
    lv_wayland_init();
    lv_disp_t * disp = lv_wayland_create_window(1280, 800, "window", NULL);
    lv_wayland_window_set_fullscreen(disp, true);

    /* Set a cursor for the touchscreen */
    lv_indev_t * touchscreen = lv_wayland_get_touchscreen(disp);

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
//   pthread_t t1, t2;

//   pthread_create(&t1, NULL, nfc_thread, NULL);
//   pthread_create(&t2, NULL, keyboard_thread, NULL);

  while (running) {
      lv_timer_handler();
      usleep(10 * 1000);
  }

//   printf("Exiting cleanly.\n");

//   // Now wait for threads to exit
//   pthread_join(t2, NULL);
//   pthread_join(t1, NULL);

  lv_deinit();
  return 0;

}
