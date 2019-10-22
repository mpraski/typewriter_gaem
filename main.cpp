#include <SFML/Graphics.hpp>
#include "engine/Engine.hpp"

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
  auto rootEntity{std::make_unique<engine::Entity>()};

  rootEntity->addComponent(std::move(pageController));

  try {
    engine::Game game{std::move(rootEntity)};

    game.run();
  } catch (std::exception &e) {
    throw std::runtime_error(e.what());
  }

  return 0;
}