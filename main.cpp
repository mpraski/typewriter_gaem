#include <SFML/Graphics.hpp>
#include "engine/Engine.hpp"
#include "engine/Utilities/function.h"

int main() {
  engine::DecisionNode sampleStory{
      {
          engine::PrintableTemplate{
              L"Through the darkness of future past, the magician longs to see, one chance out between two worlds, fire walk with me!",
              std::vector<engine::TextEffect>{
                  engine::TextEffect{engine::TextEffect::Kind::DELAY, 99, 117}.withDelay(5.0f),
                  engine::TextEffect{engine::TextEffect::Kind::COLOR, 99, 117}.withColor(sf::Color::Red)
              }
          },
          engine::PrintableTemplate{
              L"Conway",
              L"You just breathe the road."
          },
          engine::PrintableTemplate{
              L"Conway",
              L"Ah, very interesting."
          },
          engine::PrintableTemplate{
              L"Conway",
              L"is this a KR0 ripoff?"
          },
      },
      {}
  };

  auto pageController{std::make_unique<engine::PageController>(sampleStory)};
  auto keyboardInput{std::make_unique<engine::KeyboardInput>(engine::Constants::KeyboardChannel)};
  auto rootEntity{std::make_unique<engine::Entity>()};

  rootEntity->addComponent(std::move(pageController));
  rootEntity->addComponent(std::move(keyboardInput));

  auto lam{[](int s) { return s; }};
  static_assert(std::is_same_v<engine::lambda_detail::lambda_type<decltype(lam)>::arg<0>::type, int>);

  try {
    engine::Game game{std::move(rootEntity)};

    game.run();
  } catch (std::exception &e) {
    throw std::runtime_error(e.what());
  }

  return 0;
}