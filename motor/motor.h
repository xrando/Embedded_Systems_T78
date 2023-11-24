#ifndef MOTOR_H
#define MOTOR_H

#include <stdio.h>
#include <stdint.h> 

#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"

#define LEFT_INPUT_PIN                2
#define LEFT_INPUT_PIN_2              3

#define RIGHT_INPUT_PIN               6
#define RIGHT_INPUT_PIN_2             7

#define RIGHT_POLLING_PIN             14
#define LEFT_POLLING_PIN              15

#define WHEEL_CIRCUMFERENCE_CM        21.04867078
#define DISTANCE_BETWEEN_NOTCHES_CM   1.0525

#define NOTCHES                       20
#define DUTY_CYCLE                    2

// Global variables
extern int   Ki;
extern int   Kd;
extern float Kp;
extern float right_motor_distance;
extern float left_motor_distance;
extern float speed_of_right_wheel;
extern float speed_of_left_wheel;
extern float right_wheel_pid;
extern uint64_t current_time;
extern uint64_t right_last_time;
extern uint64_t left_last_time;

// Function prototypes
void motor_sensor_init();
void forward();
void backward();
void turn_left();
void turn_right();
void slow_down(int current_speed);
void stop();
void set_speed(int speed_level);
float calculate_pid(float current_speed, float desired_speed,
                    float previous_error, float integration_sum);
float calculate_speed(float time_difference);

#endif // MOTOR_H