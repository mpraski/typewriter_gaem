//
// Created by marcin on 8/18/19.
//

#ifndef TYPEWRITER_GAEM_SYNCHRONOUSLISTENERS_HPP
#define TYPEWRITER_GAEM_SYNCHRONOUSLISTENERS_HPP

#include <vector>
#include <queue>
#include <functional>
#include "Listeners.hpp"
#include "../utilities/General.hpp"

namespace engine {
template<class E>
class SynchronousListeners : public Listeners {
    using callback_t = std::function<void(const E &)>;
public:
    void remove(callback_id_t cbid) final {
      gen::remove_if(mCallbacks, [&](const auto &p) {
        return p.first == cbid;
      });
    }

    template<class F>
    void add(callback_id_t cbid, F &&cb) {
      mCallbacks.push_back(std::make_pair(cbid, std::forward<F>(cb)));
    }

    template<class I>
    void notify(I &&event) {
      mToDeliver.push(std::forward<I>(event));
    }

    void deliver() final {
      while (!mToDeliver.empty()) {
        const auto &event{mToDeliver.front()};
        for (const auto&[id, cb] : mCallbacks) {
          cb(event);
        }
        mToDeliver.pop();
      }
    }

private:
    std::vector<std::pair<callback_id_t, callback_t>> mCallbacks;
    std::queue<E> mToDeliver;
};
}

#endif //TYPEWRITER_GAEM_SYNCHRONOUSLISTENERS_HPP
