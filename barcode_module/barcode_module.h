#ifndef BARCODE_MODULE_H
#define BARCODE_MODULE_H

#include <stdio.h>
#include "pico/stdlib.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#define BARCODE_SENSOR_PIN                  0
#define DEBOUNCE_TIME_USEC                  100000
#define ARRAY_SIZE                          27
#define BARCODE_SENSE_TIME_INTERVAL_MS      50

bool    debounce               (uint gpio, uint32_t events);
void    barcode_sensor_isr     (uint gpio, uint32_t events);
void    ir_sensor_init         (uint ir_sensor_pin, void * ir_sensor_isr);
void    print_barcode          (volatile int barcode[ARRAY_SIZE]);
int     find_smallest_value    (volatile int barcode[ARRAY_SIZE]);
char *  scale_down_barcode     (volatile int barcode[ARRAY_SIZE]);
// char *  correct_barcode        (volatile int barcode[ARRAY_SIZE]);

// global variables
extern bool   g_barcode_detected;
extern int    g_barcode[ARRAY_SIZE];
extern int    g_index;

// first column is the barcode ascii, second column is the corresponding character
extern const char * barcode_values_array[44][2];

// function prototypes
bool    debounce               (uint gpio, uint32_t events);
void    barcode_sensor_isr     (uint gpio, uint32_t events);
void    ir_sensor_init         (uint ir_sensor_pin, void * ir_sensor_isr);
void    print_barcode          (volatile int barcode[ARRAY_SIZE]);
int     find_smallest_value    (volatile int barcode[ARRAY_SIZE]);
char *  scale_down_barcode     (volatile int barcode[ARRAY_SIZE]);
char *  convert_barcode        (char * barcode_string);

#endif