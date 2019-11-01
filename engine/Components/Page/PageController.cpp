//
// Created by marcin on 9/25/19.
//

#include "PageController.hpp"
#include "../../Entity.hpp"
#include "../../Engine.hpp"

engine::PageController::PageController(DecisionNode n)
    : Component{},
      mNextY{},
      mCurrentY{},
      mPrintableIDs{},
      mCurrentPrintable{},
      mDecisionNode{std::move(n)} {
  assert(!mDecisionNode.getContents().empty());
}

engine::Component::Kind engine::PageController::kind() const {
  return Component::Kind::Script;
}

void engine::PageController::onStart(engine::Entity &entity) {
  listen("new_line", [&, this](const auto &msg) {
    if (shouldScroll()) {
      notifyChannel("page_scroll_begin");
    }
  });

  listen("printable_end", [&, this](const auto &msg) {
    if (std::next(mCurrentPrintable) != std::end(mPrintableIDs)) {
      mCurrentPrintable = std::next(mCurrentPrintable);
      mCurrentY += entity.getComponent<Printable>(*mCurrentPrintable)->localBounds().height;
      notifyChannel("printable_selection", *mCurrentPrintable);
    }
  });

  addContents(entity, mDecisionNode.getContents());

  entity.nextTick([this]() {
    mCurrentPrintable = std::begin(mPrintableIDs);
    notifyChannel("printable_selection", *mCurrentPrintable);
  });
}

void engine::PageController::onEntityUpdate(engine::Entity &entity, sf::Time dt) {

}

bool engine::PageController::shouldScroll() const {
  return mCurrentY >= System::instance().effectivePageHeight();
}

std::unique_ptr<engine::Printable> engine::PageController::fromTemplate(const engine::PrintableTemplate &tpl) {
  switch (tpl.kind) {
    case PrintableTemplate::Kind::Paragraph:
      return std::make_unique<Paragraph>(tpl.text1, tpl.effects, false);
    case PrintableTemplate::Kind::Dialog:
      return std::make_unique<Dialog>(tpl.text1, tpl.text2);
  }
}

void engine::PageController::addContents(Entity &entity, const std::vector<PrintableTemplate> &contents) {
  for (const auto &tpl : contents) {
    auto printable{fromTemplate(tpl)};
    printable->move(0, mNextY);
    mNextY += printable->localBounds().height;
    mPrintableIDs.push_back(printable->getUID());

    entity.addComponent(std::move(printable));
  }
}