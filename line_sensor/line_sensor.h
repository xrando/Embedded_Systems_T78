#ifndef LINE_SENSOR_H
#define LINE_SENSOR_H

#include <stdio.h>
#include "pico/stdlib.h"

#define LEFT_IR_SENSOR_PIN 16
#define RIGHT_IR_SENSOR_PIN 2
#define DEBOUNCE_TIME_USEC  100000

// function prototypes
bool debounce (uint gpio, uint32_t events);
void left_line_sensor_isr (uint gpio, uint32_t events);
void right_line_sensor_isr (uint gpio, uint32_t events);
void ir_sensor_init (uint ir_sensor_pin, void * ir_sensor_isr);

// extern global variables for other modules to use
extern bool g_left_ir_triggered;
extern bool g_right_ir_triggered;

#endif