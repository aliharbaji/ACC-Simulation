//
// Created by allih on 26/11/2024.
//

#ifndef ACC_CAR_H
#define ACC_CAR_H

#include <mutex>

// Constants for the simulation
const double MAX_SPEED = 5.0;  // Maximum speed of the red car
const double MIN_SPEED = 0.5;  // Minimum speed (red car doesn't stop)
const double SAFE_DISTANCE = 50.0;  // The minimum safe distance to the blue car
const double ACCELERATION = 0.1;  // The amount to change speed based on distance

// Car structure
class Car {
private:
    double x;
    double y;
    double speed;
    double acceleration;
    std::mutex car_mutex;
public:
    Car(double x, double y, double speed, double acceleration = 0);

    void adjust_speed(double distance);

    double getX() {std::lock_guard<std::mutex> lock(car_mutex); return x;}
    double getY()  {std::lock_guard<std::mutex> lock(car_mutex); return y;}
    double getSpeed();
    double getAcceleration() {std::lock_guard<std::mutex> lock(car_mutex);return acceleration;}


    void setAt(double posX, double posY){
        std::lock_guard<std::mutex> lock(car_mutex);
        x = posX;
        y = posY;
    }

    void setVelocity(double vel){std::lock_guard<std::mutex> lock(car_mutex); speed = vel;}
    void setAcceleration(double acc){std::lock_guard<std::mutex> lock(car_mutex); acceleration = acc;}
    void go(double deltaTime);
};


#endif //ACC_CAR_H
