#ifndef ACC_H
#define ACC_H
#include "nlohmann/json.hpp"
using json = nlohmann::json;
using namespace std;


double get_lidar_distance(const Car& red_car, const Car& blue_car);
double get_lidar_relative_speed(const Car& red_car, const Car& blue_car);

void move_cars(Car& red_car, Car& blue_car);

void print_status(const Car& red_car, const Car& blue_car, double distance);

void adjust_speed(Car& car, double lidar_distance);

void update_cars_from_json(const std::string& file_path, Car& red_car, Car& blue_car);
void update_json_from_cars(const std::string& file_path, Car& red_car, Car& blue_car);
class AdaptiveCruiseControl{
private:
    Car& car;    // Current speed of the vehicle (km/h)
    double targetSpeed;      // Desired speed set by the driver (km/h)
    double safeDistance;     // Minimum safe distance to maintain (meters)
    double currentDistance;  // Current distance from the car ahead (meters)
    double maxAcceleration;  // Maximum acceleration rate (m/s²)
    double maxDeceleration;  // Maximum deceleration rate (m/s²)
    double relativeSpeed;
    double k1;  // Sensitivity for speed difference
    double k2;  // Sensitivity for distance difference
public:
    // Constructor
    AdaptiveCruiseControl(Car& controlledCar, double targetSpeed = 10.0, double safeDistance = 50.0,
                          double maxAcc = 10.0, double maxDec = 1.0, double relativeSpeed = 0.0,
                          double k1 = 1.0, double k2 = 2.0)
        : car(controlledCar), targetSpeed(targetSpeed), safeDistance(safeDistance),
          currentDistance(0.0), maxAcceleration(maxAcc), maxDeceleration(maxDec),
          relativeSpeed(relativeSpeed), k1(k1), k2(k2) {}



    void setSafeDistance(double distance);
    // Update the distance to the car ahead
    void updateDistance(double distance);
    // Update the relative speed to the car ahead
    void updateRelativeSpeed(double relative_speed);
    // Adjust the car's speed based on the distance and target speed
    void adjustSpeed();
    // Display current status
    void displayStatus() const;
};
#endif //ACC_H