# folder name then file name
add_executable(motor 
                motor.c
                motor.h
                # main.c
                )

# pull in common dependencies
target_link_libraries(motor pico_stdlib pico_cyw43_arch_none hardware_pwm hardware_adc hardware_gpio)

# enable usb output, enable uart output
pico_enable_stdio_usb(motor 1)
pico_enable_stdio_uart(motor 1)

# create map/bin/hex file etc.
pico_add_extra_outputs(motor 2)

# add url via pico_set_program_url
example_auto_set_url(motor 1)