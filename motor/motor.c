#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define LEFT_INPUT_PIN 2
#define LEFT_INPUT_PIN_2 3

#define POLLING_PIN_1 14
#define POLLING_PIN_2 15

#define RIGHT_INPUT_PIN 6
#define RIGHT_INPUT_PIN_2 7

#define WHEEL_CIRCUMFERENCE 0.628 // in meters (0.628m is just an example for a 20cm diameter wheel)

uint64_t last_time = 0;
float speed = 0.0; // Speed in m/s

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

void turn_left() {
    gpio_pull_up(LEFT_INPUT_PIN);
    gpio_pull_down(LEFT_INPUT_PIN_2);
    gpio_pull_down(RIGHT_INPUT_PIN);
    gpio_pull_up(RIGHT_INPUT_PIN_2);
}

void turn_right() {
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

    gpio_init(POLLING_PIN_1);
    gpio_set_dir(POLLING_PIN_1, GPIO_IN);
    gpio_pull_up(POLLING_PIN_1);

    gpio_init(POLLING_PIN_2);
    gpio_set_dir(POLLING_PIN_2, GPIO_IN);
    gpio_pull_up(POLLING_PIN_2);


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

    while (1) {
        forward();
        bool pin_state_1 = gpio_get(POLLING_PIN_1);
        bool pin_state_2 = gpio_get(POLLING_PIN_2);

        if (pin_state_1 && pin_state_2) {  // Let's assume pin goes HIGH when the magnet is close
            // uint64_t current_time = to_us_since_boot(get_absolute_time());
            // uint64_t time_difference = current_time - last_time;

            // if(last_time != 0 && time_difference > 0) {
            //     speed = WHEEL_CIRCUMFERENCE / (time_difference / 1e6f); // in m/s
            //     printf("Speed: %.2f m/s\n", speed);
            //     sleep_ms(500);
            // }

            // last_time = current_time;
            //printf("GPIO %d is HIGH\n", POLLING_PIN_1);
            printf("GPIO %d is HIGH\n", POLLING_PIN_2);
        }
        else {
            //printf("GPIO %d is LOW\n", POLLING_PIN_1);
            printf("GPIO %d is LOW\n", POLLING_PIN_2);
        }

        //backward();
        //sleep_ms(1000);
        //turn_left();
        //sleep_ms(1000);
        //turn_right();
        //sleep_ms(1000);
    }
    return 0;
}