cmake_minimum_required(VERSION 3.12)

set(CMAKE_BUILD_TYPE Release)
#set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# When in debug mode, we run Address Sanitizer to find memory errors
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g -fsanitize=address")

# list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
# include(cmake/clang-format.cmake)
include(cmake/sfml.cmake)
include(cmake/glad_glfw.cmake)

set(PROJECT_NAME fortress-commander)
project(${PROJECT_NAME})

file(GLOB SRC_FILES src/*.cpp glad/src/*.c)

add_executable(${PROJECT_NAME} ${SRC_FILES})

setup_sfml(${PROJECT_NAME})

setup_glad_glfw(${PROJECT_NAME})

# copy over ttf file
file(COPY arial.ttf DESTINATION ${CMAKE_BINARY_DIR})