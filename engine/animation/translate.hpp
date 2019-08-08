//
// Created by marcin on 8/8/19.
//

#ifndef TYPEWRITER_GAEM_TRANSLATE_HPP
#define TYPEWRITER_GAEM_TRANSLATE_HPP

#include <SFML/Graphics.hpp>
#include "animation.hpp"

namespace engine {
class translate : public drawable_animation<float, float> {
public:
    using builder = animation<float, float>::builder;
    using drawable_animation<float, float>::drawable_animation;

private:
    void apply_step(float x, float y) const override {
      transformable.move(x, y);
    }
};

class translate_vertical : public drawable_animation<float> {
public:
    using builder = animation<float>::builder;
    using drawable_animation<float>::drawable_animation;

private:
    void apply_step(float y) const override {
      transformable.move(0.f, y);
    }
};

class translate_horizontal : public drawable_animation<float> {
public:
    using builder = animation<float>::builder;
    using drawable_animation<float>::drawable_animation;

private:
    void apply_step(float x) const override {
      transformable.move(x, 0.f);
    }
};
}

#endif //TYPEWRITER_GAEM_TRANSLATE_HPP
