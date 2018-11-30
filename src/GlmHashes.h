#pragma once

#include <glm/vec2.hpp>

#include <functional>

namespace std {
  
template <>
struct hash<glm::ivec2> {
  size_t operator()(const glm::ivec2& k) const {
    return std::hash<int>()(k.x) ^ std::hash<int>()(k.y);
  }
};

template <>
struct hash<glm::vec2> {
  size_t operator()(const glm::vec2& k) const {
    return std::hash<float>()(k.x) ^ std::hash<float>()(k.y);
  }
};

} // namespace std
