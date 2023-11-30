/** @file line_sensor.c
 *
 * @brief IR sensor driver for Raspberry Pi Pico
 *
 * @par
 * COPYRIGHT NOTICE: (c) Singapore Institute of Technology
 * @author Benjamin Loh Choon How (2201590), 2023 All rights reserved.
 * 
 */

#include "line_sensor.h"
#include "barcode_module/barcode_module.h"
#include "motor/motor.h"

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
} /* line_sensor_isr() */

// combined isr for all sensors
void sensor_isr (uint gpio, uint32_t events)
{
    // for debounce
    uint64_t current_time           = time_us_64();
    uint64_t left_last_triggered    = 0;
    uint64_t right_last_triggered   = 0;
    // for barcode
    static uint                    time_counter = 0u;
    static struct repeating_timer  timer        = {0};

    switch (gpio)
    {
        case LEFT_IR_SENSOR_PIN:
            // if passed debounce
            if (current_time - left_last_triggered > DEBOUNCE_TIME_USEC)
            {
                // return true if white line detected
                g_left_ir_triggered = gpio_get(LEFT_IR_SENSOR_PIN) == 0 ? true : false;
                left_last_triggered = current_time;
            }
            break;

        case RIGHT_IR_SENSOR_PIN:
            // if passed debounce
            if (current_time - right_last_triggered > DEBOUNCE_TIME_USEC)
            {
                // return true if white line detected
                g_right_ir_triggered = gpio_get(RIGHT_IR_SENSOR_PIN) == 0 ? true : false;
                right_last_triggered = current_time;
            }
            break;

        case BARCODE_SENSOR_PIN:
            // Check for debounce
            if (!debounce(gpio, events))
            {
                return;
            }
            
            // Update the last barcode detection time
            update_last_barcode_detection_time();

            // Determine the state of the barcode sensor
            bool is_white_line = (gpio_get(BARCODE_SENSOR_PIN) == 0);

            // Print the state for debugging
            printf(is_white_line ? "White line debounce\n" : "Black line debounce\n");

            // If there's an ongoing time count, stop it and store the value
            if (time_counter != 0u)
            {
                cancel_repeating_timer(&timer);
                g_barcode[g_index++] = time_counter;
                time_counter = 0;
            }

            // Start a timer to measure the pulse width
            add_repeating_timer_ms(-BARCODE_SENSE_TIME_INTERVAL_MS, repeating_timer_callback_isr, 
                                &time_counter, &timer);

            // Update the global state
            g_barcode_detected = is_white_line;
            break;

        // motor isrs
        case RIGHT_POLLING_PIN:
            if (current_time - left_last_triggered > DEBOUNCE_TIME_USEC)
            {

                uint64_t current_time = time_us_64();
                uint64_t time_difference = current_time - right_last_time;
                right_last_time = current_time;
                speed_of_right_wheel = calculate_speed(time_difference);
                // right_wheel_pid = calculate_pid(speed_of_right_wheel, 120, 0, 0); // Adjust desired_speed as needed
                // printf("Right wheel pid: %.2f\n", right_wheel_pid);
                rightTotalDistance += DISTANCE_BETWEEN_NOTCHES_CM;

            }
            break;

        case LEFT_POLLING_PIN:
            if (current_time - left_last_triggered > DEBOUNCE_TIME_USEC)
            {

                uint64_t current_time = time_us_64();
                uint64_t time_difference = current_time - left_last_time;
                left_last_time = current_time;
                speed_of_left_wheel = calculate_speed(time_difference);
                // float left_wheel_pid = calculate_pid(speed_of_left_wheel, 120, 0, 0); // Adjust desired_speed as needed
                // printf("Left wheel pid before: %.2f\n", left_wheel_pid);
                leftTotalDistance += DISTANCE_BETWEEN_NOTCHES_CM;

            }
            break;

        default:
            break;
            
    }
} /* sensor_isr() */

// initialize ir sensors
void ir_sensor_init ()
{
    gpio_init(LEFT_IR_SENSOR_PIN);
    gpio_init(RIGHT_IR_SENSOR_PIN);
    gpio_init(BARCODE_SENSOR_PIN);
    gpio_set_dir(LEFT_IR_SENSOR_PIN, GPIO_IN);
    gpio_set_dir(RIGHT_IR_SENSOR_PIN, GPIO_IN);
    gpio_set_dir(BARCODE_SENSOR_PIN, GPIO_IN);
    gpio_set_irq_enabled_with_callback(LEFT_IR_SENSOR_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &sensor_isr);
    gpio_set_irq_enabled(RIGHT_IR_SENSOR_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_enabled(BARCODE_SENSOR_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);
} /* ir_sensor_init() */

/*** end of file ***/