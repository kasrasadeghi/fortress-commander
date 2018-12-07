#pragma once

#include "../Components.h"
#include "../ECS/System.h"
#include "../Events.h"
#include "../GameState.h"

/**
 * @brief Applies motion to entities.
 * @detail Supports simple velocity and also path planning.
 */
class MoveSystem : public ECS::System {
public:
  MoveSystem(GameState& gameState) : ECS::System(gameState) {
    ECS::ComponentTypeSet requiredComponents;
    requiredComponents.insert(TransformComponent::type);
    requiredComponents.insert(MotionComponent::type);

    setRequiredComponents(std::move(requiredComponents));
  }

  virtual void updateEntity(float dt, ECS::Entity entity) override;
  bool tileVisible(Region& region, glm::ivec2 tileCoord, glm::vec2 from);
  void recomputePath(ECS::Entity entity);
};
