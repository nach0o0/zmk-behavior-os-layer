if ((NOT CONFIG_ZMK_SPLIT) OR CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
#zephyr_library()

    target_sources(app PRIVATE src/behaviors/behavior_os_layer.c)

#target_link_libraries(app PRIVATE zmk)

endif()