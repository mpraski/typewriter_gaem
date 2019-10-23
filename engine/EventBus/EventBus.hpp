//
// Created by marcin on 8/17/19.
//

#ifndef TYPEWRITER_GAEM_EVENTBUS_HPP
#define TYPEWRITER_GAEM_EVENTBUS_HPP

#include <cstddef>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include "Listeners.hpp"
#include "../Utilities/NonCopyable.h"
#include "../Utilities/NonMovable.h"

// Add event bus for indirect communication between
// scene nodes and such. May be useful to coordinate
// complex Translations between hierarchies of nodes
namespace engine {
using namespace boost::multi_index;

template<template<typename> class ListenersImp>
class EventBus final : private NonCopyable, private NonMovable {
    using ListenersPtr = std::unique_ptr<Listeners>;
    using ListenersMap = std::unordered_map<std::type_index, ListenersPtr>;
    const constexpr static auto DEFAULT_CHANNEL = "default_channel";

    struct ListenersPayload final {
        std::string channel;
        std::type_index type;
        std::unique_ptr<Listeners> pointer;
    };

public:
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

      auto it{mCallbacks.find(boost::make_tuple(channel, gen::type_id<E>()))};
      if (it == std::end(mCallbacks)) {
        auto[itt, ok]{mCallbacks.insert(
            {
                channel,
                gen::type_id<E>(),
                std::make_unique<ListenersImp<E>>()
            }
        )};

        if (!ok) {
          throw std::runtime_error(gen::str("Failure inserting listeners <", channel, ":", typeid(E).name(), ">"));
        }

        it = itt;
        mFlattenedListeners.push_back(it->pointer.get());
      }

      auto *typed_listeners{static_cast<ListenersImp<E> *>(it->pointer.get())};
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

      auto it{mCallbacks.find(boost::make_tuple(channel, gen::type_id<E>()))};
      if (it != std::end(mCallbacks)) {
        it->pointer->remove(cbid);
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

      static_assert(!std::is_volatile<CE>::value, "Class must not be volatile");
      static_assert(!std::is_pointer<CE>::value, "Class must not be a pointer");

      auto it{mCallbacks.find(boost::make_tuple(channel, gen::type_id<CE>()))};
      if (it != std::end(mCallbacks)) {
        auto *typed_listeners{static_cast<ListenersImp<CE> *>(it->pointer.get())};
        typed_listeners->notify(std::forward<E>(event));
      }
    }

    void deliver() {
      for (auto &ptr : mFlattenedListeners) {
        ptr->deliver();
      }
    }

private:
    friend class System;

    EventBus() = default;

private:
    std::vector<Listeners *> mFlattenedListeners;
    boost::multi_index_container<
        ListenersPayload,
        indexed_by<
            hashed_unique<
                composite_key<
                    ListenersPayload,
                    member<ListenersPayload, std::string, &ListenersPayload::channel>,
                    member<ListenersPayload, std::type_index, &ListenersPayload::type>
                >
            >
        >
    > mCallbacks;
};
}

#endif //TYPEWRITER_GAEM_EVENTBUS_HPP
