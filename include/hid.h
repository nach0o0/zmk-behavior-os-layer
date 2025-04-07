#pragma once

#include <zmk/event_manager.h>

#ifdef CONFIG_RAW_HID

struct is_connected_notification {
    bool value;
};

ZMK_EVENT_DECLARE(is_connected_notification);

struct time_notification {
    uint8_t hour;
    uint8_t minute;
};

ZMK_EVENT_DECLARE(time_notification);

struct os_notification {
    char current_host_os[30];
};

ZMK_EVENT_DECLARE(os_notification);
#endif