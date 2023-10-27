#ifndef LINE_SENSOR_H
#define LINE_SENSOR_H

#include <stdio.h>
#include "pico/stdlib.h"

#define LEFT_IR_SENSOR_PIN 16
#define RIGHT_IR_SENSOR_PIN 2
#define DEBOUNCE_TIME_USEC  50000

// function prototypes
bool debounce (uint gpio, uint32_t events);
void line_sensor_isr (uint gpio, uint32_t events);
void ir_sensor_init ();

// extern global variables for other modules to use
extern bool g_left_ir_triggered;
extern bool g_right_ir_triggered;

#endif