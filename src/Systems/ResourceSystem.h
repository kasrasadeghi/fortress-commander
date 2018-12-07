#pragma once

#include "../Components.h"
#include "../ECS/System.h"
#include "../Events.h"
#include "../GameState.h"

/**
 * @brief Manages the passive resource accumulation system
 */
class ResourceSystem : public ECS::System {
  ResourceType& _resources;
  float _internalResources;

public:
  ResourceSystem(GameState& gameState, ResourceType& resources)
      : ECS::System(gameState), _resources(resources), _internalResources(_resources) {
    ECS::ComponentTypeSet requiredComponents;
    requiredComponents.insert(ResourceComponent::type);

    setRequiredComponents(std::move(requiredComponents));
  }

  virtual void updateEntity(float dt, ECS::Entity entity) override {
    if (dt < 0) {
      return;
    }

    _internalResources += ECS::Manager::getComponent<ResourceComponent>(entity).speed * dt;

    _resources = static_cast<ResourceType>(_internalResources);
  }
};
