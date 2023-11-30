project(EMBEDDED_SYSTEMS_T78)

add_executable(
    main
    main.c
    ../line_sensor/line_sensor.c
    ../magnetometer/magnetometer.c
    ../motor/motor.c
    ../ultrasonic/ultrasonic.c
    ../barcode_module/barcode_module.c
)

# pull in common dependencies
target_link_libraries(main pico_stdlib hardware_i2c hardware_adc hardware_pwm)

# Include directories for the target
target_include_directories(main PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/../magnetometer
    ${CMAKE_CURRENT_LIST_DIR}/../motor
    ${CMAKE_CURRENT_LIST_DIR}/../line_sensor
    ${CMAKE_CURRENT_LIST_DIR}/../ultrasonic
    ${CMAKE_CURRENT_LIST_DIR}/../barcode_module
)

# enable usb output
pico_enable_stdio_usb(main 1)

# create map/bin/hex file etc.
pico_add_extra_outputs(main)

# add url via pico_set_program_url
example_auto_set_url(main)