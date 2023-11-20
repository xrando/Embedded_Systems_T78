// main.c

#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"
#include "motor.h"



// PID constants
float Kp = 0.1;
int Ki = 10;
int Kd = 1;

// Global variables for calculating distance
float rightTotalDistance = 0.0;
float leftTotalDistance = 0.0;

// Global variables to store the speed of the wheels
float speed_of_right_wheel = 0.0;
float speed_of_left_wheel = 0.0;

// PID value for the right wheel
float right_wheel_pid = 0.0;

// Time variables to calculate speed
uint64_t current_time = 0;
uint64_t right_last_time = 0;
uint64_t left_last_time = 0;

// PID controller function, shifted over to motor.c
// static float calculate_pid(float current_speed, float desired_speed, float previous_error, float integration_sum) {
//     // Calculate error terms
//     float error = desired_speed - current_speed;
//     float integral = integration_sum + error;
//     float derivative = error - previous_error;

//     // Calculate PID value
//     float pid = Kp * error + Ki * integral + Kd * derivative;

//     return pid;
// }

// static float calculate_speed(float time_difference) {
//     float speed = DISTANCE_BETWEEN_NOTCHES_CM / (time_difference / 1000000.0);
//     return speed;
// }


// shifted over to sensor_isr in line_sensor
// void gpio_callback(uint gpio, uint32_t events) {
//     uint64_t current_time = time_us_64();
//     uint64_t right_trigger_time = 0;
//     uint64_t left_trigger_time = 0;

//     if (gpio == RIGHT_POLLING_PIN) {
//         if (current_time - right_trigger_time > DEBOUNCE_TIME_USEC) {
//             uint64_t current_time = time_us_64();
//             uint64_t time_difference = current_time - right_last_time;
//             right_last_time = current_time;
//             speed_of_right_wheel = calculate_speed(time_difference);
//             // right_wheel_pid = calculate_pid(speed_of_right_wheel, 120, 0, 0); // Adjust desired_speed as needed
//             // printf("Right wheel pid: %.2f\n", right_wheel_pid);
//             rightTotalDistance += DISTANCE_BETWEEN_NOTCHES_CM;
//         }
//     } else if (gpio == LEFT_POLLING_PIN) {
//         if (current_time - left_trigger_time > DEBOUNCE_TIME_USEC) {
//             uint64_t current_time = time_us_64();
//             uint64_t time_difference = current_time - left_last_time;
//             left_last_time = current_time;
//             speed_of_left_wheel = calculate_speed(time_difference);
//             // float left_wheel_pid = calculate_pid(speed_of_left_wheel, 120, 0, 0); // Adjust desired_speed as needed
//             // printf("Left wheel pid before: %.2f\n", left_wheel_pid);
//             leftTotalDistance += DISTANCE_BETWEEN_NOTCHES_CM;
//         }
//     }
// }



int main() {
    stdio_init_all();
    motor_sensor_init();
    // // Tell GPIO 0 and 1 they are allocated to the PWM
    // gpio_set_function(0, GPIO_FUNC_PWM);
    // gpio_set_function(1, GPIO_FUNC_PWM);

    // // Setting up the GPIO pins for right motor
    // gpio_init(RIGHT_POLLING_PIN);
    // gpio_set_dir(RIGHT_POLLING_PIN, GPIO_IN);
    // gpio_pull_up(RIGHT_POLLING_PIN);

    // // Setting up the GPIO pins for left motor
    // gpio_init(LEFT_POLLING_PIN);
    // gpio_set_dir(LEFT_POLLING_PIN, GPIO_IN);
    // gpio_pull_up(LEFT_POLLING_PIN);

    // // Find out which PWM slice is connected to GPIO 0 (it's slice 0)
    // uint slice_num = pwm_gpio_to_slice_num(0);

    // // Set period of 4 cycles (0 to 3 inclusive)
    // pwm_set_wrap(slice_num, 12500);

    // // Set channel A output high for one cycle before dropping
    // pwm_set_chan_level(slice_num, PWM_CHAN_A, 12500 / DUTY_CYCLE);
    // pwm_set_chan_level(slice_num, PWM_CHAN_B, 12500 / DUTY_CYCLE);

    // pwm_set_clkdiv(slice_num, 100);
    // // Set the PWM running
    // pwm_set_enabled(slice_num, true);

    // sleep_ms(2000);

    forward();
    sleep_ms(1000);
    // backward();
    // sleep_ms(1000);
    // turn_left();
    // sleep_ms(1000);
    // forward();
    // sleep_ms(2000);
    // turn_right();
    // sleep_ms(1000);
    slow_down(DUTY_CYCLE);
    sleep_ms(1000);
    set_speed(3);
    sleep_ms(1000);
    set_speed(2);
    sleep_ms(1000);
    set_speed(1);
    sleep_ms(1000);
    stop();
    sleep_ms(1000);

    // // Enable interrupts for the GPIO pin 14 and call the gpio_callback function when the interrupt is triggered when the button is pressed
    // gpio_set_irq_enabled_with_callback(RIGHT_POLLING_PIN, GPIO_IRQ_EDGE_RISE, true, &gpio_callback);
    // // Enable interrupts for the GPIO pin 15 and call the gpio_callback function when the interrupt is triggered when the button is pressed
    // gpio_set_irq_enabled_with_callback(LEFT_POLLING_PIN, GPIO_IRQ_EDGE_RISE, true, &gpio_callback);

    while (1) {
        // Wait forever, until interrupt request (IRQ) is received
        printf("Speed of right wheel: %.2f cm/s\n", speed_of_right_wheel);
        printf("Speed of left wheel: %.2f cm/s\n", speed_of_left_wheel);
        printf("Total distance travelled by right wheel: %.2f cm\n", rightTotalDistance);
        printf("Total distance travelled by left wheel: %.2f cm\n", leftTotalDistance);
        sleep_ms(2000);
    }
}
