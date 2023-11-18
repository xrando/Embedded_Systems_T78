/** @file line_sensor.h
 *
 * @brief Header file for IR sensor driver for Raspberry Pi Pico
 *
 * @par
 * COPYRIGHT NOTICE: (c) Singapore Institute of Technology
 * @author Benjamin Loh Choon How (2201590), 2023 All rights reserved.
 */

#ifndef LINE_SENSOR_H
#define LINE_SENSOR_H

#include <stdio.h>
#include "pico/stdlib.h"

#define LEFT_IR_SENSOR_PIN 20   // 16
#define RIGHT_IR_SENSOR_PIN 21  // 2
#define DEBOUNCE_TIME_USEC  50000

// function prototypes
bool debounce (uint gpio, uint32_t events);
void line_sensor_isr (uint gpio, uint32_t events);
void ir_sensor_init ();

// extern global variables for other modules to use
extern bool g_left_ir_triggered;
extern bool g_right_ir_triggered;

#endif