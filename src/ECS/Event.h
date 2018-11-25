#pragma once

#include "Component.h"
#include "Entity.h"
#include "System.h"

#include <functional>
#include <iostream>
#include <memory>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <unordered_set>

namespace ECS {
class EventManager;
class BaseEventSubscriber;
class BaseEvent;

typedef std::allocator<Entity> Allocator;
using TypeIndex = std::type_index;

using SubscriberPtrAllocator = std::allocator_traits<
    Allocator>::template rebind_alloc<std::shared_ptr<BaseEventSubscriber>>;
using EntityPtrAllocator =
    std::allocator_traits<Allocator>::template rebind_alloc<Entity*>;

using SubscriberPairAllocator =
    std::allocator_traits<Allocator>::template rebind_alloc<std::pair<
        const TypeIndex, std::vector<std::shared_ptr<BaseEventSubscriber>,
                                     SubscriberPtrAllocator>>>;
using EntityAllocator =
    std::allocator_traits<Allocator>::template rebind_alloc<Entity>;
using SubscribersMap = std::unordered_map<
    TypeIndex,
    std::vector<std::shared_ptr<BaseEventSubscriber>, SubscriberPtrAllocator>,
    std::hash<TypeIndex>, std::equal_to<TypeIndex>, SubscriberPairAllocator>;
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

  virtual void receive(EventManager* manager, const Event& event) = 0;
};

/////////////////////////////////////////////////////////////////
class EventManager {
public:
  EventManager(Allocator alloc)
      : mEntAlloc(alloc), mEntities({}, EntityPtrAllocator(alloc)),
        mSubscribers({}, 0, std::hash<TypeIndex>(), std::equal_to<TypeIndex>(),
                     SubscriberPtrAllocator(alloc)) {}

  ~EventManager() {}

  template <typename Event> void connect(EventSubscriber<Event>* subscriber) {
    auto index = getTypeIndex<Event>();
    auto it = mSubscribers.find(index);

    if (it == mSubscribers.end()) {
      std::vector<std::shared_ptr<BaseEventSubscriber>, SubscriberPtrAllocator>
          subList(mEntAlloc);
      subList.push_back(std::shared_ptr<BaseEventSubscriber>(subscriber));

      mSubscribers.insert({index, subList});
    } else {
      it->second.push_back(std::shared_ptr<BaseEventSubscriber>(subscriber));
    }
  }

  template <typename Event>
  void disconnect(EventSubscriber<Event>* subscriber) {
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

  void update() {
    if (mEvents.empty()) { return; }

    for (auto event : mEvents) { event(); }

    mEvents.clear();
  }

  // After the data structure for subscribers is modified, this should take an
  // ID
  // so that we can subscribe to multiple channels for the same event
  template <typename Event> void event(const Event* event) {
    auto it = mSubscribers.find(getTypeIndex<Event>());

    if (it != mSubscribers.end()) {
      const auto& subscribers = it->second;
      for (auto& base : subscribers) {
        auto sub = reinterpret_cast<EventSubscriber<Event>*>(base.get());

        const auto boundFunc =
            std::bind(&EventSubscriber<Event>::receive, sub, this, *event);
        mEvents.push_back(std::function<void()>(boundFunc));
      }
    }
    delete event;
  }

  template <typename Event> void broadcast(Event& event) {}

  template <typename Result, typename Event>
  void eventResult(Result* res, Entity id, const Event& event);

  template <typename Result, typename Event>
  void broadcastResult(Result* res, const Event& event);

  template <typename Event>
  void sendEvent(EventManager* eventManager, const Event& event,
                 BaseEventSubscriber* base) {
    // auto* sub = reinterpret_cast<EventSubscriber<Event>*>(base);

    // sub->receive(eventManager, event);
  }

private:
  std::vector<Entity*, EntityPtrAllocator> mEntities;

  EntityAllocator mEntAlloc;

  SubscribersMap mSubscribers;

  EventQueue mEventQueue;
  std::vector<std::function<void()>> mEvents;
};
} // namespace ECS
