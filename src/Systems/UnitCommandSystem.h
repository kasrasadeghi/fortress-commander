#pragma once

/**
 * @brief Allows the user to send commands to selected commandable entities.
 *
 */
class UnitCommandSystem : public ECS::System, ECS::EventSubscriber<MouseDownEvent> {
  std::vector<Enemy>& _enemies;

  bool _attackClickedEnemy(glm::vec2 clickedPos) {
    ECS::Entity found = ECS::InvalidEntityId;
    for (auto enemy : _enemies) {
      const ECS::Entity enemyId = enemy.id;

      glm::vec2 pos = ECS::Manager::getComponent<TransformComponent>(enemyId).pos;
      float dist = glm::distance(clickedPos, pos);

      if (dist < Unit::unit_size && found == ECS::InvalidEntityId) {
        found = enemyId;
      }
    }

    if (found == ECS::InvalidEntityId) {
      return false;
    }

    for (ECS::Entity entity : entities()) {
      bool selected = ECS::Manager::getComponent<SelectableComponent>(entity).selected;
      if (selected) {
        ECS::Manager::getComponent<AttackComponent>(entity).target = found;
      }
    }
    return true;
  }

  void _invokePositionHandler(float x, float y) {
    for (ECS::Entity entity : entities()) {
      bool selected = ECS::Manager::getComponent<SelectableComponent>(entity).selected;
      if (selected) {
        ECS::Manager::getComponent<CommandableComponent>(entity).positionHandler({x, y});
      }
    }
  }

public:
  UnitCommandSystem(GameState& gameState) : ECS::System(gameState), _enemies(gameState.enemies) {
    ECS::ComponentTypeSet requiredComponents;
    requiredComponents.insert(SelectableComponent::type);
    requiredComponents.insert(CommandableComponent::type);
    setRequiredComponents(std::move(requiredComponents));

    ECS::EventManager::connect<MouseDownEvent>(this);
  }

  void updateEntity(float dt, ECS::Entity entity) override {
    // not yet used
  }

  void receive(const MouseDownEvent& e) override {
    if (_gameState._mode == ControlMode::NONE) {
      if (e.button == GLFW_MOUSE_BUTTON_2) {
        bool foundEnemy = _attackClickedEnemy({e.x, e.y});

        if (not foundEnemy) { // invoke the position handler on the selected units
          _invokePositionHandler(e.x, e.y);
        }
      }
    }
  }
};
