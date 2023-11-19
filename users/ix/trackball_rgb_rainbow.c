#include "trackball_rgb_rainbow.h"

#include "color.h"

#include "math.h"

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

// Modify these values to adjust the scrolling speed
#define SCROLL_DIVISOR_H 6.0
#define SCROLL_DIVISOR_V 6.0

// Variables to store accumulated scroll values
float scroll_accumulated_h = 0;
float scroll_accumulated_v = 0;

// Modify these to adjust non-linear mouse scaling
#define MAX_SCALE 128
#define MIN_SCALE 1
#define GROWTH_FACTOR 16
#define MOMENTUM 0.0

// Variable to store an exponential moving average scaling factor to denoise the non-linear scaling
float accumulated_factor = MIN_SCALE;

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    if (layer_state_is(1)) {
        // Calculate and accumulate scroll values based on mouse movement and divisors
        scroll_accumulated_h += (float)mouse_report.x / SCROLL_DIVISOR_H;
        scroll_accumulated_v += (float)mouse_report.y / SCROLL_DIVISOR_V;

        // Assign integer parts of accumulated scroll values to the mouse report
        mouse_report.h = -(int8_t)scroll_accumulated_h;
        mouse_report.v = (int8_t)scroll_accumulated_v;

        // Update accumulated scroll values by subtracting the integer parts
        scroll_accumulated_h -= (int8_t)scroll_accumulated_h;
        scroll_accumulated_v -= (int8_t)scroll_accumulated_v;

        // Clear the X and Y values of the mouse report
        mouse_report.x = 0;
        mouse_report.y = 0;
    }

    else {
        // compute the size of the last mouse movement
        float mouse_length = sqrt(mouse_report.x*mouse_report.x + mouse_report.y*mouse_report.y);

        // compute an instantaneous scaling factor and update exponential moving average
        float factor =  GROWTH_FACTOR*mouse_length+MIN_SCALE;
        accumulated_factor = accumulated_factor*(1-MOMENTUM) + factor*MOMENTUM;

        if (accumulated_factor > MAX_SCALE) {
            // clamp the scaling factor to avoid overflowing mouse_report
            mouse_report.x *= MAX_SCALE;
            mouse_report.y *= MAX_SCALE;
        }
        else {
            // scale up the mouse movement by the average factor
            mouse_report.x = (int8_t)(mouse_report.x * accumulated_factor);
            mouse_report.y = (int8_t)(mouse_report.y * accumulated_factor);
        }
    }
    return mouse_report;
}
