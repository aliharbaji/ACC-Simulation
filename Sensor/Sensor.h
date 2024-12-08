//
// Created by ali-pc on 12/8/24.
//

#ifndef ACC_SENSOR_H
#define ACC_SENSOR_H

#include "../Car/Car.h"

class Sensor {
protected:
    Car& target_car;
public:
    Sensor(Car& target_car): target_car(target_car){}

    virtual double getLocation() const = 0;

    virtual double getSpeed() const = 0;

};


#endif //ACC_SENSOR_H
