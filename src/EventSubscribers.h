#pragma once

#include "ECS/Event.h"
#include "Events.h"

struct MouseMoveSubscriber : public ECS::EventSubscriber<MouseMoveEvent> {
  virtual ~MouseMoveSubscriber() = default;

  virtual void receive(ECS::EventManager* eventManager,
                       const MouseMoveEvent& event) override {
    // You can either implement the handler logic here or in a class that
    // inherits from EventSubscriber<MouseMoveEvent>
  }
};
