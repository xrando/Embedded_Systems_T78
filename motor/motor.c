// Output PWM signals on pins 0 and 1

#include "pico/stdlib.h"
#include "hardware/pwm.h"

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

void turn_left(){
    gpio_pull_down(LEFT_INPUT_PIN);
    gpio_pull_up(LEFT_INPUT_PIN_2);
    gpio_pull_up(RIGHT_INPUT_PIN);
    gpio_pull_down(RIGHT_INPUT_PIN_2);
}

void turn_right(){
    gpio_pull_up(LEFT_INPUT_PIN);
    gpio_pull_down(LEFT_INPUT_PIN_2);
    gpio_pull_down(RIGHT_INPUT_PIN);
    gpio_pull_up(RIGHT_INPUT_PIN_2);
}

int main() {

    // Tell GPIO 0 and 1 they are allocated to the PWM
    gpio_set_function(0, GPIO_FUNC_PWM);
    gpio_set_function(1, GPIO_FUNC_PWM);

    // Find out which PWM slice is connected to GPIO 0 (it's slice 0)
    uint slice_num = pwm_gpio_to_slice_num(0);

    // Set period of 4 cycles (0 to 3 inclusive)
    pwm_set_wrap(slice_num, 12500);

    // Set channel A output high for one cycle before dropping
    pwm_set_chan_level(slice_num, PWM_CHAN_A, 12500/2);
    pwm_set_chan_level(slice_num, PWM_CHAN_B, 12500/2);

    // Set initial B output high for three cycles before dropping
    // pwm_set_chan_level(slice_num, PWM_CHAN_B, 3);
    pwm_set_clkdiv(slice_num, 100);
    // Set the PWM running
    pwm_set_enabled(slice_num, true);

    for (;;) {
        // forward();
        // sleep_ms(5000);
        // backward();
        // sleep_ms(5000);
        turn_left();
        sleep_ms(5000);
        turn_right();
        sleep_ms(5000);
    }
    
    // Note we could also use pwm_set_gpio_level(gpio, x) which looks up the
    // correct slice and channel for a given GPIO.
}
