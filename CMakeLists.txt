cmake_minimum_required(VERSION 3.12)

set(CMAKE_BUILD_TYPE Release)
#set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# When in debug mode, we run Address Sanitizer to find memory errors
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -fmax-errors=1 -Wno-unused-parameter")
# set(CMAKE_CXX_FLAGS_DEBUG "-g -fsanitize=address")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
# include(cmake/clang-format.cmake)
# include(cmake/sfml.cmake)
include(cmake/glad_glfw.cmake)
include(cmake/utils.cmake)
include(cmake/gtest.cmake)
include(cmake/freetype.cmake)

set(PROJECT_NAME fortress-commander)
project(${PROJECT_NAME})

set(SRC_FILES
  src/Game.cpp
  src/Unit.cpp
  src/Enemy.cpp
  src/World.cpp
  src/Tile.cpp
  src/Path.cpp
  src/Region.cpp
  src/GameState.cpp
  src/Components.cpp
  src/Systems/MoveSystem.cpp
  src/Structure.cpp
  src/Particles.cpp
  src/Graphics/Shader.cpp
  src/Graphics/ResourceManager.cpp
  src/ECS/Manager.cpp
  glad/src/glad.c
)

link_directories(lib)

if(TEST)
  set(PROJECT_NAME "${PROJECT_NAME}_TEST")
  add_executable(${PROJECT_NAME} src/Test.cpp ${SRC_FILES})
  setup_gtest(${PROJECT_NAME})
else(TEST)
  add_executable(${PROJECT_NAME} src/main.cpp ${SRC_FILES})
endif(TEST)

setup_glad_glfw(${PROJECT_NAME})
setup_freetype(${PROJECT_NAME})


# copy over resources
copy_to_bin_dir(fonts shaders textures)
