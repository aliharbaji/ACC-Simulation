#include "Car/Car.h"
#include "ACC/ACC.h"

#include <iostream>
#include <thread>
#include <fstream>


using namespace std;

using json = nlohmann::json;

#define JSON_PATH "Utility/car_params.json"
//#define JSON_PATH "../Utility/car_params.json" // for debugging only

#define DELTA_TIME 0.01
std::atomic<bool> running(true);

int main() {
    // Initialize the cars
    Car red_car(0.0, 400, 2.0, 1);  // Starting position and speed of the red car
    Car blue_car(400, 400, 1.0); // Starting position of the blue car (obstacle)
    Car green_car(0.0, 200, 0, 0);

    json json_file;

    // Initialize ACC for front and rear bumpers
    AdaptiveCruiseControl acc_front_bumper(red_car, red_car.getSpeed() * 2,
                                           200, 5, 2, 0, 1, 0.01);
    AdaptiveCruiseControl acc_rear_bumper(red_car, red_car.getSpeed() * 2,
                                          200, 5, 2, 0, 1, 0.01);

    // Initialize thread names
    string frnt("front bumper");
    string bck("bck bumper");

    thread front_bumper_thread(accThreadFunction, ref(acc_front_bumper), ref(frnt), ref(red_car), ref(blue_car));
    thread rear_bumper_thread(accThreadFunction, ref(acc_rear_bumper), ref(bck), ref(red_car), ref(blue_car));

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

    // Move both cars
    red_car.go(DELTA_TIME);
    blue_car.go(DELTA_TIME);

    // Wait for a short period before the next update (to simulate real-time behavior)
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}
    // Stop threads and clean up
    running = false;
    front_bumper_thread.join();
    rear_bumper_thread.join();

    return 0;
}
