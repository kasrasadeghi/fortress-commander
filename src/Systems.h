#pragma once

#include <cmath>
#include <unordered_set>

#include "Components.h"

#include "ECS/System.h"
#include "Events.h"

class MoveSystem : public ECS::System {

public:
  MoveSystem(ECS::Manager& manager, ECS::EventManager& eventManager) : ECS::System(manager, eventManager) {
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

class UnitSelectSystem : public ECS::System, 
                                ECS::EventSubscriber<MouseDownEvent>, 
                                ECS::EventSubscriber<MouseMoveEvent>, 
                                ECS::EventSubscriber<MouseUpEvent> {
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
  UnitSelectSystem(ECS::Manager& manager, ECS::EventManager& eventManager) : ECS::System(manager, eventManager){
    ECS::ComponentTypeSet requiredComponents;
    requiredComponents.insert(TransformComponent::_type);
    requiredComponents.insert(SelectableComponent::_type);

    setRequiredComponents(std::move(requiredComponents));
    
    eventManager.connect<MouseDownEvent>(this);
    eventManager.connect<MouseMoveEvent>(this);
    eventManager.connect<MouseUpEvent>(this);
  }

  void updateEntity(float dt, ECS::Entity entity) override {
    if (!_selectionChanged) return;

    const sf::Vector2f pos =
        _manager.getComponent<TransformComponent>(entity).pos;
    bool inBox = pos.x >= _boxTopLeft.x && pos.x <= _boxBottomRight.x &&
                 pos.y >= _boxTopLeft.y && pos.y <= _boxBottomRight.y;

    _manager.getComponent<SelectableComponent>(entity).selected = inBox;
  }

  void receive(ECS::EventManager* mgr, const MouseDownEvent& e) {
    _mouseDragStart = sf::Vector2f(e.x, e.y);
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

  void receive(ECS::EventManager* mgr, const MouseMoveEvent& e) {
    _mousePos = sf::Vector2f(e.x, e.y);
    if (_mouseDown) {
      selectBox(_mouseDragStart, _mousePos);
      _selectionChanged = true;
    }
  }

  void receive(ECS::EventManager* mgr, const MouseUpEvent& e) {
    _mouseDown = false;
    _selectionChanged = false;
  }
};
