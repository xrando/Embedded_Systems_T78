#include "line_sensor.h"
#include "../barcode_module/barcode_module.h"
#include "../motor/motor.h"
#include "magnetometer.h"
#include "ultrasonic.h"


bool g_left_ir_triggered  = false;
bool g_right_ir_triggered = false;
#define TRIGGER_PIN         16
#define ECHO_PIN            17

float target_heading = 0.0f;
bool is_turning = false;

void start_turning(directions turn_direction) 
{
    magnetometer_data current_data = read_and_calculate_heading();
    float current_heading = current_data.heading;

    switch (turn_direction) 
    {
        case LEFT:
            // Interpret LEFT as turn left 90 degrees
            target_heading = fmod(current_heading - 90.0f + 360.0f, 360.0f);
            turn_left();
            is_turning = true;
            printf("Turning left\n");
            break;
        case RIGHT:
            // Interpret RIGHT as turn right 90 degrees
            target_heading = fmod(current_heading + 90.0f, 360.0f);
            turn_right();
            is_turning = true;
            printf("Turning right\n");
            break;
        case FRONT:
            // Interpret FRONT as going forward (no change in heading)
            target_heading = current_heading;
            forward();
            printf("Going forward\n");
            break;
        case BACK:
            // Interpret BACK as turn 180 degrees
            target_heading = fmod(current_heading + 180.0f, 360.0f);
            backward();
            printf("Going backward\n");
            break;
        default:
            // Handle unknown direction
            printf("Unknown turn direction\n");
            break;
    }
    is_turning = true;
}

void check_turn_completion() 
{
    if (is_turning) 
    {
        magnetometer_data current_data = read_and_calculate_heading();
        float heading_difference = fabs(current_data.heading - target_heading);
        
        if (heading_difference < 5.0f) 
        {  // 5 degrees tolerance
            stop();
            is_turning = false;
        }
    }
}

int main ()
{
    stdio_init_all();
    motor_sensor_init();
    // ir_sensor_init();
    // init_i2c();
    // init_magnetometer();
    // setup_init_heading();
    ultrasonic_init(TRIGGER_PIN, ECHO_PIN);
    // monitor_magnetometer();

    initial_heading = 0.0f; // define starting heading as 0.0f

    magnetometer_data data; 

    // for(;;)
    // {
    //     forward();
    //     float distance = ultrasonic_kalman_filter(ultrasonic_get_distance(TRIGGER_PIN, ECHO_PIN));  

    //     if (distance >= 30.0f)
    //     {
    //         printf("SEANNNNNNN\n");
    //         forward();
    //     }
    //     else
    //     {
    //         stop();
    //         // sleep_ms(500); // 0.5s delay before it starts reversing
    //         // backward();
    //         // sleep_ms(4000); // 4s delay before it starts turning
    //         // stop();
    //          // Here you can decide what to do next, e.g., turning or waiting
    //     }
    //     sleep_ms(250);
    // }


//     for(;;)
//     {
//         set_speed(4);
//         // forward();
//         float distance = ultrasonic_kalman_filter(ultrasonic_get_distance(TRIGGER_PIN, ECHO_PIN));  
//         // printf("Object is about %.2f cm away (Original) \n", ultrasonic_get_distance(TRIGGER_PIN, ECHO_PIN));
//         // printf("Object is about %.2f cm away (Kalman)\n", distance);

//         if (distance >= 30.0f)
//         {
//             printf("SEANNNNNNN\n");
//             forward();
//         }
//         else
//         {
//             stop();
//             // sleep_ms(500); // 0.5s delay before it starts reversing
//             // backward();
//             // sleep_ms(4000); // 4s delay before it starts turning
//             // stop();
//              // Here you can decide what to do next, e.g., turning or waiting
//         }
//         sleep_ms(250);
//         monitor_magnetometer();
//         if (set_initial_heading)
//         {
//             // Read current heading and calculate relative direction
//             data = read_and_calculate_heading();
//             check_boundary_hit(&data);

//             // Decide actions based on the relative direction
//             if (data.heading_direction == LEFT) 
//             {
//                 printf("Turn left\n");
//                 // If the robot has turned left relative to the initial heading,
//                 // you might want to adjust its course.
//             }
//             else if (data.heading_direction == RIGHT)
//             {
//                 printf("Turn right\n");
//                 // If the robot has turned right relative to the initial heading,
//                 // you might want to adjust its course.
//             }
//             else if (data.heading_direction == FRONT)
//             {
//                 printf("Go straight\n");
//                 // If the robot is going straight, you might want to adjust its course.
//             }
//             else if (data.heading_direction == BACK)
//             {
//                 printf("Go back\n");
//                 // If the robot is going back, you might want to adjust its course.
//             }
//             else
//             {
//                 printf("Unknown direction\n");
//                 // If the robot is going back, you might want to adjust its course.
//             }
//         }
//         printf("Left IR sensor status: %d\n", g_left_ir_triggered);
//         printf("Right IR sensor status: %d\n", g_right_ir_triggered);


//         if (!g_left_ir_triggered && !g_right_ir_triggered)
//         {
//             // printf("Both IR sensors never hit line\n");
//             if (distance >= 30.0f)
//             {
//                 forward();
//             }
//             else
//             {
//                 stop();
//                 sleep_ms(500); // 0.5s delay before it starts reversing
//                 backward();
//                 sleep_ms(4000); // 4s delay before it starts turning
//                 stop();
//                 // Here you can decide what to do next, e.g., turning or waiting
//             }
//         }
//         else if (g_left_ir_triggered)
//         {
//             // printf("left IR sensor hit line\n");
//             // turn right
//             start_turning(RIGHT);
//             // turn_right();
//             // check_boundary_hit(&data);

//         }
//         else if (g_right_ir_triggered)
//         {
//             // printf("right IR sensor hit line\n");
//             // turn left
//             start_turning(LEFT);
//             // turn_left();

//         }
//         else
//         {
//             // printf("Both IR sensors hit line\n");
//             // stop
//             stop();


//             //backward();
//         }
//         // After initiating a turn, continuously check if the turn is complete
//         if (is_turning) {
//             check_turn_completion();
//         }

//     }
// }

    // for(;;)
    // {
    //     set_speed(2);
    //     // forward();
    //     float distance = ultrasonic_kalman_filter(ultrasonic_get_distance(TRIGGER_PIN, ECHO_PIN));  
    //     // printf("Object is about %.2f cm away (Original) \n", ultrasonic_get_distance(TRIGGER_PIN, ECHO_PIN));
    //     // printf("Object is about %.2f cm away (Kalman)\n", distance);

    //     // if (distance >= 30.0f)
    //     // {
    //     //     printf("SEANNNNNNN\n");
    //     //     forward();
    //     // }
    //     // else
    //     // {
    //     //     stop();
    //     //     sleep_ms(500); // 0.5s delay before it starts reversing
    //     //     backward();
    //     //     sleep_ms(4000); // 4s delay before it starts turning
    //     //     stop();
    //     //      // Here you can decide what to do next, e.g., turning or waiting
    //     // }
    //     sleep_ms(250);
    //     monitor_magnetometer();
    //     // if (set_initial_heading)
    //     // {
    //     //     // Read current heading and calculate relative direction
    //     //     data = read_and_calculate_heading();
    //     //     check_boundary_hit(&data);

    //     //     // Decide actions based on the relative direction
    //     //     if (data.heading_direction == LEFT) 
    //     //     {
    //     //         printf("Turn left\n");
    //     //         // If the robot has turned left relative to the initial heading,
    //     //         // you might want to adjust its course.
    //     //     }
    //     //     else if (data.heading_direction == RIGHT)
    //     //     {
    //     //         printf("Turn right\n");
    //     //         // If the robot has turned right relative to the initial heading,
    //     //         // you might want to adjust its course.
    //     //     }
    //     //     else if (data.heading_direction == FRONT)
    //     //     {
    //     //         printf("Go straight\n");
    //     //         // If the robot is going straight, you might want to adjust its course.
    //     //     }
    //     //     else if (data.heading_direction == BACK)
    //     //     {
    //     //         printf("Go back\n");
    //     //         // If the robot is going back, you might want to adjust its course.
    //     //     }
    //     //     else
    //     //     {
    //     //         printf("Unknown direction\n");
    //     //         // If the robot is going back, you might want to adjust its course.
    //     //     }
    //     // }
    //     // printf("Left IR sensor status: %d\n", g_left_ir_triggered);
    //     // printf("Right IR sensor status: %d\n", g_right_ir_triggered);
    //     if (!g_left_ir_triggered && !g_right_ir_triggered)
    //     {
    //         // printf("Both IR sensors never hit line\n");
    //         if (distance >= 30.0f)
    //         {
    //             forward();
    //         }
    //         else
    //         {
    //             stop();
    //             sleep_ms(500); // 0.5s delay before it starts reversing
    //             backward();
    //             sleep_ms(4000); // 4s delay before it starts turning
    //             stop();
    //             // Here you can decide what to do next, e.g., turning or waiting
    //         }
    //     }
    //     else if (g_left_ir_triggered)
    //     {
    //         // printf("left IR sensor hit line\n");
    //         // turn right
    //         start_turning(RIGHT);
    //         // turn_right();
    //         // check_boundary_hit(&data);

    //     }
    //     else if (g_right_ir_triggered)
    //     {
    //         // printf("right IR sensor hit line\n");
    //         // turn left
    //         start_turning(LEFT);
    //         // turn_left();

    //     }
    //     else
    //     {
    //         // printf("Both IR sensors hit line\n");
    //         // stop
    //         stop();


    //         //backward();
    //     }
    //     // After initiating a turn, continuously check if the turn is complete
    //     if (is_turning) {
    //         check_turn_completion();
    //     }


    // }
// }

    for(;;)
    {
        set_speed(2);
        // forward();
        float distance = ultrasonic_kalman_filter(ultrasonic_get_distance(TRIGGER_PIN, ECHO_PIN));  

        // sleep_ms(250);
        // monitor_magnetometer();

        if (!g_left_ir_triggered && !g_right_ir_triggered)
        {
            // printf("Both IR sensors never hit line\n");
            if (distance >= 30.0f)
            {
                forward();
            }
            else
            {
                stop();
                // sleep_ms(500); // 0.5s delay before it starts reversing
                // backward();
                // sleep_ms(4000); // 4s delay before it starts turning
                // stop();
                // Here you can decide what to do next, e.g., turning or waiting
            }
        }
    //     else if (g_left_ir_triggered)
    //     {
    //         // printf("left IR sensor hit line\n");
    //         // turn right
    //         // start_turning(RIGHT);
    //         turn_right();
    //         // check_boundary_hit(&data);

    //     }
    //     else if (g_right_ir_triggered)
    //     {
    //         // printf("right IR sensor hit line\n");
    //         // turn left
    //         // start_turning(LEFT);
    //         turn_left();

    //     }
    //     else
    //     {
    //         // printf("Both IR sensors hit line\n");
    //         // stop
    //         stop();

    //         //backward();
    //     }
    //     // // After initiating a turn, continuously check if the turn is complete
    //     // if (is_turning) {
    //     //     check_turn_completion();
    //     // }


    }
}



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

        // // if both IR sensors are triggered, move forward (maybe need some magneto logic in here)
        // if (g_left_ir_triggered && g_right_ir_triggered)
        // {
        //     printf("Both IR sensors on line\n");
        //     // move forward
        //     forward();
        // }
        // else if (g_left_ir_triggered)
        // {
        //     printf("Only left IR sensor on line\n");
        //     // turn right
        //     turn_right();

        // }
        // else if (g_right_ir_triggered)
        // {
        //     printf("Only right IR sensor on line\n");
        //     // turn left
        //     turn_left();

        // }
        // else
        // {
        //     printf("No IR sensors on line\n");
        //     // stop
        //     stop();

        // }

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