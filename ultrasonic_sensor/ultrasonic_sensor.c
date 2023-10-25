// Get readings from ultrasonic sensor

#include "pico/stdlib.h"
#include <stdio.h>
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include <time.h>

void setupUltrasonicPins(uint trigPin, uint echoPin)
{
    gpio_init(trigPin);
    gpio_init(echoPin);

    gpio_set_dir(trigPin, GPIO_OUT);
    gpio_set_dir(echoPin, GPIO_IN);

    gpio_pull_up(echoPin);
}

float getPulse(uint trigPin, uint echoPin)
{
    time_t time1, time2;
    //int loop1 = 0;
    //int loop2 = 0;

    sleep_us(2);
    gpio_put(trigPin, 1);
    sleep_us(10);
    gpio_put(trigPin, 0);
    sleep_us(2);

    while (gpio_get(echoPin) == 0)
    {
        // loop1++;
        time1 = time_us_64();
        sleep_us(100);

    }

    while (gpio_get(echoPin) != 0)
    {
        // loop2++;
        time2 = time_us_64();
        sleep_us(100);
    }
    float pulseLength = time2 - time1;
    return pulseLength;
}

float getCm(uint trigPin, uint echoPin)
{
    int pulseLength = getPulse(trigPin, echoPin);
    // return (float)pulseLength / 2 * 0.0343;
    return ((float)pulseLength * 0.0343) / 2;
}

int main()
{
    stdio_init_all();

    sleep_ms(5000);
    printf("Starting...\n");

    uint trigPin = 4;
    uint echoPin = 5;
    setupUltrasonicPins(trigPin, echoPin);

    while (1)
    {
        printf("\n %.2f cm", getCm(trigPin, echoPin));
        sleep_ms(1500);
    }
}