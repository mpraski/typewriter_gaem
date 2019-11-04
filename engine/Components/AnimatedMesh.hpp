//
// Created by marcin on 9/25/19.
//

#ifndef TYPEWRITER_GAEM_ANIMATEDMESH_HPP
#define TYPEWRITER_GAEM_ANIMATEDMESH_HPP

#include <SFML/Config.hpp>
#include <string>
#include "Mesh.hpp"

namespace engine {
class AnimatedMesh final : public Mesh {
public:
    using Ptr = std::unique_ptr<AnimatedMesh>;

    struct Animation final {
        std::string mName;
        sf::Uint8 mBeginFrame;
        sf::Uint8 mEndFrame;
    };

    AnimatedMesh(const sf::Texture &);

    void onStart(Entity &entity) final;

    void onEntityUpdate(Entity &entity, sf::Time dt) final;

    void setNormalMap(const sf::Texture &);

    void setFrameSize(const sf::Vector2u &size);

    const sf::Vector2u &getFrameSize() const;

    void setFrameCount(sf::Uint8 count);

    sf::Uint8 getFrameCount() const;

    void setFrameRate(float rate);

    float getFrameRate() const;

    void lopp(bool looped);

    bool looped() const;

    void setAnimation(const std::string &name);

    void play(sf::Uint8 start = 0, sf::Uint8 end = 0, sf::Uint8 offset = 0);

    bool playing() const;

    void pause(bool pause);


private:
    std::unordered_map<std::string, Animation> mAnimations;

    const sf::Texture *mNormalMap;
    sf::Sprite m_sprite;
    sf::IntRect mSubRect;
    sf::Vector2u mTextureSize;
    sf::Vector2u mFrameSize;

    sf::Uint8 mFrameCount;
    sf::Uint8 mCurrentFrame;
    sf::Uint8 mFirstFrame;
    sf::Uint8 mLastFrame;

    float mFrameRate;

    bool mPlaying;
    bool mLoop;
};
}

#endif //TYPEWRITER_GAEM_ANIMATEDMESH_HPP
