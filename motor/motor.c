// motor.c

#include "motor.h"
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"


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

