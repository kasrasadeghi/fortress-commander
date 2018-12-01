#pragma once

#include <deque>
#include <glm/vec2.hpp>

#include "Region.h"

using Path = std::deque<glm::ivec2>;

template<typename T = unsigned char>
using Grid = std::array<std::array<T, world_size>, world_size>;

/// returns an empty path on failure
Path findPath(Region& region, glm::ivec2 start, glm::ivec2 end);