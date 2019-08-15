//
// Created by marcin on 8/8/19.
//

#ifndef TYPEWRITER_GAEM_TRANSLATE_HPP
#define TYPEWRITER_GAEM_TRANSLATE_HPP

#include <SFML/Graphics.hpp>
#include "animation.hpp"

namespace engine {
class translate : public transformable_animation<float, float> {
public:
    using transformable_animation<float, float>::transformable_animation;

protected:
    void apply_step(float x, float y) const final {
      transformable.move(x, y);
    }
};

class translate_vertical : public transformable_animation<float> {
public:
    using transformable_animation<float>::transformable_animation;

protected:
    void apply_step(float y) const final {
      transformable.move(0.f, y);
    }
};

class translate_horizontal : public transformable_animation<float> {
public:
    using transformable_animation<float>::transformable_animation;

protected:
    void apply_step(float x) const final {
      transformable.move(x, 0.f);
    }
};
}

#endif //TYPEWRITER_GAEM_TRANSLATE_HPP
