cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 17)
if (CMAKE_BUILD_TYPE MATCHES "Debug")
    if(NOT CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64 -Wall -pthread -fopenmp")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /openmp")
    endif()
elseif (CMAKE_BUILD_TYPE MATCHES "Release")
    if(NOT CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -m64 -Wall -mtune=native -pthread -fopenmp")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O2 /openmp")
    endif()
endif ()

add_subdirectory(LavaVk)
include_directories("LavaVk")

project(BSpline)

add_executable(BSpline bspline_assignment/bsplinesample.cpp bspline_assignment/bsplinesample.h bspline_assignment/bsplinesceneloader.cpp bspline_assignment/bsplinesceneloader.h bspline_assignment/positiontrackerscript.cpp bspline_assignment/positiontrackerscript.h bspline_assignment/bsplineanimation.cpp bspline_assignment/bsplineanimation.h)

target_link_libraries(BSpline LavaVK)

project(Particles)

add_executable(Particles particles_assignment/particlessample.cpp particles_assignment/particlessample.h particles_assignment/particlessceneloader.cpp particles_assignment/particlessceneloader.h particles_assignment/waterfallanimation.cpp particles_assignment/waterfallanimation.h)

target_link_libraries(Particles LavaVK)

project(Deferred)

add_executable(Deferred deferred_assignment/deferredsample.cpp deferred_assignment/deferredsample.h deferred_assignment/deferredsceneloader.cpp deferred_assignment/deferredsceneloader.h)

target_link_libraries(Deferred LavaVK)
