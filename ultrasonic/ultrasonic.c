#include "ultrasonic.h"

void 
init_ultrasonic (uint trigPin, uint echoPin)
{
    gpio_init(trigPin);
    gpio_init(echoPin);
    gpio_set_dir(trigPin, GPIO_OUT);
    gpio_set_dir(echoPin, GPIO_IN);
    gpio_pull_up(echoPin);
}

float 
get_pulse (uint trigPin, uint echoPin)
{
    time_t time1, time2;
    gpio_put(trigPin, 1);
    sleep_us(10);
    gpio_put(trigPin, 0);
    sleep_us(2);

    while (gpio_get(echoPin) == 0)
    {
        time1 = time_us_64();
        sleep_us(100);
    }

    while (gpio_get(echoPin) != 0)
    {
        time2 = time_us_64();
        sleep_us(100);
    }

    float pulseLength = time2 - time1;

    return pulseLength;
}

float 
get_distance (uint trigPin, uint echoPin)
{
    int pulseLength    = ultrasonic_get_pulse(trigPin, echoPin);
    float speedOfSound = 343.0;
    float distanceCm   = (pulseLength * .034) / 2;

    return distanceCm;
}

float 
calculate_kalman_filter (float U)
{
    static const float R = 40;
    static const float H = 1.00;

    static float Q       = 10;
    static float P       = 0;
    static float K       = 0;
    static float U_hat   = 0;

    K      = P * H / (H * P * H + R);
    U_hat += +K * (U - H * U_hat);
    P      = (1 - K * H) * P + Q;

    return U_hat;
}