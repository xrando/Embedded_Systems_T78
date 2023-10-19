#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

float Kp = -1.0; // Set to negative to move backwords
float Ki = 0.1;
float Kd = 0.01;

// Function to compute the control signal
float compute_pid(
    float setpoint,
    float current_value,
    float *integral,
    float *prev_error) {

    float error = setpoint - current_value;
    *integral += error;
    float derivative = error - *prev_error;
    float control_signal = Kp * error + Ki * (*integral) + Kd * derivative;
    *prev_error = error;
    return control_signal;
}

#define LEFT_INPUT_PIN 2
#define LEFT_INPUT_PIN_2 3

#define RIGHT_INPUT_PIN 6
#define RIGHT_INPUT_PIN_2 7

static void forward() {
    gpio_pull_up(LEFT_INPUT_PIN);
    gpio_pull_down(LEFT_INPUT_PIN_2);
    gpio_pull_up(RIGHT_INPUT_PIN);
    gpio_pull_down(RIGHT_INPUT_PIN_2);
}

static void backward() {
    gpio_pull_down(LEFT_INPUT_PIN);
    gpio_pull_up(LEFT_INPUT_PIN_2);
    gpio_pull_down(RIGHT_INPUT_PIN);
    gpio_pull_up(RIGHT_INPUT_PIN_2);
}

void turn_left() {
    gpio_pull_down(LEFT_INPUT_PIN);
    gpio_pull_up(LEFT_INPUT_PIN_2);
    gpio_pull_up(RIGHT_INPUT_PIN);
    gpio_pull_down(RIGHT_INPUT_PIN_2);
}

void turn_right() {
    gpio_pull_up(LEFT_INPUT_PIN);
    gpio_pull_down(LEFT_INPUT_PIN_2);
    gpio_pull_down(RIGHT_INPUT_PIN);
    gpio_pull_up(RIGHT_INPUT_PIN_2);
}

int main() {
    float setpoint = 100.0;     // Desired position
    float current_value = 0.0;  // Current position
    float integral = 0.0;       // Integral term
    float prev_error = 0.0;     // Previous error term
    float time_step = 0.1;

    int num_iterations = 100;

    stdio_init_all();

    // Tell GPIO 0 and 1 they are allocated to the PWM
    gpio_set_function(0, GPIO_FUNC_PWM);
    gpio_set_function(1, GPIO_FUNC_PWM);

    // Find out which PWM slice is connected to GPIO 0 (it's slice 0)
    uint slice_num = pwm_gpio_to_slice_num(0);

    // Set period of 4 cycles (0 to 3 inclusive)
    pwm_set_wrap(slice_num, 12500);

    // Set channel A output high for one cycle before dropping
    pwm_set_chan_level(slice_num, PWM_CHAN_A, 12500 / 2);
    pwm_set_chan_level(slice_num, PWM_CHAN_B, 12500 / 2);

    pwm_set_clkdiv(slice_num, 100);
    // Set the PWM running
    pwm_set_enabled(slice_num, true);

    for (int i = 0; i < num_iterations; i++) {
        float control_signal = compute_pid(
            setpoint,
            current_value,
            &integral,
            &prev_error);

        printf(
            "Iteration %d: Control Signal = %f, Current Position = %f\n",
            i, control_signal, current_value);

        // Thresholds for different motor behaviors
        if (control_signal > 0.1) {
            forward(); // Move forward
        } else if (control_signal < -0.1) {
            backward(); // Move backward
        } else if (control_signal > -0.05 && control_signal < 0.05) {
            // Adjust these thresholds to turn left or right
            turn_left(); // Turn left
        } else if (control_signal < 0.05 && control_signal > -0.05) {
            // Adjust these thresholds to turn left or right
            turn_right(); // Turn right
        }

        sleep_ms((int)(time_step * 1000));

        // For demonstration purposes, you can adjust current_value based on the control_signal
        // In a real-world scenario, you would update current_value based on the motor's feedback
        current_value += control_signal * 0.1; // Adjust current_value based on control signal
    }

    return 0;
}