//
// Created by ali-pc on 12/8/24.
//

#ifndef ACC_SENSOR_H
#define ACC_SENSOR_H

#include "../Car/Car.h"

class Sensor {
protected:
    Car& host_car;
    Car& target_car;
public:
    Sensor(Car& host_car, Car& target_car): host_car(host_car), target_car(target_car){}

    virtual double getDistance() = 0;

    virtual double getRelativeSpeed() = 0;

};


#endif //ACC_SENSOR_H
