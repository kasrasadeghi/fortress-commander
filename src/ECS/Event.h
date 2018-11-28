#pragma once

#include "Component.h"
#include "Entity.h"

#include <functional>
#include <iostream>
#include <memory>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace ECS {
class BaseEventSubscriber;

using TypeIndex = std::type_index;

using SubscribersMap = std::unordered_map<
    TypeIndex,
    std::vector<std::shared_ptr<BaseEventSubscriber>>>;

template <typename T> TypeIndex getTypeIndex() {
  return std::type_index(typeid(T));
}

/**
 * @brief The base class for EventSubscribers to inherit from
 * @detail Pointers to BaseEventSubscribers are stored in _subscribers,
 * the internal subscriber storage.
 */
class BaseEventSubscriber {
public:
  virtual ~BaseEventSubscriber() {}
};

/**
 * @brief Templated BaseEventSubscriber
 * @detail Allows the user to simply provide the EventSubscriber class
 * with a templated type of event, and gives the user a `receive` function
 * to handle the event in.
 *
 * @tparam Event The type of event this subscriber should handle
 */
template <typename Event> class EventSubscriber : public BaseEventSubscriber {
public:
  virtual ~EventSubscriber() {}

  virtual void receive(const Event& event) = 0;
};

/**
 * @brief The singleton manager of all events being sent or received
 * @detail Takes events, queues them, and routes them to all connected
 * subscribers on call of update.
 */
class EventManager {
  EventManager() {}

  EventManager(const EventManager&) = delete;
  void operator=(const EventManager&) = delete;

  ~EventManager() {}

  static EventManager& _getInstance() {
      static EventManager instance;

      return instance;
  }

  /**
   * @brief Connects the subsciber to the channel of Events being handled by the EventManager
   *
   * @tparam Event The type of events the subscriber receives
   * @param subscriber The instance of EventSubscriber with the Event template parameter
   */
  template <typename Event> void _connect(EventSubscriber<Event>* subscriber) {
    auto index = getTypeIndex<Event>();
    auto it = _subscribers.find(index);

    if (it == _subscribers.end()) {
      std::vector<std::shared_ptr<BaseEventSubscriber>> subList;
      subList.push_back(std::shared_ptr<BaseEventSubscriber>(subscriber));

      _subscribers.insert({index, subList});
    } else {
      it->second.push_back(std::shared_ptr<BaseEventSubscriber>(subscriber));
    }
  }

  /**
   * @brief Disonnects the subsciber from the channel of Events being handled by the EventManager
   *
   * @tparam Event The type of events the subscriber receives
   * @param subscriber The instance of EventSubscriber with the Event template parameter
   */
  template <typename Event>
  void _disconnect(EventSubscriber<Event>* subscriber) {
    auto index = getTypeIndex<Event>();
    auto it = _subscribers.find(index);

    if (it != _subscribers.end()) {
      auto secondIndex = it->second.begin();
      while (secondIndex != it->second.end() &&
             secondIndex->get() != subscriber) {
        ++secondIndex;
      }
      if (secondIndex != it->second.end()) { it->second.erase(secondIndex); }

      if (it->second.size() == 0) { _subscribers.erase(it); }
    }
  }

  /**
   * @brief Dispatches every queued event stored in _events
   */
  void _update() {
    for (auto event : _events) { event(); }

    _events.clear();
  }

  
  /**
   * @brief Give the EventManager an event to emit to all connected subscribers
   *
   * @tparam Event Only connected subscribers with the template parameter Event
   * will receive the event.
   * @param event The instance of Event that should be sent to each connected subscriber.
   */
  template <typename Event> void _event(const Event* event) {
    auto it = _subscribers.find(getTypeIndex<Event>());

    if (it != _subscribers.end()) {
      const auto& subscribers = it->second;
      for (auto& base : subscribers) {
        auto sub = reinterpret_cast<EventSubscriber<Event>*>(base.get());

        const auto boundFunc =
            std::bind(&EventSubscriber<Event>::receive, sub, *event);
        _events.push_back(std::function<void()>(boundFunc));
      }
    }
    delete event;
  }

public:
  template <typename Event> static void connect(EventSubscriber<Event>* subscriber) {
    _getInstance()._connect<Event>(subscriber);
  }
  template <typename Event>
  static void disconnect(EventSubscriber<Event>* subscriber) {
    _getInstance()._disconnect<Event>(subscriber);
  }
  static void update() {
    _getInstance()._update();
  }
  template <typename Event> static void event(const Event* event) {
    _getInstance()._event<Event>(event);
  }

private:
  SubscribersMap _subscribers;

  std::vector<std::function<void()>> _events;
};
} // namespace ECS
