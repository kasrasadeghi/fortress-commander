# Game Tech Final Project - Fortress Commander

# Game concept
## RTS
resources
- passive income
- passive income in region

enemies
- appear at edges of screen
- come to attack your base

world
- tile based world

camera
- move by going to edges
- views of part of the world

pathfinding
- pathfind to grid cell
- collision with units
- can only settle on a nonempty grid cell
- when a unit is not moving it occupies that grid cell

battle system
- no priority
  - attack any hostiles seen
- hostile priority
  - move until hostile in vision
- move priority
  - move until at location or unable to move more

unit selection
- draw a box
- click on the unit

structures
- barracks
- refinery
- base
- walls?

win/loss condition
- endless waves of enemies
- 'win' by getting the highest score

## Camera style
2D top-down view
- Easier to implement if we are worried about time
- Easier to do passable art for unless we go for some art-free or stylized theme

## Technology
[SFML](https://www.sfml-dev.org/)

# Features
## AI + motion planning
- Well-explored problem with lots of easy solutions
- Decision trees?
- A\* on grid-based world
- A\* + navmesh
  - Navmesh generation sucks
- “Potential” or “flow” fields
  - Compute the direction from each cell to a single target using a flood fill
- Flocking

## Procedural generation
- Easy because it’s all made up
- Looks really cool
- [Dungeon generation](https://www.gamasutra.com/blogs/AAdonaac/20150903/252889/Procedural_Dungeon_Generation_Algorithm.php) super cool article for dungeons or buildings maybe
- [Perlin noise](https://mrl.nyu.edu/~perlin/noise/) our lord and savior for terrain generation
  - There is also a 4D version
- [Coherent noise](http://libnoise.sourceforge.net/glossary) very good glossary for noise terminology
![tinykeep’s “dungen”](https://i.imgur.com/wM30Xyl.gif)
![some island map I made out of perlin noise](https://d2mxuefqeaa7sj.cloudfront.net/s_88FBEBEE3376E0D40867DEB07BD57349772DAC286AF57CD038EA97CB45EE3493_1542054661846_image.png)

# Division of Labor
- Game mechanics
  - Austin
  - Kasra
- Graphics
  - Logan
  - Kasra
- Art
  - Austin
  - Logan
  - Kasra
- Engine structure
  - Austin
