cmake_minimum_required(VERSION 3.12)

set(PROJECT_NAME fortress-commander)
project(${PROJECT_NAME})

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)
message(STATUS "SFML libraries found: " ${SFML_LIBRARIES})
message(STATUS "SFML include dir found: " ${SFML_INCLUDE_DIR})

file(GLOB SRC_FILES src/*.cpp)
file(GLOB INCL_FILES src/*.h)

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC
    ${INCL_FILES}
    ${SFML_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    ${SFML_LIBRARIES}
    sfml-graphics
    sfml-window
    sfml-system
)
