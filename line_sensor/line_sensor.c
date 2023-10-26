/**
 * @file line_sensor.c
 * @brief Line sensor module for car
 * @author Benjamin Loh Choon How
 */

#include "line_sensor.h"

void line_sensor_isr (uint gpio, uint32_t events) 
{
    uint64_t current_time           = time_us_64();
    uint64_t left_last_triggered    = 0;
    uint64_t right_last_triggered   = 0;

    if (gpio == LEFT_IR_SENSOR_PIN)
    {
        if (current_time - left_last_triggered > DEBOUNCE_TIME_USEC)
        {
            g_left_ir_triggered = gpio_get(LEFT_IR_SENSOR_PIN) == 0 ? true : false;
            left_last_triggered = current_time;
        }
    }
    else if (gpio == RIGHT_IR_SENSOR_PIN)
    {
        if (current_time - right_last_triggered > DEBOUNCE_TIME_USEC)
        {
            g_right_ir_triggered = gpio_get(RIGHT_IR_SENSOR_PIN) == 0 ? true : false;
            right_last_triggered = current_time;
        }
    }
}

// initialize ir sensors
void ir_sensor_init ()
{
    gpio_init(LEFT_IR_SENSOR_PIN);
    gpio_init(RIGHT_IR_SENSOR_PIN);
    gpio_set_dir(LEFT_IR_SENSOR_PIN, GPIO_IN);
    gpio_set_dir(RIGHT_IR_SENSOR_PIN, GPIO_IN);
    gpio_set_irq_enabled_with_callback(LEFT_IR_SENSOR_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &line_sensor_isr);
    gpio_set_irq_enabled(RIGHT_IR_SENSOR_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);
}
