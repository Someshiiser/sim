cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR) # It tells the minimum cmake version neede to compile the code and gives error if probelm.
project(Simulation) # tells cmake to create a project and name the project.

find_package(Geant4 REQUIRED ui_all vis_all) # first find the package geant4 and then we need user interfacea nd visualization.

include(${Geant4_USE_FILE}) # tells cmake to include files for the geant4 in this path.

file(GLOB sources ${PROJECT_SOURCE_DIR}/*.cc) # it create a global variable "sources" and then {} is the folder where our script is positioned. /*.cc tell the cmake to take all source files from that folder and compile it later with gcc or c++.
file(GLOB headers ${PROJECT_SOURCE_DIR}/*.hh)

add_executable(sim sim.cc ${sources} ${headers})
target_link_libraries(sim ${Geant4_LIBRARIES})

add_custom_target(Simulation DEPENDS sim)
