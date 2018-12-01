#pragma once

#include <deque>
#include <glm/vec2.hpp>

#include "Region.h"

using Path = std::deque<glm::ivec2>;

/// returns an empty path on failure
Path findPath(Region& region, glm::ivec2 start, glm::ivec2 end);