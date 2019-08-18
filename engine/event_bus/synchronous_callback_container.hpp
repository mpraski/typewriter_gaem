//
// Created by marcin on 8/18/19.
//

#ifndef TYPEWRITER_GAEM_SYNCHRONOUS_CALLBACK_CONTAINER_HPP
#define TYPEWRITER_GAEM_SYNCHRONOUS_CALLBACK_CONTAINER_HPP

#include <vector>
#include <queue>
#include <functional>
#include "callback_container.hpp"
#include "../utilities/general.hpp"

namespace engine {
template<class E>
class synchronous_callback_container : public callback_container {
    using callback_t = std::function<void(const E &)>;
public:
    void remove(callback_id_t cbid) final {
      gen::remove_if(callbacks, [&](const auto &p) {
        return p.first == cbid;
      });
    }

    template<class F>
    void add(callback_id_t cbid, F &&cb) {
      callbacks.push_back(std::make_pair(cbid, std::forward<F>(cb)));
    }

    void notify(E &&event) {
      to_deliver.push(std::move(event));
    }

    void deliver() final {
      while (!to_deliver.empty()) {
        const auto &event{to_deliver.front()};
        for (const auto&[id, cb] : callbacks) {
          cb(event);
        }
        to_deliver.pop();
      }
    }

private:
    std::vector<std::pair<callback_id_t, callback_t>> callbacks;
    std::queue<E> to_deliver;
};
}

#endif //TYPEWRITER_GAEM_SYNCHRONOUS_CALLBACK_CONTAINER_HPP
