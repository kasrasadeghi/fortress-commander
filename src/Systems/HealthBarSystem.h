#pragma once

#include "../ECS/System.h"
#include "../GameState.h"
#include "../Components.h"

class HealthBarSystem : public ECS::System {
public:
  HealthBarSystem(GameState& gameState) : ECS::System(gameState) {
    ECS::ComponentTypeSet requiredComponents;
    requiredComponents.insert(TransformComponent::type);
    requiredComponents.insert(HealthComponent::type);

    setRequiredComponents(std::move(requiredComponents));
  }

  virtual void updateEntity(float dt, ECS::Entity entity) override {
    const glm::vec2 offset(0, 0.8);
    const glm::vec2 maxSize(1.0, 0.1);
    constexpr float borderSize = 0.1;
    auto& pos = ECS::Manager::getComponent<TransformComponent>(entity).pos;
    auto& healthComponent = ECS::Manager::getComponent<HealthComponent>(entity);
    
    const float f = static_cast<float>(healthComponent.health) / healthComponent.maxHealth;
    glm::vec2 size(maxSize.x * f, maxSize.y);

    if (f < 1.f) {
      RectangleBatch healthBar;
      healthBar.add().color({.1, .1, .1, 0.5f}).position(pos + offset).size(maxSize + glm::vec2(borderSize, borderSize));
      healthBar.add().color({1 - f, f, 0, 0.75f}).position(pos + offset - glm::vec2((1 - size.x) / 2.f, 0)).size(size);
      healthBar.draw(_gameState._view);
    }
  }
};