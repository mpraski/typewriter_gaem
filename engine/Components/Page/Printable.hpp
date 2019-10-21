//
// Created by marcin on 9/26/19.
//

#ifndef TYPEWRITER_GAEM_PRINTABLE_HPP
#define TYPEWRITER_GAEM_PRINTABLE_HPP

#include "../Mesh.hpp"
#include "../../TextEffect.hpp"
#include "../../Utilities/Cached.hpp"

namespace engine {
class Printable : public Mesh {
    using BackInserter = std::back_insert_iterator<TextEffect>;
    using EffectIt = std::vector<TextEffect>::const_iterator;

    enum class State {
        Printing,
        Complete,
        Pending,
        Scrolling
    };

    enum class Displacement {
        Center,
        Right,
        None
    };

    struct TextModifiers final {
        unsigned mFontSize = 12u;
        float mLineSpacing = 1.f;
        float mLineSpacingMargin = 1.f;
        bool mBold = false;
        bool mUnderlined = false;
        bool mStrikeThrough = false;
        bool mUppercase = false;
        Displacement mDisplacement = Displacement::None;
        size_t mDisplacementModeEnd = 0;
        size_t mDisplacementModeEndPrev = 0;
        float mItalicShear = 0.f;
        float mTypingDelayFactor = 1.f;
        float mLetterSpacingFactor = 1.f;
        sf::Color mTextColor = sf::Color::White;
    };

public:
    using Ptr = std::unique_ptr<Printable>;

    Printable(
        std::wstring contents,
        const std::vector<TextEffect> &textEffects
    );

    size_t currentCharacter() const;

    const std::wstring &to_string() const;

    wchar_t operator[](size_t idx) const;

    size_t length() const;

    void onStart(Entity &entity);

    void onEntityUpdate(Entity &entity, sf::Time dt) final;

protected:
    void preprocess();

    void offsetEffects(size_t idx = 0, int amount = 1);

private:
    void loadEffects(size_t idx);

    void applyTextEffects(size_t idx);

    void removeTextEffects(size_t idx);

    void breakLineAt(size_t idx);

    void injectLineAt(size_t idx);

    void redraw();

    void newLine();

    float measureText(size_t begin, size_t end);

    EffectIt findDisplacementEffect(Displacement d) const;

    EffectIt findEffect(enum TextEffect::Kind kind) const;

    float displacementSpacing(Displacement d, float width) const;

    static inline void trimStart(std::wstring &s) {
      s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](wchar_t ch) {
        return !std::iswblank(ch);
      }));
    }

    static inline void trimEnd(std::wstring &s) {
      s.erase(std::find_if(s.rbegin(), s.rend(), [](wchar_t ch) {
        return !std::iswblank(ch);
      }).base(), s.end());
    }

    static inline void addNewLine(std::wstring &s) {
      s.insert(s.end(), L'\n');
    }

protected:
    Cached<State> mState;
    bool mNeedsUpdate;
    float mMaxWidth;
    float mX;
    float mY;
    size_t mCurrentCharacter;
    std::wstring mContents;
    std::vector<TextEffect> mActiveEffects;
    std::unordered_map<size_t, std::vector<TextEffect>> mEffects;
    mutable TextModifiers mModifiers;
};

using PrintablePtr = Printable::Ptr;
}

#endif //TYPEWRITER_GAEM_PRINTABLE_HPP
