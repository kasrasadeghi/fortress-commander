
#include "Tile.h"

const std::unordered_map<Tile, const TileType> TileProperties::_data{
    {Tile::NONE, TileType{.color = {1.f, 0.f, 1.f, 1.f}, .walkable = false}},
    {Tile::GRASS, TileType{.color = {0.3f, 0.8f, 0.2f, 1.f}, .walkable = true}},
    {Tile::WATER, TileType{.color = {0.1f, 0.2f, 0.7f, 1.f}, .walkable = false}}};