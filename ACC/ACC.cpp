#include <iostream>
#include <cmath>
#include <thread>
#include <chrono>
#include <vector>
#include <cstdlib>
#include <fstream>
#include "nlohmann/json.hpp"  // You'll need to install this JSON library

#include "../Car/Car.h"
#include "ACC.h"
#include "../Utility/globals.h"


// Function to write car parameters to JSON file
void update_json_from_cars(const std::string& json_file_path, Car& red_car, Car& blue_car) {
    // Create a JSON object to hold car parameters
    json json_file;

    // Red car parameters
    json_file["red_car"] = {
        {"x", red_car.getX()},
        {"y", red_car.getY()},
        {"velocity", red_car.getSpeed()},
        {"acceleration", red_car.getAcceleration()
        }
    };

    // Blue car parameters
    json_file["blue_car"] = {
        {"x", blue_car.getX()},
        {"y", blue_car.getY()},
        {"velocity", blue_car.getSpeed()},
        {"acceleration", blue_car.getAcceleration()
        }

    };

    // Write to the specified file path
    std::ofstream file(json_file_path);
    if (file.is_open()) {
        file << json_file.dump(4);  // Pretty print with 4-space indentation
        file.close();
    } else {
        std::cerr << "Error: Could not open file '" << json_file_path << "' for writing.\n";
    }
}


// Function to simulate the LIDAR scan and get distance to blue car
double get_lidar_distance( Car& red_car,  Car& blue_car) {
    double distance = std::sqrt(std::pow(blue_car.getX() - red_car.getX(), 2) + std::pow(blue_car.getY() - red_car.getY(), 2));
    return distance;
}

double get_lidar_relative_speed( Car& red_car, Car& blue_car) {
    double rel_speed = blue_car.getSpeed() - red_car.getSpeed();
    return rel_speed;
}

void adjust_speed(Car& car, double lidar_distance){
    car.adjust_speed(lidar_distance);
}

// Function to print car positions and speeds for visualization
void print_status( Car& red_car,  Car& blue_car, double distance) {
    std::cout << "Red Car - X: " << red_car.getX() << ", Speed: " << red_car.getSpeed() << " m/s\n";
    std::cout << "Blue Car - X: " << blue_car.getX() << "\n";
    std::cout << "Distance to Blue Car: " << distance << " meters\n";
    std::cout << "-----------------------------------------\n";
}

void update_cars_from_json(const std::string& file_path, Car& red_car, Car& blue_car) {
    std::ifstream file(file_path);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open JSON file." << std::endl;
        return;
    }

    json car_data;
    try {
        file >> car_data;
        // Update red car
        red_car.setAt(car_data["red_car"]["x"].get<double>(), car_data["red_car"]["y"].get<double>());
        red_car.setVelocity(car_data["red_car"]["velocity"].get<double>());
        red_car.setAcceleration(car_data["red_car"]["acceleration"].get<double>());;

        // Update blue car
        blue_car.setAt(car_data["blue_car"]["x"].get<double>(), car_data["blue_car"]["y"].get<double>());
        blue_car.setVelocity(car_data["blue_car"]["velocity"].get<double>());
       blue_car.setAcceleration(car_data["blue_car"]["acceleration"].get<double>());;

    } catch (const json::exception& e) {
        std::cerr << "Error: Failed to parse JSON. " << e.what() << std::endl;
    }
}

void AdaptiveCruiseControl::setSafeDistance(double distance) {
    if (distance > 0) {
        safeDistance = distance;
        std::cout << "Safe distance set to " << safeDistance << " meters.\n";
    } else {
        std::cout << "Invalid distance. Must be greater than 0.\n";
    }
}

void AdaptiveCruiseControl::updateDistance(double distance) {
    currentDistance = distance;
    std::cout << "Current distance from the car ahead: " << currentDistance << " meters.\n";
}

void AdaptiveCruiseControl::updateRelativeSpeed(double relative_speed) {
    relativeSpeed = relative_speed;
    std::cout << "Current relative speed from the car ahead: " << currentDistance << " meters/s\n";
}

void AdaptiveCruiseControl::adjustSpeed() {

    double currentSpeed = car.getSpeed();  // Current speed of the car (km/h)
    double distanceDifference = currentDistance - safeDistance;
    double acceleration;
    // If the distance is greater than the safe distance, we should start accelerating smoothly
    if (distanceDifference > 0) {
        // Calculate the desired acceleration to reach the target speed
        double speedDifference = targetSpeed - currentSpeed;
        acceleration = k1 * speedDifference + k2 * distanceDifference;

        // Smooth acceleration: limit the acceleration to a maximum value and adjust based on the distance difference
        double smoothFactor = std::min(distanceDifference / 100.0, 1.0);  // Smooth factor varies from 0 to 1
        acceleration = smoothFactor * std::min(std::max(acceleration, 0.0), maxAcceleration);

        // Print debugging info
        std::cout << "Accelerating smoothly to reach target speed. Acceleration = " << acceleration << " m/s².\n";
    } else {
        // If the distance is too small, decelerate proportionally to maintain safety
        acceleration = std::min(std::abs(distanceDifference) / 2, maxDeceleration);
        car.setAcceleration(-acceleration);
        std::cout << "Too close! Decelerating: Acceleration = " << -acceleration << " m/s².\n";
        return;
    }

    // Set the car's acceleration to the calculated value
    car.setAcceleration(acceleration);
}


void AdaptiveCruiseControl::displayStatus() const {
    std::cout << "Car Position: (" << car.getX() << ", " << car.getY() << ")\n"
                  << "Car Speed: " << car.getSpeed() << " km/h\n"
                  << "Car Acceleration: " << car.getAcceleration() << " m/s²\n"
                  << "Target Speed: " << targetSpeed << " km/h\n"
                  << "Safe Distance: " << safeDistance << " meters\n"
                  << "Current Relative Speed: " << relativeSpeed << " meters/s\n"
                  << "Current Distance: " << currentDistance << " meters\n";
    cout << "****************************" << endl << endl;
}



// Thread function for ACC control
void accThreadFunction(std::shared_ptr<AdaptiveCruiseControl> acc, string& name, Car& red_car, Car& blue_car) {
    while (running) {
        cout << name << " Thread is running..." << endl;
        acc->displayStatus();
        // Get the distance between the red car and the blue car using LIDAR
        double lidar_distance = acc->getCar().getFrontBumperDistance();

        // Get the relative speed between the red car and the blue car using LIDAR
        double relative_speed = acc->getCar().getFrontBumperRelativeSpeed();
//        double relative_speed = get_lidar_relative_speed(red_car, blue_car);

        // Update ACC parameters
        acc->updateRelativeSpeed(relative_speed);
        acc->updateDistance(lidar_distance);

        // Update test car's parameters
        acc->adjustSpeed();

        // Print ACC status
        acc->displayStatus();

        // Pause to simulate real-time updates, Mutex is automatically unlocked when `lock` goes out of scope
        this_thread::sleep_for(chrono::milliseconds(100));
    }
    cout << "ACC thread stopping..." << endl;
}
