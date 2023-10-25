add_executable(ultrasonic_sensor
        ultrasonic_sensor.c
        )

# pull in common dependencies
target_link_libraries(ultrasonic_sensor pico_stdlib hardware_gpio hardware_timer)

# create map/bin/hex file etc.
pico_add_extra_outputs(ultrasonic_sensor)

# add url via pico_set_program_url
example_auto_set_url(ultrasonic_sensor)

pico_enable_stdio_usb(ultrasonic_sensor 1)