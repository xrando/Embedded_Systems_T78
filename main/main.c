#include "line_sensor.h"
#include "../barcode_module/barcode_module.h"
#include "../motor/motor.h"
#include "magnetometer.h"
#include "ultrasonic.h"
// #include "wifi.h"
// #include "Iwipopts.h"
// #include "cgi.h"
// #include "ssi.h"


// bool g_left_ir_triggered  = false;
// bool g_right_ir_triggered = false;
#define TRIGGER_PIN         16
#define ECHO_PIN            17

float target_heading = 0.0f;
bool is_turning = false;

void
start_turning (directions turn_direction) 
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

void
check_turn_completion () 
{
    if (is_turning) 
    {

        magnetometer_data current_data = read_and_calculate_heading();
        float heading_difference = fabs(current_data.heading - target_heading);
        
        if (heading_difference < 5.0f) 
        {  
            
            // 5 degrees tolerance
            stop();
            is_turning = false;

        }

    }
}

void
rotate_left () 
{
    float init_head = get_current_heading();
    float target_heading = fmod(init_head - 90.0 + 360.0, 360.0);

    turn_left();  // Replace with your function to start turning left

    while (1) 
    {

        float curr_heading = get_current_heading();
        if (is_within_range(curr_heading, target_heading, 5.0)) 
        {

            // Within 5 degrees of target heading, stop turning
            stop();  // Replace with your function to stop turning
            break;

        }

    }
}

void
rotate_right() 
{
    float init_heading = get_current_heading();
    float target_heading = fmod(init_heading + 90.0, 360.0);

    turn_right();  // Replace with your function to start turning right

    while (1) 
    {
        float current_heading = get_current_heading();
        if (is_within_range(current_heading, target_heading, 5.0)) 
        {

            // Within 5 degrees of target heading, stop turning
            stop();  // Replace with your function to stop turning
            break;

        }
    }
}

bool
is_within_range (float heading, float target, float range) 
{
    float diff = fmod(fabs(heading - target + 360.0), 360.0);
    return (diff <= range || diff >= (360.0 - range));
}

void
main_loop() 
{
    for (;;) 
    {
        set_speed(4);
        float distance = ultrasonic_kalman_filter(ultrasonic_get_distance(TRIGGER_PIN, ECHO_PIN));  
        printf("Distance: %.2f cm\n", distance);

        if (distance < 10.0f) 
        {

            handle_obstacle();

        } 
        else 
        {

            handle_no_obstacle();

        }

        sleep_ms(250);
        monitor_magnetometer();
        process_sensor_data(distance);
    }
}

void
handle_obstacle() 
{
    stop();
    set_speed(1);
    backward();
    sleep_ms(950);
    stop();
    sleep_ms(1000);
    // Additional actions for obstacle handling can be added here
}

void
handle_no_obstacle() 
{
    forward();
    // Additional actions when there's no obstacle can be added here
}

void
process_sensor_data(float distance)
{
    magnetometer_data data;

    if (!g_left_ir_triggered && !g_right_ir_triggered) 
    {

        if (distance < 30.0f) 
        {

            stop();
            sleep_ms(500);
            backward();
            sleep_ms(4000);
            stop();

        } 
        else 
        {

            forward();

        }
    } 
    else if (g_left_ir_triggered) 
    {

        start_turning(RIGHT);

    } else if (g_right_ir_triggered) 
    {

        start_turning(LEFT);

    }

    if (is_turning) 
    {

        check_turn_completion();

    }

    if (set_initial_heading) 
    {

        data = read_and_calculate_heading();
        check_boundary_hit(&data);
        handle_heading_direction(data.heading_direction);

    }
}

void
handle_heading_direction(HeadingDirection direction) 
{
    switch (direction) {
        case LEFT:
            printf("Turn left\n");
            break;
        case RIGHT:
            printf("Turn right\n");
            break;
        case FRONT:
            printf("Go straight\n");
            break;
        case BACK:
            printf("Go back\n");
            break;
        default:
            printf("Unknown direction\n");
            break;
    }
}

// int main() {
//     // Initial setup code here
//     main_loop();
//     return 0;
// }

int
main ()
{
    stdio_init_all();
    motor_sensor_init();
    ir_sensor_init();
    // init_i2c();
    // init_magnetometer();
    // setup_init_heading();
    // wifi_init(); //Sean added 
    ultrasonic_init(TRIGGER_PIN, ECHO_PIN);

    initial_heading = 0.0f; // define starting heading as 0.0f

    // float distance = 40.0f;
    set_speed(1);
    // to be used for collision detection
    sleep_ms(3000);

    main_loop();
}    