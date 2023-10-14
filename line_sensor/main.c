#include "line_sensor.h"

// init global variables
bool g_left_ir_triggered = false;
bool g_right_ir_triggered = false;

int main () 
{
    stdio_init_all();
    ir_sensor_init();
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