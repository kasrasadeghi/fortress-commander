#pragma once

#include <cstdint>
#include <limits>

namespace ECS {
// Entity is just an identifier, mapping to by ComponentStores
using Entity = std::size_t;

static const Entity InvalidEntityId = 0;
static const Entity MaxEntityId = std::numeric_limits<Entity>::max();
} // namespace ECS
