//
// Created by marcin on 7/20/19.
//

#ifndef TYPEWRITER_GAEM_TEXTEFFECT_HPP
#define TYPEWRITER_GAEM_TEXTEFFECT_HPP

#include <type_traits>
#include <functional>
#include <SFML/Graphics.hpp>

namespace engine {
class TextEffect {
public:
    enum class Kind {
        BOLD,
        ITALIC,
        UPPERCASE,
        UNDERLINE,
        STRIKE_THROUGH,
        DELAY,
        SPACING,
        COLOR,
        TEXTURE,
        CENTER,
        RIGHT,
        FONT_SIZE
    };

    TextEffect(
        Kind kind,
        size_t begin,
        size_t end
    );

    TextEffect &withDelay(float df);

    TextEffect &withLetterSpacing(float ls);

    TextEffect &withColor(sf::Color c);

    TextEffect &withTexture(std::string t);

    TextEffect &withFontSize(float fz);

    inline bool operator==(const TextEffect &other) const noexcept {
      return mKind == other.mKind
             && mBegin == other.mBegin
             && mEnd == other.mEnd;
    }

    inline bool operator!=(const TextEffect &other) const noexcept {
      return !(*this == other);
    }

public:
    Kind mKind;
    size_t mBegin;
    size_t mEnd;
    float mDelayFactor;
    float mLetterSpacingFactor;
    sf::Color mColor;
    std::string mTexture;
    float mFontSizeFactor;

    const constexpr static enum TextEffect::Kind STATIC_EFFECTS[] = {
        TextEffect::Kind::BOLD,
        TextEffect::Kind::ITALIC,
        TextEffect::Kind::UPPERCASE
    };
};
}

namespace std {
template<>
class hash<engine::TextEffect> {
public :
    size_t operator()(const engine::TextEffect &e) const {
      return static_cast<size_t>(e.mKind) ^ e.mBegin ^ e.mEnd;
    }
};

template<>
class equal_to<engine::TextEffect> {
public:
    bool operator()(const engine::TextEffect &l, const engine::TextEffect &r) const {
      return l == r;
    }
};
}

#endif //TYPEWRITER_GAEM_TEXTEFFECT_HPP
