Adaptive Cruise Control Simulation

Overview

This project simulates an Adaptive Cruise Control (ACC) system for a car (referred to as red_car) that adjusts its speed based on real-time data from a LiDAR sensor. 
The ACC unit ensures safe driving by controlling the speed of red_car based on the target speed, maximum acceleration, 
and the distance between red_car and a moving test car (referred to as blue_car). 
The goal of this project is to create a scalable and multi-threaded simulation that can handle inputs from multiple LiDAR sensors and simulate multiple moving cars on the road.

Features

* Realistic ACC Control: Simulates an ACC unit that smoothly accelerates, decelerates, and maintains a safe distance from the car ahead.

  The forumla used is:
  
  ```a = k1​⋅ speedDifference + k2​ ⋅ distanceDifference```
  
    a: Acceleration or deceleration of the host vehicle (m/s²).
  
    speedDifference: The difference in speed between the target speed and the current speed of the host vehicle (targetSpeed−currentSpeedtargetSpeed−currentSpeed).
  
    distanceDifference: The difference between the current distance to the vehicle in front and the desired safe distance (currentDistance − safeDistance).

  k_1​: Sensitivity to speed difference. This determines how aggressively the system will accelerate or decelerate to match the target speed.

  k_2​: Sensitivity to distance difference. This determines how the system adjusts based on the distance to the car in front to maintain a safe following distance.
* LiDAR Simulation: Mimics the output of a LiDAR sensor to provide real-time data for distance and speed measurements.
  
* Dynamic Speed Adjustment: The red_car accelerates until it reaches the target speed and maintains a safe distance from the blue_car, then smoothly decelerates to match the blue_car's speed.
* Multi-threading Support (not yet implemented): Future implementation will include multi-threading for handling input from multiple LiDAR sensors and simulating the behavior of multiple moving cars.


Future Enhancements

* Multi-threaded LiDAR Input: Implement multi-threading to receive and process data from multiple LiDAR sensors simultaneously.
* Multiple Cars Simulation: Add more moving cars to simulate complex traffic scenarios.
* User Interface: Build an interface to modify parameters and view simulation outputs in real-time.
* Advanced Control Algorithms: Enhance the ACC unit with more sophisticated algorithms for better speed matching and distance management.

The project will be extended to support multi-threading, allowing for real-time data handling from multiple LiDAR sensors.
Multiple moving cars will be added to simulate real traffic scenarios.


Installations and Usage

0. Install requirements:
```bash
sudo apt-get install nlohmann-json3-dev
pip3 install pygame
```
1. Clone the repository:
  ```bash
  git clone https://github.com/yourusername/acc-simulation.git
  cd acc-simulation
```

2. Build the project:
  ```bash
  make clean
  make
  make run
 ```
  

