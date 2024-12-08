import json
import os

import pygame
import numpy as np
import random
import math
import time

from pygame import font

# Path to JSON
JSON_PATH = 'Utility/car_params.json'
# Absolute path to the Utility directory
# utility_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), 'Utility'))
# file_path = os.path.join(utility_dir, 'car_params.json')

# Constants for the simulation
SCREEN_WIDTH = 800
SCREEN_HEIGHT = 600
SCAN_RANGE = 100  # Maximum range of the LIDAR in pixels
NUM_RAYS = 360  # Number of rays in the LIDAR scan
FPS = 60  # Frames per second for real-time simulation

# Colors
RED = (255, 0, 0)
BLUE = (0, 0, 255)
BLACK = (0, 0, 0)
WHITE = (255, 255, 255)

# Car initial positions
BLUE_CAR_POS = [400, 300]  # Red car (our vehicle)
RED_CAR_POS = [100, 300]  # Blue car (obstacle)

# Car velocities (will be updated from file)
car_velocity = 3
blue_car_velocity = 0

# Simulation initialization
pygame.init()
screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT))
pygame.display.set_caption("LIDAR Simulation with Speed Sync")
clock = pygame.time.Clock()
font = pygame.font.Font(None, 24)

car_params = {
    'red_car': {'velocity': car_velocity, 'acceleration': 0, 'x': RED_CAR_POS[0], 'y': RED_CAR_POS[1]},
    'blue_car': {'velocity': blue_car_velocity, 'acceleration': 0, 'x': BLUE_CAR_POS[0], 'y': BLUE_CAR_POS[1]}
}


def read_car_params_from_json():
    """Read car parameters from a JSON file"""
    global car_params, RED_CAR_POS, BLUE_CAR_POS
    try:
        if os.path.exists(JSON_PATH):
            with open(JSON_PATHlear
                    , 'r') as f:
                new_params = json.load(f)

                # Update car parameters
                car_params = new_params

                # Update car positions
                RED_CAR_POS[0] = car_params['red_car']['x']
                RED_CAR_POS[1] = car_params['red_car']['y']
                BLUE_CAR_POS[0] = car_params['blue_car']['x']
                BLUE_CAR_POS[1] = car_params['blue_car']['y']

                # Print detailed car information
                print("Red Car Parameters:")
                print(f"  Velocity: {car_params['red_car']['velocity']} m/s")
                print(f"  Position: ({car_params['red_car']['x']}, {car_params['red_car']['y']})")
                print(f"  Acceleration: {car_params['red_car']['acceleration']} m/s²")

                print("\nBlue Car Parameters:")
                print(f"  Velocity: {car_params['blue_car']['velocity']} m/s")
                print(f"  Position: ({car_params['blue_car']['x']}, {car_params['blue_car']['y']})")
                print(f"  Acceleration: {car_params['blue_car']['acceleration']} m/s²")
                print("-" * 50)  # Separator
    except Exception as e:
        print(f"Error reading car parameters: {e}")


# LIDAR Sensor simulation (same as before)
def simulate_lidar_scan(red_car_pos):
    lidar_data = []

    # Create a set of random positions for objects (like the blue car)
    # Blue car represents an obstacle in the environment
    blue_car_distance = np.sqrt((BLUE_CAR_POS[0] - red_car_pos[0]) ** 2 + (BLUE_CAR_POS[1] - red_car_pos[1]) ** 2)

    # LIDAR scanning
    for angle in range(NUM_RAYS):
        min_distance = SCAN_RANGE  # Initialize with max range

        # Check the distance to the blue car in each direction
        angle_rad = math.radians(angle)
        x_offset = math.cos(angle_rad) * blue_car_distance
        y_offset = math.sin(angle_rad) * blue_car_distance

        distance_to_blue_car = np.sqrt((BLUE_CAR_POS[0] - (red_car_pos[0] + x_offset)) ** 2 +
                                       (BLUE_CAR_POS[1] - (red_car_pos[1] + y_offset)) ** 2)

        # If the distance is within the range of the LIDAR, update the minimum distance
        if distance_to_blue_car < min_distance:
            min_distance = distance_to_blue_car
        lidar_data.append(min_distance)

    return lidar_data


# Function to plot LIDAR scan visualization
def plot_lidar_data(red_car_pos, lidar_data):
    for angle in range(NUM_RAYS):
        angle_rad = math.radians(angle)
        distance = lidar_data[angle]
        end_x = red_car_pos[0] + math.cos(angle_rad) * distance
        end_y = red_car_pos[1] + math.sin(angle_rad) * distance
        pygame.draw.line(screen, WHITE, red_car_pos, (end_x, end_y), 1)


# Main loop
running = True
speed_check_counter = 0  # To control how often we check speeds
while running:
    screen.fill(BLACK)

    # Read updated car parameters from file less frequently
    speed_check_counter += 1
    if speed_check_counter >= 60:  # Check every 60 frames (about once per second)
        read_car_params_from_json()


        speed_check_counter = 0

    # Event handling
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    # Move the red car (our vehicle) based on keyboard input and current velocity
    keys = pygame.key.get_pressed()
    velocity = car_params['red_car']['velocity']
    if keys[pygame.K_LEFT]:
        RED_CAR_POS[0] -= velocity
    if keys[pygame.K_RIGHT]:
        RED_CAR_POS[0] += velocity
    if keys[pygame.K_UP]:
        RED_CAR_POS[1] -= velocity
    if keys[pygame.K_DOWN]:
        RED_CAR_POS[1] += velocity

    # Move the blue car (obstacle) automatically
    # blue_velocity = car_params['blue_car']['velocity']
    # BLUE_CAR_POS[0] += blue_velocity
    # if BLUE_CAR_POS[0] > SCREEN_WIDTH:
    #     BLUE_CAR_POS[0] = 0  # Reset the blue car to the left side of the screen

    # write_car_params_to_json()
    # Simulate LIDAR scan for the red car
    lidar_data = simulate_lidar_scan(RED_CAR_POS)

    # Plot LIDAR data (simulate the rays)
    plot_lidar_data(RED_CAR_POS, lidar_data)

    # Draw the cars
    pygame.draw.rect(screen, RED, (RED_CAR_POS[0] - 20, RED_CAR_POS[1] - 10, 40, 20))  # Red car
    pygame.draw.rect(screen, BLUE, (BLUE_CAR_POS[0] - 20, BLUE_CAR_POS[1] - 10, 40, 20))  # Blue car

    # Display car information on screen
    red_info = font.render(
        f"Red Car: V={car_params['red_car']['velocity']:.2f}, "
        f"A={car_params['red_car']['acceleration']:.2f}, "
        f"Pos=({RED_CAR_POS[0]:.2f}, {RED_CAR_POS[1]:.2f})",
        True, WHITE
    )
    blue_info = font.render(
        f"Blue Car: V={car_params['blue_car']['velocity']:.2f}, "
        f"A={car_params['blue_car']['acceleration']:.2f}, "
        f"Pos=({BLUE_CAR_POS[0]:.2f}, {BLUE_CAR_POS[1]:.2f})",
        True, WHITE
    )
    screen.blit(red_info, (10, 10))
    screen.blit(blue_info, (10, 40))

    # Update the display
    pygame.display.flip()

    # Limit the frame rate
    clock.tick(FPS)

# Quit the simulation
pygame.quit()
