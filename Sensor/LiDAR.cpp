//
// Created by ali-pc on 12/8/24.
//

#include "LiDAR.h"
#include "../Utility/globals.h"

double LiDAR::getDistance() {
    double distance = std::sqrt(std::pow(target_car.getX() - host_car.getX(), 2) +
            std::pow(target_car.getY() - host_car.getY(), 2));
    return distance;
}

double LiDAR::getRelativeSpeed() {
    double rel_speed = target_car.getSpeed() - host_car.getSpeed();
    return rel_speed;
}
