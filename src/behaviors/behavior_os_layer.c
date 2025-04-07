#define DT_DRV_COMPAT zmk_behavior_os_layer


#include <zephyr/device.h>
#include <drivers/behavior.h>
#include <zephyr/logging/log.h>

#include <zmk/keymap.h>
#include <zmk/behavior.h>

LOG_MODULE_REGISTER(behavior_os_layer, CONFIG_ZMK_LOG_LEVEL);

#if DT_HAS_COMPAT_STATUS_OKAY(DT_DRV_COMPAT)

static char current_host_os[30] = "windows";

struct behavior_os_layer_config {
    int linux_layer;
    int windows_layer;
    int macos_layer;
    int android_layer;
    int ios_layer;
    int default_layer;
};

static int behavior_os_layer_keymap_binding_pressed(struct zmk_behavior_binding *binding,
                                                    struct zmk_behavior_binding_event event) {
                                                        
    LOG_DBG("Behavior os-layer pressed. Current hardcoded OS: %s", current_host_os);
    const struct behavior_os_layer_config *cfg = zmk_behavior_get_binding(binding->behavior_dev)->config;
    
    int layer_to_activate = cfg->default_layer;

    if (strcmp(current_host_os, "linux") == 0) {
        layer_to_activate = cfg->linux_layer;
    } else if (strcmp(current_host_os, "windows") == 0) {
        layer_to_activate = cfg->windows_layer;
    } else if (strcmp(current_host_os, "macos") == 0) {
        layer_to_activate = cfg->macos_layer;
    } else if (strcmp(current_host_os, "android") == 0) {
        layer_to_activate = cfg->android_layer;
    } else if (strcmp(current_host_os, "ios") == 0) {
        layer_to_activate = cfg->ios_layer;
    } else {
        layer_to_activate = cfg->default_layer;
    }

    LOG_DBG("position %d layer %d ", event.position, layer_to_activate);
    zmk_keymap_layer_activate(layer_to_activate);
    return ZMK_BEHAVIOR_OPAQUE;
}

static int behavior_os_layer_keymap_binding_released(struct zmk_behavior_binding *binding,
                                                     struct zmk_behavior_binding_event event) {
    
    const struct behavior_os_layer_config *cfg = zmk_behavior_get_binding(binding->behavior_dev)->config;

    int layer_to_deactivate = cfg->default_layer;

    if (strcmp(current_host_os, "linux") == 0) {
        layer_to_deactivate = cfg->linux_layer;
    } else if (strcmp(current_host_os, "windows") == 0) {
        layer_to_deactivate = cfg->windows_layer;
    } else if (strcmp(current_host_os, "macos") == 0) {
        layer_to_deactivate = cfg->macos_layer;
    } else if (strcmp(current_host_os, "android") == 0) {
        layer_to_deactivate = cfg->android_layer;
    } else if (strcmp(current_host_os, "ios") == 0) {
        layer_to_deactivate = cfg->ios_layer;
    } else {
        layer_to_deactivate = cfg->default_layer;
    }

    LOG_DBG("Deactivating layer: %d", layer_to_deactivate);
    zmk_keymap_layer_deactivate(layer_to_deactivate);
    return ZMK_BEHAVIOR_OPAQUE;
}

static const struct behavior_driver_api behavior_os_layer_driver_api = {
    .binding_pressed = behavior_os_layer_keymap_binding_pressed,
    .binding_released = behavior_os_layer_keymap_binding_released,
};


#define OSL_INST(n) \
    static const struct behavior_os_layer_config behavior_os_layer_config_##n = { \
        .linux_layer = DT_INST_PROP(n, linux_layer), \
        .windows_layer = DT_INST_PROP(n, windows_layer), \
        .macos_layer = DT_INST_PROP(n, macos_layer), \
        .android_layer = DT_INST_PROP(n, android_layer), \
        .ios_layer = DT_INST_PROP(n, ios_layer), \
        .default_layer = DT_INST_PROP(n, default_layer), \
    }; \
    BEHAVIOR_DT_INST_DEFINE(n, \
        NULL, NULL, \
        NULL, &behavior_os_layer_config_##n, \
        POST_KERNEL, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT, \
        &behavior_os_layer_driver_api);

#endif
