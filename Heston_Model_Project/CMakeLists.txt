cmake_minimum_required(VERSION 3.15)
project(HestonModelProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")

# Recherche Boost et Eigen via vcpkg
find_package(Boost REQUIRED)
find_package(Eigen3 REQUIRED)

# Ajout des includes
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${EIGEN3_INCLUDE_DIR})

# Fichiers source
add_executable(heston_model
    main.cpp
    Model.cpp
    CallPrice.cpp
    Calibration.cpp
)

# Lien avec Boost (Eigen est header-only)
target_link_libraries(heston_model PRIVATE ${Boost_LIBRARIES})
