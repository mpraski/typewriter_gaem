//
// Created by marcin on 9/25/19.
//

#include "PageController.hpp"

engine::Component::Kind engine::PageController::kind() const {
  return Component::Kind::Script;
}

void engine::PageController::onStart(engine::Entity &entity) {
  listen("new_line", [&, this](const auto &msg) {
    if (shouldScroll()) {
      notify_channel("page_scroll_begin");
      entity.addComponent(
          std::make_unique<TranslateVertical>(
              TranslateVertical::from(0.f)
                  .to(-System::instance().mLineSpacing)
                  .during(60)
                  .via(tweeny::easing::quadraticInOut)
                  .onFinish([this]() {
                    notify_channel("page_scroll_end");
                  })
          )
      );
    }
  });
}

void engine::PageController::onEntityUpdate(engine::Entity &entity, sf::Time dt) {

}

bool engine::PageController::shouldScroll() const {
  return mNextY >= System::instance().effectivePageHeight();
}
