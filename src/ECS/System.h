#pragma once

#include <algorithm>
#include <cstdint>
#include <functional>
#include <iostream>
#include <memory>
#include <set>

#include "Component.h"
#include "Entity.h"
#include "Event.h"

class GameState;

namespace ECS {
class Manager;

class System {
  using EntitySet = std::set<Entity>;

  /**
   * @brief The set of ComponentTypeIds that are required for this System
   */
  ComponentTypeSet _requiredComponents;

  /**
   * @brief The set of entities that are being managed by this System
   */
  EntitySet _matchingEntities;

protected:
  GameState& _gameState;

  /**
   * @brief Derived Systems should set their required components here
   * @detail To set the required components for a derived System, one
   * should create a ComponentTypeSet in the constructor of the specialized System,
   * add the Component::type for each required component, and call 
   * this function using move semantics.
   *
   * @param requiredComponents The set of required components
   *
   * \code{.cpp}
   * // Example use
   * class TestSystem : public ECS::System {
   *   TestSystem(GameState& gameState) : ECS::System(gameState) {
   *     ECS::ComponentTypeSet requiredComponents;
   *     requiredComponents.insert(TestComponent::type);
   *
   *     setRequiredComponents(std::move(requiredComponents));
   *   }
   * }
   * \endcode
   *
   */
  void setRequiredComponents(ComponentTypeSet&& requiredComponents) {
    _requiredComponents = std::move(requiredComponents);
  }

public:
  using Ptr = std::shared_ptr<System>;

  explicit System(GameState& gameState) : _gameState(gameState) {}
  virtual ~System() = default;

  /**
   * @brief Get the set of required components for this System
   *
   * @return The set of required components
   */
  const ComponentTypeSet& getRequiredComponents() const {
    return _requiredComponents;
  }

  /**
   * @brief Add the provided Entity to this System's EntitySet
   *
   * @param entity The Entity to register
   *
   * @return Success in inserting the Entity
   */
  bool registerEntity(Entity entity) {
    return _matchingEntities.insert(entity).second;
  }

  /**
   * @brief Removes the Entity from this System's EntitySet
   *
   * @param entity The Entity to unregister
   *
   * @return Success in removing the Entity
   */
  std::size_t unregisterEntity(Entity entity) {
    return _matchingEntities.erase(entity);
  }

  /**
   * @brief Tells whether the Entity is in this System's EntitySet
   *
   * @param entity The Entity to check for
   *
   * @return Presence of the Entity in the EntitySet
   */
  bool hasEntity(Entity entity) const {
    return _matchingEntities.find(entity) != _matchingEntities.end();
  }

  /**
   * @brief Updates the system
   * @detail Calls `updateEntity(dt, entity)` on every Entity in this System's EntitySet.
   * This can be overridden in derived Systems to update other parts of the System, but
   * the override should be implemented in a refinement-style approach, rather than replacement.
   *
   * @param dt The amount of time that has passed since the last call (in seconds)
   *
   * @return Returns the amount of entities that were updated
   */
  virtual std::size_t update(float dt) {
    std::size_t updatedEntities = 0;

    for (auto entity = _matchingEntities.begin(); entity != _matchingEntities.end(); ++entity) {
      updateEntity(dt, *entity); // each user class should specialize this
                                 // pure virtual function
      ++updatedEntities;
    }

    return updatedEntities;
  }

  /**
   * @brief Derived Systems should update the System for a specific Entity
   * @detail When creating a System implementation, it is necessary for the user
   * to implement some sort of update logic here, specialized for his or her System.
   *
   * @param dt The amount of time that has passed since the last call (in seconds)
   * @param entity The Entity to update with respect to this System
   */
  virtual void updateEntity(float dt, Entity entity) = 0;

  /**
   * @brief Gets the set of Entities this System is managing
   *
   * @return The set of Entities this System is managing
   */
  const EntitySet& entities() {
    return _matchingEntities;
  }
};
} // namespace ECS
