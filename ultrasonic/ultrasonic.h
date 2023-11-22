// ultrasonic.h

#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include "pico/stdlib.h"
#include <time.h>

// #define TRIG_PIN 4
// #define ECHO_PIN 5

void init_ultrasonic(uint trigPin, uint echoPin);
float get_pulse(uint trigPin, uint echoPin);
float get_distance(uint trigPin, uint echoPin);
float calculate_kalman_filter(float U);

#endif // ULTRASONIC_H