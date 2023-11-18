#include "trackball_rgb_rainbow.h"

#include "color.h"

void housekeeping_task_user(void) {
  if (is_keyboard_master()) {
    static uint32_t timer = 0;
    static HSV color = { .h = 0, .s = 255, .v = 255 };

    if (timer_elapsed32(timer) < 400)
        return;

    timer = timer_read32();

    // increase hue -> change color
    color.h++;

    RGB rgb = hsv_to_rgb(color);
    pimoroni_trackball_set_rgbw(rgb.r, rgb.g, rgb.b, 0);
  }
}
