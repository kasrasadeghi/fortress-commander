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

  Manager();
  Manager(const Manager&) = delete;
  void operator=(const Manager&) = delete;
  ~Manager();

  static Manager& _getInstance() {
    static Manager instance;

    return instance;
  }

  template <typename C> bool _createComponentStore() {
    static_assert(std::is_base_of<Component, C>::value,
                  "C must be a descendant of Component");
    static_assert(C::type != InvalidComponentType,
                  "C's types must not be invalid");

    return _componentStores
        .insert(std::make_pair(
            C::type, AbstractComponentStore::Ptr(new ComponentStore<C>())))
        .second;
  }

  template <typename C> ComponentStore<C>& _getComponentStore() {
    auto componentStoreIt = _componentStores.find(C::type);
    if (componentStoreIt == _componentStores.end()) {
      throw std::runtime_error("The ComponentStore does not exist");
    }

    return reinterpret_cast<ComponentStore<C>&>(*componentStoreIt->second);
  }

  template <typename C> C& _getComponent(ECS::Entity entity) {
    return getComponentStore<C>().get(entity);
  }

  void _addSystem(const System::Ptr& systemPtr);

  Entity _createEntity() {
    _entities.insert(std::make_pair(_lastEntity + 1, ComponentTypeSet()));
    return ++_lastEntity;
  }

  template <typename C> bool _addComponent(const Entity entity, C&& component) {
    static_assert(std::is_base_of<Component, C>::value,
                  "C must be a descendant of Component");
    static_assert(C::type != InvalidComponentType,
                  "C's types must not be invalid");

    auto entityIt = _entities.find(entity);
    if (entityIt == _entities.end()) {
      throw std::runtime_error("The entity does not exist");
    }

    entityIt->second.insert(C::type);
    return getComponentStore<C>().add(entity, std::move(component));
  }

  std::size_t _registerEntity(const Entity entity);
  std::size_t _unregisterEntity(const Entity entity);
  std::size_t _update(float dt);

public:
  template <typename C> static bool createComponentStore() {
    return _getInstance()._createComponentStore<C>();
  }
  template <typename C> static ComponentStore<C>& getComponentStore() {
    return _getInstance()._getComponentStore<C>();
  }
  template <typename C> static C& getComponent(ECS::Entity entity) {
    return _getInstance()._getComponent<C>(entity);
  }
  template <typename C> static bool addComponent(const Entity entity, C&& component) {
    return _getInstance()._addComponent<C>(entity, std::forward<C>(component));
  }
  static void addSystem(const System::Ptr& systemPtr) {
    return _getInstance()._addSystem(systemPtr);
  }
  static Entity createEntity() {
    return _getInstance()._createEntity();
  }
  static std::size_t registerEntity(const Entity entity) {
    return _getInstance()._registerEntity(entity);
  }
  static std::size_t unregisterEntity(const Entity entity) {
    return _getInstance()._unregisterEntity(entity);
  }
  static std::size_t update(float dt) {
    return _getInstance()._update(dt);
  }
};
} // namespace ECS
