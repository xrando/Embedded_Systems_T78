add_executable(
    magnetometer
    magnetometer.h
    magnetometer.c
    main.c)

# pull in common dependencies
target_link_libraries(magnetometer pico_stdlib hardware_i2c)

# enable usb output, enable uart output
pico_enable_stdio_usb(magnetometer 1)
pico_enable_stdio_uart(magnetometer 1)

# create map/bin/hex file etc.
pico_add_extra_outputs(magnetometer)

# add url via pico_set_program_url
example_auto_set_url(magnetometer)