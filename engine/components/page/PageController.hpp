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
#include "../translations/TranslateVertical.hpp"

namespace engine {
class PageController : public Component {
public:
    Kind kind() const override;

    void onStart(Entity &entity) override;

    void onEntityUpdate(Entity &entity, sf::Time dt) override;

private:
    bool shouldScroll() const;

    static std::unique_ptr<Printable> fromTemplate(const PrintableTemplate &tlp);

    void addContents(const std::vector<PrintableTemplate> &contents);

private:
    float mNextY;
    std::vector<sf::Uint64> mPrintableIDs;
    DecisionNode mDecisionNode;
};
}

#endif //TYPEWRITER_GAEM_PAGECONTROLLER_HPP
