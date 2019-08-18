//
// Created by marcin on 8/17/19.
//

#ifndef TYPEWRITER_GAEM_EVENT_BUS_HPP
#define TYPEWRITER_GAEM_EVENT_BUS_HPP

#include <cstddef>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include "callback_container.hpp"
#include "synchronous_callback_container.hpp"

// Add event bus for indirect communication between
// scene nodes and such. May be useful to coordinate
// complex animations between hierarchies of nodes
namespace engine {
class event_bus {
    using type_id_t = std::size_t;
    using callback_container_ptr = std::unique_ptr<callback_container>;
    const constexpr static auto DEFAULT_CHANNEL = "default_channel";
public:
    static event_bus &get_instance() {
      static event_bus instance;
      return instance;
    }

    event_bus(const event_bus &p) = delete;

    event_bus(event_bus &&p) = delete;

    event_bus &operator=(const event_bus &p) = delete;

    event_bus &operator=(event_bus &&p) = delete;

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

      using typed_container_t = synchronous_callback_container<E>;

      auto tid{gen::type_id<E>()};
      auto &chan{callbacks[channel]};
      if (chan.find(tid) == std::end(chan)) {
        chan.insert(std::make_pair(tid, std::make_unique<typed_container_t>()));
      }

      auto &container{chan[tid]};
      auto *typed_container{static_cast<typed_container_t *>(container.get())};
      typed_container->add(cbid, std::forward<F>(cb));
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
      auto &chan{callbacks[channel]};
      if (auto it{chan.find(tid)}; it != std::end(chan)) {
        it->remove(cbid);
      }
    }

    void unlisten_all(callback_id_t cbid) {
      for (auto&[chan, cbs] : callbacks) {
        for (auto&[id, vect] : cbs) {
          vect->remove(cbid);
        }
      }
    }

    template<class E>
    void notify(E &&event) {
      notify(DEFAULT_CHANNEL, std::forward<E>(event));
    }

    template<class E>
    void notify(const std::string &channel, E &&event) {
      using CE = typename std::remove_const<typename std::remove_reference<E>::type>::type;
      using typed_container_t = synchronous_callback_container<CE>;

      static_assert(!std::is_const<CE>::value, "Class must not be const");
      static_assert(!std::is_volatile<CE>::value, "Class must not be volatile");
      static_assert(!std::is_reference<CE>::value, "Class must not be a reference");
      static_assert(!std::is_pointer<CE>::value, "Class must not be a pointer");

      auto tid{gen::type_id<CE>()};
      auto &chan{callbacks[channel]};
      if (auto it{chan.find(tid)}; it == std::end(chan)) {
        return;
      }

      auto &container{chan[tid]};
      auto *typed_container{static_cast<typed_container_t *>(container.get())};
      typed_container->notify(std::forward<E>(event));
    }

    void deliver() {
      for (auto&[chan, cbs] : callbacks) {
        for (auto&[id, vect] : cbs) {
          vect->deliver();
        }
      }
    }

private:
    event_bus() = default;

private:
    std::unordered_map<std::string,
        std::unordered_map<type_id_t, callback_container_ptr>
    > callbacks;
};
}

#endif //TYPEWRITER_GAEM_EVENT_BUS_HPP
