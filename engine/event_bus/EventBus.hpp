//
// Created by marcin on 8/17/19.
//

#ifndef TYPEWRITER_GAEM_EVENTBUS_HPP
#define TYPEWRITER_GAEM_EVENTBUS_HPP

#include <cstddef>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include "Listeners.hpp"
#include "SynchronousListeners.hpp"

// Add event bus for indirect communication between
// scene nodes and such. May be useful to coordinate
// complex translations between hierarchies of nodes
namespace engine {
class EventBus {
    using type_id_t = std::size_t;
    using ListenersPtr = std::unique_ptr<Listeners>;
    using ListenersMap = std::unordered_map<type_id_t, ListenersPtr>;
    const constexpr static auto DEFAULT_CHANNEL = "default_channel";
public:
    static auto &instance() {
      static EventBus instance;
      return instance;
    }

    EventBus(const EventBus &p) = delete;

    EventBus(EventBus &&p) = delete;

    EventBus &operator=(const EventBus &p) = delete;

    EventBus &operator=(EventBus &&p) = delete;

    template<class E, class F>
    void listen(callback_id_t cbid, F &&cb) {
      listen<E, F>(DEFAULT_CHANNEL, cbid, std::forward<F>(cb));
    }

    template<class E, class F>
    void listen(const std::string &channel, callback_id_t cbid, F &&cb) {
      static_assert(!std::is_const<E>::value, "Class must not be const");
      static_assert(!std::is_volatile<E>::value, "Class must not be volatile");
      static_assert(!std::is_reference<E>::value, "Class must not be a reference");
      static_assert(!std::is_pointer<E>::value, "Class must not be a pointer");

      using typed_listeners_t = SynchronousListeners<E>;

      auto tid{gen::type_id<E>()};
      auto &chan{mCallbacks[channel]};
      if (chan.find(tid) == std::end(chan)) {
        auto[it, ok]{chan.insert(std::make_pair(tid, std::make_unique<typed_listeners_t>()))};
        if (ok) {
          mFlattenedListeners.push_back(it->second.get());
        }
      }

      auto &listeners{chan[tid]};
      auto *typed_listeners{static_cast<typed_listeners_t *>(listeners.get())};
      typed_listeners->add(cbid, std::forward<F>(cb));
    }

    template<class E>
    void unlisten(callback_id_t cbid) {
      unlisten<E>(DEFAULT_CHANNEL, cbid);
    }

    template<class E>
    void unlisten(const std::string &channel, callback_id_t cbid) {
      static_assert(!std::is_const<E>::value, "Class must not be const");
      static_assert(!std::is_volatile<E>::value, "Class must not be volatile");
      static_assert(!std::is_reference<E>::value, "Class must not be a reference");
      static_assert(!std::is_pointer<E>::value, "Class must not be a pointer");

      auto tid{gen::type_id<E>()};
      auto &chan{mCallbacks[channel]};
      if (auto it{chan.find(tid)}; it != std::end(chan)) {
        it->second.remove(cbid);
      }
    }

    void unlisten_all(callback_id_t cbid) {
      for (auto &ptr : mFlattenedListeners) {
        ptr->remove(cbid);
      }
    }

    template<class E>
    void notify(E &&event) {
      notify(DEFAULT_CHANNEL, std::forward<E>(event));
    }

    template<class E>
    void notify(const std::string &channel, E &&event) {
      using CE = typename std::remove_const<typename std::remove_reference<E>::type>::type;
      using typed_listeners_t = SynchronousListeners<CE>;

      static_assert(!std::is_volatile<CE>::value, "Class must not be volatile");
      static_assert(!std::is_pointer<CE>::value, "Class must not be a pointer");

      auto tid{gen::type_id<CE>()};
      auto &chan{mCallbacks[channel]};
      if (auto it{chan.find(tid)}; it != std::end(chan)) {
        auto *typed_listeners{static_cast<typed_listeners_t *>(it->second.get())};
        typed_listeners->notify(std::forward<E>(event));
      }
    }

    void deliver() {
      for (auto &ptr : mFlattenedListeners) {
        ptr->deliver();
      }
    }

private:
    EventBus() = default;

private:
    std::unordered_map<std::string, ListenersMap> mCallbacks;
    std::vector<Listeners *> mFlattenedListeners;
};
}

#endif //TYPEWRITER_GAEM_EVENTBUS_HPP
