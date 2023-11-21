project(EMBEDDED_SYSTEMS_T78)

# Create an executable file from the main.c in the main subdirectory
add_executable(
    main
    main.c
    ../line_sensor/line_sensor.c
    ../magnetometer/magnetometer.c
    ../motor/motor.c
    ../ultrasonic/ultrasonic.c
    ../barcode_module/barcode_module.c
    # ../web-ui/wifi.c

    # Include other .c files as necessary
)

# pull in common dependencies
target_link_libraries(main pico_stdlib hardware_i2c hardware_adc hardware_pwm)

# Include directories for the target
target_include_directories(main PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/../magnetometer
    ${CMAKE_CURRENT_LIST_DIR}/../motor
    ${CMAKE_CURRENT_LIST_DIR}/../line_sensor
    ${CMAKE_CURRENT_LIST_DIR}/../ultrasonic
    # ${CMAKE_CURRENT_LIST_DIR}/../barcode_module
    # ${CMAKE_CURRENT_LIST_DIR}/../web-ui
    # ... Add other directories as necessary
)

# enable usb output, enable uart output
pico_enable_stdio_usb(main 1)
pico_enable_stdio_uart(main 1)

# Generate additional build output files (like .bin and .uf2)
pico_add_extra_outputs(main)

# add url via pico_set_program_url
example_auto_set_url(main)