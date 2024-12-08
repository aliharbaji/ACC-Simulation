cmake_minimum_required(VERSION 3.24)
project(ACC)

set(CMAKE_CXX_STANDARD 14)

include_directories(venv/include)
include_directories(venv/include/site)
include_directories(venv/include/site/python3.9)
include_directories(venv/include/site/python3.9/pygame)
include_directories(venv/include/site/python3.9/pygame/include)
include_directories(venv/Lib/site-packages/numpy/_core/include)
include_directories(venv/Lib/site-packages/numpy/_core/include/numpy)
include_directories(venv/Lib/site-packages/numpy/_core/include/numpy/random)
include_directories(venv/Lib/site-packages/numpy/f2py/src)

# Fetch the JSON library
include(FetchContent)
FetchContent_Declare(json
    URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz
)
FetchContent_MakeAvailable(json)

add_executable(ACC
        ACC/ACC.cpp
        main.cpp
        Car/Car.cpp
        ACC/ACC.h
        Utility/globals.h)

# Link the json library
target_link_libraries(ACC PRIVATE nlohmann_json::nlohmann_json)