add_executable(barcode_sensor barcode_sensor.c)

# pull in common dependencies
target_link_libraries(barcode_sensor pico_stdlib)

# enable usb output
pico_enable_stdio_usb(barcode_sensor 1)

# create map/bin/hex file etc.
pico_add_extra_outputs(barcode_sensor)

# add url via pico_set_program_url
example_auto_set_url(barcode_sensor)