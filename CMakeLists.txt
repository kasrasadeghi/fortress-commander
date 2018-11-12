cmake_minimum_required(VERSION 3.12)

set(PROJECT_NAME fortress-commander)
project(${PROJECT_NAME})

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

file(GLOB SRC_FILES src/*.cpp)
add_executable(${PROJECT_NAME} ${SRC_FILES})
target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system)