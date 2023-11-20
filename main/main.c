#include "line_sensor.h"
#include "../barcode_module/barcode_module.h"
#include "../motor/motor.h"
#include "magnetometer.h"

bool g_left_ir_triggered = false;
bool g_right_ir_triggered = false;

int main ()
{
    stdio_init_all();
    motor_sensor_init();
    ir_sensor_init();
    init_i2c();
    init_magnetometer();
    // monitor_magnetometer();

    for(;;)
    {
        forward();
        sleep_ms(250);

    }
}

// // init global variables
// bool         collision_detected          = false;

// noneed
// bool         g_left_ir_triggered         = false;
// bool         g_right_ir_triggered        = false;
// bool         g_barcode_detected          = false;
// int          g_barcode[ARRAY_SIZE]       = {0};
// int          g_index                     = 0;
// const char * barcode_values_array[44][2] = 
// {
//     {"111221211","0"},
//     {"211211112","1"},
//     {"112211112","2"},
//     {"212211111","3"},
//     {"111221112","4"},
//     {"211221111","5"},
//     {"112221111","6"},
//     {"111211212","7"},
//     {"211211211","8"},
//     {"112211211","9"},
//     {"211112112","A"},
//     {"112112112","B"},
//     {"212112111","C"},
//     {"111122112","D"},
//     {"211122111","E"},
//     {"112122111","F"},
//     {"111112212","G"},
//     {"211112211","H"},
//     {"112112211","I"},
//     {"111122211","J"},
//     {"211111122","K"},
//     {"112111122","L"},
//     {"212111121","M"},
//     {"111121122","N"},
//     {"211121121","O"},
//     {"112121121","P"},
//     {"111111222","Q"},
//     {"211111221","R"},
//     {"112111221","S"},
//     {"111121221","T"},
//     {"221111112","U"},
//     {"122111112","V"},
//     {"222111111","W"},
//     {"121121112","X"},
//     {"221121111","Y"},
//     {"122121111","Z"},
//     {"121111212","-"},
//     {"221111211","."},
//     {"122111211"," "},
//     {"121212111","$"},
//     {"121211121","/"},
//     {"121112121","+"},
//     {"111212121","%"},
//     {"121121211","*"}
// };

// int main () 
// {
//     stdio_init_all();
//     ir_sensor_init();
//     motor_sensor_init();

//     for (;;)
//     {

//         // printf("Left IR sensor status: %d\n", g_left_ir_triggered);
//         // printf("Right IR sensor status: %d\n", g_right_ir_triggered);
//         // sleep_ms(250);


//         // start moving
//         forward();

//         // if both IR sensors are triggered, move forward (maybe need some magneto logic in here)
//         if (g_left_ir_triggered && g_right_ir_triggered)
//         {
//             printf("Both IR sensors on line\n");
//             // move forward
//             forward();
//         }
//         else if (g_left_ir_triggered)
//         {
//             printf("Only left IR sensor on line\n");
//             // turn right
//             turn_right();

//         }
//         else if (g_right_ir_triggered)
//         {
//             printf("Only right IR sensor on line\n");
//             // turn left
//             turn_left();

//         }
//         else
//         {
//             printf("No IR sensors on line\n");
//             // stop
//             stop();

//         }

//         /* if detect collision, stop (idk how ultrasonic sensor code works 
//         but easiest way is just set flag when detech smth lesser than a 
//         certain distance away from car)*/
//         if (collision_detected)
//         {
//             printf("Collision detected\n");
//             // stop
//             stop();
//             // reverse
//             backward();
//         }

//         // if barcode detected, slow down
//         if (g_barcode_detected)
//         {
//             printf("Barcode detected\n");
//             // change speed
//             slow_down(DUTY_CYCLE);
//         }
        
//         // if all barcode ascii is collected
//         if (g_index == ARRAY_SIZE)
//         {

//             // print out pulse width of barcode
//             print_barcode(g_barcode);
            
//             // scale down barcode & store in string
//             char * barcode_ascii_string = scale_down_barcode(g_barcode);
//             printf("%s\n", barcode_ascii_string);

//             // convert barcode ascii to barcode value
//             char * barcode_value_string = convert_barcode(barcode_ascii_string);

//             // print barcode value
//             printf("%s\n", barcode_value_string);
            
//             // reset g_index
//             g_index = 0;

//         }
        
//         sleep_ms(250);
//     }

//     return 0;
// }