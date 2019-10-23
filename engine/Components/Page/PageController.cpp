//
// Created by marcin on 9/25/19.
//

#include "PageController.hpp"
#include "../../Entity.hpp"

engine::PageController::PageController(DecisionNode n)
    : Component{},
      mNextY{},
      mPrintableIDs{},
      mDecisionNode{std::move(n)} {

}

engine::Component::Kind engine::PageController::kind() const {
  return Component::Kind::Script;
}

void engine::PageController::onStart(engine::Entity &entity) {
  listen("new_line", [&, this](const auto &msg) {
    if (shouldScroll()) {
      notifyChannel("page_scroll_begin");
      entity.addComponent(
          std::make_unique<TranslateVertical>(
              TranslateVertical::from(0.f)
                  .to(-System::instance().mLineSpacing)
                  .during(60)
                  .via(tweeny::easing::quadraticInOut)
                  .onFinish([this]() {
                    notifyChannel("page_scroll_end");
                  })
          )
      );
    }
  });

  listen("printable_end", [&, this](const auto &msg) {
    if (std::next(mCurrentPrintable) != std::end(mPrintableIDs)) {
      mCurrentPrintable = std::next(mCurrentPrintable);
      notifyChannel("printable_selection", *mCurrentPrintable);
    }
  });

  addContents(entity, mDecisionNode.getContents());

  notifyChannel("printable_selection", mPrintableIDs[0]);
}

void engine::PageController::onEntityUpdate(engine::Entity &entity, sf::Time dt) {

}

bool engine::PageController::shouldScroll() const {
  return mNextY >= System::instance().effectivePageHeight();
}

std::unique_ptr<engine::Printable> engine::PageController::fromTemplate(const engine::PrintableTemplate &tpl) {
  switch (tpl.kind) {
    case PrintableTemplate::Kind::Paragraph:
      return std::make_unique<Paragraph>(tpl.text1, tpl.effects, true);
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
