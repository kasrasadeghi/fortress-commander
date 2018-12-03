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
  using EntityToComponentTypeSetMap = std::unordered_map<Entity, ComponentTypeSet>;
  using ComponentTypeToStoreMap = std::unordered_map<ComponentTypeId, AbstractComponentStore::Ptr>;
  using SystemContainer = std::vector<System::Ptr>;

  /**
   * @brief The highest ID Entity that has been created. Used for ID creation and allocation
   */
  Entity _lastEntity;

  /**
   * @brief Indicates whether the last entity has reached MaxEntityId yet
   */
  bool _reachedCapacity;

  /**
   * @brief Map from Entity to the set of ComponentTypes that are associated with it
   */
  EntityToComponentTypeSetMap _entities;

  /**
   * @brief Map from ComponentTypeId to its corresponding ComponentStore
   */
  ComponentTypeToStoreMap _componentStores;

  /**
   * @brief Container of Systems that we are managed
   */
  SystemContainer _systems;

  Manager();
  ~Manager();

  Manager(const Manager&) = delete;
  void operator=(const Manager&) = delete;

  static Manager& _getInstance() {
    static Manager instance;

    return instance;
  }

  /**
   * @brief Creates a new ComponentStore
   *
   * @tparam C The type of component we want to store
   *
   * @return Success of creation and insertion
   */
  template <typename C>
  bool _createComponentStore() {
    static_assert(std::is_base_of<Component, C>::value, "C must be a descendant of Component");
    static_assert(C::type != InvalidComponentTypeId, "C's types must not be invalid");

    return _componentStores
        .insert(std::make_pair(C::type, AbstractComponentStore::Ptr(new ComponentStore<C>())))
        .second;
  }

  /**
   * @brief Gets the ComponentStore corresponding to C
   *
   * @tparam C The type of component store we're looking for
   *
   * @return The ComponentStore of type C
   */
  template <typename C>
  ComponentStore<C>& _getComponentStore() {
    // Find the appropriate component store for component type C
    auto componentStoreIt = _componentStores.find(C::type);
    if (componentStoreIt == _componentStores.end()) {
      throw std::runtime_error("The ComponentStore does not exist");
    }

    // We know the type here, so we can cast the AbstractComponentStore into the templatized version
    return reinterpret_cast<ComponentStore<C>&>(*componentStoreIt->second);
  }

  /**
   * @brief Gets the component of type C held by entity
   *
   * @tparam C The type of component
   * @param entity The entity we are looking for the component on
   *
   * @return A reference to the instance of C corresponding to entity
   */
  template <typename C>
  C& _getComponent(ECS::Entity entity) {
    return getComponentStore<C>().get(entity);
  }

  /**
   * @brief Adds the given system to the SystemContainer
   *
   * @param systemPtr A pointer to the system at hand
   */
  void _addSystem(const System::Ptr& systemPtr);

  /**
   * @brief Creates an Entity
   *
   * @detail Linearly increments IDs for new entities until capacity is reached,
   *         then searches for empty slots.
   */
  Entity _createEntity() {
    if (_lastEntity == MaxEntityId) {
      _reachedCapacity = true;
    }

    if (_reachedCapacity) {
      _lastEntity = 1;
      while (_lastEntity != MaxEntityId && _entities.find(_lastEntity) != _entities.end()) {
        ++_lastEntity;
      }

      if (_lastEntity == MaxEntityId) {
        throw std::runtime_error("Reached the capacity of Entities");
      }
    }

    _entities.insert(std::make_pair(++_lastEntity, ComponentTypeSet()));
    return _lastEntity;
  }

  /**
   * @brief Deletes the given entity id and unregisters it from all relevant systems
   *
   * @param id The entity to delete
   */
  bool _deleteEntity(Entity id) {
    _unregisterEntity(id);
    return _entities.erase(id);
  }

  /**
   * @brief Associates the instance of C with the provided Entity
   *
   * @tparam C The type of component
   * @param entity The entity we are adding the component to
   * @param component The instance of C that should be associated with entity
   *
   * @return Success of creation and insertion into the ComponentStore
   */
  template <typename C>
  bool _addComponent(const Entity entity, C&& component) {
    static_assert(std::is_base_of<Component, C>::value, "C must be a descendant of Component");
    static_assert(C::type != InvalidComponentTypeId, "C's types must not be invalid");

    // Find the appropriate Entity -> ComponentTypeSet entry
    auto entityIt = _entities.find(entity);
    if (entityIt == _entities.end()) {
      throw std::runtime_error("The entity does not exist");
    }

    // Insert the new component's type into the entity's corresponding ComponentTypeSet
    entityIt->second.insert(C::type);
    return _getComponentStore<C>().add(entity, std::move(component));
  }

  /**
   * @brief Register the provided Entity with the Systems for which it fulfills the required
   * components
   * @detail Checks each System contained in the SystemContainer to see if entity contains the
   * required components. If it does, we register entity with the corresponding system.
   *
   * @param entity The Entity to register
   *
   * @return The number of systems entity was added to
   */
  std::size_t _registerEntity(const Entity entity);

  /**
   * @brief Unregister the provided Entity from each System it was previously registered with
   *
   * @param entity The Entity to unregister
   *
   * @return The number of systems from which entity was removed
   */
  std::size_t _unregisterEntity(const Entity entity);

  /**
   * @brief Update each System contained in the SystemContainer
   *
   * @param dt The amount of time since the last call to `update`
   *
   * @return The number of updated systems
   */
  std::size_t _update(float dt);

  /**
   * @brief Clears every Entity from the Manager
   *
   * @return Success in clearing the underlying data structure
   */
  bool clear();

public:
  template <typename C>
  static bool createComponentStore() {
    return _getInstance()._createComponentStore<C>();
  }
  template <typename C>
  static ComponentStore<C>& getComponentStore() {
    return _getInstance()._getComponentStore<C>();
  }
  template <typename C>
  static C& getComponent(ECS::Entity entity) {
    return _getInstance()._getComponent<C>(entity);
  }
  template <typename C>
  static bool addComponent(const Entity entity, C&& component) {
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
  static bool deleteEntity(Entity id) {
    return _getInstance()._deleteEntity(id);
  }
  template <typename C>
  static bool hasComponent(Entity id) {
    return _getInstance()._getComponentStore<C>().has(id);
  }
};
} // namespace ECS
