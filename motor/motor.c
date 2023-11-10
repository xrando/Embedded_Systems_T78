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

// Debounce time in microseconds
#define DEBOUNCE_TIME_USEC 50000

// Duty cycle for the PWM
// 2 in this case is 50% duty cycle as the range is 0 to 4
#define DUTY_CYCLE 2

// PID constants
float Kp = 0.1;
int Ki = 10;
int Kd = 1;

// Global variables for calculating distance
float rightTotalDistance = 0.0;
float leftTotalDistance = 0.0;

// Global variables to store the speed of the wheels
volatile float speed_of_right_wheel = 0.0;
volatile float speed_of_left_wheel = 0.0;

// PID value for the right wheel
volatile float right_wheel_pid = 0.0;

// Time variables to calculate speed
uint16_t current_time = 0;
uint64_t right_last_time = 0;
uint64_t left_last_time = 0;

// PID controller function
static float calculate_pid(float current_speed, float desired_speed, float previous_error, float integration_sum) {

    // Calculate error terms
    float error = desired_speed - current_speed;
    float integral = integration_sum + error;
    float derivative = error - previous_error;

    // Calculate PID value
    float pid = Kp * error + Ki * integral + Kd * derivative;

    
    return pid;
}

static float calculate_speed(float time_difference){
    float speed = DISTANCE_BETWEEN_NOTCHES_CM / (time_difference/1000000.0);
    return speed;
}

void gpio_callback(uint gpio, uint32_t events) {
    uint64_t current_time = time_us_64();
    uint64_t right_trigger_time = 0;
    uint64_t left_trigger_time = 0;

    if (gpio == RIGHT_POLLING_PIN)
    {
        if(current_time - right_trigger_time > DEBOUNCE_TIME_USEC){
            uint64_t current_time = time_us_64();
            uint64_t time_difference = current_time - right_last_time;
            right_last_time = current_time;
            speed_of_right_wheel = calculate_speed(time_difference);
            // right_wheel_pid = calculate_pid(speed_of_right_wheel, 120, 0, 0); // Adjust desired_speed as needed
            // printf("Right wheel pid: %.2f\n", right_wheel_pid);
            rightTotalDistance += DISTANCE_BETWEEN_NOTCHES_CM;
        }
    }
    else if (gpio == LEFT_POLLING_PIN){
        if(current_time - left_trigger_time > DEBOUNCE_TIME_USEC){
            uint64_t current_time = time_us_64();
            uint64_t time_difference = current_time - left_last_time;
            left_last_time = current_time;
            speed_of_left_wheel = calculate_speed(time_difference);
            // float left_wheel_pid = calculate_pid(speed_of_left_wheel, 120, 0, 0); // Adjust desired_speed as needed
            // printf("Left wheel pid before: %.2f\n", left_wheel_pid);
            leftTotalDistance += DISTANCE_BETWEEN_NOTCHES_CM;
        }    
    }
}

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

static void turn_left() {
    gpio_pull_down(LEFT_INPUT_PIN);  // gpio_pull_down(LEFT_INPUT_PIN)
    gpio_pull_up(LEFT_INPUT_PIN_2);
    gpio_pull_up(RIGHT_INPUT_PIN);
    gpio_pull_down(RIGHT_INPUT_PIN_2);
}

static void turn_right() {
    gpio_pull_up(LEFT_INPUT_PIN);
    gpio_pull_down(LEFT_INPUT_PIN_2);
    gpio_pull_down(RIGHT_INPUT_PIN);  // gpio_pull_down(RIGHT_INPUT_PIN);
    gpio_pull_up(RIGHT_INPUT_PIN_2);
}

static void stop() {
    gpio_pull_up(LEFT_INPUT_PIN);
    gpio_pull_up(LEFT_INPUT_PIN_2);
    gpio_pull_up(RIGHT_INPUT_PIN);
    gpio_pull_up(RIGHT_INPUT_PIN_2);
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
    pwm_set_chan_level(slice_num, PWM_CHAN_A, 12500 / DUTY_CYCLE);
    pwm_set_chan_level(slice_num, PWM_CHAN_B, 12500 / DUTY_CYCLE);

    pwm_set_clkdiv(slice_num, 100);
    // Set the PWM running
    pwm_set_enabled(slice_num, true);
    
    sleep_ms(2000);

    forward();
    sleep_ms(2000);
    // backward();
    // sleep_ms(2000);
    turn_left();
    sleep_ms(2000);
    forward();
    sleep_ms(2000);
    turn_right();
    sleep_ms(2000);
    forward();
    sleep_ms(2000);
    stop();
    sleep_ms(1000);

    // Enable interrupts for the GPIO pin 14 and call the gpio_callback function when the interrupt is triggered when the button is pressed
    gpio_set_irq_enabled_with_callback(RIGHT_POLLING_PIN, GPIO_IRQ_EDGE_RISE, true, &gpio_callback);
    // Enable interrupts for the GPIO pin 15 and call the gpio_callback function when the interrupt is triggered when the button is pressed
    gpio_set_irq_enabled_with_callback(LEFT_POLLING_PIN, GPIO_IRQ_EDGE_RISE, true, &gpio_callback);

    while (1) {
        // Wait forever, until interrupt request (IRQ) is received
        printf("Speed of right wheel: %.2f cm/s\n", speed_of_right_wheel);
        printf("Speed of left wheel: %.2f cm/s\n", speed_of_left_wheel);
        printf("Total distance travelled by right wheel: %.2f cm\n", rightTotalDistance);
        printf("Total distance travelled by left wheel: %.2f cm\n", leftTotalDistance);
        sleep_ms(2000);
    }
}