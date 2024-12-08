#include <iostream>
#include "Car/Car.h"
#include "ACC/ACC.h"
#include <thread>
#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#define JSON_PATH "Utility/car_params.json"
#define DELTA_TIME 0.01

using namespace std;
int main() {
    // Initialize the cars
    Car red_car(0.0, 400, 2.0, 1);  // Starting position and speed of the red car
    Car blue_car(400, 400, 1.0); // Starting position of the blue car (obstacle)

    json json_file;
    AdaptiveCruiseControl acc(red_car, red_car.getSpeed() * 2, 200, 5, 2, 0, 1, 1);

    const std::string json_file_path = JSON_PATH;

    // Check if the file exists
    std::ifstream input_file(json_file_path);
    if (input_file.is_open()) {
        try {
            input_file >> json_file; // Parse JSON from the file
            std::cout << "Successfully loaded JSON file.\n";
        } catch (const json::parse_error& e) {
            std::cerr << "Error parsing JSON: " << e.what() << '\n';
            json_file = {}; // Initialize as empty JSON
        }
    } else {
        std::cout << "JSON file not found. Initializing new JSON object.\n";
        json_file = {}; // Initialize as empty JSON
    }
    input_file.close();


    // Simulation loop
    while (true) {

    // Update JSON with the latest car states
    update_json_from_cars(json_file_path, red_car, blue_car);

    // Update cars' positions and velocities from JSON
    update_cars_from_json(json_file_path, red_car, blue_car);

    // Get the distance between the red car and the blue car using LIDAR
    double lidar_distance = get_lidar_distance(red_car, blue_car);

    // Get the relative speed between the red car and the blue car using LIDAR
    double relative_speed = get_lidar_relative_speed(red_car, blue_car);

    acc.updateRelativeSpeed(relative_speed);

    acc.updateDistance(lidar_distance);

    acc.adjustSpeed();

    // Move both cars
    red_car.go(DELTA_TIME);
    blue_car.go(DELTA_TIME);

    // Print status
    acc.displayStatus();

    // Wait for a short period before the next update (to simulate real-time behavior)
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

    return 0;
}
