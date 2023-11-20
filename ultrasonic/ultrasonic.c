// // Get readings from ultrasonic sensor

// #include "pico/stdlib.h"
// #include <stdio.h>
// #include "hardware/gpio.h"
// #include "hardware/timer.h"
// #include <time.h>

// void init(uint trigPin, uint echoPin)
// {
//     stdio_init_all();

//     gpio_init(trigPin);
//     gpio_init(echoPin);

//     gpio_set_dir(trigPin, GPIO_OUT);
//     gpio_set_dir(echoPin, GPIO_IN);

//     gpio_pull_up(echoPin);
// }

// float getPulse(uint trigPin, uint echoPin)
// {
//     time_t time1, time2;

//     gpio_put(trigPin, 1);
//     sleep_us(10);
//     gpio_put(trigPin, 0);
//     sleep_us(2);

//     while (gpio_get(echoPin) == 0)
//     {
//         // loop1++;
//         time1 = time_us_64();
//         sleep_us(100);

//     }

//     while (gpio_get(echoPin) != 0)
//     {
//         // loop2++;
//         time2 = time_us_64();
//         sleep_us(100);
//     }

//     float pulseLength = time2 - time1;
//     return pulseLength;
// }

// float getCm(uint trigPin, uint echoPin)
// {
//     int pulseLength = getPulse(trigPin, echoPin);
//     float speedOfSound = 343.0;

//     // Convert pulse length to distance in centimeters
//     //float distanceCm = (pulseLength * speedOfSound) / 20000.0; // Dividing by 20000 to convert from microseconds to seconds
//     float distanceCm = (pulseLength * .034)/2;
//     return distanceCm;
//     //return (float)pulseLength / 2 * 0.0343;
//     //return ((float)pulseLength * 0.0343) / 2;
// }

// float kalman(float U){
//     static const float R = 40;
//     static const float H = 1.00;
//     static float Q = 10;
//     static float P = 0;
//     static float U_hat = 0;
//     static float K = 0;
//     K = P*H/(H*P*H+R);
//     U_hat += + K*(U-H*U_hat);
//     P = (1-K*H)*P+Q;
//     return U_hat;
// }

// int main()
// {
//     uint trigPin = 4;
//     uint echoPin = 5;

//     printf("Main running...\n");
//     sleep_ms(5000);

//     init(trigPin, echoPin);

//     while (1)
//     {
//         printf("Object is about %.2f cm away (Original) \n", getCm(trigPin, echoPin));
//         printf("Object is about %.2f cm away (Kalman)\n", kalman(getCm(trigPin, echoPin)));
//         sleep_ms(500);
//     }
// }

// ultrasonic.c

#include "ultrasonic.h"

void ultrasonic_init(uint trigPin, uint echoPin)
{
    // stdio_init_all();

    gpio_init(trigPin);
    gpio_init(echoPin);

    gpio_set_dir(trigPin, GPIO_OUT);
    gpio_set_dir(echoPin, GPIO_IN);

    gpio_pull_up(echoPin);
}

float ultrasonic_get_pulse(uint trigPin, uint echoPin)
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

float ultrasonic_get_distance(uint trigPin, uint echoPin)
{
    int pulseLength = ultrasonic_get_pulse(trigPin, echoPin);
    float speedOfSound = 343.0;

    float distanceCm = (pulseLength * .034) / 2;
    return distanceCm;
}

float ultrasonic_kalman_filter(float U)
{
    static const float R = 40;
    static const float H = 1.00;
    static float Q = 10;
    static float P = 0;
    static float U_hat = 0;
    static float K = 0;

    K = P * H / (H * P * H + R);
    U_hat += +K * (U - H * U_hat);
    P = (1 - K * H) * P + Q;

    return U_hat;
}