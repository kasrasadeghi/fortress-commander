#pragma once

#include "../ECS/System.h"
#include "../Components.h"
#include "../GameState.h"
#include "../Graphics/Instanced.h"
#include "../Graphics/Light.h"

class LightRenderingSystem : public ECS::System {
  CircleBatch _testCircles;
  LightBatch _lights;

public:
  LightRenderingSystem(GameState& gameState) : ECS::System(gameState) {
    ECS::ComponentTypeSet requiredComponents;
    requiredComponents.insert(TransformComponent::type);
    requiredComponents.insert(LightComponent::type);

    setRequiredComponents(std::move(requiredComponents));
  }

  std::size_t update(float dt) override {
    _lights.instances.clear();
    _testCircles.instances.clear();
    auto result = ECS::System::update(dt);
    _lights.draw(_gameState._view);
    _testCircles.draw(_gameState._view);
    return result;
  }

  void updateEntity(float dt, ECS::Entity entity) override {
    auto& light = ECS::Manager::getComponent<LightComponent>(entity);
    auto& pos = ECS::Manager::getComponent<TransformComponent>(entity).pos;
    _testCircles.add()
      .position(pos)
      .size({0.2, 0.2})
      .color({1, 0, 0, 1});
    _lights.add()
      .position(pos)
      .color(light.color)
      .intensity(light.intensity);
  }
};
