//
// Created by allih on 26/11/2024.
//
#include "Car.h"


Car::Car(double x, double y, double speed, double acceleration) : x(x), y(y), speed(speed), acceleration(acceleration){}

void Car::adjust_speed(double distance) {
    if (distance < SAFE_DISTANCE) {
        speed -= ACCELERATION;  // Slow down
        if (speed < MIN_SPEED) {
            speed = MIN_SPEED;  // Red car won't stop
        }
    }else if (distance > SAFE_DISTANCE * 2) {
        speed += ACCELERATION;  // Speed up
        if (speed > MAX_SPEED) {
            speed = MAX_SPEED;  // Cap the speed to MAX_SPEED
        }
    }
}

void Car::go(double deltaTime) {
    deltaTime = 1;
    // Update position
    double distance = speed * deltaTime + 0.5 * acceleration * deltaTime * deltaTime;
    x += distance;

    // Update speed
    speed += acceleration * deltaTime;

    // Ensure speed does not go below zero
    if (speed < 0) speed = 0;
//    std::cout << "Car moved. New Position: (" << x << ", " << y << ")"
//              << ", Speed: " << speed << " m/s, Acceleration: " << acceleration << " m/s²\n";
    if(x > 800){
        x = 0;
    }

}

