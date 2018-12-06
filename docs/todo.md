# view and basic rendering > active
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
- toggle debug mode
  - doesn't render pathfinding or enemy line of sight
- visualize unit/enemy attack
- textures
>- todo
- health bar
- view.zoom for scroll wheel?
  - https://www.youtube.com/watch?v=FSoFJAmh96g
- remove keyboard view controls ?
- fog of war ?
- extract debug draw from unit/enemy draw
- bullet flash
- view edge lock bug
  - moving to far to the edge freaks out the display
- enemy incoming indicator
- death animation

# enemy AI > active
- make enemy spawn not bound to framerate, but a timer
  - waves
- attack units
- attack structures
- follow unit if in view range
- attack structures in view range
- bug: units keep attacking for a bit afterwords
>- todo
- wander towards center even if blocked
- hordes
- potential field AI
- light for units
- constexpr movement speed for types
- variable movement speed for movement calculation
- random movement speed for zombies?

# selection, commands > active
- select a unit
- move command
- attack command
- attack move command
>- todo
- shift select, control select
- stop move

# map gen > maybe
>- todo
- load a map from a file
- make a map

# battle system > active
- ranged attack
- health
- melee attack
- passive hostile detection
>- todo
- attack-move
- check walk until in attack range
- should units attack while moving ?
- should attack closest enemy, not first one found
- should stop attacking target when moving out of range
- unit veterancy ? 

# structures and resources > active
- build structures (MouseDown)
- structure holo
- draw structures
- make structures not build on other structures
- check if structure construction is in bounds
- sell blocks
>- todo
- main construction structure
- color illegal area for construction
- multiblock structures?
- resources to place unit
- area of valid construction

# pathfinding > active
- floodfill
- cache path
- farthest tile you can see
- show path
- farthest tile you can see from you current target
- bug: seesPoint only checks if your center can see, but not if your sides can see
- don't walk in water
- update cached path on command
- bug: doesn't actually go to the tile farthest along the path that you can see, 
  - it goes to the tile farthest along the path that is contiguously visible
- update path on structure construction
- update path on structure sell
- don't collide with walls
>- todo
- don't get stuck on walls
- command group
  - group a command unit's pathfinding if they're adj
- A*
  - JPS optimization
- multithreaded pathfinding (nuclear option)
  - pathfinding update queue and asynchronous approach ?
  - job queue
- constrained Delaunay triangulation ?

# advanced graphics > stalled
- text rendering
- line rendering
- sprite rendering
- particle system
>- todo
- 2D texture atlas ?

# graphics optimization > stalled
- instanced colors too ?
>- todo
- instanced colors by sectioning ? 
- geometry shaders 

# project meta > active
- clang-format
>- todo
- make GameState members not have underscores because they're not private
- maybe not take a const ref to a shared ptr in addSystem ?
- move _performAttack to AttackComponent

# graphics: ergonomics and refactoring > done
- terrain paint
- shape renaming
- single position constructor
- alpha value color
- base shape instance
- vertex array and vertex buffer #refactoring
>- todo

# world > done
- add a world that can be drawn
- click the map to change the tile
  - click and drag
  - click scroll the side
- render the world based on the tiles
- clean up tile coloring
- it just crashes sometimes??
  - the cell flips were out of bounds
- extract region
- extract tile
>- todo

# units > done
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
- check if unit construction is in bounds
>- todo

# ecs and event manager > done
- create
- static
- entity delete
>- todo

# glfw-opengl > done
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
