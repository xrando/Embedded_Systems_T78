// motor.h

#ifndef MOTOR_H
#define MOTOR_H

#include <stdint.h>  // Library for uint64_t data type

// Pins to control the Left Motor
#define LEFT_INPUT_PIN 2
#define LEFT_INPUT_PIN_2 3

// Pins to control the Right Motor
#define RIGHT_INPUT_PIN 6
#define RIGHT_INPUT_PIN_2 7

// Right Motor IR Sensor Pin
#define RIGHT_POLLING_PIN 14

// Left Motor IR Sensor Pin
#define LEFT_POLLING_PIN 15

// Wheel Circumference in cm
#define WHEEL_CIRCUMFERENCE_CM 21.04867078

// Distance between notches in cm
#define DISTANCE_BETWEEN_NOTCHES_CM 1.0525

// Encoder Notches
#define NOTCHES 20

// Debounce time in microseconds
// #define DEBOUNCE_TIME_USEC 50000

// Duty cycle for the PWM
// 2 in this case is 50% duty cycle
#define DUTY_CYCLE 2

// Global variables
extern float Kp;
extern int   Ki;
extern int   Kd;
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
