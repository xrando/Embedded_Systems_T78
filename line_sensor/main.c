/** @file main.c
 *
 * @brief Main file to test IR sensor driver for Raspberry Pi Pico
 *
 * @par
 * COPYRIGHT NOTICE: (c) Singapore Institute of Technology
 * @author Benjamin Loh Choon How (2201590), 2023 All rights reserved.
 */

#include "line_sensor.h"

// init global variables
bool g_left_ir_triggered = false;
bool g_right_ir_triggered = false;

int main () 
{
    stdio_init_all();
    ir_sensor_init();
    for (;;)
    {

        printf("Left IR sensor status: %d\n", g_left_ir_triggered);
        printf("Right IR sensor status: %d\n", g_right_ir_triggered);
        sleep_ms(250);
        
    }

    return 0;
} /* main() */

/*** end of file ***/