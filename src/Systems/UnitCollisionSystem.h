#pragma once

class UnitCollisionSystem : public ECS::System {
  std::unordered_map<ECS::Entity, bool> _movable;

public:
  UnitCollisionSystem(GameState& gameState) : ECS::System(gameState) {
    ECS::ComponentTypeSet requiredComponents;
    requiredComponents.insert(TransformComponent::type);

    setRequiredComponents(std::move(requiredComponents));
  }

  bool registerEntity(ECS::Entity entity) {
    _movable.insert({entity, ECS::Manager::getComponentStore<MotionComponent>().has(entity)});

    return ECS::System::registerEntity(entity);
  }

  void updateEntity(float dt, ECS::Entity entity) override {
    const glm::vec2& pos = ECS::Manager::getComponent<TransformComponent>(entity).pos;
    for (ECS::Entity other : entities()) {
      bool eMovable = _movable.at(entity);
      bool oMovable = _movable.at(other);
      if (not eMovable && not oMovable) {
        continue;
      }

      // make entities that are just standing around easy to push
      if (ECS::Manager::hasComponent<MotionComponent>(entity) &&
          ECS::Manager::getComponent<MotionComponent>(entity).hasTarget) {
        if (ECS::Manager::hasComponent<MotionComponent>(other) &&
            not ECS::Manager::getComponent<MotionComponent>(other).hasTarget) {
          eMovable = false;
        }
      }

      const glm::vec2& otherPos = ECS::Manager::getComponent<TransformComponent>(other).pos;
      const float dist = glm::distance(pos, otherPos);
      if (dist == 0) {
        continue;
      } else if (dist < Unit::unit_size * 2) {
        float overlapDistance = Unit::unit_size * 2 - dist;
        glm::vec2 displacement = glm::normalize(otherPos - pos) * overlapDistance;

        if (eMovable && oMovable) {
          displacement *= 0.5f;
        }
        if (eMovable) {
          ECS::Manager::getComponent<TransformComponent>(entity).translate(-displacement);
        }
        if (oMovable) {
          ECS::Manager::getComponent<TransformComponent>(other).translate(displacement);
        }
      }
    }
  }
};
