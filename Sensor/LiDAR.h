//
// Created by ali-pc on 12/8/24.
//

#ifndef ACC_LIDAR_H
#define ACC_LIDAR_H
#include "../Utility/globals.h"

class LiDAR : public Sensor {
private:

public:
    LiDAR(Car& host_car, Car& target_car): Sensor(host_car, target_car){}

    double getDistance() override;

    double getRelativeSpeed() override;
};


#endif //ACC_LIDAR_H
