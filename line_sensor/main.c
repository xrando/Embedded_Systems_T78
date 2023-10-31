#include "line_sensor.h"

// init global variables
bool g_left_on_line = true;
bool g_right_on_line = true;

int main () 
{
    stdio_init_all();
    ir_sensor_init();
    for (;;)
    {

        printf("Left IR sensor status: %d\n", g_left_on_line);
        printf("Right IR sensor status: %d\n", g_right_on_line);
        sleep_ms(250);
        
    }

    return 0;
}