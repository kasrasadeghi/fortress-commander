#pragma once

#include <algorithm>
#include <cstdint>
#include <functional>
#include <iostream>
#include <memory>
#include <set>

#include "../GameState.h"
#include "Component.h"
#include "Entity.h"
#include "Event.h"

namespace ECS {
class Manager;

class System {
  ComponentTypeSet _requiredComponents;
  std::set<Entity> _matchingEntities;

protected:
  GameState& _gameState;

public:
  using Ptr = std::shared_ptr<System>;

  explicit System(GameState& gameState) : _gameState(gameState) {}
  virtual ~System(){};

  const ComponentTypeSet& getRequiredComponents() const {
    return _requiredComponents;
  }

  // Add the entity to this System's set of entities
  // Returns success in adding the entity
  bool registerEntity(Entity entity) {
    return _matchingEntities.insert(entity).second;
  }

  // Removes the entity from this System's set of entities
  // Returns success in removing the entity
  std::size_t unregisterEntity(Entity entity) {
    return _matchingEntities.erase(entity);
  }

  // Returns true if the entity is in this System's set of entities
  bool hasEntity(Entity entity) const {
    return _matchingEntities.find(entity) != _matchingEntities.end();
  }

  // Call every updateEntity(dt, entity) on this System's set of entities
  // Returns the amount of entities that were updated
  virtual std::size_t update(float dt) {
    std::size_t updatedEntities = 0;

    for (auto entity = _matchingEntities.begin(); entity != _matchingEntities.end(); ++entity) {
      updateEntity(dt, *entity); // each user class should specialize this
                                 // virtual pure function
      ++updatedEntities;
    }

    return updatedEntities;
  }

  // Each user class should specialize this pure virtual method to change
  // how the system updates
  virtual void updateEntity(float dt, Entity entity) = 0;

  const std::set<ECS::Entity>& entities() {
    return _matchingEntities;
  }

protected:
  void setRequiredComponents(ComponentTypeSet&& requiredComponents) {
    _requiredComponents = std::move(requiredComponents);
  }
};
} // namespace ECS
