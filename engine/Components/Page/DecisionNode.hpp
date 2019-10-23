//
// Created by marcin on 9/27/19.
//

#ifndef TYPEWRITER_GAEM_DECISIONNODE_HPP
#define TYPEWRITER_GAEM_DECISIONNODE_HPP

#include <string>
#include <memory>
#include <vector>
#include "TextEffect.hpp"

namespace engine {
struct PrintableTemplate final {
    enum class Kind {
        Paragraph,
        Dialog
    };

    PrintableTemplate(std::wstring content, std::vector<TextEffect> &&effects);

    PrintableTemplate(std::wstring person, std::wstring speech);

    Kind kind;
    std::wstring text1;
    std::wstring text2;
    std::vector<TextEffect> effects;
};

class DecisionNode {
public:
    DecisionNode(
        std::vector<PrintableTemplate> &&contents,
        std::vector<std::pair<PrintableTemplate, DecisionNode>> &&choices
    );

    const std::vector<PrintableTemplate> &getContents() const;

    const std::vector<std::pair<PrintableTemplate, DecisionNode>> &getChoices() const;

private:
    std::vector<PrintableTemplate> mContents;
    std::vector<std::pair<PrintableTemplate, DecisionNode>> mChoices;
};
}

#endif //TYPEWRITER_GAEM_DECISIONNODE_HPP
