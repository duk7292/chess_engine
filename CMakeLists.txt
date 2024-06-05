cmake_minimum_required(VERSION 3.2 )

# Set the C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(ChessEngine)

# Find Torch
set(CMAKE_PREFIX_PATH /home/duk/Dependecies/libtorch/share/cmake/Torch)

find_package(Torch REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")

# Include directories for your project
include_directories(${TORCH_INCLUDE_DIRS} "./Chess_Library")

# Collect source files
file(GLOB SOURCES "*.cpp")
file(GLOB CHESS_LIB_SOURCES "Chess_Library/*.cpp")

# Define the executable
add_executable(${PROJECT_NAME} ${SOURCES} ${CHESS_LIB_SOURCES})

# Link against LibTorch
target_link_libraries(${PROJECT_NAME} "${TORCH_LIBRARIES}")