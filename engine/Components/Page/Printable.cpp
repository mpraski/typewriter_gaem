//
// Created by marcin on 9/26/19.
//

#include "Printable.hpp"
#include "../Translations/TranslateVertical.hpp"
#include "../../Entity.hpp"

engine::Printable::Printable(
    std::wstring contents,
    const std::vector<TextEffect> &textEffects
) : Mesh{},
    mState{State::Pending},
    mNeedsUpdate{false},
    mMaxWidth{System::instance().effectivePageWidth()},
    mX{},
    mY{static_cast<float>(System::instance().mFontSize)},
    mCurrentCharacter{},
    mContents{std::move(contents)},
    mActiveEffects{},
    mEffects{},
    mModifiers{} {
  for (const auto &e: textEffects) {
    mEffects[e.mBegin].push_back(e);
  }

  trimStart(mContents);
  trimEnd(mContents);
  addNewLine(mContents);
}


size_t engine::Printable::currentCharacter() const {
  return mCurrentCharacter;
}

const std::wstring &engine::Printable::to_string() const {
  return mContents;
}

wchar_t engine::Printable::operator[](size_t idx) const {
  return mContents[idx];
}

size_t engine::Printable::length() const {
  return mContents.length();
}

void engine::Printable::loadEffects(size_t idx) {
  if (auto k{mEffects.find(idx)}; k != mEffects.end())
    std::copy(
        std::cbegin(k->second),
        std::cend(k->second),
        std::back_inserter(mActiveEffects)
    );
}

void engine::Printable::breakLineAt(size_t idx) {
  if (!std::iswblank(mContents[idx])) {
    throw std::runtime_error(gen::str("Symbol ", mContents[idx], " is not blank"));
  }
  mContents[idx] = L'\n';
}

void engine::Printable::injectLineAt(size_t idx) {
  mContents.insert(mContents.begin() + idx, L'\n');
  offsetEffects(idx);
}

void engine::Printable::offsetEffects(size_t idx, int amount) {
  decltype(mEffects) newEffects;
  newEffects.reserve(mEffects.size());
  for (auto &[k, v] : mEffects) {
    auto modifiedKey{k};
    auto modifiedEffects{std::move(v)};
    if (k >= idx) {
      modifiedKey += amount;
      for (auto &e : modifiedEffects) {
        e.mBegin += amount;
        e.mEnd += amount;
      }
    }
    newEffects.insert(std::make_pair(modifiedKey, std::move(modifiedEffects)));
  }
  mEffects = std::move(newEffects);
}

void engine::Printable::preprocess() {
  float lineWidth{};
  float textHeight{};
  float fromLastBlank{};
  size_t lastBlank{std::numeric_limits<size_t>::max()};
  size_t len{mContents.length()};
  sf::Uint32 currChar, mPrevChar{sf::Utf32::decodeWide(mContents[0])};

  for (size_t i = 0; i < len; ++i) {
    applyTextEffects(i);
    currChar = sf::Utf32::decodeWide(mContents[i]);

    if (currChar == L'\r')
      continue;

    if (mModifiers.mDisplacement != Displacement::None && !mModifiers.mDisplacementModeEnd) {
      auto d_effect{findDisplacementEffect(mModifiers.mDisplacement)};
      mModifiers.mDisplacementModeEnd = d_effect->mEnd;
      lineWidth = 0;
    }

    if (mModifiers.mDisplacementModeEnd && i != mModifiers.mDisplacementModeEnd) {
      continue;
    } else {
      mModifiers.mDisplacementModeEnd = 0;
    }

    lineWidth += System::instance().mFont->getKerning(mPrevChar, currChar, mModifiers.mFontSize);

    if ((currChar == L' ') || (currChar == L'\n') || (currChar == L'\t')) {
      switch (currChar) {
        case L' ':
          lineWidth += System::instance().mWhitespaceWidth + mModifiers.mLetterSpacingFactor;
          if (i) {
            lastBlank = i;
            fromLastBlank = lineWidth;
          }
          break;
        case L'\t':
          lineWidth += System::instance().mWhitespaceWidth * 4.f;
          if (i) {
            lastBlank = i;
            fromLastBlank = lineWidth;
          }
          break;
        case L'\n':
          lineWidth = 0;
          textHeight += System::instance().mLineSpacing;
          break;
        default:
          break;
      }
    } else {
      const auto &glyph{System::instance().mFont->getGlyph(currChar, mModifiers.mFontSize, mModifiers.mBold)};
      lineWidth += glyph.advance + System::instance().mLetterSpacing + mModifiers.mLetterSpacingFactor;
    }

    mPrevChar = currChar;
    removeTextEffects(i);

    if (lineWidth > mMaxWidth) {
      if (lastBlank == std::numeric_limits<size_t>::max()) {
        injectLineAt(i);
        len++;
        lineWidth = 0;
      } else {
        breakLineAt(lastBlank);
        lineWidth -= fromLastBlank;
        fromLastBlank = 0;
      }
      lastBlank = std::numeric_limits<size_t>::max();
      textHeight += System::instance().mLineSpacing;
    }
  }

  mBounds = sf::FloatRect{sf::Vector2f{}, sf::Vector2f{mMaxWidth, textHeight}};
  mActiveEffects.clear();
}

float engine::Printable::measureText(size_t begin, size_t end) {
  float textWidth{};
  sf::Uint32 currChar, prevChar{sf::Utf32::decodeWide(mContents[begin])};
  for (size_t i = begin; i < end; ++i) {
    applyTextEffects(i);
    currChar = sf::Utf32::decodeWide(mContents[i]);

    if (currChar == L'\r')
      continue;

    textWidth += System::instance().mFont->getKerning(prevChar, currChar, mModifiers.mFontSize);

    if ((currChar == L' ') || (currChar == L'\n') || (currChar == L'\t')) {
      switch (currChar) {
        case L' ':
          textWidth += System::instance().mWhitespaceWidth + mModifiers.mLetterSpacingFactor;
          break;
        case L'\t':
          textWidth += System::instance().mWhitespaceWidth * 4.f;
          break;
        case L'\n':
          return textWidth;
        default:
          break;
      }
    } else {
      const auto &glyph{System::instance().mFont->getGlyph(currChar, mModifiers.mFontSize, mModifiers.mBold)};
      textWidth += glyph.advance + System::instance().mLetterSpacing + mModifiers.mLetterSpacingFactor;
    }

    prevChar = currChar;
    removeTextEffects(i);
  }

  return textWidth;
}

engine::Printable::EffectIt engine::Printable::findDisplacementEffect(engine::Printable::Displacement d) const {
  switch (d) {
    case Displacement::Center:
      return findEffect(TextEffect::Kind::Center);
    case Displacement::Right:
      return findEffect(TextEffect::Kind::Right);
    default:
      throw std::runtime_error("Wrong displacement value");
  }
}

engine::Printable::EffectIt engine::Printable::findEffect(TextEffect::Kind kind) const {
  return gen::find_if(mActiveEffects, [&](const auto &e) {
    return e.mKind == kind;
  });
}

float engine::Printable::displacementSpacing(engine::Printable::Displacement d, float width) const {
  switch (d) {
    case Displacement::Center:
      return mMaxWidth / 2.f - width / 2.f;
    case Displacement::Right:
      return mMaxWidth - width;
    default:
      throw std::runtime_error("Wrong displacement value");
  }
}

void engine::Printable::applyTextEffects(size_t idx) {
  loadEffects(idx);

  mModifiers.mFontSize = System::instance().mFontSize;

  for (const auto &e : mActiveEffects) {
    switch (e.mKind) {
      case TextEffect::Kind::Bold:
        mModifiers.mBold = true;
        break;
      case TextEffect::Kind::Italic:
        mModifiers.mItalicShear = 0.209f;
        break;
      case TextEffect::Kind::Underline:
        mModifiers.mUnderlined = true;
        break;
      case TextEffect::Kind::StrikeThrough:
        mModifiers.mStrikeThrough = true;
        break;
      case TextEffect::Kind::Uppercase:
        mModifiers.mUppercase = true;
        break;
      case TextEffect::Kind::Delay:
        mModifiers.mTypingDelayFactor = e.mDelayFactor;
        break;
      case TextEffect::Kind::Spacing:
        mModifiers.mLetterSpacingFactor = e.mLetterSpacingFactor;
        break;
      case TextEffect::Kind::Color:
        mModifiers.mTextColor = e.mColor;
        break;
      case TextEffect::Kind::Texture:
        //text_texture = &mSystem->get_textures(e.mTexture, "text");
        break;
      case TextEffect::Kind::Center:
        mModifiers.mDisplacement = Displacement::Center;
        break;
      case TextEffect::Kind::Right:
        mModifiers.mDisplacement = Displacement::Right;
        break;
      case TextEffect::Kind::FontSize:
        //font_size = std::lround(static_cast<float>(mSystem->font_size) * e.mFontSizeFactor);
        //fsize(current_printable) = font_size;
        break;
    }
  }
}

void engine::Printable::removeTextEffects(size_t idx) {
  gen::remove_if(mActiveEffects, [&](const auto &e) {
    return idx == e.mEnd;
  });

  mModifiers.mBold = false;
  mModifiers.mUnderlined = false;
  mModifiers.mStrikeThrough = false;
  mModifiers.mUppercase = false;
  mModifiers.mDisplacement = Displacement::None;
  mModifiers.mItalicShear = 0.f;
  mModifiers.mLetterSpacingFactor = 1.f;
  mModifiers.mTypingDelayFactor = 1.f;
  mModifiers.mTextColor = sf::Color::White;
}

void engine::Printable::onStart(engine::Entity &entity) {
  listen<sf::Uint64>("printable_selection", [this](auto id) {
    if (getUID() == id) {
      mState = State::Printing;
    }
  });

  listen("page_scroll_begin", [&, this](const auto &msg) {
    mState = State::Scrolling;
    entity.addComponent(
        std::make_unique<TranslateVertical>(
            TranslateVertical::from(0.f)
                .to(-System::instance().mLineSpacing)
                .during(60)
                .via(tweeny::easing::quadraticInOut)
                .onFinish([this]() {
                  notifyChannel(getChannel());
                })
        ), this
    );
  });

  listen(getChannel(), [this](const auto &msg) {
    System::logger().info("rollback");
    mState.rollback();
  });

  mTexture = &System::instance().mFont->getTexture(mModifiers.mFontSize);
}

void engine::Printable::onEntityUpdate(engine::Entity &entity, sf::Time dt) {
  switch (*mState) {
    case State::Scrolling:
    case State::Pending:
      return;
    case State::Complete:
      redraw();
      return;
    case State::Printing:
      if (!entity.hasElapsed(
          sf::milliseconds(static_cast<float>(System::instance().mTypingDelay) * mModifiers.mTypingDelayFactor))) {
        return;
      }
  }

  if (mCurrentCharacter) {
    removeTextEffects(mCurrentCharacter - 1);
  }
  applyTextEffects(mCurrentCharacter);

  auto prevChar{sf::Utf32::decodeWide(mContents[mCurrentCharacter ? mCurrentCharacter - 1 : 0])};
  auto currChar{sf::Utf32::decodeWide(mContents[mCurrentCharacter])};

  // Skip to avoid glitches
  if (currChar == L'\r')
    return;

  if (mModifiers.mDisplacement != Displacement::None && !mModifiers.mDisplacementModeEnd) {
    auto d{mModifiers.mDisplacement};
    auto d_effect{findDisplacementEffect(d)};
    mModifiers.mDisplacementModeEnd = d_effect->mEnd + 1;
    auto width{measureText(d_effect->mBegin, d_effect->mEnd)};
    auto spacing{displacementSpacing(d, width)};
    if (mModifiers.mDisplacementModeEnd != mCurrentCharacter - 1) {
      mY += System::instance().mLineSpacing;
    }
    mX = spacing;
    applyTextEffects(mCurrentCharacter);
    mModifiers.mDisplacementModeEndPrev = mModifiers.mDisplacementModeEnd;
  }

  if (mModifiers.mDisplacementModeEnd && mCurrentCharacter == mModifiers.mDisplacementModeEnd) {
    mModifiers.mDisplacementModeEnd = 0;
    mY += System::instance().mLineSpacing;
    mX = 0.f;
  }

  mX += System::instance().mFont->getKerning(prevChar, currChar, mModifiers.mFontSize);

  // Handle special characters
  if ((currChar == L' ') || (currChar == L'\n') || (currChar == L'\t')) {
    switch (currChar) {
      case L' ':
        mX += System::instance().mWhitespaceWidth + mModifiers.mLetterSpacingFactor;
        break;
      case L'\t':
        mX += System::instance().mWhitespaceWidth * 4.f;
        break;
      case L'\n':
        newLine();
        break;
      default:
        break;
    }
  } else {
    const auto &glyph{System::instance().mFont->getGlyph(currChar, mModifiers.mFontSize, mModifiers.mBold)};

    gen::addGlyphQuad(
        mVertices,
        sf::Vector2f(mX, mY),
        mModifiers.mTextColor,
        glyph,
        mModifiers.mItalicShear
    );

    auto advance{glyph.advance + System::instance().mLetterSpacing + mModifiers.mLetterSpacingFactor};

    if (mModifiers.mUnderlined) {
      gen::addLine(
          mVertices,
          mX,
          advance,
          mY,
          mModifiers.mTextColor,
          System::instance().mUnderlineOffset,
          System::instance().mUnderlineThickness
      );
    }

    auto xBounds{System::instance().mFont->getGlyph(L'x', mModifiers.mFontSize, mModifiers.mBold).bounds};
    float strikeThroughOffset{xBounds.top + xBounds.height / 2.f};

    if (mModifiers.mStrikeThrough) {
      gen::addLine(
          mVertices,
          mX,
          advance,
          mY,
          mModifiers.mTextColor,
          strikeThroughOffset,
          System::instance().mUnderlineThickness
      );
    }

    mX += advance;

    AudioSystem::instance().playRandom("typewriter_clicks");
  }

  if (++mCurrentCharacter == mContents.length()) {
    mState = State::Complete;
    notifyChannel("printable_end");
  }
}

void engine::Printable::redraw() {
  mVertices.clear();
  mActiveEffects.clear();

  mX = 0.f;
  mY = static_cast<float>(System::instance().mFontSize);

  sf::Uint32 currChar, prevChar{sf::Utf32::decodeWide(mContents[0])};
  for (size_t i = 0; i < mContents.length(); ++i) {
    applyTextEffects(i);
    currChar = sf::Utf32::decodeWide(mContents[i]);

    if (currChar == L'\r')
      return;

    if (mModifiers.mDisplacement != Displacement::None && !mModifiers.mDisplacementModeEnd) {
      auto d{mModifiers.mDisplacement};
      auto d_effect{findDisplacementEffect(d)};
      mModifiers.mDisplacementModeEnd = d_effect->mEnd + 1;
      auto width{measureText(d_effect->mBegin, d_effect->mEnd)};
      auto spacing{displacementSpacing(d, width)};
      if (mModifiers.mDisplacementModeEnd != i - 1) {
        mY += System::instance().mLineSpacing;
      }
      mX = spacing;
      applyTextEffects(i);
      mModifiers.mDisplacementModeEndPrev = mModifiers.mDisplacementModeEnd;
    }

    if (mModifiers.mDisplacementModeEnd && mCurrentCharacter == mModifiers.mDisplacementModeEnd) {
      mModifiers.mDisplacementModeEnd = 0;
      mY += System::instance().mLineSpacing;
      mX = 0.f;
    }

    mX += System::instance().mFont->getKerning(prevChar, currChar, mModifiers.mFontSize);

    if ((currChar == L' ') || (currChar == L'\n') || (currChar == L'\t')) {
      switch (currChar) {
        case L' ':
          mX += System::instance().mWhitespaceWidth + mModifiers.mLetterSpacingFactor;
          break;
        case L'\t':
          mX += System::instance().mWhitespaceWidth * 4.f;
          break;
        case L'\n':
          mY += System::instance().mLineSpacing;
          mX = 0.f;
          break;
      }
    } else {
      const auto &glyph{System::instance().mFont->getGlyph(currChar, mModifiers.mFontSize, mModifiers.mBold)};

      gen::addGlyphQuad(
          mVertices,
          sf::Vector2f(mX, mY),
          mModifiers.mTextColor,
          glyph,
          mModifiers.mItalicShear
      );

      auto advance{glyph.advance + System::instance().mLetterSpacing + mModifiers.mLetterSpacingFactor};

      if (mModifiers.mUnderlined) {
        gen::addLine(
            mVertices,
            mX,
            advance,
            mY,
            mModifiers.mTextColor,
            System::instance().mUnderlineOffset,
            System::instance().mUnderlineThickness
        );
      }

      auto xBounds{System::instance().mFont->getGlyph(L'x', mModifiers.mFontSize, mModifiers.mBold).bounds};
      float strikeThroughOffset{xBounds.top + xBounds.height / 2.f};

      if (mModifiers.mStrikeThrough) {
        gen::addLine(
            mVertices,
            mX,
            advance,
            mY,
            mModifiers.mTextColor,
            strikeThroughOffset,
            System::instance().mUnderlineThickness
        );
      }

      mX += advance;
    }

    prevChar = currChar;
    removeTextEffects(i);
  }
}

void engine::Printable::newLine() {
  mY += System::instance().mLineSpacing;
  mX = 0.f;

  notifyChannel("new_line");
}