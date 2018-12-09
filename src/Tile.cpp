
#include "Tile.h"

// clang-format off
const std::unordered_map<Tile, const TileType> TileProperties::_data{
    {Tile::NONE, TileType{
        .color = {1.f, 0.f, 1.f, 1.f}, 
        .walkable = false,
        .texOffset = 0, // TODO: texture
        }},
    {Tile::GRASS, TileType{
        .color = {0.3f, 0.8f, 0.2f, 1.f}, 
        .walkable = true,
        .texOffset = 1,
        }},
    {Tile::SAND, TileType{
        .color = {0.6f, 0.6f, 0.4f, 1.f},
        .walkable = true,
        .texOffset = 3,
    }},
    {Tile::WATER, TileType{
        .color = {0.1f, 0.2f, 0.7f, 1.f}, 
        .walkable = false,
        .texOffset = 4,
        }},
    };
// clang-format on