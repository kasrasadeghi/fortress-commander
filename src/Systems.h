#pragma once

#include <cmath>
#include <unordered_set>

#include "Components.h"

#include "ECS/System.h"
#include "Events.h"
#include "GameState.h"
#include "Unit.h"

/**
 * @brief Applies motion to entities.
 * @detail Supports simple velocity and also path planning.
 */
class MoveSystem : public ECS::System {

  /**
   * @brief Update motion to move toward target
   */
  void updatePath(float dt, ECS::Entity entity) {
    TransformComponent& transform = _manager.getComponent<TransformComponent>(entity);
    MotionComponent& motion = _manager.getComponent<MotionComponent>(entity);
    const float speed = glm::length(motion.velocity);

    glm::vec2 target_dx(motion.target.x - transform.pos.x, motion.target.y - transform.pos.y);
    float len = glm::length(target_dx);
    if (len < speed * dt) {
      transform.pos = motion.target;
      motion.velocity = {0, 0};
      motion.hasTarget = false;
    } else if (len > 0) {
      motion.velocity.x = target_dx.x / len * motion.movementSpeed;
      motion.velocity.y = target_dx.y / len * motion.movementSpeed;
    }
  }

public:
  MoveSystem(ECS::Manager& manager, ECS::EventManager& eventManager, GameState& gameState) : ECS::System(manager, eventManager, gameState) {
    ECS::ComponentTypeSet requiredComponents;
    requiredComponents.insert(TransformComponent::_type);
    requiredComponents.insert(MotionComponent::_type);

    setRequiredComponents(std::move(requiredComponents));
  }

  virtual void updateEntity(float dt, ECS::Entity entity) override {
    TransformComponent& transform = _manager.getComponent<TransformComponent>(entity);
    MotionComponent& motion = _manager.getComponent<MotionComponent>(entity);
    
    if (motion.hasTarget) {
      updatePath(dt, entity);
    }
    transform.pos += motion.velocity * dt;
  }
};

/**
 * @brief Allows user to select entities that have a position and are selectable.
 * @detail The user can left-click to select an entity or left-click and drag to select several.
 */
class UnitSelectSystem : public ECS::System, 
                                ECS::EventSubscriber<MouseDownEvent>, 
                                ECS::EventSubscriber<MouseMoveEvent>, 
                                ECS::EventSubscriber<MouseUpEvent> {
  constexpr static float _dragStartThreshold = 0.5;

  bool _mouseDown = false;
  glm::vec2 _mouseDragStart, _mousePos;

  glm::vec2 _boxTopLeft, _boxBottomRight;
  bool _selectionChanged = false;

  void selectClicked(glm::vec2 clickedPos) {
    for (ECS::Entity entity : entities()) {
      _manager.getComponent<SelectableComponent>(entity).selected = false;
    }
    for (ECS::Entity entity : entities()) {
      glm::vec2 pos = _manager.getComponent<TransformComponent>(entity).pos;
      float dist = glm::distance(clickedPos, pos);

      if (dist < Unit::unit_size) {
        _manager.getComponent<SelectableComponent>(entity).selected = true;
        break;
      }
    }
  }

public:
  UnitSelectSystem(ECS::Manager& manager, ECS::EventManager& eventManager, GameState& gameState) : ECS::System(manager, eventManager, gameState){
    ECS::ComponentTypeSet requiredComponents;
    requiredComponents.insert(TransformComponent::_type);
    requiredComponents.insert(SelectableComponent::_type);

    setRequiredComponents(std::move(requiredComponents));
    
    eventManager.connect<MouseDownEvent>(this);
    eventManager.connect<MouseMoveEvent>(this);
    eventManager.connect<MouseUpEvent>(this);
  }

  std::size_t update(float dt) override {
    if (_selectionChanged) {
      // draw box around selection
      auto size_axes = _boxBottomRight - _boxTopLeft;
      InstancedRectangle(_boxTopLeft.x, _boxTopLeft.y)
        .size(size_axes.x, size_axes.y)
        .color(0.8, 0.8, 1, 0.4)
        .draw(_gameState._view);
    }
    return ECS::System::update(dt); 
  }

  void updateEntity(float dt, ECS::Entity entity) override {
    if (!_selectionChanged) return;

    const glm::vec2 pos =
        _manager.getComponent<TransformComponent>(entity).pos;
    bool inBox = pos.x >= _boxTopLeft.x && pos.x <= _boxBottomRight.x &&
                 pos.y >= _boxTopLeft.y && pos.y <= _boxBottomRight.y;

    _manager.getComponent<SelectableComponent>(entity).selected = inBox;
  }

  void receive(ECS::EventManager* mgr, const MouseDownEvent& e) {
    if (_gameState._mode == ControlMode::NONE) {
      if (e.button == GLFW_MOUSE_BUTTON_1) {
        // record first corner of selection
        _mouseDragStart = {e.x, e.y};
        _mouseDown = true;
        selectClicked(_mouseDragStart);
      }
    }
  }

  void receive(ECS::EventManager* mgr, const MouseMoveEvent& e) {
    _mousePos = {e.x, e.y};

    if (_mouseDown) {
      float box_diagonal = glm::distance(_mousePos, _mouseDragStart);

      if (box_diagonal > _dragStartThreshold) {
        const glm::vec2& a = _mouseDragStart, b = _mousePos;
        _boxTopLeft = glm::vec2(std::min(a.x, b.x), std::min(a.y, b.y));
        _boxBottomRight = glm::vec2(std::max(a.x, b.x), std::max(a.y, b.y));
        _selectionChanged = true;
      }
    }
  }

  void receive(ECS::EventManager* mgr, const MouseUpEvent& e) {
    _mouseDown = false;
    _selectionChanged = false;
  }
};

/**
 * @brief Allows the user to send commands to selected commandable entities.
 * 
 */
class UnitCommandSystem : public ECS::System,
                                 ECS::EventSubscriber<MouseDownEvent> {
public:
  UnitCommandSystem(ECS::Manager& manager, ECS::EventManager& eventManager, GameState& gameState) : ECS::System(manager, eventManager, gameState) {
    ECS::ComponentTypeSet requiredComponents;
    requiredComponents.insert(SelectableComponent::_type);
    requiredComponents.insert(CommandableComponent::_type);
    setRequiredComponents(std::move(requiredComponents));

    eventManager.connect<MouseDownEvent>(this);
  }

  void updateEntity(float dt, ECS::Entity entity) override {
    // not yet used 
  }

  void receive(ECS::EventManager* mgr, const MouseDownEvent& e) override {
    if (_gameState._mode == ControlMode::NONE) {
      if (e.button == GLFW_MOUSE_BUTTON_2) {
        // command selected units to move
        for (ECS::Entity entity : entities()) {
          bool selected = _manager.getComponent<SelectableComponent>(entity).selected;
          if (selected)
            _manager.getComponent<CommandableComponent>(entity).positionHandler({e.x, e.y});
        }
      }
    }
  }
};