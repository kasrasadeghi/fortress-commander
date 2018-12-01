#pragma once
#include "Config.h"
#include <array>

template<typename T = unsigned char>
using Grid = std::array<std::array<T, world_size>, world_size>;