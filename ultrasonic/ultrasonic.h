// ultrasonic.h

#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include "pico/stdlib.h"
#include <time.h>

// #define TRIG_PIN 4
// #define ECHO_PIN 5

void ultrasonic_init(uint trigPin, uint echoPin);
float ultrasonic_get_pulse(uint trigPin, uint echoPin);
float ultrasonic_get_distance(uint trigPin, uint echoPin);
float ultrasonic_kalman_filter(float U);

#endif // ULTRASONIC_H