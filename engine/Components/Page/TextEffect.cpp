//
// Created by marcin on 7/20/19.
//

#include "TextEffect.hpp"

engine::TextEffect::TextEffect(
    Kind kind,
    size_t begin,
    size_t end
) : mKind{kind},
    mBegin{begin},
    mEnd{end},
    mDelayFactor{1.f},
    mLetterSpacingFactor{0.f},
    mColor{sf::Color::White},
    mTexture{""},
    mFontSizeFactor{1.f} {

}

engine::TextEffect &engine::TextEffect::withDelay(float df) {
  mDelayFactor = df;
  return *this;
}

engine::TextEffect &engine::TextEffect::withLetterSpacing(float ls) {
  mLetterSpacingFactor = ls;
  return *this;
}

engine::TextEffect &engine::TextEffect::withColor(sf::Color c) {
  mColor = c;
  return *this;
}

engine::TextEffect &engine::TextEffect::withTexture(std::string t) {
  mTexture = std::move(t);
  return *this;
}

engine::TextEffect &engine::TextEffect::withFontSize(float fz) {
  mFontSizeFactor = fz;
  return *this;
}