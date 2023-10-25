add_executable(
        motor
        motor.c
        )

# pull in common dependencies and additional pwm hardware support
<<<<<<< HEAD
target_link_libraries(motor pico_stdlib hardware_pwm hardware_adc) # hardware_pwm hardware_adc

# create map/bin/hex file etc.
pico_add_extra_outputs(motor 1)

# add url via pico_set_program_url
example_auto_set_url(motor 1)
=======
target_link_libraries(motor pico_stdlib hardware_pwm)

# create map/bin/hex file etc.
pico_add_extra_outputs(motor)

# add url via pico_set_program_url
example_auto_set_url(motor)
>>>>>>> 6be1434 (cmakelist done)
