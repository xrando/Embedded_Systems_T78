#include "motor.h"

// PID constants
float Kp = 0.1f;
int   Ki = 10;
int   Kd = 1;

// Variables to store the distance travelled by the wheels individually
float left_motor_distance  = 0.0f;
float right_motor_distance = 0.0f;

// Variables to store the speed of the wheels
float speed_of_left_wheel  = 0.0f;
float speed_of_right_wheel = 0.0f;

// Variables for PID calculation and time
float right_wheel_pid      = 0.0f;
uint64_t current_time      = 0.0f;
uint64_t left_last_time    = 0.0f;
uint64_t right_last_time   = 0.0f;

// Initialize the motor and IR sensors for wheel encoders
void 
motor_sensor_init ()
{
    // Set GPIO 0 and 1 to run PWM
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

    // Set to count up and wrap around
    pwm_set_wrap(slice_num, 12500);

    // Set duty cycle using period
    pwm_set_chan_level(slice_num, PWM_CHAN_A, 12500 / DUTY_CYCLE);
    pwm_set_chan_level(slice_num, PWM_CHAN_B, 12500 / DUTY_CYCLE);

    // Set maximum clock speed to be divided by 100
    pwm_set_clkdiv(slice_num, 100);

    // Set the PWM running
    pwm_set_enabled(slice_num, true);

    // Enable interrupts for the GPIO pins
    gpio_set_irq_enabled(RIGHT_POLLING_PIN, GPIO_IRQ_EDGE_RISE, true);
    gpio_set_irq_enabled(LEFT_POLLING_PIN, GPIO_IRQ_EDGE_RISE, true);
}

// Function to move the car backward
void 
backward () 
{
    gpio_pull_down(LEFT_INPUT_PIN);
    gpio_pull_up(LEFT_INPUT_PIN_2);
    gpio_pull_down(RIGHT_INPUT_PIN);
    gpio_pull_up(RIGHT_INPUT_PIN_2);
}

// Function to move the car forward
void 
forward () 
{
    gpio_pull_up(LEFT_INPUT_PIN);
    gpio_pull_down(LEFT_INPUT_PIN_2);
    gpio_pull_up(RIGHT_INPUT_PIN);
    gpio_pull_down(RIGHT_INPUT_PIN_2);
}

// Function to turn the car right
void 
turn_right () 
{
    gpio_pull_down(LEFT_INPUT_PIN);
    gpio_pull_up(LEFT_INPUT_PIN_2);
    gpio_pull_up(RIGHT_INPUT_PIN);
    gpio_pull_down(RIGHT_INPUT_PIN_2);
}

// Function to turn the car left
void 
turn_left () 
{
    gpio_pull_up(LEFT_INPUT_PIN);
    gpio_pull_down(LEFT_INPUT_PIN_2);
    gpio_pull_down(RIGHT_INPUT_PIN);
    gpio_pull_up(RIGHT_INPUT_PIN_2);
}

// Function to slow down the car
void 
slow_down (int current_speed) 
{
    if (current_speed >= 4) 
    {
        return;
    }
    // Slows down the motor by 25% every iteration
    int target_speed = current_speed + 1;
    while (current_speed < target_speed) 
    {
        set_speed(++current_speed);
        sleep_ms(1000);
    }
}

// Function to stop the car
void 
stop () 
{
    gpio_pull_up(LEFT_INPUT_PIN);
    gpio_pull_up(LEFT_INPUT_PIN_2);
    gpio_pull_up(RIGHT_INPUT_PIN);
    gpio_pull_up(RIGHT_INPUT_PIN_2);
}

// Function to set the speed of the motor using Duty Cycle
void 
set_speed (int speed_level) 
{
    uint slice_num = pwm_gpio_to_slice_num(0);

    pwm_set_wrap(slice_num, 12500);

    // Division variable
    int div = 12500 / speed_level;

    pwm_set_chan_level(slice_num, PWM_CHAN_A, div);
    pwm_set_chan_level(slice_num, PWM_CHAN_B, div);

    pwm_set_clkdiv(slice_num, 100);
    pwm_set_enabled(slice_num, true);
}

// PID controller function
float 
calculate_pid (float current_speed, float desired_speed,
               float previous_error, float integration_sum) 
{
    // Calculate error terms
    float error      = desired_speed - current_speed;
    float integral   = integration_sum + error;
    float derivative = error - previous_error;

    // Calculate PID value
    float pid = Kp * error + Ki * integral + Kd * derivative;

    return pid;
}

// Function to calculate the speed of the wheels
float 
calculate_speed (float time_difference) 
{
    float speed = DISTANCE_BETWEEN_NOTCHES_CM / (time_difference / 1000000.0);

    return speed;
}