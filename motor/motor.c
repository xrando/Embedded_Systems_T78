#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"

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

// Enccoder Notches
#define NOTCHES 20

// Counter to keep track of the number of notches
volatile int right_motor_counter = 0;
volatile int left_motor_counter = 0;

// Gloabl variables to store the speed of the wheels
volatile float speed_of_right_wheel = 0.0;
volatile float speed_of_left_wheel = 0.0;

// Time variables to calculate speed
uint64_t right_last_time = 0;
uint64_t left_last_time = 0;

/*  static float calculate_distance (int numberOfNotches) {
    // float distancePerNotch = WHEEL_CIRCUMFERENCE_CM / NOTCHES;
    // float totalDistanceTravelled = (float)numberOfNotches * distancePerNotch;
    //return totalDistanceTravelled;
}   */

static float calculate_speed(float time_difference){
    // uint64_t current_time = time_us_64();
    // float time_difference = (current_time - last_time) / 1000000.0;
    // printf("Current Time: %lld\n", current_time);
    float speed = DISTANCE_BETWEEN_NOTCHES_CM / (time_difference/1000000.0);
    // printf("Time Difference: %.2f\n", time_difference);
    // printf("Speed: %.2f cm/s\n", speed);
    // last_time = current_time;

    return speed;
}

void gpio_callback(uint gpio, uint32_t events) {
    if (gpio == RIGHT_POLLING_PIN)
    {
        uint64_t current_time = time_us_64();
        uint64_t time_difference = current_time - right_last_time;
        right_last_time = current_time;
        // float totalDistanceTravelled = calculate_distance(right_motor_counter);
        speed_of_right_wheel = calculate_speed(time_difference);
        right_motor_counter++;
    }
    else if (gpio == LEFT_POLLING_PIN){
        uint64_t current_time = time_us_64();
        uint64_t time_difference = current_time - left_last_time;
        left_last_time = current_time;
        // float totalDistanceTravelled = calculate_distance(left_motor_counter);
        speed_of_left_wheel = calculate_speed(time_difference);
        left_motor_counter++;
    
    }
}

static void forward() {
    gpio_pull_down(LEFT_INPUT_PIN);
    gpio_pull_up(LEFT_INPUT_PIN_2);
    gpio_pull_down(RIGHT_INPUT_PIN);
    gpio_pull_up(RIGHT_INPUT_PIN_2);
}

static void backward() {
    gpio_pull_up(LEFT_INPUT_PIN);
    gpio_pull_down(LEFT_INPUT_PIN_2);
    gpio_pull_up(RIGHT_INPUT_PIN);
    gpio_pull_down(RIGHT_INPUT_PIN_2);
}

static void turn_left() {
    gpio_pull_up(LEFT_INPUT_PIN);
    gpio_pull_down(LEFT_INPUT_PIN_2);
    gpio_pull_down(RIGHT_INPUT_PIN);
    gpio_pull_up(RIGHT_INPUT_PIN_2);
}

static void turn_right() {
    gpio_pull_down(LEFT_INPUT_PIN);
    gpio_pull_up(LEFT_INPUT_PIN_2);
    gpio_pull_up(RIGHT_INPUT_PIN);
    gpio_pull_down(RIGHT_INPUT_PIN_2);
}

int main() {
    
    stdio_init_all();

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
    pwm_set_chan_level(slice_num, PWM_CHAN_A, 12500 / 2);
    pwm_set_chan_level(slice_num, PWM_CHAN_B, 12500 / 2);

    pwm_set_clkdiv(slice_num, 100);
    // Set the PWM running
    pwm_set_enabled(slice_num, true);
    
    forward();

    // Enable interrupts for the GPIO pin 14 and call the notchCount function when the interrupt is triggered when the button is pressed
    gpio_set_irq_enabled_with_callback(RIGHT_POLLING_PIN, GPIO_IRQ_EDGE_RISE, true, &gpio_callback);
    // Enable interrupts for the GPIO pin 15 and call the notchcount2 function when the interrupt is triggered when the button is pressed
    gpio_set_irq_enabled_with_callback(LEFT_POLLING_PIN, GPIO_IRQ_EDGE_RISE, true, &gpio_callback);

    while (1) {
        // Wait forever, until interrupt request (IRQ) is received
        printf("Speed of right wheel: %.2f cm/s\n", speed_of_right_wheel);
        sleep_ms(500);
        printf("Speed of left wheel: %.2f cm/s\n", speed_of_left_wheel);
        sleep_ms(500);
    }
}