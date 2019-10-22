//
// Created by Marcin Praski on 22.10.19.
//

#ifndef TYPEWRITER_GAEM_SYNCHRONOUSLISTENERS_H
#define TYPEWRITER_GAEM_SYNCHRONOUSLISTENERS_H

#include <vector>
#include <functional>
#include "Listeners.hpp"
#include "../Utilities/General.hpp"

namespace engine {
template<class E>
class SynchronousListeners final : public Listeners {
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
      for (const auto&[id, cb] : mCallbacks) {
        cb(event);
      }
    }

    void deliver() final {

    }

private:
    std::vector<std::pair<callback_id_t, callback_t>> mCallbacks;
};
}
#endif //TYPEWRITER_GAEM_SYNCHRONOUSLISTENERS_H
