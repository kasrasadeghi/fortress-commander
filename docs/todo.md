# project meta
- clang-format
>- todo

# view and basic rendering
- render grid
- view grid
- keyboard move view
- mouse move view
- sleep for framerate limiting
- start the camera at one of the 4 corners (chose top left)
- lock the camera to view
- bug: it crashes when detecting mouse position
  - mouse position was out of the world sometimes
- clean up lock camera code
- add framerate
- start camera in middle of world
>- todo
- view.zoom for scroll wheel?
  - https://www.youtube.com/watch?v=FSoFJAmh96g

# ecs and event manager
- create
- static
>- todo
- entity delete

# world
- add a world that can be drawn
- click the map to change the tile
  - click and drag
  - click scroll the side
- render the world based on the tiles
- clean up tile coloring
- it just crashes sometimes??
  - the cell flips were out of bounds
>- todo
- extract region
- extract tile ?
  - put static holo in there

# units
- draw a single unit
- add a unit in UNIT mode
- draw multiple units stored in vector
- properly utilize tile_size so we can change it (needed for fonts)
- extract world
- use world_size
  - replace _region.size() with world_size constexpr in World
- modes (completed 4 or 5 ago)
  - build. unit, etc
- move units
- center unit creation holo
- render unit centered at pos
- extract tile_size from world
  - so we don't have cyclic dependencies and post-scaling
  - check radius of unit
- extract holos
>- todo

# enemies
- make enemy spawn not bound to framerate, but a timer
  - waves
>- todo

# selection, commands
- select a unit
- move command
- attack command
- attack move command
>- todo
- shift select, control select

# map gen
- load a map from a file
- make a map

# battle system
- ranged attack
- health
>- todo
- melee attack

# structures and resources
- build structures (MouseDown)
- draw structures
>- todo
- special base structure
- check if unit construction is in bounds
- check if structure construction is in bounds
- color illegal area for construction
- multiblock structures?

# pathfinding
- floodfill
- cache path
- farthest tile you can see
- show path
- farthest tile you can see from you current target
- bug: seesPoint only checks if your center can see, but not if your sides can see
- don't walk in water
- update cached path on command
>- todo
- bug: doesn't actually go to the tile farthest along the path that you can see, 
  - it goes to the tile farthest along the path that is contiguously visible
- update cached path on world update
- group a command unit's pathfinding if they're adj
- A*
- don't collide with walls
- constrained Delaunay triangulation ?

# glfw-opengl
- cache shaders
- rectangle shape
- render tiles
- fixed rectangleshape memory leak
- correct screen size #view
- keyboard movement #view
- mouse callback #view
- mouse movement #view
- lock view to world #view
- set modes
- cpu culling #graphics
- instanced rectangle optimization #graphics
- instanced arrays #graphics
- draw circle #graphics
- unit placement
- units moving
- unit holo
- terrain holo
>- feature parity!
- rotation
>- todo

# advanced graphics
- text rendering
- line rendering
>- todo
- sprite rendering
- text renderer takes in a view and draws to the view ?

# ergonomics and refactoring
- terrain paint
- shape renaming
- single position constructor
- alpha value color
- base shape instance
- vertex array and vertex buffer #refactoring
>- todo

# graphics optimization
>- todo
- geometry shaders 
- instanced colors too ?
- instanced colors by sectioning ? 
- sending more instances at once ? 
- precomputation of noninstanced vertex rectangles ? 