/**
 * @file line_sensor.c
 * @brief Line sensor module for car
 * @author Benjamin Loh Choon How
 */

#include "line_sensor.h"
#include "../barcode_module/barcode_module.h"
#include "../motor/motor.h"

// void line_sensor_isr (uint gpio, uint32_t events) 
// {
//     uint64_t current_time           = time_us_64();
//     uint64_t left_last_triggered    = 0;
//     uint64_t right_last_triggered   = 0;

//     if (gpio == LEFT_IR_SENSOR_PIN)
//     {
//         if (current_time - left_last_triggered > DEBOUNCE_TIME_USEC)
//         {
//             g_left_ir_triggered = gpio_get(LEFT_IR_SENSOR_PIN) == 0 ? true : false;
//             left_last_triggered = current_time;
//         }
//     }
//     else if (gpio == RIGHT_IR_SENSOR_PIN)
//     {
//         if (current_time - right_last_triggered > DEBOUNCE_TIME_USEC)
//         {
//             g_right_ir_triggered = gpio_get(RIGHT_IR_SENSOR_PIN) == 0 ? true : false;
//             right_last_triggered = current_time;
//         }
//     }
// }

// bool g_left_ir_triggered = false;
// bool g_right_ir_triggered = false;

// // combined isr for all sensors
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
        // case LEFT_IR_SENSOR_PIN:
        //     // if passed debounce
        //     if (current_time - left_last_triggered > DEBOUNCE_TIME_USEC)
        //     {
        //         // return true if white line detected
        //         g_left_ir_triggered = gpio_get(LEFT_IR_SENSOR_PIN) == 0 ? true : false;
        //         left_last_triggered = current_time;
        //     }
        //     break;
        // case RIGHT_IR_SENSOR_PIN:
        //     // if passed debounce
        //     if (current_time - right_last_triggered > DEBOUNCE_TIME_USEC)
        //     {
        //         // return true if white line detected
        //         g_right_ir_triggered = gpio_get(RIGHT_IR_SENSOR_PIN) == 0 ? true : false;
        //         right_last_triggered = current_time;
        //     }
        //     break;
        case BARCODE_SENSOR_PIN:
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
}

bool
repeating_left_ir_callback_isr (struct repeating_timer *p_timer) 
{
    // get gpio pin result and update global variable
    g_left_ir_triggered = gpio_get(LEFT_IR_SENSOR_PIN) == 0 ? true : false;
    return true;
}

bool
repeating_right_ir_callback_isr (struct repeating_timer *p_timer) 
{
    // get gpio pin result and update global variable
    g_right_ir_triggered = gpio_get(RIGHT_IR_SENSOR_PIN) == 0 ? true : false;
    return true;
}

// initialize ir sensors
void ir_sensor_init ()
{
    static struct repeating_timer  timer1        = {0};
    static struct repeating_timer  timer2        = {0};
    gpio_init(LEFT_IR_SENSOR_PIN);
    gpio_init(RIGHT_IR_SENSOR_PIN);
    gpio_init(BARCODE_SENSOR_PIN);
    gpio_set_dir(LEFT_IR_SENSOR_PIN, GPIO_IN);
    gpio_set_dir(RIGHT_IR_SENSOR_PIN, GPIO_IN);
    gpio_set_dir(BARCODE_SENSOR_PIN, GPIO_IN);
    gpio_set_irq_enabled_with_callback(BARCODE_SENSOR_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &sensor_isr);
    // gpio_set_irq_enabled(RIGHT_IR_SENSOR_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);
    // gpio_set_irq_enabled(BARCODE_SENSOR_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);
    // add repeating timer for ir sensors
    add_repeating_timer_ms(-LINE_SENSOR_TIMER_INTERVAL_MS, &repeating_left_ir_callback_isr, NULL, &timer1);
    add_repeating_timer_ms(-LINE_SENSOR_TIMER_INTERVAL_MS, &repeating_right_ir_callback_isr, NULL, &timer2);
}
