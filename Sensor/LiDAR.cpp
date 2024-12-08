//
// Created by ali-pc on 12/8/24.
//

#include "LiDAR.h"
#include "../Utility/globals.h"

double LiDAR::getLocation() const{

    return target_car.getX();
}

double LiDAR::getSpeed() const{
    return target_car.getSpeed();

}
