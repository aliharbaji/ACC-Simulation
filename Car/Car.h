//
// Created by allih on 26/11/2024.
//

#ifndef ACC_CAR_H
#define ACC_CAR_H

#include "../Utility/globals.h"
#include "../ACC/ACC.h"
#include "../Sensor/Sensor.h"
class Sensor;

#include <memory>
using namespace std;

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
    shared_ptr<AdaptiveCruiseControl> acc;
    shared_ptr<Sensor> front_bumper;
    shared_ptr<Sensor> rear_bumper;

    std::mutex car_mutex;
public:

    Car(double x, double y, double speed, double acceleration, std::shared_ptr<AdaptiveCruiseControl> acc = nullptr,
        std::shared_ptr<Sensor> front_bumper = nullptr, std::shared_ptr<Sensor> rear_bumper = nullptr);


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
    void setACC(shared_ptr<AdaptiveCruiseControl> acc_ptr) {
        acc = acc_ptr;
    }
//    // Accessor for front bumper distance scan
//    double getFrontBumperDistanceScan() const {
//        return front_bumper ? front_bumper->getDistance() : 0.0;
//    }
//    // Accessor for rear bumper distance scan
//    double getRearBumperDistanceScan() const {
//        return rear_bumper ? rear_bumper->getDistance() : 0.0;
//    }
//    double getFrontBumperRelativeSpeed() const{
//        return front_bumper ? front_bumper->getRelativeSpeed() : 0.0;
//    }
//    double getRearBumperRelativeSpeed() const{
//        return rear_bumper ? rear_bumper->getRelativeSpeed() : 0.0;
//    }
    void setFrontSensor(shared_ptr<Sensor> sensor){
        front_bumper = sensor;
    }
    void setRearSensor(shared_ptr<Sensor> sensor){
        rear_bumper = sensor;
    }

    double getFrontBumperRelativeSpeed() const;
    double getFrontBumperDistance() const;
};


#endif //ACC_CAR_H
