/**
 * @brief Line sensor module for car
 */

#include "line_sensor.h"


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
void line_sensor_isr (uint gpio, uint32_t events) 
{

    //ir_triggered = true;
    // if passed debounce
    if (debounce(gpio, events))
    {
        if (gpio == LEFT_IR_SENSOR_PIN)
        {
            // set state of left line sensor
            printf("left line sensor debounce\n");
            g_left_ir_triggered = true;
        }
        else if (gpio == RIGHT_IR_SENSOR_PIN)
        {
            // set state of right line sensor
            printf("right line sensor debounce\n");
            g_right_ir_triggered = true;
        }
        else
        {
            // Do nothing
        }
        
    }

}

// init ir sensor on gpio pin
void ir_sensor_init ()
{
    gpio_init(LEFT_IR_SENSOR_PIN);
    gpio_init(RIGHT_IR_SENSOR_PIN);
    gpio_set_dir(LEFT_IR_SENSOR_PIN, GPIO_IN);
    gpio_set_dir(RIGHT_IR_SENSOR_PIN, GPIO_IN);
    gpio_set_irq_enabled_with_callback(LEFT_IR_SENSOR_PIN, GPIO_IRQ_EDGE_RISE, true, &line_sensor_isr);
    gpio_set_irq_enabled(RIGHT_IR_SENSOR_PIN, GPIO_IRQ_EDGE_RISE, true);
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