#include "line_sensor.h"
#include "barcode_module.h"
#include "motor.h"
#include "magnetometer.h"
#include "ultrasonic.h"

#define ECHO_PIN            17
#define TRIGGER_PIN         16

bool is_turning           = false;
bool g_left_ir_triggered  = false;
bool g_right_ir_triggered = false;

float target_heading      = 0.0f;

/*!
 * @brief Initiates a turning action based on the specified direction.
 *
 * Reads current heading from a magnetometer, interprets the desired direction,
 * and starts the corresponding turn (left, right, forward, or backward).
 * Updates the target heading accordingly.
 *
 * @param turn_direction Desired turning direction.
 */
void 
start_turning (directions turn_direction) 
{
    magnetometer_data current_data      = read_and_calculate_heading();
    float             current_heading   = current_data.heading;

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
            printf("Unknown turn direction\n");
            break;

    }

    is_turning = true;
} /* start_turning() */

/*!
 * @brief Checks if a turning action has been completed.
 *
 * If the car is currently in a turning state, it reads the current heading
 * from a magnetometer, calculates the difference with the target heading,
 * and stops the turn if the difference is less than 5.0 degrees.
 */
void 
check_turn_completion () 
{
    if (is_turning) 
    {

        magnetometer_data current_data = read_and_calculate_heading();

        float heading_difference = fabs(current_data.heading - target_heading);
        
        if (heading_difference < 5.0f) 
        {  

            stop();
            is_turning = false;

        }

    }

} /* check_turn_completion() */

/*!
 * @brief Initiates a left rotation by 90 degrees.
 *
 * Reads the initial heading, calculates the target heading for a left turn,
 * and begins the turn. It continuously checks the current heading and stops
 * the rotation when within 5.0 degrees of the target.
 */
void 
rotate_left () 
{
    float init_head       = get_current_heading();
    float target_heading  = fmod(init_head - 90.0 + 360.0, 360.0);

    turn_left();

    for(;;)
    {
        float curr_heading = get_current_heading();
        
        if (is_within_range(curr_heading, target_heading, 5.0)) 
        {
            
            stop();
            break;

        }

    }
} /* rotate_left() */

/*!
 * @brief Initiates a right rotation by 90 degrees.
 *
 * Reads the initial heading, calculates the target heading for a right turn,
 * and begins the turn. It continuously checks the current heading and stops
 * the rotation when within 5.0 degrees of the target.
 */
void 
rotate_right () 
{
    float init_heading = get_current_heading();
    float target_heading = fmod(init_heading + 90.0, 360.0);

    turn_right();

    for(;;)
    {
        float current_heading = get_current_heading();
        
        if (is_within_range(current_heading, target_heading, 5.0)) 
        {

            stop(); 
            break;

        }

    }
} /* rotate_right() */

int 
main ()
{
    stdio_init_all();
    motor_sensor_init();
    ir_sensor_init();
    init_i2c();
    init_magnetometer();
    setup_init_heading();
    init_ultrasonic(TRIGGER_PIN, ECHO_PIN);

    magnetometer_data data;
    initial_heading = 0.0f; 

    for(;;)
    {
        set_speed(1);
        float distance = calculate_kalman_filter(get_distance
                                                  (TRIGGER_PIN, ECHO_PIN));  

        // Check if the car is on either the left or right boundary
        if (!g_left_ir_triggered && !g_right_ir_triggered)
        {

            // Check if the car is within 30cm of the obstacle
            if (distance >= 30.0f)
            {

                forward();

            }
            else
            {

                stop();
                sleep_ms(500); 
                backward();
                sleep_ms(3000); 
                stop();

            }

        }
        else if (g_left_ir_triggered)
        {

            start_turning(RIGHT);

        }
        else if (g_right_ir_triggered)
        {

            start_turning(LEFT);

        }
        else
        {

            stop();
            backward();

        }

        sleep_ms(250);
        monitor_magnetometer();
        
        if (set_initial_heading)
        {

            data = read_and_calculate_heading();
            check_boundary_hit(&data);

            if (data.heading_direction == LEFT) 
            {

                printf("Turn left\n");
                turn_left();

            }
            else if (data.heading_direction == RIGHT)
            {

                printf("Turn right\n");
                turn_right();

            }
            else if (data.heading_direction == FRONT)
            {

                printf("Go straight\n");
                forward();

            }
            else if (data.heading_direction == BACK)
            {

                printf("Go back\n");
                backward();

            }
            else
            {

                printf("Unknown direction\n");
                stop();

            }

        }
        if (is_turning) 
        {

            check_turn_completion();
            
        }
    }
} /* main() */

/*** end of file ***/