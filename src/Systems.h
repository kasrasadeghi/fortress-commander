#pragma once

#include "Components.h"

#include "ECS/System.h"
#include "Events.h"
#include "GameState.h"
#include "Unit.h"

#include "World.h"

#include <cmath>
#include <unordered_set>
#include <deque>

/**
 * @brief Applies motion to entities.
 * @detail Supports simple velocity and also path planning.
 */
class MoveSystem : public ECS::System {

public:
  static Path findPath(Region& region, glm::ivec2 start, glm::ivec2 end);
  MoveSystem(GameState& gameState) : ECS::System(gameState) {
    ECS::ComponentTypeSet requiredComponents;
    requiredComponents.insert(TransformComponent::type);
    requiredComponents.insert(MotionComponent::type);

    setRequiredComponents(std::move(requiredComponents));
  }

  virtual void updateEntity(float dt, ECS::Entity entity) override;
};

/**
 * @brief Allows user to select entities that have a position and are selectable.
 * @detail The user can left-click to select an entity or left-click and drag to select several.
 */
class UnitSelectSystem : public ECS::System,
                         ECS::EventSubscriber<MouseDownEvent>,
                         ECS::EventSubscriber<MouseMoveEvent>,
                         ECS::EventSubscriber<MouseUpEvent> {
  constexpr static float _dragStartThreshold = 0.5 * tile_size;

  bool _mouseDown = false;
  glm::vec2 _mouseDragStart, _mousePos;

  glm::vec2 _boxTopLeft, _boxBottomRight;
  bool _selectionChanged = false;

  void _selectClicked(glm::vec2 clickedPos) {
    ECS::Entity found = ECS::InvalidEntityId;
    for (ECS::Entity entity : entities()) {
      glm::vec2 pos = ECS::Manager::getComponent<TransformComponent>(entity).pos;
      float dist = glm::distance(clickedPos, pos);

      ECS::Manager::getComponent<SelectableComponent>(entity).selected = false;

      if (dist < Unit::unit_size && found == ECS::InvalidEntityId) {
        found = entity;
      }
    }

    if (found != ECS::InvalidEntityId) {
      ECS::Manager::getComponent<SelectableComponent>(found).selected = true;
    }
  }

public:
  UnitSelectSystem(GameState& gameState) : ECS::System(gameState) {
    ECS::ComponentTypeSet requiredComponents;
    requiredComponents.insert(TransformComponent::type);
    requiredComponents.insert(SelectableComponent::type);

    setRequiredComponents(std::move(requiredComponents));

    ECS::EventManager::connect<MouseDownEvent>(this);
    ECS::EventManager::connect<MouseMoveEvent>(this);
    ECS::EventManager::connect<MouseUpEvent>(this);
  }

  std::size_t update(float dt) override {
    if (_selectionChanged) {
      // draw box around selection
      auto size_axes = _boxBottomRight - _boxTopLeft;
      RectangleBatch().add()
          .position((_boxTopLeft + _boxBottomRight) / 2.f)
          .size(size_axes)
          .color({0.8, 0.8, 1, 0.4})
          .draw(_gameState._view);
    }
    return ECS::System::update(dt);
  }

  void updateEntity(float dt, ECS::Entity entity) override {
    if (!_selectionChanged) return;

    const glm::vec2 pos = ECS::Manager::getComponent<TransformComponent>(entity).pos;
    bool inBox = pos.x >= _boxTopLeft.x && pos.x <= _boxBottomRight.x && pos.y >= _boxTopLeft.y &&
                 pos.y <= _boxBottomRight.y;

    ECS::Manager::getComponent<SelectableComponent>(entity).selected = inBox;
  }

  void receive(const MouseDownEvent& e) {
    if (_gameState._mode == ControlMode::NONE) {
      if (e.button == GLFW_MOUSE_BUTTON_1) {
        // record first corner of selection
        _mouseDragStart = {e.x, e.y};
        _mouseDown = true;
        _selectClicked(_mouseDragStart);
      }
    }
  }

  void receive(const MouseMoveEvent& e) {
    _mousePos = {e.x, e.y};

    if (_mouseDown) {
      float box_diagonal = glm::distance(_mousePos, _mouseDragStart);

      if (box_diagonal > _dragStartThreshold) {
        const glm::vec2 &a = _mouseDragStart, b = _mousePos;
        _boxTopLeft = glm::vec2(std::min(a.x, b.x), std::min(a.y, b.y));
        _boxBottomRight = glm::vec2(std::max(a.x, b.x), std::max(a.y, b.y));
        _selectionChanged = true;
      }
    }
  }

  void receive(const MouseUpEvent& e) {
    _mouseDown = false;
    _selectionChanged = false;
  }
};

/**
 * @brief Allows the user to send commands to selected commandable entities.
 *
 */
class UnitCommandSystem : public ECS::System, ECS::EventSubscriber<MouseDownEvent> {
  std::vector<Enemy>& _enemies;
  
  bool _attackClickedEnemy(glm::vec2 clickedPos) {
    ECS::Entity found = ECS::InvalidEntityId;
    for (auto enemy : _enemies) {
      const ECS::Entity enemyId = enemy._id;

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

/**
 * @brief Facilitates battle between Units and Enemies
 *
 */
class BattleSystem : public ECS::System { 
  void _performAttack(const ECS::Entity entity, const ECS::Entity target) {
    if (entity == ECS::InvalidEntityId || target == ECS::InvalidEntityId) {
      return;
    }

    auto& targetHealth = ECS::Manager::getComponent<HealthComponent>(target).health;
    auto entityStrength = ECS::Manager::getComponent<AttackComponent>(entity).strength;

    // Augment the target's health according to the attack strength
    targetHealth -= entityStrength;

    // Reset the attack timer so we will attack again after attackCooldown
    ECS::Manager::getComponent<AttackComponent>(entity).attackTimer = 0.f;
  }

  void _die(const ECS::Entity entity) {
    std::cout << "Entity " << entity << " has died." << std::endl;
    
    // TODO: Implement death
  }

public:
  BattleSystem(GameState& gameState) : ECS::System(gameState) {
    ECS::ComponentTypeSet requiredComponents;
    requiredComponents.insert(TransformComponent::type);
    requiredComponents.insert(HealthComponent::type);
    requiredComponents.insert(AttackComponent::type);

    setRequiredComponents(std::move(requiredComponents));
  }

  void updateEntity(float dt, ECS::Entity entity) override {
    if (ECS::Manager::getComponent<HealthComponent>(entity).health <= 0) {
      _die(entity);
    }

   auto target = ECS::Manager::getComponent<AttackComponent>(entity).target;

    if (target == ECS::InvalidEntityId) {
      return;
    }
 
    auto pos = ECS::Manager::getComponent<TransformComponent>(entity).pos;
    auto targetPos = ECS::Manager::getComponent<TransformComponent>(target).pos;

    auto dist = glm::distance(pos, targetPos);

    auto& attackTimer = ECS::Manager::getComponent<AttackComponent>(entity).attackTimer;
    auto& entityBattling = ECS::Manager::getComponent<AttackComponent>(entity).battling;

    entityBattling = dist < 5.f; // TODO: don't hardcode the RoI, perform raycast?
    
    if (entityBattling) {
      attackTimer += dt;

      if (attackTimer > ECS::Manager::getComponent<AttackComponent>(entity).attackCooldown) {
        _performAttack(entity, target);
      }
    } else { // Reset the timer so we attack immediately on engaging
      attackTimer = ECS::Manager::getComponent<AttackComponent>(entity).attackCooldown; 
    }
  }
};

class UnitCollisionSystem : public ECS::System {
public:
  UnitCollisionSystem(GameState& gameState) : ECS::System(gameState) {
    ECS::ComponentTypeSet requiredComponents;
    requiredComponents.insert(TransformComponent::type);

    setRequiredComponents(std::move(requiredComponents));
  }

  void updateEntity(float dt, ECS::Entity entity) override {
    const glm::vec2& pos = ECS::Manager::getComponent<TransformComponent>(entity).pos;
    for (ECS::Entity other : entities()) {
      if (not ECS::Manager::getComponent<TransformComponent>(entity).movable ||
	  not ECS::Manager::getComponent<TransformComponent>(other).movable) {
        continue;
      }

      const glm::vec2& otherPos = ECS::Manager::getComponent<TransformComponent>(other).pos;
      const float dist = glm::distance(pos, otherPos);
      if (dist == 0)
        continue;
      if (dist < Unit::unit_size * 2) {
        float overlapDistance = Unit::unit_size * 2 - dist;
        glm::vec2 halfOverlap = glm::normalize(otherPos - pos) * overlapDistance / 2.f;
        
        ECS::Manager::getComponent<TransformComponent>(entity).translate(-halfOverlap);
        ECS::Manager::getComponent<TransformComponent>(other).translate(halfOverlap);
      }
    }
  }
};
