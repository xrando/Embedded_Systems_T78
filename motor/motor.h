// motor.h

#ifndef MOTOR_H
#define MOTOR_H

// Pins to control the Left Motor
#define LEFT_INPUT_PIN 2
#define LEFT_INPUT_PIN_2 3

// Pins to control the Right Motor
#define RIGHT_INPUT_PIN 6
#define RIGHT_INPUT_PIN_2 7

void forward();
void backward();
void turn_left();
void turn_right();
void stop();

#endif // MOTOR_H
