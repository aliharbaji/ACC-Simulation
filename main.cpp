#include "Utility/globals.h"
#include <memory>
#include "Car/Car.h"
#include "Sensor/Sensor.h"
#include "Sensor/LiDAR.h"

extern std::atomic<bool> running; // Declaration of the global variable

int main() {
    // Initialize the cars
    Car red_car(0.0, 400.0, 1.0, 1.0);  // Host car
    Car blue_car(400, 400.0, 1.0, 0.0); // Starting position of the blue car (obstacle)

    // Initialize ACC Unit
    std::shared_ptr<AdaptiveCruiseControl> front_acc = std::make_shared<AdaptiveCruiseControl>(red_car);
    red_car.setACC(front_acc);

    // Initialize sensors
    std::shared_ptr<LiDAR> lidar = std::make_shared<LiDAR>(std::ref(blue_car));
    red_car.setFrontSensor(lidar);

    // Initialize json_file
    json json_file;

    // Initialize threads
    string frnt("front bumper"); // thread name
    thread front_bumper_thread(accThreadFunction, ref(front_acc), ref(frnt), ref(red_car), ref(blue_car));

    // Sleep for 1 sec before starting
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    const std::string json_file_path = JSON_PATH;
    // Check if the file exists
    ifstream input_file(json_file_path);
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

    return 0;
}
