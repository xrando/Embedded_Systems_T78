/** @file main.c
 *
 * @brief Main file to test barcode driver for Raspberry Pi Pico
 *
 * @par
 * COPYRIGHT NOTICE: (c) Singapore Institute of Technology
 * @author Benjamin Loh Choon How (2201590), 2023 All rights reserved.
 */

#include "barcode_module.h"



int main ()
{
    // init ir sensor
    stdio_init_all();
    ir_sensor_init();

    for (;;)
    {

        // if all barcode ascii is collected
        if (g_barcode[18] != 0)
        {
            // temp barcode array
            int temp_barcode[9] = {0};

            // copy barcode array from index 10 to 18, to temp barcode array
            memcpy(temp_barcode, &g_barcode[10], sizeof(temp_barcode));

            printf("temp barcode: ");
            for (int i = 0; i < TEMP_ARRAY_SIZE; i++)
            {
                printf("[%d]", temp_barcode[i]);
            }
            printf("\n");

            // scale down barcode & store in string
            char * barcode_ascii_string = scale_down_barcode(temp_barcode);
            printf("%s\n", barcode_ascii_string);

            // convert barcode ascii to barcode value
            char * barcode_value_string = convert_barcode(barcode_ascii_string);

            // print barcode value
            printf("%s\n", barcode_value_string);
            
            // reset g_index
            g_index = 0;

            // clear barcode array
            memset(g_barcode, 0, sizeof(g_barcode));

        }

        // print out pulse width of barcode
        print_barcode(g_barcode);
        sleep_ms(1000);

    }

    return 0;

} /* main() */

/*** end of file ***/