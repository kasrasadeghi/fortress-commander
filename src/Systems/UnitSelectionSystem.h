#pragma once

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

  std::size_t _selectionCount;
  glm::vec2 _selectionCentroid;

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
      _selectionCount = 1;
      _selectionCentroid = ECS::Manager::getComponent<TransformComponent>(found).pos;
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
      RectangleBatch()
          .add()
          .position((_boxTopLeft + _boxBottomRight) / 2.f)
          .size(size_axes)
          .color({0.8, 0.8, 1, 0.4})
          .draw(_gameState._view);
    }

    _selectionCount = 0;
    _selectionCentroid = {0, 0};
    auto result = ECS::System::update(dt);
    
    if (_selectionCount > 0) {
      _selectionCentroid /= static_cast<float>(_selectionCount);
    
      for (ECS::Entity e : entities()) {
        auto& selectable = ECS::Manager::getComponent<SelectableComponent>(e);
        if (selectable.selected) {
          selectable.selectionCentroid = _selectionCentroid;
        }
      }

      RectangleBatch()
          .add()
          .position(_selectionCentroid)
          .size({0.1, 0.1})
          .color({0.5, 1.0, 0.0, 0.5})
          .draw(_gameState._view);
    }

    return result;
  }

  void updateEntity(float dt, ECS::Entity entity) override {
    auto& selected = ECS::Manager::getComponent<SelectableComponent>(entity).selected;

    if (_selectionChanged) {
      const glm::vec2 pos = ECS::Manager::getComponent<TransformComponent>(entity).pos;
      bool inBox = pos.x >= _boxTopLeft.x && pos.x <= _boxBottomRight.x && pos.y >= _boxTopLeft.y &&
                   pos.y <= _boxBottomRight.y;

      selected = inBox;
    }
    
    if (selected) {
      _selectionCount += 1;
      _selectionCentroid += ECS::Manager::getComponent<TransformComponent>(entity).pos;
    }
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

