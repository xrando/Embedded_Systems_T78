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
#include "../line_sensor/line_sensor.h"

// init global variables
bool         g_barcode_detected          = false;
int          g_barcode[ARRAY_SIZE]       = {0};
int          g_index                     = 0;
uint64_t     last_barcode_detection_time = 0;
const char * barcode_values_array[44][2] = 
{
    {"111221211","0"},
    {"211211112","1"},
    {"112211112","2"},
    {"212211111","3"},
    {"111221112","4"},
    {"211221111","5"},
    {"112221111","6"},
    {"111211212","7"},
    {"211211211","8"},
    {"112211211","9"},
    {"211112112","A"},
    {"112112112","B"},
    {"212112111","C"},
    {"111122112","D"},
    {"211122111","E"},
    {"112122111","F"},
    {"111112212","G"},
    {"211112211","H"},
    {"112112211","I"},
    {"111122211","J"},
    {"211111122","K"},
    {"112111122","L"},
    {"212111121","M"},
    {"111121122","N"},
    {"211121121","O"},
    {"112121121","P"},
    {"111111222","Q"},
    {"211111221","R"},
    {"112111221","S"},
    {"111121221","T"},
    {"221111112","U"},
    {"122111112","V"},
    {"222111111","W"},
    {"121121112","X"},
    {"221121111","Y"},
    {"122121111","Z"},
    {"121111212","-"},
    {"221111211","."},
    {"122111211"," "},
    {"121212111","$"},
    {"121211121","/"},
    {"121112121","+"},
    {"111212121","%"},
    {"121121211","*"}
};

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

// // to integrate with isr in line sensor module
// void
// barcode_sensor_isr (uint gpio, uint32_t events) 
// {
//     static uint                    time_counter = 0u;
//     static struct repeating_timer  timer        = {0};

//     // Check for debounce
//     if (!debounce(gpio, events))
//     {
//         return;
//     }
    
//     // Update the last barcode detection time
//     update_last_barcode_detection_time();

//     // Determine the state of the barcode sensor
//     bool is_white_line = (gpio_get(BARCODE_SENSOR_PIN) == 0);

//     // Print the state for debugging
//     printf(is_white_line ? "White line debounce\n" : "Black line debounce\n");

//     // If there's an ongoing time count, stop it and store the value
//     if (time_counter != 0u)
//     {
//         cancel_repeating_timer(&timer);
//         g_barcode[g_index++] = time_counter;
//         time_counter = 0;
//     }

//     // Start a timer to measure the pulse width
//     add_repeating_timer_ms(-BARCODE_SENSE_TIME_INTERVAL_MS, repeating_timer_callback_isr, 
//                            &time_counter, &timer);

//     // Update the global state
//     g_barcode_detected = is_white_line;

// }


// // init ir sensor on gpio pin (replace with function from line_sensor.c)
// void
// ir_sensor_init () 
// {
//     gpio_init(BARCODE_SENSOR_PIN);
//     gpio_set_dir(BARCODE_SENSOR_PIN, GPIO_IN);
//     gpio_set_irq_enabled_with_callback(BARCODE_SENSOR_PIN, GPIO_IRQ_EDGE_RISE 
//                                         | GPIO_IRQ_EDGE_FALL, 
//                                         true, &barcode_sensor_isr);
//     // repeating timer for clearing barcode array
//     static struct repeating_timer repeating_barcode_clear_timer = {0};
//     add_repeating_timer_ms(-BARCODE_RESET_TIMEOUT_MS, repeating_barcode_clear_callback_isr, 
//                            NULL, &repeating_barcode_clear_timer);
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
scale_down_barcode (volatile int barcode[TEMP_ARRAY_SIZE])
{
    // Find the smallest and largest values in the barcode array
    int     smallest_value         = INT_MAX;
    int     largest_value          = INT_MIN;

    for (int i = 0; i < TEMP_ARRAY_SIZE; i++) 
    {

        if (barcode[i] < smallest_value) 
        {

            smallest_value = barcode[i];

        }
        else if (barcode[i] > largest_value) 
        {

            largest_value = barcode[i];

        }

    }
    // print smallest & largest value
    printf("smallest value: %d\n", smallest_value);
    printf("largest value: %d\n", largest_value);

    // Calculate a threshold to distinguish high and low values
    int     threshold              = (largest_value - smallest_value) / 2;
    char *  barcode_string_ascii   = (char * ) malloc (TEMP_ARRAY_SIZE + 1);
    int     string_index           = 0;

    for (int i = 0; i < TEMP_ARRAY_SIZE; i++) 
    {
        // Store & scale down ascii of barcode value
        if (barcode[i] >= threshold) 
        {

            barcode_string_ascii[string_index++] = '2';

        } 
        else 
        {

            barcode_string_ascii[string_index++] = '1';

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

void
reset_barcode_array () 
{
    memset(g_barcode, 0, sizeof(g_barcode));
    g_index = 0;
    g_barcode_detected = false;
    printf("Barcode array reset due to inactivity.\n");
}

void
update_last_barcode_detection_time () 
{
    last_barcode_detection_time = time_us_64();
}

// Call this function periodically
void
check_and_reset_barcode_array ()
{
    if ((time_us_64() - last_barcode_detection_time) > BARCODE_RESET_TIMEOUT_MS * 1000) {
        reset_barcode_array();
    }
}

bool
repeating_barcode_clear_callback_isr (struct repeating_timer *p_timer) 
{
    // Periodically check and reset barcode array
    check_and_reset_barcode_array();
    return true;
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