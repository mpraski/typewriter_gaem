//
// Created by marcin on 8/18/19.
//

#ifndef TYPEWRITER_GAEM_SYNCHRONOUS_LISTENERS_HPP
#define TYPEWRITER_GAEM_SYNCHRONOUS_LISTENERS_HPP

#include <vector>
#include <queue>
#include <functional>
#include "listeners.hpp"
#include "../utilities/general.hpp"

namespace engine {
template<class E>
class synchronous_listeners : public listeners {
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

    template<class I>
    void notify(I &&event) {
      to_deliver.push(std::forward<I>(event));
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

#endif //TYPEWRITER_GAEM_SYNCHRONOUS_LISTENERS_HPP
