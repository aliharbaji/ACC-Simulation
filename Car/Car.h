//
// Created by allih on 26/11/2024.
//

#ifndef ACC_CAR_H
#define ACC_CAR_H

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
public:
    Car(double x, double y, double speed, double acceleration = 0);

    void adjust_speed(double distance);

    double getX() const {return x;}
    double getY() const {return y;}
    double getSpeed() const {return speed;}
    double getAcceleration() const{return acceleration;}

//    void move(double distX, double distY){
//        x += distX;
//        y += distY;
//    }

    void setAt(double posX, double posY){
        x = posX;
        y = posY;
    }

    void setVelocity(double vel){speed = vel;}
    void setAcceleration(double acc){acceleration = acc;}
    void go(double deltaTime);
};


#endif //ACC_CAR_H
