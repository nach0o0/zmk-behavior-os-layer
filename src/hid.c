#include <hid.h>
#include <raw_hid/events.h>

#include <zephyr/kernel.h>

#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

ZMK_EVENT_IMPL(is_connected_notification);
ZMK_EVENT_IMPL(os_notification);

typedef enum {
    _TIME = 0xAA, // random value, must match companion app
    _VOLUME,
    _OS,
    _LAYOUT,
} hid_data_type;

static bool is_connected = false;

static void on_disconnect_timer(struct k_timer *dummy) {
    LOG_INF("raise_connection_notification: false");
    is_connected = false;
    raise_is_connected_notification((struct is_connected_notification){.value = false});
}

K_TIMER_DEFINE(disconnect_timer, on_disconnect_timer, NULL);

static void process_raw_hid_data(uint8_t *data) {
    LOG_INF("display_process_raw_hid_data - received data_type %u", data[0]);

    // raise disconnect notification after 65 seconds of inactivity
    k_timer_start(&disconnect_timer, K_SECONDS(65), K_NO_WAIT);
    if (!is_connected) {
        LOG_INF("raise_connection_notification: true");
        is_connected = true;
        raise_is_connected_notification((struct is_connected_notification){.value = true});
    }

    uint8_t data_type = data[0];
    switch (data_type) {
    case _OS:
        uint8_t os_name_length = data[1];
        struct os_notification notification = {0};

        if (os_name_length > sizeof(notification.current_host_os) - 1) {
            os_name_length = sizeof(notification.current_host_os) - 1;
        }

        memcpy(notification.current_host_os, &data[2], os_name_length);
        notification.current_host_os[os_name_length] = '\0';

        raise_os_notification(notification);

        break;
    }
}

static int raw_hid_received_event_listener(const zmk_event_t *eh) {
    struct raw_hid_received_event *event = as_raw_hid_received_event(eh);
    if (event) {
        process_raw_hid_data(event->data);
    }

    return ZMK_EV_EVENT_BUBBLE;
}

ZMK_LISTENER(process_raw_hid_event, raw_hid_received_event_listener);
ZMK_SUBSCRIPTION(process_raw_hid_event, raw_hid_received_event);