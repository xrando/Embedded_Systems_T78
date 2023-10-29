#include <stdio.h>
#include <string.h>
#include <math.h>  

#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"

#define CUSTOM_I2C_SDA_PIN 0  // GP0 for SDA
#define CUSTOM_I2C_SCL_PIN 1  // GP1 for SCL
#define MAGNETOMETER_ADDR  0x1E  
#define i2c_default_port   i2c0

// Define magneto registers values from LSM303DLHC (MY-511) datasheet 
#define MY511_CRA_REG_M            0x00
#define MY511_CRB_REG_M            0x01
#define MY511_MR_REG_M             0x02
#define MY511_OUT_X_H_M            0x03
#define MY511_OUT_X_L_M            0x04
#define MY511_OUT_Z_H_M            0x05
#define MY511_OUT_Z_L_M            0x06
#define MY511_OUT_Y_H_M            0x07
#define MY511_OUT_Y_L_M            0x08
#define MY511_SR_REG_M             0x09

typedef struct 
{
    int16_t      mag[3];
    float        heading;
    const char * direction;
} magnetometer_data;

// Function prototypes
static void       init_i2c ();
static void       init_magnetometer ();
static void       monitor_magnetometer ();
static void       magnetometer_read_raw (int16_t mag[3]);
static void       calculate_heading (magnetometer_data *data);
static const char * heading_direction (float heading);
magnetometer_data read_and_calculate_heading ();

static void
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

static void
init_magnetometer () 
{
    uint8_t config[2] = {0};
    
	// Select CRA register(0x00)
	// Data output rate = 15Hz(0x10)
	config[0] = MY511_CRA_REG_M;
	config[1] = 0x10;
    i2c_write_blocking(i2c_default_port, MAGNETOMETER_ADDR, config, 2, false);

	// Select CRB register(0x01)
	// Set gain = +/- 1.3g(0x20)
	config[0] = MY511_CRB_REG_M;
	config[1] = 0x20;
    i2c_write_blocking(i2c_default_port, MAGNETOMETER_ADDR, config, 2, false);

    // Continuous conversion mode
    uint8_t buf[] = {MY511_MR_REG_M, MY511_CRA_REG_M};  
    i2c_write_blocking(i2c_default_port, MAGNETOMETER_ADDR, buf, 2, false);
}

static void
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

static const char *
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

static void
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

magnetometer_data read_and_calculate_heading () 
{
    magnetometer_data data;
    magnetometer_read_raw(data.mag);
    calculate_heading(&data);
    return data;
}

static void
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

int 
main() 
{
    stdio_init_all();
    init_i2c();
    init_magnetometer();
    monitor_magnetometer();
}