/** @file magnetometer.h
 *
 * @brief Header file for magneometer driver for Raspberry Pi Pico
 *
 * @par
 * COPYRIGHT NOTICE: (c) Singapore Institute of Technology
 * @author Low Hong Sheng Jovian (2203654), 2023 All rights reserved.
 */

#ifndef MAGNETOMETER_H
#define MAGNETOMETER_H

#include <stdint.h>

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"

#define CUSTOM_I2C_SDA_PIN 0  // GP0 for SDA
#define CUSTOM_I2C_SCL_PIN 1  // GP1 for SCL
#define MAGNETOMETER_ADDR  0x1E  
#define i2c_default_port   i2c0

// Define magneto registers values from LSM303DLHC (MY-511) datasheet 
#define MY511_CRA_REG_M            0x00
#define MY511_CRB_REG_M            0x01
#define MY511_MR_REG_M             0x02
#define MY511_OUT_X_H_M            0x03
#define MY511_OUT_X_L_M            0x04
#define MY511_OUT_Z_H_M            0x05
#define MY511_OUT_Z_L_M            0x06
#define MY511_OUT_Y_H_M            0x07
#define MY511_OUT_Y_L_M            0x08
#define MY511_SR_REG_M             0x09

typedef struct 
{
    int16_t      mag[3];
    float        heading;
    const char * direction;
} magnetometer_data;

// Function prototypes
void init_i2c();
void init_magnetometer();
void monitor_magnetometer();
void magnetometer_read_raw(int16_t mag[3]);
void calculate_heading(magnetometer_data *data);
const char* heading_direction(float heading);
magnetometer_data read_and_calculate_heading();

#endif 