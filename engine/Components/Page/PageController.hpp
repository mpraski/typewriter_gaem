//
// Created by marcin on 9/25/19.
//

#ifndef TYPEWRITER_GAEM_PAGECONTROLLER_HPP
#define TYPEWRITER_GAEM_PAGECONTROLLER_HPP

#include "../Component.hpp"
#include "Printable.hpp"
#include "DecisionNode.hpp"
#include "Paragraph.hpp"
#include "Dialog.hpp"
#include "../../System.hpp"
#include "../Translations/TranslateVertical.hpp"

namespace engine {
class PageController final : public Component {
public:
    explicit PageController(DecisionNode n);

    Kind kind() const final;

    void onStart(Entity &entity) final;

    void onEntityUpdate(Entity &entity, sf::Time dt) final;

private:
    bool shouldScroll() const;

    static std::unique_ptr<Printable> fromTemplate(const PrintableTemplate &tlp);

    void addContents(Entity &entity, const std::vector<PrintableTemplate> &contents);

private:
    float mNextY;
    float mCurrentY;
    std::vector<sf::Uint64> mPrintableIDs;
    std::vector<sf::Uint64>::iterator mCurrentPrintable;
    DecisionNode mDecisionNode;
};
}

#endif //TYPEWRITER_GAEM_PAGECONTROLLER_HPP
