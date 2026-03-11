#include "lvgl.h"
#include <unistd.h>

int main(void)
{
    lv_init();

    /* Display driver init should go here */

    lv_obj_t *label = lv_label_create(lv_scr_act());
    lv_label_set_text(label, "Display Manager Started");
    lv_obj_center(label);

    while (1) {
        lv_timer_handler();
        usleep(5000);
    }

    return 0;
}