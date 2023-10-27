#include "line_sensor.h"

// init global variables
bool g_left_ir_triggered = false;
bool g_right_ir_triggered = false;

int main () 
{
    stdio_init_all();
    ir_sensor_init();
    for (;;)
    {

        printf("Left IR sensor status: %d\n", g_left_ir_triggered);
        printf("Right IR sensor status: %d\n", g_right_ir_triggered);
        sleep_ms(250);
        
    }

    return 0;
}