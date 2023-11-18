/** @file magnetometer.c
 *
 * @brief Magneometer driver for Raspberry Pi Pico
 *
 * @par
 * COPYRIGHT NOTICE: (c) Singapore Institute of Technology
 * @author Low Hong Sheng Jovian (2203654), 2023 All rights reserved.
 */

#include "magnetometer.h"

/*!
 * @brief Initializes I2C for peripheral communication.
 *
 * Configures I2C with a 400 kHz clock, sets up GPIO pins for I2C (SDA and SCL),
 * enables pull-up resistors, and registers these pins for I2C use. This setup
 * is essential for connecting and communicating with low-speed peripherals
 * like sensors.
 */
void
init_i2c () 
{
    // Initialize I2C with 400 kHz clock speed
    i2c_init(i2c_default_port, 400 * 1000);  

    gpio_set_function(CUSTOM_I2C_SDA_PIN, GPIO_FUNC_I2C);  
    gpio_set_function(CUSTOM_I2C_SCL_PIN, GPIO_FUNC_I2C);  

    gpio_pull_up(CUSTOM_I2C_SDA_PIN); 
    gpio_pull_up(CUSTOM_I2C_SCL_PIN);  

    // Register GPIO pins used for I2C in the binary information
    bi_decl(bi_2pins_with_func(CUSTOM_I2C_SDA_PIN, CUSTOM_I2C_SCL_PIN,
                               GPIO_FUNC_I2C));
}

/*!
 * @brief Initializes the magnetometer for data acquisition.
 *
 * This function configures the magnetometer by setting its registers through
 * I2C communication. It selects and sets the CRA register to a data output
 * rate of 15Hz and the CRB register to a gain of +/- 1.3g. It also configures
 * the magnetometer to operate in continuous conversion mode, allowing it to
 * continuously measure and update its data registers.
 */
void
init_magnetometer () 
{
    uint8_t config[2] = {0};
    
	config[0] = MY511_CRA_REG_M;
	config[1] = 0x10; // Data output rate = 15Hz(0x10)
    i2c_write_blocking(i2c_default_port, MAGNETOMETER_ADDR, config, 2, false);

	config[0] = MY511_CRB_REG_M;
	config[1] = 0x20; // Set gain = +/- 1.3g(0x20)
    i2c_write_blocking(i2c_default_port, MAGNETOMETER_ADDR, config, 2, false);

    // Continuous conversion mode
    uint8_t buf[] = {MY511_MR_REG_M, MY511_CRA_REG_M};  
    i2c_write_blocking(i2c_default_port, MAGNETOMETER_ADDR, buf, 2, false);
}

/*!
 * @brief Reads raw data from the magnetometer.
 *
 * This function communicates with the magnetometer via I2C to obtain raw
 * magnetic field data. It sequentially reads the values from the magnetometer's
 * registers corresponding to the X, Y, and Z axes. These values are then
 * combined to form 16-bit integers representing the magnetic field strength
 * along each axis.
 *
 * @param mag[3] An array to store the raw magnetic field strengths for
 *               X, Y, and Z axes.
 */
void
magnetometer_read_raw (int16_t mag[3]) 
{
    uint8_t buffer[6];

    for (int i = 0; i < 6; i++) 
    {
        uint8_t reg = MY511_OUT_X_H_M + i;  // Adjust the register address
        i2c_write_blocking(i2c_default_port, MAGNETOMETER_ADDR, &reg, 1, true);
        i2c_read_blocking(i2c_default_port, MAGNETOMETER_ADDR, &buffer[i], 1,
                          false);
    }

    mag[0] = (int16_t) (buffer[0] << 8) | buffer[1];  // X
    mag[1] = (int16_t) (buffer[4] << 8) | buffer[5];  // Y
    mag[2] = (int16_t) (buffer[2] << 8) | buffer[3];  // Z
}

/*!
 * @brief Determines the compass direction from a given heading angle.
 *
 * This function takes a heading angle in degrees and returns the corresponding
 * compass direction as a string. The heading angle is compared against 
 * predefined ranges that correspond to the eight principal wind directions:
 * North, Northeast, East, Southeast, South, Southwest, West, and Northwest.
 *
 * @param heading The heading angle in degrees, typically obtained from
 * a magnetometer.
 * @return A string representing the compass direction corresponding to
 * the heading angle.
 */
const char *
heading_direction (float heading) 
{
    if (heading >= 22.5f && heading < 67.5f)
        return "Northeast";
    else if (heading >= 67.5f && heading < 112.5f)
        return "East";
    else if (heading >= 112.5f && heading < 157.5f)
        return "Southeast";
    else if (heading >= 157.5f && heading < 202.5f)
        return "South";
    else if (heading >= 202.5f && heading < 247.5f)
        return "Southwest";
    else if (heading >= 247.5f && heading < 292.5f)
        return "West";
    else if (heading >= 292.5f && heading < 337.5f)
        return "Northwest";
    else
        return "North"; // Covers North and remaining degrees towards Northeast
}

/*!
 * @brief Calculates and stores the heading and compass direction from
          magnetometer data.
 *
 * Computes the heading from the X and Y magnetic fields, adjusts it to
 * a 0-360 degree range, and determines the corresponding compass direction.
 * The results are stored in the provided magnetometer_data structure.
 *
 * @param data Pointer to magnetometer_data structure for storing the
 *             computed values.
 */
void
calculate_heading(magnetometer_data *data) 
{
    data->heading = atan2(data->mag[1], data->mag[0]) * (180.0f / M_PI);

    // Ensure heading is between 0 and 360 degrees
    if (data->heading < 0)
    {
        data->heading += 360.0f;
    } 

    // Convert heading to a direction (e.g. North, South, East, West, etc.)
    data->direction = heading_direction(data->heading);
}

/*!
 * @brief Reads raw magnetometer data and calculates the heading and direction.
 *
 * This function first reads the raw data from the magnetometer using 
 * `magnetometer_read_raw`. It then calculates the heading and the corresponding
 * compass direction based on this raw data by calling `calculate_heading`.
 * The final heading and direction are encapsulated in the `magnetometer_data`
 * structure.
 *
 * @return A `magnetometer_data` structure containing the raw magnetic field
 *         values (X, Y, Z), the calculated heading, and the compass direction.
 */
magnetometer_data read_and_calculate_heading () 
{
    magnetometer_data data;
    magnetometer_read_raw(data.mag);
    calculate_heading(&data);
    return data;
}

/*!
 * @brief Continuously monitors and outputs magnetometer readings.
 *
 * In an infinite loop, this function reads magnetometer data, calculates the
 * heading, and prints the X, Y, Z values, heading in degrees, and compass
 * direction. It pauses for 200 milliseconds between iterations to regulate
 * data processing and output frequency.
 */
void
monitor_magnetometer () 
{
    for (;;) 
    {
        magnetometer_data mag_data = read_and_calculate_heading();
        printf("Mag. X = %d, Y = %d, Z = %d, Heading = %.2f°, Direction = %s\n",
               mag_data.mag[0], mag_data.mag[1], mag_data.mag[2], 
               mag_data.heading, mag_data.direction);
        sleep_ms(200);
    }
}