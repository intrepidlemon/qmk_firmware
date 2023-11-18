#pragma once

#undef MASTER_LEFT
#undef EE_HANDS
#undef MASTER_RIGHT

#define SERIAL_USART_TX_PIN GP1

#ifdef POINTING_DEVICE_POSITION_LEFT
#define MASTER_LEFT
#else
#define MASTER_RIGHT
#endif


#ifdef POINTING_DEVICE_POSITION_LEFT
#define POINTING_DEVICE_ROTATION_270
#endif
#ifdef POINTING_DEVICE_POSITION_RIGHT
#define POINTING_DEVICE_ROTATION_90
#endif
#if defined(POINTING_DEVICE_POSITION_THUMB) || defined(POINTING_DEVICE_POSITION_MIDDLE)
#endif

#ifdef POINTING_DEVICE_TRACKPOINT_VENDOR
// The default streaming mode is flaky and doesn't always work. This setting doesn't seem to affect performance.
#define PS2_MOUSE_USE_REMOTE_MODE

// Serial uses PIO0, change PS2 to PIO1.
#define PS2_PIO_USE_PIO1

#define PS2_MOUSE_INVERT_X
#define PS2_MOUSE_INVERT_Y
#define PS2_DATA_PIN    GP2
#define PS2_CLOCK_PIN   GP3

// Start faster when keyboard resets.
#define PS2_MOUSE_INIT_DELAY 500
#endif

// Common settings to both busywait and interrupt.
#if defined(PS2_DRIVER_BUSYWAIT) || defined(PS2_DRIVER_INTERRUPT)
#define PS2_MOUSE_INVERT_X
#define PS2_MOUSE_INVERT_Y

#define PS2_CLOCK_PIN   D3
#define PS2_DATA_PIN    B4

#define PS2_MOUSE_USE_REMOTE_MODE
#endif

#ifdef PS2_DRIVER_INTERRUPT
#define PS2_INT_INIT()  do {    \
    EICRA |= ((1<<ISC31) |      \
              (0<<ISC30));      \
} while (0)
#define PS2_INT_ON()  do {      \
    EIMSK |= (1<<INT3);         \
} while (0)
#define PS2_INT_OFF() do {      \
    EIMSK &= ~(1<<INT3);        \
} while (0)
#define PS2_INT_VECT   INT3_vect
#endif

#define RP2040_FLASH_GD25Q64CS

#define TAP_CODE_DELAY 100
