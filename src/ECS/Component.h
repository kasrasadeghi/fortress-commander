#pragma once

#include <cstdint>
#include <memory>
#include <set>
#include <unordered_map>

#include "Entity.h"

namespace ECS {
using ComponentTypeId = std::size_t;
using ComponentTypeSet = std::set<ComponentTypeId>;

static const ComponentTypeId InvalidComponentTypeId = 0;

/**
 * @brief The user should write a specialized component class that inherits
 * from this class, storing its own data and setting its own type
 */
struct Component {
  static const ComponentTypeId type = InvalidComponentTypeId;
};

/**
 * @brief Used to store base pointers to ComponentStores of arbitrary types in backing
 * data structures
 */
struct AbstractComponentStore {
  using Ptr = std::unique_ptr<AbstractComponentStore>;
};

/**
 * @brief Mapping from each Entity that has a component C to
 * its corresponding instance of C
 *
 * @tparam C The type of Component
 */
template <typename C>
class ComponentStore : public AbstractComponentStore {
  static_assert(std::is_base_of<Component, C>::value,
                "C must be a Component or be derived from Component");
  static_assert(C::type != InvalidComponentTypeId, "C must not be of invalid component type");

  using EntityToComponentMap = std::unordered_map<Entity, C>;

  /**
   * @brief Map from Entity to instances of the Component type C
   */
  EntityToComponentMap _store;

  static const ComponentTypeId type = C::type;

public:
  ComponentStore() = default;
  ~ComponentStore() = default;

  /**
   * @brief Add a new Entity-Component pair to the store
   *
   * @param entity The Entity to add to the ComponentStore
   * @param component The instance of C to add to entity
   *
   * @return Success on adding the pair
   */
  bool add(const Entity entity, C&& component) {
    return _store.insert(std::make_pair(entity, std::move(component))).second;
  }

  /**
   * @brief Remove the Entity-Component pair from the store
   *
   * @param entity The Entity to remove from the ComponentStore
   *
   * @return Success on removing the pair
   */
  bool remove(Entity entity) {
    return _store.find(entity) != _store.end();
  }

  /**
   * @brief Checks for the Entity in the ComponentStore
   *
   * @param entity The Entity to check for possession of C
   *
   * @return Presnce of an Entity that contains C
   */
  bool has(Entity entity) const {
    return _store.find(entity) != _store.end();
  }

  /**
   * @brief Get the instance of C that is associated with the Entity
   *
   * @param entity The Entity whose reference to C we are getting
   *
   * @return A reference to the instance of C corresponding to entity
   */
  C& get(Entity entity) {
    return _store.at(entity);
  }

  /**
   * @brief Keep a copy of the instance of C associated with the Entity,
   * then remove the Entity-Component pair from the internal store
   *
   * @param entity The Entity whose instance of C we are dealing with
   *
   * @return A copy of the instance of C
   */
  C extract(Entity entity) {
    C component = std::move(_store.at(entity));
    _store.erase(entity);

    return component;
  }
};
} // namespace ECS
