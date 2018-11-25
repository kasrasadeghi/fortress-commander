#pragma once

#include <cstdint>

namespace ECS {
// Entity is just an identifier, mapping to a set of component types in Manager
using Entity = std::size_t;

static const Entity InvalidEntity = 0;
} // namespace ECS
