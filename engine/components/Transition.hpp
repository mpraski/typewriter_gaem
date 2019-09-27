//
// Created by marcin on 8/8/19.
//

#ifndef TYPEWRITER_GAEM_TRANSITION_HPP
#define TYPEWRITER_GAEM_TRANSITION_HPP

#include <cstddef>
#include <functional>
#include <tuple>
#include <utility>
#include "../utilities/Tuples.hpp"
#include "tweeny.h"
#include "Component.hpp"

namespace engine {
template<class... Ts>
class Transition : public Component {
    using callback = std::function<void()>;
    const constexpr static auto ANIMATION_END = std::numeric_limits<size_t>::max();
protected:
    class Builder {
    public:
        Builder()
            : mLoop{},
              mDuration{},
              mOnFinish{[] {}},
              mOnStep{[] {}},
              mTween{} {
        }

        Builder &loop(bool l = true) {
          mLoop = l;
          return *this;
        }

        template<class... Args>
        Builder &from(Args &&... from) {
          mTween = mTween.from(std::forward<Args>(from)...);
          return *this;
        }

        template<class... Args>
        Builder &to(Args &&... to) {
          mTween = mTween.to(std::forward<Args>(to)...);
          return *this;
        }

        Builder &during(size_t steps) {
          mDuration = steps;
          mTween = mTween.during(steps);
          return *this;
        }

        template<class E>
        Builder &via(E &&e) {
          mTween = mTween.via(std::forward<E>(e));
          return *this;
        }

        template<class C>
        Builder &onFinish(C &&callback) {
          mOnFinish = std::forward<C>(callback);
          return *this;
        }

        template<class C>
        Builder &onStep(C &&callback) {
          mOnStep = std::forward<C>(callback);
          return *this;
        }

    private:
        friend class Transition<Ts...>;

        bool mLoop;
        size_t mDuration;
        callback mOnFinish;
        callback mOnStep;
        tweeny::tween<Ts...> mTween;
    };

public:
    explicit Transition(const Builder &b)
        : mLoop{},
          mCurrStep{ANIMATION_END},
          mDuration{b.mDuration},
          mOnFinish{b.mOnFinish},
          mOnStep{b.mOnStep},
          mTween{b.mTween},
          mCurrDelta{},
          mApplyStepFun{[&](Ts &&... ts) {
            applyStep(std::move(ts)...);
          }} {
      if (!mDuration) {
        throw std::invalid_argument("Duration cannot be zero");
      }
    }

    ~Transition() = default;

    template<class... Args>
    static Builder from(Args &&... from) {
      return Builder().from(std::forward<Args>(from)...);
    }

    void start() {
      mCurrStep = 0;
      mCurrDelta = std::tuple<Ts...>{};
      mTween.seek(0);
    }

    Kind kind() const final {
      return Component::Kind::Script;
    }

    void onStart(Entity &entity) final {
      start();
    }

    void onEntityUpdate(Entity &entity, sf::Time dt) final {
      if (mCurrStep < mDuration) {
        std::apply(mApplyStepFun, delta());
        mOnStep();
        mCurrStep++;
      } else if (mCurrStep != ANIMATION_END) {
        mCurrStep = ANIMATION_END;
        mOnFinish();
        if (mLoop) {
          start();
        } else {
          destroy();
        }
      }
    }

protected:
    virtual void applyStep(Ts...) = 0;

    std::tuple<Ts...> delta() {
      auto step{as_tuple(mTween.step(1u))};
      auto result{step - mCurrDelta};
      mCurrDelta = std::move(step);
      return result;
    }

private:
    bool mLoop;
    size_t mCurrStep;
    size_t mDuration;
    callback mOnFinish;
    callback mOnStep;
    tweeny::tween<Ts...> mTween;
    std::tuple<Ts...> mCurrDelta;
    std::function<void(Ts &&...)> mApplyStepFun;
};
}

#endif //TYPEWRITER_GAEM_TRANSITION_HPP
