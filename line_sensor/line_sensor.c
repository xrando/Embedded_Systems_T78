/**
 * @brief Line sensor module for car
 */

#include "line_sensor.h"
// init global variables
bool g_left_ir_triggered = false;
bool g_right_ir_triggered = false;



// global variables for other modules to use

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
// code calling this needs to set the state of the ir sensor to false after reading
void left_line_sensor_isr (uint gpio, uint32_t events) 
{

    //ir_triggered = true;
    // if passed debounce
    if (debounce(gpio, events))
    {
        // set state of left line sensor
        printf("left line sensor debounce\n");
        g_left_ir_triggered = true;
    }

}
// code calling this needs to set the state of the ir sensor to false after reading
void right_line_sensor_isr (uint gpio, uint32_t events) 
{

    // if passed debounce
    if (debounce(gpio, events))
    {
        // set state of right line sensor
        printf("right line sensor debounce\n");
        g_right_ir_triggered = true;
    }

}

// init ir sensor on gpio pin
void ir_sensor_init (uint ir_sensor_pin, void * ir_sensor_isr) 
{
    gpio_init(ir_sensor_pin);
    gpio_set_dir(ir_sensor_pin, GPIO_IN);
    gpio_set_irq_enabled_with_callback(ir_sensor_pin, GPIO_IRQ_EDGE_RISE, true, ir_sensor_isr);
}


//barcode scanner
char * barcode_scanner (void)
{
    char * barcode = "";
    






    // clear left and right ir sensor triggered flags
    // g_left_ir_triggered = false;
    // g_right_ir_triggered = false;

    return barcode;

}