#include <iostream>
#include <SFML/Graphics.hpp>

#include "engine/engine.h"

int main() {
  sf::RenderWindow window(sf::VideoMode(200, 200), "FSML works");

  auto resources{engine::make_resources(
      "./res/fonts",
      "./res/sounds"
  )};

  auto sounds{engine::sounds{resources}};

  engine::page page{
      resources,
      {
          engine::paragraph{
              L"Through the darkness of future past, the magician longs to see, one chance out between two worlds, fire walk with me!",
              {
                  engine::text_effect{engine::text_effect::kind::DELAY, 100, 118}.with_delay(5.0f),
                  engine::text_effect{engine::text_effect::kind::COLOR, 100, 118}.with_color(sf::Color::Red)
              }
          },
          engine::paragraph{
              L"Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum."}
      }
  };

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    window.clear();
    window.draw(page);
    window.display();

    if (!page.text_end())
      page.advance();
  }

  return 0;
}