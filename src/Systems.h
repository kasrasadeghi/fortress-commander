#pragma once

#include <unordered_set>
#include <cmath>

#include "Components.h"

#include "ECS/System.h"

class MoveSystem : public ECS::System {

public:
  MoveSystem(ECS::Manager& manager) : ECS::System(manager) {
    ECS::ComponentTypeSet requiredComponents;
    requiredComponents.insert(TransformComponent::_type);
    requiredComponents.insert(MotionComponent::_type);

    setRequiredComponents(std::move(requiredComponents));
  }

  virtual void updateEntity(float dt, ECS::Entity entity) override {
    sf::Vector2f& pos = _manager.getComponent<TransformComponent>(entity).pos;
    sf::Vector2f& vel = _manager.getComponent<MotionComponent>(entity).velocity;
    pos += vel * dt;
  }
};

class UnitSelectSystem : public ECS::System {
  constexpr static float _dragStartThreshold = 0.2;
  
  bool _mouseDown = false;
  sf::Vector2f _mouseDragStart, _mousePos;

  sf::Vector2f _boxTopLeft, _boxBottomRight;
  bool _selectionChanged = false;

  void selectBox(sf::Vector2f a, sf::Vector2f b) {
    _boxTopLeft = sf::Vector2f(std::min(a.x, b.x), std::min(a.y, b.y));
    _boxBottomRight = sf::Vector2f(std::max(a.x, b.x), std::max(a.y, b.y));
  }

public:
  UnitSelectSystem(ECS::Manager& manager) : ECS::System(manager) {
    ECS::ComponentTypeSet requiredComponents;
    requiredComponents.insert(TransformComponent::_type);
    requiredComponents.insert(SelectableComponent::_type);

    setRequiredComponents(std::move(requiredComponents));
  }

  void updateEntity(float dt, ECS::Entity entity) override {
    if (!_selectionChanged)
      return;
    std::cout << "CHAINGING SAOFJAOSFJAOSF" << std::endl;
    std::cout << " (" << _mousePos.x << ", " << _mousePos.y << ")" << std::endl;
    
    const sf::Vector2f pos = _manager.getComponent<TransformComponent>(entity).pos;
    bool inBox = pos.x >= _boxTopLeft.x && pos.x <= _boxBottomRight.x &&
                 pos.y >= _boxTopLeft.y && pos.y <= _boxBottomRight.y;
    
    _manager.getComponent<SelectableComponent>(entity).selected = inBox;
  }

  void handleMouseDown(float x, float y) {
    _mouseDragStart = sf::Vector2f(x, y);
    _mouseDown = true;

    /*
    forEachEntity([this, x, y](ECS::Entity entity) {
      sf::Vector2f pos = _manager.getComponent<TransformComponent>(entity).pos;

      float dx = x - pos.x;
      float dy = y - pos.y;

      float dist = sqrt(dx*dx + dy*dy);

      bool clicked = dist < Unit::unit_size;

      _manager.getComponent<SelectableComponent>(entity).selected = clicked;
    });
    */
  }

  void handleMouseMove(float x, float y) {
    _mousePos = sf::Vector2f(x, y);
    if (_mouseDown) {
      selectBox(_mouseDragStart, _mousePos);      
      _selectionChanged = true;
    }
  }

  void handleMouseUp() {
    _mouseDown = false;
    _selectionChanged = false;
  }
};
