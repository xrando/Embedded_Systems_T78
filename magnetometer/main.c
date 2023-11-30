/** @file magnetometer.c
 *
 * @brief Main file to test magneometer driver for Raspberry Pi Pico
 *
 * @par
 * COPYRIGHT NOTICE: (c) Singapore Institute of Technology
 * @author Low Hong Sheng Jovian (2203654), 2023 All rights reserved.
 */

#include "magnetometer.h"

int 
main() 
{
    stdio_init_all();
    init_i2c();
    init_magnetometer();
    monitor_magnetometer();
} /* main() */

/*** end of file ***/