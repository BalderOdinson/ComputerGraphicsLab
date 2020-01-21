cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-m64 -Wall")

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
