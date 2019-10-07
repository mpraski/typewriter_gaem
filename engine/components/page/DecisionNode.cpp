//
// Created by marcin on 9/27/19.
//

#include "DecisionNode.hpp"

engine::PrintableTemplate::PrintableTemplate(std::wstring content, std::vector<TextEffect> &&effects)
    : kind{PrintableTemplate::Kind::Paragraph},
      text1{std::move(content)},
      text2{},
      effects{std::move(effects)} {

}

engine::PrintableTemplate::PrintableTemplate(std::wstring person, std::wstring speech)
    : kind{PrintableTemplate::Kind::Dialog},
      text1{std::move(person)},
      text2{std::move(speech)},
      effects{} {

}

engine::DecisionNode::DecisionNode(std::vector<PrintableTemplate> &&contents,
                                   std::vector<std::pair<PrintableTemplate, DecisionNode>> &&choices)
    : mContents{std::move(contents)},
      mChoices{std::move(choices)} {

}

const std::vector<engine::PrintableTemplate> &engine::DecisionNode::getContents() const {
  return mContents;
}

const std::vector<std::pair<engine::PrintableTemplate, engine::DecisionNode>> &
engine::DecisionNode::getChoices() const {
  return mChoices;
}
