#include <stdio.h>
#include "pico/stdlib.h"
#include <math.h>

#define BARCODE_SENSOR_PIN 0
#define DEBOUNCE_TIME_USEC  100000
#define ARRAY_SIZE 9

bool debounce               (uint gpio, uint32_t events);
void barcode_sensor_isr     (uint gpio, uint32_t events);
void ir_sensor_init         (uint ir_sensor_pin, void * ir_sensor_isr);
void print_barcode          (volatile int barcode[ARRAY_SIZE]);
int  find_smallest_value    (volatile int barcode[ARRAY_SIZE]);
void scale_down_barcode     (volatile int barcode[ARRAY_SIZE]);


volatile bool barcode_detected = false;
volatile int barcode[ARRAY_SIZE] = {0};
volatile int index = 0;

// barcode values 2d array, first column is ascii value,
// second column is barcode value
volatile char * barcode_values_array[44][2] = 
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

// Function to debounce button
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
    // Print time counter
    //printf("Time: %us\n", (*(uint *) p_timer->user_data)++);
    (*(uint *) p_timer->user_data)++;
    return true;
}

 void barcode_sensor_isr (uint gpio, uint32_t events) 
{
    static uint                    time_counter = 0u;
    static struct repeating_timer  timer        = {0};

    // if passed debounce
    if (debounce(gpio, events))
    {
        // if sensor is black, low signal
        if (gpio_get(BARCODE_SENSOR_PIN) == 0)
        {
            // set state of barcode sensor
            printf("black line debounce\n");
            
            if (time_counter != 0u)
            {
                // stop counting time
                cancel_repeating_timer(&timer);
                // store in array
                barcode[index] = time_counter;
                index++;
                time_counter = 0;
            }
            // count time for black
            add_repeating_timer_ms(-50, repeating_timer_callback_isr, 
                                &time_counter, &timer);
            
            
            
            barcode_detected = true;
        }
        else
        {
            // set state of barcode sensor
            printf("white line debounce\n");
            if (time_counter != 0u)
            {
                // stop counting time
                cancel_repeating_timer(&timer);
                //printf("Final Time: %us\n", time_counter);
                // store in array
                barcode[index] = time_counter;
                index++;
                time_counter = 0;
            }
            // count time for white
            add_repeating_timer_ms(-50, repeating_timer_callback_isr, 
                              &time_counter, &timer);
            
            
            barcode_detected = false;
        }
    }

}


// init ir sensor on gpio pin
void ir_sensor_init (uint ir_sensor_pin, void * ir_sensor_isr) 
{
    gpio_init(ir_sensor_pin);
    gpio_set_dir(ir_sensor_pin, GPIO_IN);
    gpio_set_irq_enabled_with_callback(ir_sensor_pin, GPIO_IRQ_EDGE_RISE 
                                        | GPIO_IRQ_EDGE_FALL, 
                                        true, ir_sensor_isr);
}

// print barcode
void print_barcode (volatile int barcode[ARRAY_SIZE])
{
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        printf("[%d]", barcode[i]);
    }
    printf("\n");
}

// function to find smallest value in barcode array
int find_smallest_value (volatile int barcode[ARRAY_SIZE])
{
    int smallest_value = barcode[0];
    for (int i = 1; i < ARRAY_SIZE; i++)
    {
        if (barcode[i] != 0)
        {
            if (barcode[i] < smallest_value)
            {
                smallest_value = barcode[i];
            }
        }
    }
    return smallest_value;
}

// function to scale down values in barcode array
void scale_down_barcode (volatile int barcode[ARRAY_SIZE])
{
    int smallest_value = find_smallest_value(barcode);
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        if (barcode[i] != 0)
        {
            barcode[i] = floor(barcode[i] / smallest_value);
            if (barcode[i] > 1)
            {
                barcode[i] = 2;
            }
        }
    }
}


int main ()
{
    stdio_init_all();
    ir_sensor_init(BARCODE_SENSOR_PIN, &barcode_sensor_isr);

    

    for (;;)
    {
        // print state of barcode sensor
        
        //print barcode
        //print_barcode(barcode);
        //sleep_ms(1000);

        // if barcode is full
        if (index == ARRAY_SIZE)
        {
            // scale down barcode
            scale_down_barcode(barcode);
            //print barcode
            print_barcode(barcode);
            // reset index
            index = 0;
        }

    }


    



    return 0;
}