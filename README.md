# Fortress Commander
Fortress Commander is a game about protecting your fortress from incoming waves of enemies. It descends from the RTS genre, and incorporates many classic RTS game mechanics, such as passive resource gathering, a battle system, and the assignment of actions and destinations to controllable entities (to which the game engine finds the shortest path). The game is implemented in its own custom Entity-Component System and pub-sub Event Manager. The graphics were entirely written in OpenGL.

# Features
- An Entity-Component-System framework that we use extensively in the game
- An event manager used for decoupling events from their handlers
- A tiling system complete with unit/structure creation and world editing
- A\* pathfinding system
- An OpenGL-based 2D graphics interface
- Input Handling
- Atlassing Texture System
- A window manager wrapped around GLFW3
- Text Rendering from Freetype
- Perlin Noise-based Terrain Generation

## Systems
Fortress Commander implements the following systems:
- Pathfinding
- Battle
- Particles
- EnemySpawning
- Resources
- Health
- UnitCommand

## Running
If you have the correct dependencies installed, simply run `make`. The cmake build will be triggered, and the makefile will run the game binary for you.

## Dependencies
GLFW3
FreeType

## Tools
clang-format
gdb
valgrind
CMake
GNU Make
OpenGL
GLFW3
FreeType
