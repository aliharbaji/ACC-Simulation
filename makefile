# Variables
ACC_DIR = ACC
CAR_DIR = Car
UTILITY_DIR = Utility
SENSOR_DIR = Sensor
SIMULATION_DIR = Simulation
PYTHON_SIMULATOR = $(SIMULATION_DIR)/LIDAR_sim.py
CPP_FILES = main.cpp $(ACC_DIR)/ACC.cpp $(CAR_DIR)/Car.cpp $(SENSOR_DIR)/Sensor.cpp $(SENSOR_DIR)/LiDAR.cpp $(UTILITY_DIR)/globals.cpp
EXECUTABLE = main
CXX = g++
PYTHON = python3
CXX_FLAGS = -std=c++17 -pthread # Add threading support and other flags if needed

# Targets
.PHONY: all clean run install-deps

all: $(EXECUTABLE)

$(EXECUTABLE): $(CPP_FILES)
	@echo "Compiling C++ files..."
	$(CXX) $(CXX_FLAGS) $(CPP_FILES) -o $(EXECUTABLE)
	@echo "Compilation successful."

install-deps:
	@echo "Installing Python dependencies..."
	pip3 install pygame
	@echo "Python dependencies installed."

run: install-deps all
	@echo "Running Python simulator..."
	$(PYTHON) $(PYTHON_SIMULATOR) &
	@if [ $$? -eq 0 ]; then \
		echo "Python simulator ran successfully."; \
	else \
		echo "Python simulator failed. Exiting."; \
		exit 1; \
	fi

	@echo "Running C++ program..."
	./$(EXECUTABLE)
	@if [ $$? -eq 0 ]; then \
		echo "C++ program ran successfully."; \
	else \
		echo "C++ program failed."; \
		exit 1; \
	fi

clean:
	@echo "Cleaning up..."
	rm -f $(EXECUTABLE)
	@echo "Cleanup complete."
