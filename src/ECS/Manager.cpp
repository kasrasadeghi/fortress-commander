#include "Manager.h"

#include <algorithm>
#include <iostream>

namespace ECS {
Manager::Manager()
    : _lastEntity(InvalidEntity), _entities(), _componentStores(), _systems() {}

Manager::~Manager() {}

void Manager::addSystem(const System::Ptr& systemPtr) {
  if (!systemPtr || systemPtr->getRequiredComponents().empty()) {
    throw std::runtime_error("System should specify required components");
  }

  _systems.push_back(systemPtr);
}

std::size_t Manager::registerEntity(const Entity entity) {
  std::size_t associatedSystems = 0;

  auto entityIt = _entities.find(entity);
  if (entityIt == _entities.end()) {
    throw std::runtime_error("The entity doesn't exist");
  }

  auto entityComponents = entityIt->second;

  int systemNum = 0;
  for (auto system = _systems.begin(); system != _systems.end(); ++system) {
    const auto& systemRequiredComponents = (*system)->getRequiredComponents();

    if (std::includes(entityComponents.begin(), entityComponents.end(),
                      systemRequiredComponents.begin(),
                      systemRequiredComponents.end())) {
      (*system)->registerEntity(entity);
      ++associatedSystems;
    }
  }

  return associatedSystems;
}

std::size_t Manager::unregisterEntity(const Entity entity) {
  std::size_t associatedSystems = 0;

  auto entityIt = _entities.find(entity);
  if (entityIt == _entities.end()) {
    throw std::runtime_error("The entity doesn't exist");
  }

  auto entityComponents = (*entityIt).second;

  for (auto system = _systems.begin(); system != _systems.end(); ++system) {
    associatedSystems += (*system)->unregisterEntity(entity);
  }

  return associatedSystems;
}

std::size_t Manager::updateEntities(float dt) {
  std::size_t updatedEntities = 0;

  for (auto system = _systems.begin(); system != _systems.end(); ++system) {
    updatedEntities += (*system)->updateEntities(dt);
  }

  return updatedEntities;
}
} // namespace ECS
