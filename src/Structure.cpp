#include "Structure.h"
#include "Game.h"

#include "World.h"

// clang-format off
const std::unordered_map<StructureType, const StructureData> StructureProperties::_data{
    {StructureType::NONE, StructureData{
        .health = 0,
        .resourceSpeed = 0,
        .cost = 0,
        .texOffset = 0
    }},
    {StructureType::DEFAULT, StructureData{
        .health = 500,
        .resourceSpeed = 1.0f,
        .cost = 100.f,
        .texOffset = 2
    }},
    {StructureType::BASE, StructureData{
        .health = 1000,
        .resourceSpeed = 3.0f,
        .cost = 0.f,
        .texOffset = 6
    }},
    {StructureType::WALL, StructureData{
        .health = 500,
        .resourceSpeed = 0.f,
        .cost = 25.f,
        .texOffset = 7
    }}
};
// clang-format on

Structure::Structure(glm::vec2 pos, World& world, StructureType t)
    : id(ECS::Manager::createEntity()) {
  auto& prop = StructureProperties::of(t);

  health = prop.health;
  resourceSpeed = prop.resourceSpeed;
  cost = prop.cost;
  texOffset = prop.texOffset;

  ECS::Manager::addComponent<TransformComponent>(id, TransformComponent(world, pos, 0.f));
  ECS::Manager::addComponent<HealthComponent>(id, HealthComponent(health));
  ECS::Manager::addComponent<ResourceComponent>(id, ResourceComponent(resourceSpeed));
  ECS::Manager::addComponent<LightComponent>(id, LightComponent({1.f, 0.95f, 0.85f, 1.f}, 3.f));

  ECS::Manager::registerEntity(id);
}

glm::vec2 Structure::pos() const {
  return ECS::Manager::getComponent<TransformComponent>(id).pos;
}
