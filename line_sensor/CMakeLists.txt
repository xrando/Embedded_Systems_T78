project(EMBEDDED_SYSTEMS_T78)

add_executable(
        line_sensor
        line_sensor.c
        line_sensor.h
        main.c
        ../motor/motor.c
        ../barcode_module/barcode_module.c
        )
        
# Specify the include directories
target_include_directories(line_sensor PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../motor
        ${CMAKE_CURRENT_SOURCE_DIR}/../barcode_module
)

# pull in common dependencies
target_link_libraries(line_sensor pico_stdlib)

# enable usb output
pico_enable_stdio_usb(line_sensor 1)

# create map/bin/hex file etc.
pico_add_extra_outputs(line_sensor)

# add url via pico_set_program_url
example_auto_set_url(line_sensor)