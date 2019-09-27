//
// Created by marcin on 9/25/19.
//

#ifndef TYPEWRITER_GAEM_PAGECONTROLLER_HPP
#define TYPEWRITER_GAEM_PAGECONTROLLER_HPP

#include "../Component.hpp"
#include "../../System.hpp"
#include "../translations/TranslateVertical.hpp"

namespace engine {
class PageController : public Component {
public:
    Kind kind() const override;

    void onStart(Entity &entity) override;

    void onEntityUpdate(Entity &entity, sf::Time dt) override;

private:
    bool shouldScroll() const;

private:
    float mNextY;
    std::vector<sf::Uint64> mPrintableIDs;
};
}

#endif //TYPEWRITER_GAEM_PAGECONTROLLER_HPP
