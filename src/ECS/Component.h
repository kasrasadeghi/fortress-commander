#pragma once

#include <cstdint>
#include <memory>
#include <set>

namespace ECS {
using ComponentType = std::size_t;
using ComponentTypeSet = std::set<ComponentType>;

static const ComponentType InvalidComponentType = 0;

// The user should write a specialized component class that inherits
// from this class, storing its own data and setting its own type
struct Component {
  static const ComponentType _type = InvalidComponentType;
};

// Used to store base pointers to ComponentStores of arbitrary types in data structures
struct AbstractComponentStore {
  using Ptr = std::unique_ptr<AbstractComponentStore>;
};

// Mapping from each entity that has a component C to 
// the instance of C that it contains
template <typename C>
class ComponentStore : public AbstractComponentStore {
  static_assert(std::is_base_of<Component, C>::value, "C must be a Component or be derived from Component");
  static_assert(C::_type != InvalidComponentType, "C must not be of invalid component type");

  std::unordered_map<Entity, C> _store;
  static const ComponentType _type = C::_type;

public:
  ComponentStore() {}
  ~ComponentStore() {}

  // Add a new entity-component pair to the map
  // Returns success on adding the pair
  bool add(const Entity entity, C&& component) {
    return _store.insert(std::make_pair(entity, std::move(component))).second;
  }

  // Remove the entity-component pair from the map
  // Returns success on removing the pair
  bool remove(Entity entity) {
    return _store.find(entity) != _store.end();
  }

  // Returns true if there is an entity that contains C
  bool has(Entity entity) const {
    return _store.find(entity) != _store.end();
  }

  // Get the instance of C that is contained in the entity
  C& get(Entity entity) {
    return _store.at(entity);
  }

  // Keep a copy of the instance of C contained by the entity,
  // then remove the entity-component pair from the internal map
  // Returns the copy of the instance of C
  C extract(Entity entity) {
    C component = std::move(_store.at(entity));
    _store.erase(entity);

    return component;
  }

  const std::unordered_map<Entity, C>& getComponents() {
    return _store;
  }
};
}
