cmake_minimum_required(VERSION 3.15)
project(HestonModelProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")

# Search Boost and Eigen using vcpkg
find_package(Boost REQUIRED)
find_package(Eigen3 REQUIRED)

# Add include
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${EIGEN3_INCLUDE_DIR})

# Source files
add_executable(heston_model
    main.cpp
    Model.cpp
    CallPrice.cpp
    CSVReader.cpp
    Calibration.cpp
)

# Link with Boost and QuantLib (Eigen is header-only)
target_link_libraries(heston_model PRIVATE
    ${Boost_LIBRARIES}
)