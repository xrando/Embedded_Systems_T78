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
float right_motor_distance = 0.0;
float left_motor_distance = 0.0;

// Global variables to store the speed of the wheels
float speed_of_right_wheel = 0.0;
float speed_of_left_wheel = 0.0;

// PID value for the right wheel
float right_wheel_pid = 0.0;

// Time variables to calculate speed
uint64_t current_time = 0;
uint64_t right_last_time = 0;
uint64_t left_last_time = 0;

int main() {
    stdio_init_all();
    motor_sensor_init();
    forward();
    sleep_ms(1000);
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

    for(;;)
    {
        // Wait forever, until interrupt request (IRQ) is received
        printf("Speed of right wheel: %.2f cm/s\n", speed_of_right_wheel);
        printf("Speed of left wheel: %.2f cm/s\n", speed_of_left_wheel);
        printf("Total distance travelled by right wheel: %.2f cm\n",
                right_motor_distance);
        printf("Total distance travelled by left wheel: %.2f cm\n",
                left_motor_distance);
        sleep_ms(2000);
    }
}