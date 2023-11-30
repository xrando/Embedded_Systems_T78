project(EMBEDDED_SYSTEMS_T78)

add_executable(
    barcode_module
    barcode_module.c
    main.c
    barcode_module.h
)

# pull in common dependencies
target_link_libraries(barcode_module pico_stdlib)

# enable usb output
pico_enable_stdio_usb(barcode_module 1)

# create map/bin/hex file etc.
pico_add_extra_outputs(barcode_module)

# add url via pico_set_program_url
example_auto_set_url(barcode_module)