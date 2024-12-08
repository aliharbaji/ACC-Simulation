#ifndef GLOBALS_H
#define GLOBALS_H

// Libraries
#include <atomic>
#include <iostream>
#include <thread>
#include <fstream>
#include <nlohmann/json.hpp>
#include <mutex>
#include <memory>

#include "../Car/Car.h"
#include "../ACC/ACC.h"
#include "../Sensor/Sensor.h"

using namespace std;
using json = nlohmann::json;

extern std::atomic<bool> running; // Declaration of the global variable

#define JSON_PATH "Utility/car_params.json"
//#define JSON_PATH "../Utility/car_params.json" // for debugging only

#define DELTA_TIME 0.01

#endif // GLOBALS_H