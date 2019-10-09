#include "Manager.h"

#include <algorithm>
#include <iostream>

namespace ECS {
Manager::Manager() : _lastEntity(InvalidEntityId) {}

Manager::~Manager() {}

void Manager::_addSystem(const System::Ptr& systemPtr) {
  if (systemPtr->getRequiredComponents().empty()) {
    throw std::runtime_error("System should specify required components");
  }

  _systems.push_back(systemPtr);
}

std::size_t Manager::_registerEntity(const Entity entity) {
  std::size_t associatedSystems = 0;

  // Find the appropritate Entity -> ComponentTypeSet entry
  auto entityIt = _entities.find(entity);
  if (entityIt == _entities.end()) {
    throw std::runtime_error("The given entity doesn't exist");
  }

  auto entityComponents = entityIt->second;

  // For each system in _systems, let's perform a check to see if the registered
  // entity has all the components required by the system
  for (auto& system : _systems) {
    const auto& systemRequiredComponents = system->getRequiredComponents();

    if (std::includes(entityComponents.begin(), entityComponents.end(),
                      systemRequiredComponents.begin(), systemRequiredComponents.end())) {
      // If the entity applies to the system criteria, then let's register it with the system
      system->registerEntity(entity);
      ++associatedSystems;
    }
  }

  return associatedSystems;
}

std::size_t Manager::_unregisterEntity(const Entity entity) {
  std::size_t associatedSystems = 0;

  // Find the appropritate Entity -> ComponentTypeSet entry
  auto entityIt = _entities.find(entity);
  if (entityIt == _entities.end()) {
    return associatedSystems; // the entity doesn't exist, so maybe it has been manually unregistered
  }

  auto entityComponents = entityIt->second;

  for (auto& system : _systems) {
    associatedSystems += system->unregisterEntity(entity);
  }

  return associatedSystems;
}

std::size_t Manager::_update(float dt) {
  std::size_t updatedSystems = 0;

  // Update each system in _systems with the dt given to Manager
  for (auto& system : _systems) {
    if (system->update(dt)) {
      ++updatedSystems;
    }
  }

  for (auto id : _deleteSet) {
    _unregisterEntity(id);
    _entities.erase(id);
  }

  return updatedSystems;
}

bool Manager::_clear() {
  _entities.clear();

  return _entities.size() == 0;
}

bool Manager::_hasEntity(Entity entity) const {
  return _entities.find(entity) != _entities.end();
}
} // namespace ECS
