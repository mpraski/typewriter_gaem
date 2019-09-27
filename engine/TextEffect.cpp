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

engine::TextEffect::Kind engine::TextEffect::kind() const {
  return mKind;
}

size_t engine::TextEffect::begin() const {
  return mBegin;
}

size_t engine::TextEffect::end() const {
  return mEnd;
}

float engine::TextEffect::delayFactor() const {
  return mDelayFactor;
}

float engine::TextEffect::letterSpacingFactor() const {
  return mLetterSpacingFactor;
}

const sf::Color &engine::TextEffect::color() const {
  return mColor;
}

const std::string &engine::TextEffect::texture() const {
  return mTexture;
}

float engine::TextEffect::fontSizeFactor() const {
  return mFontSizeFactor;
}

void engine::TextEffect::setKind(engine::TextEffect::Kind mKind) {
  TextEffect::mKind = mKind;
}

void engine::TextEffect::setBegin(size_t mBegin) {
  TextEffect::mBegin = mBegin;
}

void engine::TextEffect::setEnd(size_t mEnd) {
  TextEffect::mEnd = mEnd;
}

void engine::TextEffect::setDelayFactor(float mDelayFactor) {
  TextEffect::mDelayFactor = mDelayFactor;
}

void engine::TextEffect::setLetterSpacingFactor(float mLetterSpacingFactor) {
  TextEffect::mLetterSpacingFactor = mLetterSpacingFactor;
}

void engine::TextEffect::setColor(const sf::Color &mColor) {
  TextEffect::mColor = mColor;
}

void engine::TextEffect::setTexture(const std::string &mTexture) {
  TextEffect::mTexture = mTexture;
}

void engine::TextEffect::setFontSizeFactor(float mFontSizeFactor) {
  TextEffect::mFontSizeFactor = mFontSizeFactor;
}
