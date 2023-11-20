project(EMBEDDED_SYSTEMS_T78)

add_executable(line_sensor
        line_sensor.c
        main.c
        line_sensor.h
        ../motor/motor.h
        ../barcode_module/barcode_module.h
        )

# pull in common dependencies
target_link_libraries(line_sensor pico_stdlib)

# enable usb output
pico_enable_stdio_usb(line_sensor 1)

# create map/bin/hex file etc.
pico_add_extra_outputs(line_sensor)

# add url via pico_set_program_url
example_auto_set_url(line_sensor)