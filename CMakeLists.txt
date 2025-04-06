zephyr_library_sources(src/hid.c)
target_sources(src/behaviors/behavior_os_layer.c)
zephyr_include_directories(include)
zephyr_include_directories(${APPLICATION_SOURCE_DIR}/include)