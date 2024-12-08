//
// Created by allih on 26/11/2024.
//
#include "Car.h"
#include "../Sensor/Sensor.h"

Car::Car(double x, double y, double speed, double acceleration, std::shared_ptr<AdaptiveCruiseControl> acc,
         std::shared_ptr<Sensor> front_bumper, std::shared_ptr<Sensor> rear_bumper)
        : x(x), y(y), speed(speed), acceleration(acceleration),
          acc(acc), front_bumper(front_bumper), rear_bumper(rear_bumper) {}



void Car::adjust_speed(double distance) {
    std::lock_guard<std::mutex> lock(car_mutex);
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
    std::lock_guard<std::mutex> lock(car_mutex);
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

double Car::getSpeed() {
    std::lock_guard<std::mutex> lock(car_mutex);
    return speed;

}

double Car::getFrontBumperRelativeSpeed() const{
    double relative_speed = front_bumper->getSpeed() - this->speed;

    return relative_speed;
}

double Car::getFrontBumperDistance() const{
//    double distance = std::sqrt(std::pow(front_bumper->getLocation() - this->x, 2) + std::pow(blue_car.getY() - red_car.getY(), 2));
    double distance = abs(front_bumper->getLocation() - this->x);
    return distance;
}

