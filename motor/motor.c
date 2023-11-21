// motor.c

#include "motor.h"
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"
#include <stdio.h>
#include <stdint.h>

float Kp = 0.1f;
int Ki = 10;
int Kd = 1;
float rightTotalDistance = 0.0f;
float leftTotalDistance = 0.0f;
float speed_of_right_wheel = 0.0f;
float speed_of_left_wheel = 0.0f;
float right_wheel_pid = 0.0f;
uint64_t current_time = 0.0f;
uint64_t right_last_time = 0.0f;
uint64_t left_last_time = 0.0f;

// init motor
void motor_sensor_init()
{
    // Tell GPIO 0 and 1 they are allocated to the PWM
    gpio_set_function(0, GPIO_FUNC_PWM);
    gpio_set_function(1, GPIO_FUNC_PWM);

    // Setting up the GPIO pins for right motor
    gpio_init(RIGHT_POLLING_PIN);
    gpio_set_dir(RIGHT_POLLING_PIN, GPIO_IN);
    gpio_pull_up(RIGHT_POLLING_PIN);

    // Setting up the GPIO pins for left motor
    gpio_init(LEFT_POLLING_PIN);
    gpio_set_dir(LEFT_POLLING_PIN, GPIO_IN);
    gpio_pull_up(LEFT_POLLING_PIN);

    // Find out which PWM slice is connected to GPIO 0 (it's slice 0)
    uint slice_num = pwm_gpio_to_slice_num(0);

    // Set period of 4 cycles (0 to 3 inclusive)
    pwm_set_wrap(slice_num, 12500);

    // Set channel A output high for one cycle before dropping
    pwm_set_chan_level(slice_num, PWM_CHAN_A, 12500 / DUTY_CYCLE);
    pwm_set_chan_level(slice_num, PWM_CHAN_B, 12500 / DUTY_CYCLE);

    pwm_set_clkdiv(slice_num, 100);
    // Set the PWM running
    pwm_set_enabled(slice_num, true);
    gpio_set_irq_enabled(RIGHT_POLLING_PIN, GPIO_IRQ_EDGE_RISE, true);
    gpio_set_irq_enabled(LEFT_POLLING_PIN, GPIO_IRQ_EDGE_RISE, true);
}

void backward() {
    gpio_pull_down(LEFT_INPUT_PIN);
    gpio_pull_up(LEFT_INPUT_PIN_2);
    gpio_pull_down(RIGHT_INPUT_PIN);
    gpio_pull_up(RIGHT_INPUT_PIN_2);
}

void forward() {
    gpio_pull_up(LEFT_INPUT_PIN);
    gpio_pull_down(LEFT_INPUT_PIN_2);
    gpio_pull_up(RIGHT_INPUT_PIN);
    gpio_pull_down(RIGHT_INPUT_PIN_2);
}

void turn_right() {
    gpio_pull_down(LEFT_INPUT_PIN);
    gpio_pull_up(LEFT_INPUT_PIN_2);
    gpio_pull_up(RIGHT_INPUT_PIN);
    gpio_pull_down(RIGHT_INPUT_PIN_2);
}

void turn_left() {
    gpio_pull_up(LEFT_INPUT_PIN);
    gpio_pull_down(LEFT_INPUT_PIN_2);
    gpio_pull_down(RIGHT_INPUT_PIN);
    gpio_pull_up(RIGHT_INPUT_PIN_2);
}

void slow_down(int current_speed) {
        if (current_speed >= 4) {   // If duty cycle is already at 100%, don't increase it
            return;
        }
        int target_speed = current_speed + 1; // Calculate the target speed (increase by 25%)
        while (current_speed < target_speed) {
            set_speed(++current_speed); // Increase duty cycle by 25%
            sleep_ms(1000);
    }
}

void stop() {
    gpio_pull_up(LEFT_INPUT_PIN);
    gpio_pull_up(LEFT_INPUT_PIN_2);
    gpio_pull_up(RIGHT_INPUT_PIN);
    gpio_pull_up(RIGHT_INPUT_PIN_2);
}

uint32_t pwm_set_freq_duty(unsigned int slice_num, unsigned int chan, uint32_t f, int d)
{
    uint32_t clock = 125000000;
    uint32_t divider16 = clock / f / 4096 + (clock % (f * 4096) != 0);
    if (divider16 / 16 == 0)
    {
        divider16 = 16;
    }
    uint32_t wrap = clock * 16 / divider16 / f - 1;
    pwm_set_clkdiv_int_frac(slice_num, divider16 / 16, divider16 & 0xF);
    pwm_set_wrap(slice_num, wrap);
    pwm_set_chan_level(slice_num, chan, wrap * d / 100);
    return wrap;
}

void set_speed(int speed_level) { // level is between 0 and 4, 2 being 50% duty cycle
    uint slice_num = pwm_gpio_to_slice_num(0);

    // Set period of 4 cycles (0 to 3 inclusive)
    pwm_set_wrap(slice_num, 12500);

    // Division variable
    int div = 12500 / speed_level;

    pwm_set_chan_level(slice_num, PWM_CHAN_A, div);
    pwm_set_chan_level(slice_num, PWM_CHAN_B, div);

    pwm_set_clkdiv(slice_num, 100);
    // Set the PWM running
    pwm_set_enabled(slice_num, true);
}

// void set_speed(int speed_level) 
// {
//     uint slice_num = pwm_gpio_to_slice_num(0);
//     uint chan1 = PWM_CHAN_A; // Assuming you are using channel A
//     uint chan2 = PWM_CHAN_B; // Assuming you are using channel B

//     // Safety check for speed_level
//     if (speed_level < 1 || speed_level > 4) {
//         pwm_set_chan_level(slice_num, chan1, 0); // Set duty cycle to 0 (motor stop)
//         pwm_set_chan_level(slice_num, chan2, 0); // Set duty cycle to 0 (motor stop)
//         return;
//     }

//     // Calculate duty cycle based on speed_level
//     int div = 12500 / speed_level; // Original logic for calculating duty cycle

//     // Set duty cycle for PWM channels
//     pwm_set_chan_level(slice_num, chan1, div);
//     pwm_set_chan_level(slice_num, chan2, div);

//     // No need to change frequency or re-enable PWM as it's already set in init
// }

// PID controller function
float calculate_pid(float current_speed, float desired_speed, float previous_error, float integration_sum) {
    // Calculate error terms
    float error = desired_speed - current_speed;
    float integral = integration_sum + error;
    float derivative = error - previous_error;

    // Calculate PID value
    float pid = Kp * error + Ki * integral + Kd * derivative;

    return pid;
}

float calculate_speed(float time_difference) {
    float speed = DISTANCE_BETWEEN_NOTCHES_CM / (time_difference / 1000000.0);
    return speed;
}

