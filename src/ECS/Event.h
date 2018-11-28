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
class EventManager;
class BaseEventSubscriber;
class BaseEvent;

using TypeIndex = std::type_index;

using SubscribersMap = std::unordered_map<
    TypeIndex,
    std::vector<std::shared_ptr<BaseEventSubscriber>>,
    std::hash<TypeIndex>, std::equal_to<TypeIndex>>;
using EventQueue = std::unordered_map<BaseEventSubscriber*, unsigned>;

template <typename T> TypeIndex getTypeIndex() {
  return std::type_index(typeid(T));
}

/////////////////////////////////////////////////////////////////
class BaseEvent {
public:
  typedef std::size_t Family;

  virtual ~BaseEvent() {}

protected:
  static Family mFamilyCounter;
};

template <typename Derived> class Event : public BaseEvent {
public:
  static Family family() {
    static Family family = mFamilyCounter++;
    return family;
  }
};

/////////////////////////////////////////////////////////////////
class BaseEventSubscriber {
public:
  virtual ~BaseEventSubscriber() {}
};

/////////////////////////////////////////////////////////////////
template <typename Event> class EventSubscriber : public BaseEventSubscriber {
public:
  virtual ~EventSubscriber() {}

  virtual void receive(const Event& event) = 0;
};

/////////////////////////////////////////////////////////////////
class EventManager {
  EventManager()
      : mEntities({}),
        mSubscribers({}, 0, std::hash<TypeIndex>(), std::equal_to<TypeIndex>()) {}

  EventManager(const EventManager&) = delete;
  void operator=(const EventManager&) = delete;

  ~EventManager() {}

  static EventManager& _getInstance() {
      static EventManager instance;

      return instance;
  }

  template <typename Event> void _connect(EventSubscriber<Event>* subscriber) {
    auto index = getTypeIndex<Event>();
    auto it = mSubscribers.find(index);

    if (it == mSubscribers.end()) {
      std::vector<std::shared_ptr<BaseEventSubscriber>> subList;
      subList.push_back(std::shared_ptr<BaseEventSubscriber>(subscriber));

      mSubscribers.insert({index, subList});
    } else {
      it->second.push_back(std::shared_ptr<BaseEventSubscriber>(subscriber));
    }
  }

  template <typename Event>
  void _disconnect(EventSubscriber<Event>* subscriber) {
    auto index = getTypeIndex<Event>();
    auto it = mSubscribers.find(index);

    if (it != mSubscribers.end()) {
      auto secondIndex = it->second.begin();
      while (secondIndex != it->second.end() &&
             secondIndex->get() != subscriber) {
        ++secondIndex;
      }
      if (secondIndex != it->second.end()) { it->second.erase(secondIndex); }

      if (it->second.size() == 0) { mSubscribers.erase(it); }
    }
  }

  void _update() {
    if (mEvents.empty()) { return; }

    for (auto event : mEvents) { event(); }

    mEvents.clear();
  }

  // After the data structure for subscribers is modified, this should take an
  // ID
  // so that we can subscribe to multiple channels for the same event
  template <typename Event> void _event(const Event* event) {
    auto it = mSubscribers.find(getTypeIndex<Event>());

    if (it != mSubscribers.end()) {
      const auto& subscribers = it->second;
      for (auto& base : subscribers) {
        auto sub = reinterpret_cast<EventSubscriber<Event>*>(base.get());

        const auto boundFunc =
            std::bind(&EventSubscriber<Event>::receive, sub, *event);
        mEvents.push_back(std::function<void()>(boundFunc));
      }
    }
    delete event;
  }

  // template <typename Event> void broadcast(Event& event) {}

  // template <typename Result, typename Event>
  // void eventResult(Result* res, Entity id, const Event& event);

  // template <typename Result, typename Event>
  // void broadcastResult(Result* res, const Event& event);

  // template <typename Event>
  // void sendEvent(EventManager* eventManager, const Event& event,
  //                BaseEventSubscriber* base) {
  //   auto* sub = reinterpret_cast<EventSubscriber<Event>*>(base);

  //   sub->receive(event);
  // }

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
  std::vector<Entity*> mEntities;

  SubscribersMap mSubscribers;

  EventQueue mEventQueue;
  std::vector<std::function<void()>> mEvents;
};
} // namespace ECS
