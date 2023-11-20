/** @file barcode_module.h
 *
 * @brief Header file for barcode driver for Raspberry Pi Pico
 *
 * @par
 * COPYRIGHT NOTICE: (c) Singapore Institute of Technology
 * @author Benjamin Loh Choon How (2201590), 2023 All rights reserved.
 */

#ifndef BARCODE_MODULE_H
#define BARCODE_MODULE_H

#include <stdio.h>
#include "pico/stdlib.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#define BARCODE_SENSOR_PIN                  4 // 0 
//#define DEBOUNCE_TIME_USEC                  100000
#define ARRAY_SIZE                          27
#define BARCODE_SENSE_TIME_INTERVAL_MS      50

// global variables
extern bool   g_barcode_detected;
extern int    g_barcode[ARRAY_SIZE];
extern int    g_index;

// first column is the barcode ascii, second column is the corresponding character
extern const char * barcode_values_array[44][2];

// function prototypes
bool    debounce                    (uint gpio, uint32_t events);
void    barcode_sensor_isr          (uint gpio, uint32_t events);
//void    ir_sensor_init              ();
void    print_barcode               (volatile int barcode[ARRAY_SIZE]);
char *  scale_down_barcode          (volatile int barcode[ARRAY_SIZE]);
char *  string_reverse              (char * string);
char *  convert_barcode             (char * barcode_string);

// Declaration of the ISR function
bool repeating_timer_callback_isr(struct repeating_timer *t);

//void    process_and_print_barcode   () 

#endif