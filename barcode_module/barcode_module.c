/** @file barcode_module.c
 *
 * @brief Barcode driver for Raspberry Pi Pico
 *
 * @par
 * COPYRIGHT NOTICE: (c) Singapore Institute of Technology
 * @author Benjamin Loh Choon How (2201590), 2023 All rights reserved.
 * 
 */

#include "barcode_module.h"

// Function to debounce button (use debounce from line_sensor.c)
bool debounce (uint gpio, uint32_t events) 
{
    // Get start time
    uint64_t start_time = time_us_64();

    // Initial state of event, if rising edge then set to 1, else set to 0
    int initial_state = events & GPIO_IRQ_EDGE_RISE ? 1 : 0;

    // Wait for debounce time
    while (time_us_64() < start_time + DEBOUNCE_TIME_USEC)
    {

        // If the state is unstable during debounce time
        if (initial_state != gpio_get(gpio))
        {
            return false;
        }

    }

    return true;
}

bool
repeating_timer_callback_isr (struct repeating_timer *p_timer) 
{
    // Increment counter
    (*(uint *) p_timer->user_data)++;
    return true;
}

// to integrate with isr in line sensor module
void
barcode_sensor_isr (uint gpio, uint32_t events) 
{
    static uint                    time_counter = 0u;
    static struct repeating_timer  timer        = {0};

    // if passed debounce
    if (debounce(gpio, events))
    {
        // if line is white
        if (gpio_get(BARCODE_SENSOR_PIN) == 0)
        {
            // set state of barcode sensor
            printf("White line debounce\n");
            
            if (time_counter != 0u)
            {
                // stop counting time
                cancel_repeating_timer(&timer);
                // store in array
                g_barcode[g_index] = time_counter;
                g_index++;
                time_counter = 0;
            }
            // measure pulse width of black line
            add_repeating_timer_ms(-BARCODE_SENSE_TIME_INTERVAL_MS, repeating_timer_callback_isr, 
                                &time_counter, &timer);
            g_barcode_detected = true;
        }
        else
        {
            // set state of barcode sensor
            printf("Black line debounce\n");
            if (time_counter != 0u)
            {
                // stop counting time
                cancel_repeating_timer(&timer);
                //printf("Final Time: %us\n", time_counter);
                // store in array
                g_barcode[g_index] = time_counter;
                g_index++;
                time_counter = 0;
            }
            // measure pulse width of white line
            add_repeating_timer_ms(-BARCODE_SENSE_TIME_INTERVAL_MS, repeating_timer_callback_isr, 
                              &time_counter, &timer);
            g_barcode_detected = false;
        }
    }

}


// init ir sensor on gpio pin (replace with function from line_sensor.c)
// void
// ir_sensor_init () 
// {
//     gpio_init(BARCODE_SENSOR_PIN);
//     gpio_set_dir(BARCODE_SENSOR_PIN, GPIO_IN);
//     gpio_set_irq_enabled_with_callback(BARCODE_SENSOR_PIN, GPIO_IRQ_EDGE_RISE 
//                                         | GPIO_IRQ_EDGE_FALL, 
//                                         true, &barcode_sensor_isr);
// }

// print barcode (for debugging)
void
print_barcode (volatile int barcode[ARRAY_SIZE])
{
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        printf("[%d]", barcode[i]);
    }
    printf("\n");
}

// function to scale down ascii in barcode array & return string representation
char *
scale_down_barcode (volatile int barcode[ARRAY_SIZE])
{
    //int  smallest_value  = find_smallest_value(barcode);
    // Find the smallest and largest values in the barcode array
    int     smallest_value         = INT_MAX;
    int     largest_value          = INT_MIN;

    for (int i = 0; i < ARRAY_SIZE; i++) 
    {

        if (barcode[i] != 0 && (i >= 9 && i < 18))
        {

            if (barcode[i] < smallest_value) 
            {

                smallest_value = barcode[i];

            }

            if (barcode[i] > largest_value) 
            {

                largest_value = barcode[i];

            }

        }

    }

    // Calculate a threshold to distinguish high and low values
    int     threshold              = (largest_value + smallest_value) / 2;
    char *  barcode_string_ascii   = (char * ) malloc (ARRAY_SIZE + 1);
    int     string_index           = 0;

    for (int i = 0; i < ARRAY_SIZE; i++) 
    {
        // Only store & scale down ascii of barcode value, ignore the 2 *
        if (barcode[i] != 0 && (i >= 9 && i < 18)) 
        {

            // Scale down based on the threshold
            if (barcode[i] >= threshold) 
            {

                barcode_string_ascii[string_index++] = '2';

            } 
            else 
            {

                barcode_string_ascii[string_index++] = '1';
            }
            
        }

    }

    barcode_string_ascii[string_index] = '\0';

    return barcode_string_ascii;
}

// reverse string
char *
string_reverse(char * string) 
{

    int length = strlen(string);
    char * reverse_string = (char *) malloc (length + 1);

    for (int i = 0; i < length; i++) 
    {

        reverse_string[i] = string[length - i - 1];

    }

    reverse_string[length] = '\0';

    return reverse_string;

}

// convert barcode ascii to barcode value
char *
convert_barcode (char * barcode_string)
{

    char * barcode_value = "";
    // compare barcode to barcode values array
    for (int i = 0; i < 44; i++)
    {

        // if barcode string matches barcode ascii, return barcode value
        if (strcmp(barcode_string, barcode_values_array[i][0]) == 0)
        {

            barcode_value = barcode_values_array[i][1];

        }

        // check for reverse barcode ascii
        else if (strcmp(barcode_string, string_reverse(barcode_values_array[i][0])) == 0)
        {

            barcode_value = barcode_values_array[i][1];

        }

    }

    return barcode_value;

}

// process and print barcode
// void
// process_and_print_barcode () 
// {

//     // if all barcode ascii is collected
//     if (g_barcode[18] != 0) 
//     {

//         // temp barcode array
//         int temp_barcode[TEMP_ARRAY_SIZE] = {0};

//         // copy barcode array from index 10 to 18, to temp barcode array
//         memcpy(temp_barcode, &g_barcode[10], sizeof(temp_barcode));

//         printf("temp barcode: ");
//         for (int i = 0; i < TEMP_ARRAY_SIZE; i++) 
//         {

//             printf("[%d]", temp_barcode[i]);

//         }

//         printf("\n");

//         // scale down barcode & store in string
//         char * barcode_ascii_string = scale_down_barcode(temp_barcode);
//         printf("%s\n", barcode_ascii_string);

//         // convert barcode ascii to barcode value
//         char * barcode_value_string = convert_barcode(barcode_ascii_string);

//         // print barcode value
//         printf("%s\n", barcode_value_string);

//         // reset g_index
//         g_index = 0;

//         // clear barcode array
//         memset(g_barcode, 0, sizeof(g_barcode));

//     }

// }