//
// Created by marcin on 9/27/19.
//

#ifndef TYPEWRITER_GAEM_CACHED_HPP
#define TYPEWRITER_GAEM_CACHED_HPP

#include <algorithm>

template<typename T>
class Cached final {
    static_assert(std::is_default_constructible_v<T>, "T must be default constructible");
    static_assert(std::is_copy_constructible_v<T>, "T must be copy constructible");
    static_assert(std::is_copy_assignable_v<T>, "T must be copy assignable");
public:
    constexpr Cached()
        : mCurrent{},
          mPrevious{} {}

    constexpr explicit Cached(const T &initial)
        : mCurrent{initial},
          mPrevious{initial} {}

    constexpr Cached(const Cached &m)
        : mCurrent{m.mCurrent},
          mPrevious{m.mPrevious} {}

    constexpr Cached(Cached &&m) noexcept
        : mCurrent{std::move(m.mCurrent)},
          mPrevious{std::move(m.mPrevious)} {}

    constexpr Cached &operator=(Cached &&m) noexcept {
      mCurrent = std::move(m.mCurrent);
      mPrevious = std::move(m.mPrevious);
      return *this;
    }

    constexpr Cached &operator=(const Cached &m) {
      mCurrent = m.mCurrent;
      mPrevious = m.mPrevious;
      return *this;
    }

    constexpr Cached &operator=(const T &t) {
      mPrevious = mCurrent;
      mCurrent = t;
      return *this;
    }

    constexpr Cached &operator=(T &&t) {
      mPrevious = std::move(mCurrent);
      mCurrent = std::move(t);
      return *this;
    }

    constexpr const T &operator*() const {
      return mCurrent;
    }

    constexpr T &operator*() {
      return mCurrent;
    }

    constexpr const T &current() const {
      return mCurrent;
    }

    constexpr T &current() {
      return mCurrent;
    }

    constexpr const T &previous() const {
      return mPrevious;
    }

    constexpr T &previous() {
      return mPrevious;
    }

    constexpr explicit operator bool() const {
      return mCurrent.operator bool();
    }

    constexpr Cached &rollback() {
      std::swap(mCurrent, mPrevious);
      return *this;
    }

private:
    T mCurrent;
    T mPrevious;
};

#endif //TYPEWRITER_GAEM_CACHED_HPP
