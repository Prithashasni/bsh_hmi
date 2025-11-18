
#include <math.h>
#include "../../lvgl/lvgl.h"
#include "styles.h"
#include "colors.h"

lv_style_t style_scr_backdrop;
lv_style_t style_scr_menu;

void init_style()
{
    init_style_backdrop();
}

void init_style_backdrop()
{
    lv_style_init(&style_scr_backdrop);
    lv_style_set_bg_color(&style_scr_backdrop, lv_color_hex(0x1E293B));
    lv_style_set_border_color(&style_scr_backdrop, lv_color_hex(0x1d314e));

    lv_style_init(&style_scr_menu);
    lv_style_set_bg_color(&style_scr_menu, lv_color_hex(0x1b3789));
    lv_style_set_border_color(&style_scr_menu, lv_color_hex(0x162d4a));
    lv_style_set_radius(&style_scr_menu, 0);
    lv_style_set_bg_opa(&style_scr_menu, LV_OPA_70);

}