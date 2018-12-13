#pragma once

#include "ECS/Manager.h"

#include "Components.h"

#include "Config.h"
#include "Graphics.h"

class World;

enum class StructureType { NONE, DEFAULT, BASE, WALL };

namespace std {

template <>
struct hash<StructureType> {
  size_t operator()(const StructureType& t) const {
    return std::hash<int>()(static_cast<int>(t));
  }
};

} // namespace std

struct StructureData {
  HealthValue health;
  ResourceType resourceSpeed;
  ResourceType cost;
  int texOffset;
};

class StructureProperties {
  static const std::unordered_map<StructureType, const StructureData> _data;

public:
  static const StructureData& of(StructureType t) {
    return _data.at(t);
  }
};

class Structure {
public:
  const ECS::Entity id;

  HealthValue health;
  ResourceType resourceSpeed;
  ResourceType cost;
  int texOffset;

  Structure(glm::vec2 pos, World&, StructureType t = StructureType::DEFAULT);

  Structure& operator=(const Structure& o) {
    ECS::Entity& _id = const_cast<ECS::Entity&>(id);
    _id = o.id;
    health = o.health;
    resourceSpeed = o.resourceSpeed;
    cost = o.cost;
    texOffset = o.texOffset;

    return *this;
  }

  glm::vec2 pos() const;
};
