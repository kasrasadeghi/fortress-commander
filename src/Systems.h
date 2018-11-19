#pragma once

#include "Components.h"

#include "ECS/System.h"

class MoveSystem : public ECS::System {
  constexpr static float _moveAmount = 1.f;

public:
  MoveSystem(ECS::Manager& manager) : ECS::System(manager) {
    ECS::ComponentTypeSet requiredComponents;
    requiredComponents.insert(TransformComponent::_type);

    setRequiredComponents(std::move(requiredComponents));
  }

  virtual void updateEntity(float dt, ECS::Entity entity) override {
    _manager.getComponentStore<TransformComponent>().get(entity).pos += sf::Vector2f(_moveAmount, _moveAmount);
    std::cout << "update " << dt << "\t" << entity << std::endl;
  }
};
