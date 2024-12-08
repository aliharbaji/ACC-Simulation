//
// Created by ali-pc on 12/8/24.
//

#ifndef ACC_LIDAR_H
#define ACC_LIDAR_H
#include "../Utility/globals.h"
#include "Sensor.h"

class LiDAR : public Sensor {
private:

public:
    LiDAR(Car& target_car): Sensor(target_car){}

    double getLocation() const override;

    double getSpeed() const override;
};


#endif //ACC_LIDAR_H
