#pragma once

#include "ECS/Manager.h"

#include "Components.h"

#include "Config.h"
#include "Graphics.h"

class World;

class Structure {
public:
  const ECS::Entity id;

  constexpr static HealthValue health = 500;

  Structure(glm::vec2 pos, World&);

  glm::vec2 pos() const;  

  inline bool operator==(const Structure& o) const {
    return pos() == o.pos();
  }
};

namespace std {
template <>
struct hash<glm::vec2> {
  size_t operator()(const glm::vec2& k) const {
    return std::hash<float>()(k.x) ^ std::hash<float>()(k.y);
  }
};
} // namespace std

namespace std {
template <>
struct hash<Structure> {
  size_t operator()(const Structure& k) const {
    return std::hash<glm::vec2>()(k.pos());
  }
};
} // namespace std