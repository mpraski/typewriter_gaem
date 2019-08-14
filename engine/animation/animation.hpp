//
// Created by marcin on 8/8/19.
//

#ifndef TYPEWRITER_GAEM_ANIMATION_HPP
#define TYPEWRITER_GAEM_ANIMATION_HPP

#include <cstddef>
#include <functional>
#include <tuple>
#include <utility>
#include <iostream>
#include "../utilities/tuples.hpp"
#include "tweeny.h"

namespace engine {
template<class... Ts>
class animation {
    using callback = std::function<void()>;
    const constexpr static auto ANIMATION_END = std::numeric_limits<size_t>::max();
protected:
    class builder {
    public:
        builder()
            : _loop{},
              _prime{},
              _duration{},
              _on_finish{[] {}},
              _on_step{[] {}},
              _tween{} {
        }

        builder &loop(bool l = true) {
          _loop = l;
          return *this;
        }

        builder &prime(bool p = true) {
          _prime = p;
          return *this;
        }

        template<class... Args>
        builder &from(Args &&... from) {
          _tween = _tween.from(std::forward<Args>(from)...);
          return *this;
        }

        template<class... Args>
        builder &to(Args &&... to) {
          _tween = _tween.to(std::forward<Args>(to)...);
          return *this;
        }

        builder &during(size_t steps) {
          _duration = steps;
          _tween = _tween.during(steps);
          return *this;
        }

        template<class E>
        builder &via(E &&e) {
          _tween = _tween.via(std::forward<E>(e));
          return *this;
        }

        template<class C>
        builder &on_finish(C &&cb) {
          _on_finish = std::forward<C>(cb);
          return *this;
        }

        template<class C>
        builder &on_step(C &&cb) {
          _on_step = std::forward<C>(cb);
          return *this;
        }

    private:
        friend class animation<Ts...>;

        bool _loop;
        bool _prime;
        size_t _duration;
        callback _on_finish;
        callback _on_step;
        tweeny::tween<Ts...> _tween;
    };

public:
    explicit animation(const builder &b)
        : _curr_delta{},
          _curr_step{ANIMATION_END},
          _duration{b._duration},
          _on_finish{b._on_finish},
          _on_step{b._on_step},
          _tween{b._tween},
          _loop{b._loop},
          _apply_step_fun{[&](Ts &&... ts) {
            apply_step(std::move(ts)...);
          }} {
      if (!_duration) {
        throw std::invalid_argument("Duration cannot be zero");
      }
      if (b._prime) {
        prime();
      }
    }

    template<class... Args>
    static builder from(Args &&... from) {
      return builder().from(std::forward<Args>(from)...);
    }

    bool running() const {
      return _curr_step != ANIMATION_END;
    }

    void prime() {
      _curr_step = 0;
      _curr_delta = std::tuple<Ts...>{};
      _tween.seek(0);
    }

    void step() {
      if (_curr_step < _duration) {
        std::apply(_apply_step_fun, delta());
        _on_step();
        _curr_step++;
      } else if (_curr_step != ANIMATION_END) {
        _curr_step = ANIMATION_END;
        _on_finish();
        if (_loop) {
          prime();
        }
      }
    }

protected:
    virtual void apply_step(Ts... vals) const = 0;

    std::tuple<Ts...> delta() {
      auto step{as_tuple(_tween.step(1u))};
      auto result{step - _curr_delta};
      _curr_delta = std::move(step);
      return result;
    }

private:
    std::tuple<Ts...> _curr_delta;
    size_t _curr_step;
    size_t _duration;
    callback _on_finish;
    callback _on_step;
    tweeny::tween<Ts...> _tween;
    bool _loop;
    std::function<void(Ts &&...)> _apply_step_fun;
};

// The transformable referenced here must exist throughout the lifetime of
// this animation
template<class... Ts>
class transformable_animation : public animation<Ts...> {
    using builder = typename animation<Ts...>::builder;
public:
    transformable_animation(
        sf::Transformable &t,
        const builder &b
    ) : animation<Ts...>(b),
        transformable{t} {

    };

protected:
    sf::Transformable &transformable;
};
}

#endif //TYPEWRITER_GAEM_ANIMATION_HPP
