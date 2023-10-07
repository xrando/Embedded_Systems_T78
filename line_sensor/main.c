#include "line_sensor.h"

int main () 
{
    stdio_init_all();
    // TOFIX: if one of the sensor is not connected, the pin will be assigned to the other isr
    // e.g if LEFT_IR_SENSOR_PIN is not connected, then
    // LEFT_IR_SENSOR_PIN will be assigned to right_line_sensor_isr
    ir_sensor_init(LEFT_IR_SENSOR_PIN, &left_line_sensor_isr);
    ir_sensor_init(RIGHT_IR_SENSOR_PIN, &right_line_sensor_isr);

    while (1) 
    {
        if (g_left_ir_triggered && g_right_ir_triggered) 
        {
            printf("Both IR sensors triggered!\n");
            // barcode scanner function
            g_left_ir_triggered = false;
            g_right_ir_triggered = false;
            sleep_ms(1000); // Wait for a while to avoid multiple triggers
        }
        else if (g_right_ir_triggered) 
        {
            printf("right IR sensor triggered!\n");
            g_right_ir_triggered = false;
            sleep_ms(1000); // Wait for a while to avoid multiple triggers
        }
        else if (g_left_ir_triggered)
        {
            printf("left IR sensor triggered!\n");
            g_left_ir_triggered = false;
            sleep_ms(1000); // Wait for a while to avoid multiple triggers
        }
    }

    return 0;
}