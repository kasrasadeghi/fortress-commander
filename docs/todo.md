# misc
## done
- render grid
- view grid
- keyboard move view
- mouse move view
- sleep for framerate limiting
- add a world that can be drawn
- clang-format
- start the camera at one of the 4 corners (chose top left)
- lock the camera to view
- it crashes when detecting mouse position
- clean up lock camera code
- click the map to change the tile
  - click and drag
  - click scroll the side
- render the world based on the tiles
- clean up tile coloring
- it just crashes sometimes??
  - the cell flips were out of bounds
- standardize semi-java case naming conventions (only under for private);
- draw a single unit
- add a unit in UNIT mode
- draw multiple units stored in vector
- properly utilize tile_size so we can change it (needed for fonts)
- add framerate
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

## todo
- unit selection
- extract tile
  - put static holo in there
- build structures instead of flipping cells
  - maybe a region edit mode?
- check if unit construction is in bounds
- check if structure construction is in bounds
- color illegal area for construction
- multiblock structures?
- view.zoom for scroll wheel?
  - https://www.youtube.com/watch?v=FSoFJAmh96g
- load a map from a file
- make a map

# glfw-opengl
- cache shaders
- rectangle shape
- render tiles
- fixed rectangleshape memory leak
- graphics optimization
  - cpu culling
  - instanced rectangle optimization

  - instanced colors too ?
  - vertex instances instead of uniforms

- make view actually work
  - correct screen size
  - keyboard movement

  - mouse movement

- unit placement
- terrain paint
- holos
- units moving