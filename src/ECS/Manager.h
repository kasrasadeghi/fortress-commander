#pragma once

#include <cstdint>
#include <map>
#include <unordered_map>
#include <vector>

#include "Component.h"
#include "Entity.h"
#include "System.h"

namespace ECS {
class Manager {
  Entity _lastEntity;
  std::unordered_map<Entity, ComponentTypeSet> _entities;
  std::map<ComponentType, AbstractComponentStore::Ptr> _componentStores;
  std::vector<System::Ptr> _systems;

public:
  Manager();
  ~Manager();

  template <typename C> bool createComponentStore() {
    static_assert(std::is_base_of<Component, C>::value,
                  "C must be a descendant of Component");
    static_assert(C::_type != InvalidComponentType,
                  "C's types must not be invalid");

    return _componentStores
        .insert(std::make_pair(
            C::_type, AbstractComponentStore::Ptr(new ComponentStore<C>())))
        .second;
  }

  template <typename C> ComponentStore<C>& getComponentStore() {
    auto componentStoreIt = _componentStores.find(C::_type);
    if (componentStoreIt == _componentStores.end()) {
      throw std::runtime_error("The ComponentStore does not exist");
    }

    return reinterpret_cast<ComponentStore<C>&>(*componentStoreIt->second);
  }

  template <typename C> C& getComponent(ECS::Entity entity) {
    return getComponentStore<C>().get(entity);
  }

  void addSystem(const System::Ptr& systemPtr);

  Entity createEntity() {
    _entities.insert(std::make_pair(_lastEntity + 1, ComponentTypeSet()));
    return ++_lastEntity;
  }

  template <typename C> bool addComponent(const Entity entity, C&& component) {
    static_assert(std::is_base_of<Component, C>::value,
                  "C must be a descendant of Component");
    static_assert(C::_type != InvalidComponentType,
                  "C's types must not be invalid");

    auto entityIt = _entities.find(entity);
    if (entityIt == _entities.end()) {
      throw std::runtime_error("The entity does not exist");
    }

    entityIt->second.insert(C::_type);
    return getComponentStore<C>().add(entity, std::move(component));
  }

  std::size_t registerEntity(const Entity entity);
  std::size_t unregisterEntity(const Entity entity);
  std::size_t update(float dt);
};
} // namespace ECS
